#include "../../stdafx.h"
#include <d3d11.h>
#include "../../engine/Visuals.h"
#include "../../engine/Mesh.h"
#include "Driver.h"
#include "Render.h"
#include "../Generic/BufferTraits.h"
#include "../Generic/Mesh.h"
#include "../Generic/Topology.h"
#include "../Generic/IndexSizes.h"
#include "ConstantBufferBag.hpp"
#include "../../engine/Camera.h"
#include "ShaderCompiler.h"
#include "Vertexs.h"
#include "../../util/Overload.hpp"

namespace Engine::Visuals {
	namespace dx = Graphics::D3D11;
	namespace math = DirectX;

	namespace Basic
	{
		void compile_shaders();

		struct iMesh {
			dx::VertexBuffer	vb{ nullptr };
			dx::IndexBuffer		ib{ nullptr };
			uint32_t			index_count;

			using vertex_type = Engine::Mesh::vertex_type;
			using index_definition = Engine::Mesh::index_definition;
			using index_type = typename index_definition::type;
			using topology_type = typename Graphics::Generic::Topology::TriangleList;
		};

		struct iMaterial
		{
			Graphics::Generic::RGBA emissive;
			Graphics::Generic::RGBA diffuse;
			Graphics::Generic::RGBA ambient;
			Graphics::Generic::RGBA specular;
			float specular_power;
			uint32_t    UseTexture;     // 4 bytes
			float  Padding1;        // 4 bytes
			float  Padding2;		// 4 bytes
			//----------------------------------- (16 byte boundary)
		};  // Total:               // 80 bytes ( 5 * 16 )

		struct iLightDescription {
			Engine::Vector4      Position;               // 16 bytes
   //----------------------------------- (16 byte boundary)
			Engine::Vector4		 Direction;              // 16 bytes
			//----------------------------------- (16 byte boundary)
			Engine::Vector4		Color;                  // 16 bytes
			//----------------------------------- (16 byte boundary)
			float       SpotAngle;              // 4 bytes
			float       ConstantAttenuation;    // 4 bytes
			float       LinearAttenuation;      // 4 bytes
			float       QuadraticAttenuation;   // 4 bytes
			//----------------------------------- (16 byte boundary)
			uint32_t	LightType;              // 4 bytes
			uint32_t    Enabled;                // 4 bytes
			uint32_t    Padding[2];             // 8 bytes
		};


		const int MAXLIGHTS = 8;
		const int DIRECTIONAL_LIGHT = 0;
		const int POINT_LIGHT = 1;
		const int SPOT_LIGHT = 2;


		namespace CB {

			struct VSFrame {
				float timeElapsedSinceLastFrame[4];
			};

			struct VSItem {
				Engine::Matrix     WorldMatrix;
				Engine::Matrix     InverseTransposeWorldMatrix;
				Engine::Matrix     WorldViewProjectionMatrix;
			};

			using PSItem = iMaterial;

			struct PSFrame {
				Engine::Vector4		EyeView;
				Engine::Vector4		GlobalAmbient;
				iLightDescription   Lights[MAXLIGHTS];
			};

		}

		using driver_vertex = dx::VertexDescription< Engine::Visuals::Basic::iMesh::vertex_type>;
		
		
		driver_vertex low_level_vertex_description;
		std::vector<iMesh> meshes;
		std::vector<iMaterial> materials;
		dx::Render render;
		dx::ConstantBufferBag<CB::VSFrame, CB::VSItem> vsBuffers;
		dx::ConstantBufferBag<CB::PSItem,CB::PSFrame> psBuffers;

		dx::VertexShader	vsShader{ nullptr };
		dx::PixelShader		psShader{ nullptr };
		
		const iMesh & getMesh(Visual id) {
			return meshes[id.value];
		}

		const iMaterial & getMaterial(Visual id) {
			return materials[id.value];
		}


		void Init(dx::Driver * driver) {
			render.setDevice(driver);

			render.registerVertexDescription<driver_vertex>(low_level_vertex_description);
			
			CB::VSFrame vsFrame{};
			CB::VSItem vsItem{ };
			CB::PSFrame psFrame{};
			CB::PSItem psItem{};

			render.initalizeConstantBufferBag(vsBuffers, vsFrame, vsItem);
			render.initalizeConstantBufferBag(psBuffers, psItem, psFrame);
			render.initalizeIA<iMesh>();
			render.initalizeVS(vsBuffers);
			render.initalizePS(psBuffers);

			compile_shaders();

		}

		ScreenSize Get_ScreenSize() {
			return { render.mDevice->getWidth(), render.mDevice->getHeight() };
		}


		Visual	Create(Engine::MeshView meshview, Engine::Material mat) {
			namespace BufferTraits = Graphics::Generic::BufferTraits;
			auto vb = render.createVertexBuffer<BufferTraits::Default>(meshview.vertex_data);
			auto ib = render.createIndexBuffer<BufferTraits::Default>(meshview.index_data);
			meshes.push_back({ vb,ib, static_cast<uint32_t>(meshview.index_data.size() / sizeof(iMesh::index_definition::type) )});

			materials.push_back({ mat.emissive, mat.diffuse, mat.ambient, mat.specular, mat.specular_power, false });


			return { static_cast<uint64_t>(std::distance(meshes.begin(), std::prev(meshes.end()))) };
		}

		Engine::Vector4 Vec4FromRGBA(Graphics::Generic::RGBA color) {
			return { color.red(), color.green(), color.blue(), color.alpha() };
		}

		Engine::Vector4 Vec4FromVec3(const Engine::Vector3 & vec3, float w = 0) {
			return { vec3.x, vec3.y, vec3.z, w };
		}

		void Render_SetupLights(const input & state) noexcept {
			
			struct overloads {
				iLightDescription operator ()(Engine::Lights::Directional & dir) const noexcept {
					return iLightDescription{
						{0,0,0,0},
						Vec4FromVec3(dir.direction),
						Vec4FromRGBA(dir.color),
						0,0,0,0,
						DIRECTIONAL_LIGHT, true };
				};

				iLightDescription operator ()(Engine::Lights::Point & point) const noexcept {
					return iLightDescription{
						Vec4FromVec3(point.position),
						{0,0,0,0},
						Vec4FromRGBA(point.color),
						0,point.ConstantAttenuation,point.LinearAttenuation,point.QuadraticAttenuation,
						POINT_LIGHT, true };
				};

				iLightDescription operator ()(Engine::Lights::Spot & spot) const noexcept {
					return iLightDescription{
						Vec4FromVec3(spot.position),
						Vec4FromVec3(spot.direction),
						Vec4FromRGBA(spot.color),
						spot.SpotAngle,spot.ConstantAttenuation,spot.LinearAttenuation,spot.QuadraticAttenuation,
						SPOT_LIGHT, true };

				};
			};
		
			
			CB::PSFrame lightData;
			memset(&lightData.Lights, 0, sizeof(lightData.Lights));
			lightData.GlobalAmbient = Vec4FromRGBA(state.GlobalAmbientLight);

			for (int i = 0; i < MAXLIGHTS; ++i) {
				if (i < state.lights.size()) {
					const auto & var = state.lights[i];
					//std::visit(Util::overload{ [](auto & i) {return iLightDescription{}; } }, state.lights[i]);
					lightData.Lights[i] = std::visit(Util::overload{
						[](const Engine::Lights::Directional & dir) -> iLightDescription {
						return iLightDescription{
							{0,0,0,0},
							Vec4FromVec3(dir.direction),
							Vec4FromRGBA(dir.color),
							0,0,0,0,
							DIRECTIONAL_LIGHT, true };
					},
						[](const Engine::Lights::Point & point) -> iLightDescription {
						return iLightDescription{
							Vec4FromVec3(point.position),
							{0,0,0,0},
							Vec4FromRGBA(point.color),
							0,point.ConstantAttenuation,point.LinearAttenuation,point.QuadraticAttenuation,
							POINT_LIGHT, true };

					},
						[](const Engine::Lights::Spot & spot) -> iLightDescription {
						return iLightDescription{
							Vec4FromVec3(spot.position),
							Vec4FromVec3(spot.direction),
							Vec4FromRGBA(spot.color),
							spot.SpotAngle,spot.ConstantAttenuation,spot.LinearAttenuation,spot.QuadraticAttenuation,
							SPOT_LIGHT, true };

					}
				}, var);

					
				}
				else {
					lightData.Lights[i].Enabled = false;
				}
			}

			lightData.GlobalAmbient = Vec4FromRGBA(state.GlobalAmbientLight);
			Math::XMStoreFloat4(&lightData.EyeView, state.cam.getPosition());


			render.writeBuffer(psBuffers, lightData);
		}

		void Render(input && state) {
			Engine::fMatrix view = state.cam.getView();
			Engine::fMatrix proj = state.cam.getProj();

			Render_SetupLights(state);

			render.clear();

			for (auto & item : state.states) {
				Engine::fMatrix world = math::XMLoadFloat4x4(&item.world);
				Engine::fMatrix world_view_proj = Math::XMMatrixMultiply(Math::XMMatrixMultiply(world, view), proj);
				
				
				CB::VSItem itemcb;
				CB::PSItem itemps{ getMaterial(item.id) };
				
				render.writeBuffer(psBuffers, itemps);

				itemcb.WorldMatrix = item.world;
				Math::XMStoreFloat4x4(&itemcb.WorldViewProjectionMatrix, world_view_proj);
				Math::XMStoreFloat4x4(&itemcb.InverseTransposeWorldMatrix, Math::XMMatrixTranspose(Math::XMMatrixInverse(nullptr, world)));
				


				auto current_mesh = getMesh(item.id);
				render.meshBind<iMesh::vertex_type, iMesh::index_definition>(current_mesh.vb, current_mesh.ib);
				render.writeBuffer(vsBuffers, itemcb);
				render.meshRender(current_mesh.index_count);
			}
			render.present();
		}





		void compile_shaders() {
			const char * psShaderChar = 
R"(
#define MAX_LIGHTS 8

// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D Texture : register(t0);
sampler Sampler : register(s0);

struct _Material
{
    float4  Emissive;       // 16 bytes
    //----------------------------------- (16 byte boundary)
    float4  Ambient;        // 16 bytes
    //------------------------------------(16 byte boundary)
    float4  Diffuse;        // 16 bytes
    //----------------------------------- (16 byte boundary)
    float4  Specular;       // 16 bytes
    //----------------------------------- (16 byte boundary)
    float   SpecularPower;  // 4 bytes
    bool    UseTexture;     // 4 bytes
    float2  Padding;        // 8 bytes
    //----------------------------------- (16 byte boundary)
};  // Total:               // 80 bytes ( 5 * 16 )

cbuffer MaterialProperties : register(b0)
{
    _Material Material;
};

struct Light
{
    float4      Position;               // 16 bytes
    //----------------------------------- (16 byte boundary)
    float4      Direction;              // 16 bytes
    //----------------------------------- (16 byte boundary)
    float4      Color;                  // 16 bytes
    //----------------------------------- (16 byte boundary)
    float       SpotAngle;              // 4 bytes
    float       ConstantAttenuation;    // 4 bytes
    float       LinearAttenuation;      // 4 bytes
    float       QuadraticAttenuation;   // 4 bytes
    //----------------------------------- (16 byte boundary)
    int         LightType;              // 4 bytes
    bool        Enabled;                // 4 bytes
    int2        Padding;                // 8 bytes
    //----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16 byte boundary)

cbuffer LightProperties : register(b1)
{
    float4 EyePosition;                 // 16 bytes
    //----------------------------------- (16 byte boundary)
    float4 GlobalAmbient;               // 16 bytes
    //----------------------------------- (16 byte boundary)
    Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
};  // Total:                           // 672 bytes (42 * 16 byte boundary)

float4 DoDiffuse( Light light, float3 L, float3 N )
{
    float NdotL = max( 0, dot( N, L ) );
    return light.Color * NdotL;
}

float4 DoSpecular( Light light, float3 V, float3 L, float3 N )
{
    // Phong lighting.
    float3 R = normalize( reflect( -L, N ) );
    float RdotV = max( 0, dot( R, V ) );

    // Blinn-Phong lighting
    //float3 H = normalize( L + V );
    //float NdotH = max( 0, dot( N, H ) );

    return light.Color * pow( RdotV, Material.SpecularPower );
}

float DoAttenuation( Light light, float d )
{
    return 1.0f / ( light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d );
}

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

LightingResult DoPointLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;

    float3 L = ( light.Position - P ).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );

    result.Diffuse = DoDiffuse( light, L, N ) * attenuation;
    result.Specular = DoSpecular( light, V, L, N ) * attenuation;

    return result;
}

LightingResult DoDirectionalLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;

    float3 L = -light.Direction.xyz;

    result.Diffuse = DoDiffuse( light, L, N );
    result.Specular = DoSpecular( light, V, L, N );

    return result;
}

float DoSpotCone( Light light, float3 L )
{
    float minCos = cos( light.SpotAngle );
    float maxCos = ( minCos + 1.0f ) / 2.0f;
    float cosAngle = dot( light.Direction.xyz, -L );
    return smoothstep( minCos, maxCos, cosAngle ); 
}

LightingResult DoSpotLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;

    float3 L = ( light.Position - P ).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );
    float spotIntensity = DoSpotCone( light, L );

    result.Diffuse = DoDiffuse( light, L, N ) * attenuation * spotIntensity;
    result.Specular = DoSpecular( light, V, L, N ) * attenuation * spotIntensity;

    return result;
}

LightingResult ComputeLighting( float4 P, float3 N )
{
    float3 V = normalize( EyePosition - P ).xyz;

    LightingResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0} };

    [unroll]
    for( int i = 0; i < MAX_LIGHTS; ++i )
    {
        LightingResult result = { {0, 0, 0, 0}, {0, 0, 0, 0} };

        if ( !Lights[i].Enabled ) continue;
        
        switch( Lights[i].LightType )
        {
        case DIRECTIONAL_LIGHT:
            {
                result = DoDirectionalLight( Lights[i], V, P, N );
            }
            break;
        case POINT_LIGHT: 
            {
                result = DoPointLight( Lights[i], V, P, N );
            }
            break;
        case SPOT_LIGHT:
            {
                result = DoSpotLight( Lights[i], V, P, N );
            }
            break;
        }
        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);

    return totalResult;
}

struct PixelShaderInput
{
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float2 TexCoord     : TEXCOORD0;
};

float4 main( PixelShaderInput IN ) : SV_TARGET
{
    LightingResult lit = ComputeLighting( IN.PositionWS, normalize(IN.NormalWS) );
    
    float4 emissive = Material.Emissive;
    float4 ambient = Material.Ambient * GlobalAmbient;
    float4 diffuse = Material.Diffuse * lit.Diffuse;
    float4 specular = Material.Specular * lit.Specular;

    float4 texColor = { 1, 1, 1, 1 };
    
    if ( Material.UseTexture )
    {
        texColor = Texture.Sample( Sampler, IN.TexCoord );
    }

    float4 finalColor = ( emissive + ambient + diffuse + specular ) * texColor;

    return finalColor;
}
)";

			const char * vsShaderChar = 
R"(

cbuffer PerFrame : register( b0 ) 
{
	float4 random;
}

cbuffer PerObject : register( b1 )
{
    matrix WorldMatrix;
    matrix InverseTransposeWorldMatrix;
    matrix WorldViewProjectionMatrix;
}

struct AppData
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float2 TexCoord     : TEXCOORD0;
    float4 Position     : SV_Position;
};

VertexShaderOutput main( AppData IN )
{
    VertexShaderOutput OUT;

    OUT.Position = mul( WorldViewProjectionMatrix, float4( IN.Position, 1.0f ) );
    OUT.PositionWS = mul( WorldMatrix, float4( IN.Position, 1.0f ) );
    OUT.NormalWS = mul( (float3x3)InverseTransposeWorldMatrix, IN.Normal );
    OUT.TexCoord = IN.TexCoord;

    return OUT;
}
)";

			
			auto shaderPSCompiled = dx::ShaderCompiler::compile_pixelshader(psShaderChar);
			auto shaderVSCompiled = dx::ShaderCompiler::compile_vertexshader(vsShaderChar);

			psShader = render.createPSShader(shaderPSCompiled.asSpan());
			vsShader = render.createVSShader(shaderVSCompiled.asSpan());

			render.vsShaderBind(vsShader);
			render.psShaderBind(psShader);

		}
	}
}



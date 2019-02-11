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

namespace Engine::Visuals {
	namespace dx = Graphics::D3D11;
	namespace math = DirectX;

	/*namespace detail {

		using Mesh = Graphics::Generic::Mesh<dx::VertexBuffer, dx::IndexBuffer,
			Engine::Mesh::vertex_type,
			Graphics::Generic::Topology::TriangleList,
			Engine::Mesh::index_definition>;

	}*/

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
			bool    UseTexture;     // 4 bytes
			float  Padding1;        // 4 bytes
			float  Padding2;		// 4 bytes
			//----------------------------------- (16 byte boundary)
		};  // Total:               // 80 bytes ( 5 * 16 )

		namespace CB {
			struct InfreqVS {
				Engine::Matrix proj;
			};

			struct InfreqPS {
				float doNothingForNow[4];
			};

			struct Frame {
				float timeElapsedSinceLastFrame[4];
			};

			struct ItemVS {
				Engine::Matrix     WorldMatrix;
				Engine::Matrix     InverseTransposeWorldMatrix;
				Engine::Matrix     WorldViewProjectionMatrix;
			};

			struct ItemPS {
				Engine::Vector4 diffuse;
			};
		}

		using driver_vertex = dx::VertexDescription< Engine::Visuals::Basic::iMesh::vertex_type>;
		
		
		driver_vertex low_level_vertex_description;
		std::vector<iMesh> meshes;
		dx::Render render;
		dx::ConstantBufferBag<CB::InfreqVS, CB::Frame, CB::ItemVS> vsBuffers;
		dx::ConstantBufferBag<CB::InfreqPS, CB::Frame, CB::ItemPS> psBuffers;

		dx::VertexShader	vsShader{ nullptr };
		dx::PixelShader		psShader{ nullptr };



		const iMesh & getMesh(Visual id) {
			return meshes[id.value];
		}



		void Init(dx::Driver * driver) {
			render.setDevice(driver);

			render.registerVertexDescription<driver_vertex>(low_level_vertex_description);
			
			CB::InfreqVS infreqVs{};
			CB::Frame frame{ 0.f };
			CB::ItemVS itemVS{};

			render.initalizeConstantBufferBag(vsBuffers, infreqVs, frame, itemVS);
			render.initalizeIA<iMesh>();
			render.initalizeVS(vsBuffers);

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
			return { static_cast<uint64_t>(std::distance(meshes.begin(), std::prev(meshes.end()))) };
		}

		void Render(input && state) {
			Engine::fMatrix view = state.cam.getView();
			CB::InfreqVS proj;
			Math::XMStoreFloat4x4(&proj.proj, state.cam.getProj());
			render.writeBuffer(vsBuffers, proj);
			render.clear();

			for (auto & item : state.states) {
				Engine::fMatrix world = math::XMLoadFloat4x4(&item.world);
				CB::ItemVS itemVS;
				math::XMStoreFloat4x4(&itemVS.world_view,
					world * view);
				

				auto current_mesh = getMesh(item.id);
				render.meshBind<iMesh::vertex_type, iMesh::index_definition>(current_mesh.vb, current_mesh.ib);
				render.writeBuffer(vsBuffers, itemVS);
				render.meshRender(current_mesh.index_count);
			}
			render.present();
		}





		void compile_shaders() {
			const char * psShaderChar = 
R"(
struct PIn {
	float4 position : SV_POSITION;
	float4 normal : TEXCOORD2;
	float2 uv : TEXCOORD0;
};

float4 main(PIn pin) : SV_TARGET
{
	return float4(pin.normal.x+ pin.uv.x, pin.normal.y+ pin.uv.y, pin.normal.z+ pin.uv.x,1.0);
}
)";

			const char * vsShaderChar = 
R"(
cbuffer PerSession : register(b0)
{
	matrix proj;
}

cbuffer PerFrame : register(b1)
{
	float4 time;
}

cbuffer PerItem : register(b2)
{
	matrix world_view;
}

struct VIn {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TexCoord;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 normal : TEXCOORD2;
	float2 uv : TEXCOORD0;
};

VOut main(VIn input)
{
	VOut outpt;
	matrix m;
	m = mul( proj, world_view );
	outpt.position = mul(m, float4(input.position, 1.0f));
	outpt.normal = float4(input.normal,1);
	outpt.uv = input.uv;

	return outpt;
}
)";

			//pos = mul( world_view, position );
			
			auto shaderPSCompiled = dx::ShaderCompiler::compile_pixelshader(psShaderChar);
			auto shaderVSCompiled = dx::ShaderCompiler::compile_vertexshader(vsShaderChar);

			psShader = render.createPSShader(shaderPSCompiled.asSpan());
			vsShader = render.createVSShader(shaderVSCompiled.asSpan());

			render.vsShaderBind(vsShader);
			render.psShaderBind(psShader);

		}
	}
}
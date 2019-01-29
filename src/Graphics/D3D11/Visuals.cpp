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

namespace Engine::Visuals {
	namespace dx = Graphics::D3D11;
	namespace math = DirectX;

	namespace detail {

		using Mesh = Graphics::Generic::Mesh<dx::VertexBuffer, dx::IndexBuffer,
			Engine::Mesh::vertex_type,
			Graphics::Generic::Toplogy::TriangleList,
			Engine::Mesh::index_definition>;

		using SortKeyType = uint64_t;

		struct visual_mesh {
			Mesh mesh;
		};

		struct visual_texture {

		};

		struct visual_material {
			Graphics::Generic::RGBA diffuse;
			Graphics::Generic::RGBA specular;
			float roughness;
			float reflectiveness;
		};

		std::vector<visual_mesh> vMeshes;
		std::vector<visual_texture> vTex;
		std::vector<visual_material> vMats;



		std::unique_ptr<dx::Driver> driver{ nullptr };

	}

	namespace Basic
	{
		struct iMesh {
			dx::VertexBuffer	vb{ nullptr };
			dx::IndexBuffer		ib{ nullptr };
			uint32_t			index_count;

			using vertex_type = Engine::Mesh::vertex_type;
			using index_definition = Engine::Mesh::index_definition;
			using index_type = typename index_definition::type;
		};

		namespace CB {
			struct InfreqVS {
				Engine::Matrix proj;
			};

			struct InfreqPS {
				float doNothingForNow;
			};

			struct Frame {
				float timeElapsedSinceLastFrame;
			};

			struct ItemVS {
				Engine::Matrix world_view;
			};

			struct ItemPS {
				Engine::Vector4 diffuse;
			};
		}


		std::vector<iMesh> meshes;
		dx::Render render;
		dx::ConstantBufferBag<CB::InfreqVS, CB::Frame, CB::ItemVS> vsBuffers;
		dx::ConstantBufferBag<CB::InfreqPS, CB::Frame, CB::ItemPS> psBuffers;

		dx::VertexShader	vsShader{ nullptr };
		dx::PixelShader		psShader{ nullptr };

		Camera camera;

		const iMesh & getMesh(Visual id) {
			return meshes[id.value];
		}



		void Init(dx::Driver * driver) {
			render.setDevice(driver);
			camera.setPerspective(driver->getWidth(), driver->getHeight(), 0.001f, 1000.f);
			camera.lookAt({ 0.f,0.f,0.f });
			camera.setPosition({ 0.f,0.f,-5.f });

			CB::InfreqVS infreqVs{ camera.getProjData() };
			CB::Frame frame{ 0.f };
			CB::ItemVS itemVS;
			math::XMStoreFloat4x4(&itemVS.world_view, math::XMMatrixTranspose(math::XMMatrixIdentity()));

			render.initalizeConstantBufferBag(vsBuffers, infreqVs, frame, itemVS);
			render.initalizeIA<iMesh>();
			render.initalizeVS(vsBuffers);

			compile_shaders();

		}


		Visual	Create(Engine::MeshView meshview, Engine::Material mat) {
			namespace BufferTraits = Graphics::Generic::BufferTraits;
			auto vb = render.createVertexBuffer<BufferTraits::Default>(meshview.vertex_data);
			auto ib = render.createIndexBuffer<BufferTraits::Default>(meshview.index_data);
			meshes.push_back({ vb,ib, static_cast<uint32_t>(meshview.index_data.size()) });
			return { static_cast<uint64_t>(std::distance(meshes.begin(), std::prev(meshes.end()))) };
		}

		void Render(input && state) {
			// Skip the sorting for now
			Engine::fMatrix view = DirectX::XMLoadFloat4x4(&state.view);

			render.clear();
			for (auto & item : state.states) {
				Engine::fMatrix world = math::XMLoadFloat4x4(&item.world);
				CB::ItemVS itemVS;
				math::XMStoreFloat4x4(&itemVS.world_view,
					math::XMMatrixTranspose(math::XMMatrixMultiply(world, view))
				);

				auto current_mesh = getMesh(item.id);
				render.meshBind<iMesh::vertex_type, iMesh::index_definition>(current_mesh.vb, current_mesh.ib);
				render.writeBuffer(vsBuffers, itemVS);
				render.meshRender(current_mesh.index_count);
			}
			render.present();
		}





		void compile_shaders() {
			const char * psShaderChar = R"( float4 main(float4 position : SV_POSITION, float4 normal : Normal, float2 uv : TexCoord) : SV_TARGET
			{
				return float4(normal.x + uv.x,normal.y + uv.y,normal.z + uv.x,1.0);
			} )";

			const char * vsShaderChar = R"(

				cbuffer PerSession : register(b0)
				{
					matrix proj;
				}

				cbuffer PerFrame : register(b1)
				{
					float time;
				}

				cbuffer PerItem  : register(b2)
				{
					matrix world_view;
				}


				struct VOut {
					float4 position : SV_POSITION;
					float4 normal : Normal;
					float2 uv : TexCoord;
				};

				VOut main(  float4 position : Position, float4 normal : Normal, float2 uv : TexCoord ) 
				{
					VOut v;
					float4 pos;

					pos = mul( position, world_view );
					v.position = mul( pos, proj );
					v.normal = normal;
					v.uv = uv;

					return v;
				}		)";

			auto shaderPSCompiled = dx::ShaderCompiler::compile(psShaderChar);
			auto shaderVSCompiled = dx::ShaderCompiler::compile(vsShaderChar);

			psShader = render.createPSShader(shaderPSCompiled.asSpan());
			vsShader = render.createVSShader(shaderVSCompiled.asSpan());

			render.vsShaderBind(vsShader);
			render.psShaderBind(psShader);

		}
	}
}
#include "../../stdafx.h"
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
		std::vector<SortKeyType> vSorts;
		uint64_t			idNext{ 0 };


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
			CB::ItemVS itemVS{ math::XMMatrixTranspose(math::XMMatrixIdentity()) };

			render.initalizeConstantBufferBag(vsBuffers, infreqVs, frame, itemVS);
		}


		Visual	Create(Engine::MeshView meshview, Engine::Material mat) {
			namespace BufferTraits = Graphics::Generic::BufferTraits;
			auto vb = detail::driver->createVertexBuffer<BufferTraits::Default>(meshview.vertex_data);
			auto ib = detail::driver->createIndexBuffer<BufferTraits::Default>(meshview.index_data);
			meshes.emplace_back( vb,ib, meshview.index_data.length());
			return { std::distance(meshes.begin(), std::prev(meshes.end())) };
		}

		void Render(input && state) {
			// Skip the sorting for now
			Engine::fMatrix view = DirectX::XMLoadFloat4x4(&state.view);
			
			render.clear();
			for (auto & item : state.states) {
				auto current_mesh = getMesh(item.id);
				render.meshBind(current_mesh.vb,current_mesh.ib);
				render->drawMesh(*matrix);
			}
			detail::render->end();
		}
		
	}

	

	}




}
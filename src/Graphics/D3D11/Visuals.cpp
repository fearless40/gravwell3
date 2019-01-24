#include "../../stdafx.h"
#include "../../engine/Visuals.h"
#include "../../engine/Mesh.h"
#include "Driver.h"
#include "Render.h"
#include "../Generic/BufferTraits.h"
#include "../Generic/Mesh.h"
#include "../Generic/Topology.h"
#include "../Generic/IndexSizes.h"

namespace Engine::Visuals {
	namespace dx = Graphics::D3D11;

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
		};
		
		namespace CB {
			struct Infreq {
				Engine::Matrix proj;
			};

			struct Frame {
				float timeElapsedSinceLastFrame;
			};

			struct Item {
				Engine::Matrix world_view;
			};
		}

		struct D3DMembers {
			comptr<ID3D11DeviceContext> mRender{ nullptr };
			dx::Driver				*	mDevice{ nullptr };
			comptr<ID3D11Buffer>		mPerGame{ nullptr };
			comptr<ID3D11Buffer>		mPerFrame{ nullptr };
			comptr<ID3D11Buffer>		mPerItem{ nullptr };
			comptr<ID3D11Buffer>		mUser{ nullptr };
		};

		std::vector<iMesh> meshes;
		D3DMembers d3d;
		//std::vector<Visual::value_type> free_list;

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
			
			detail::render->clear();
			for (auto[visualID, matrix] : data) {
				detail::render->meshBind(detail::vMeshes[visualID->get()].mesh);
				detail::render->drawMesh(*matrix);
			}
			detail::render->end();
		}

	}

	

	}




}
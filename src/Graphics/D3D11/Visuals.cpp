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
		std::unique_ptr<dx::Render> render{ nullptr };
	}


	Visual	Create(Engine::MeshView meshview, Engine::Material mat) {
		namespace BufferTraits = Graphics::Generic::BufferTraits;
		auto vb = detail::driver->createVertexBuffer<BufferTraits::Default>(meshview.vertex_data);
		auto ib = detail::driver->createIndexBuffer<BufferTraits::Default>(meshview.index_data);
		detail::Mesh m{ vb,ib, meshview.index_data.size_bytes() / sizeof(detail::Mesh::index_type),0 };


	}

	void RenderBasicID(gsl::span<Visual> vId, gsl::span<Engine::Matrix> matrixs) {
		// Skip the sorting for now
		detail::render->begin(0, 0);
		detail::render->clear();
		for (int i{ 0 }; i < vId.size(); ++i) {
			detail::render->meshBind(detail::vMeshes[vId[i].value].mesh);
			detail::render->drawMesh(matrixs[i]);
		}
		detail::render->end();
	}



}
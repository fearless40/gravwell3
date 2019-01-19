#pragma once

namespace Engine {
	struct Mesh {

	};


	struct MeshView {
		gsl::span<std::byte> vertex_data;
		gsl::span<std::byte> index_data;

	};
}
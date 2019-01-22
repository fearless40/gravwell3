
#include <gsl/span>
#include <memory>
#include "ID.h"
#include "MathTypes.h"
#include "Mesh.h"
#include "Material.h"


namespace Engine::Visuals {
	
	struct Basic {};
	struct Custom {};
	struct ParticleSystem {};
	struct MeshAtlas {};

	using Visual = ID<Basic, 8>;

	template<typename VisualType>
	struct VisualStates {
		std::unique_ptr<VisualType> visual_data;
		std::unique_ptr<Engine::Matrix> matrix_data;
		std::size_t visual_count;
	};

	
	void RenderBasicID(gsl::span<Visual> vId, gsl::span<Engine::Matrix> matrixs);
	

	Visual	Create(Engine::MeshView mesh, Engine::Material mat);

	/*Visual	CopyMesh(Visual src, Engine::Material mat);
	Visual	CopyMaterial(Visual src, Engine::MeshView mesh);
	void	Remove(Visual src);
	void	SetTag(Visual src, uint32_t tag);
	void	SetName(Visual src, std::string name);
	*/
	//Render is a async function call 
	// Everything within Render is synchronous
	template <typename VisualType>
	void Render(VisualStates<VisualType> && state) {} //No body for unknown VisualTypes

	
	template <>
	void Render<Visual>(VisualStates<Visual> && state) {
		impl::RenderBasicID({ state.visual_data.get(), state.visual_count },
			{ state.matrix_data.get(), state.visual_count });
	}
}
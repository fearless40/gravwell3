
#include <gsl/span>
#include <memory>
#include "ID.h"
#include "MathTypes.h"
#include "Mesh.h"
#include "Material.h"
#include "../util/MultiIterator.hpp"


namespace Engine::Visuals {
	
	struct Basic {};
	struct Custom {};
	struct ParticleSystem {};
	struct MeshAtlas {};

	using Visual = ID<Basic, std::uint64_t>;

	template<typename VisualIDType>
	struct VisualStates {
		std::unique_ptr<VisualIDType> visual_data;
		std::unique_ptr<Engine::Matrix> matrix_data;
		std::size_t visual_count;
		Engine::Matrix	view;

		using iterator = Util::MultiIterator<VisualIDType, Engine::Matrix>;

		auto begin() { return iterator{0, visual_data.get(), matrix_data.get()}; }
		auto end() { return iterator::EndIterator{ visual_count }; }
	};

	
	void RenderBasicID(VisualStates<Visual> && data);
	

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
		RenderBasicID(std::forward< VisualStates<Visual> >(state));
	}
}
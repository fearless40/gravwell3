
#include <gsl/span>
#include "ID.h"
#include "MathTypes.h"


namespace Engine::Visuals {
	
	struct Basic {};
	struct Custom {};
	struct ParticleSystem {};

	using Visual = ID<Basic, 8>;

	template<typename VisualType>
	struct VisualStates {
		gsl::span<T> visual_list;
		gsl::span<Engine::Position> position_list;
		gsl::span<Engine::Rotation> rotation_list;
	};

	template<typename VisualType>
	struct VisualStates_Matrix {
		gsl::span<T> visual_list;
		gsl::span<Engine::Matrix> matrix_list;
	};

	namespace impl {
		void RenderBasicID(gsl::span<Visual> && vId, gsl::span<Engine::Matrix> && matrixs);
	}


	Visual CreateVisual(Engine::Mesh mesh, Engine::Material mat);
	
	//async function
	template <typename VisualType>
	void Render(VisualStates<VisualType> && state) {} //No body for unknown VisualTypes

	template <>
	void Render<Visual>(VisualStates<Visual> && state) {
		
	}
}
#pragma once
#include "MathTypes.h"
#include "../Graphics/Generic/Color.h"

namespace Engine {
	struct Material {
		Graphics::Generic::RGBA diffuse;
		Graphics::Generic::RGBA specular;
		float roughness;
		float reflectiveness;
		// Texture stuff
	};
}
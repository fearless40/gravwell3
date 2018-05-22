#pragma once

namespace Graphics::Generic {
	
	enum class VideoOutputs {
		Primary,
		Secondary
	};

	enum class VideoCards {
		Primary,
		Secondary
	};

	struct DisplayMode {
		unsigned int width = 0, height = 0;
		struct RefreshRate {
			unsigned int numerator = 0;
			unsigned int denominator = 0;
		} refreshrate;
	};

	struct DisplayCreation {
		bool useVsync = false;
		bool useFullscreen = false;
	};

	enum class BufferBinding
	{
		Default,
		Immutable,
		Dynamic,
		Staging
	};
}

#include "Buffer.h"
#include "Buffer1D.h"
#include "Buffer2D.h"
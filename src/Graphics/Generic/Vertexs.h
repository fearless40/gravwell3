#pragma once

#include <DirectXMath.h>

namespace Graphics::Generic::Vertex {
	enum class Names
	{
		Pos = 0,
		Pos_Color = 1,
		Pos_UV = 2,
		Pos_Normal_UV = 3,
	};

	struct Position {
		DirectX::XMFLOAT4 position;
	};

	struct PositionColor {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
	};

	struct PositionUV {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 uv;
	};

	struct PositionNormalUV {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 uv;
	};
}
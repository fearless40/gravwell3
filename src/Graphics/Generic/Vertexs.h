#pragma once

#include <DirectXMath.h>

namespace Graphics::Generic 
{ 
	namespace VertexTypes
	{
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
			PositionNormalUV() = default;

			PositionNormalUV(const PositionNormalUV&) = default;
			PositionNormalUV& operator=(const PositionNormalUV&) = default;

			PositionNormalUV(PositionNormalUV&&) = default;
			PositionNormalUV& operator=(PositionNormalUV&&) = default;

			PositionNormalUV(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& textureCoordinate)
				: position(position),
				normal(normal),
				textureCoordinate(textureCoordinate)
			{ }

			PositionNormalUV(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR textureCoordinate)
			{
				XMStoreFloat3(&this->position, position);
				XMStoreFloat3(&this->normal, normal);
				XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
			}

			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 textureCoordinate;

		};
	}

	template <typename Base> 
	struct VertexDescription {
		using vertex_type = typename Base::vertex_type;
		using base_type = Base;
		auto asBase() { return static_cast<Base &>(*this); }
	};
}
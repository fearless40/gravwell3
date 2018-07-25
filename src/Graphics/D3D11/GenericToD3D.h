#pragma once

#include "../Generic/Driver.h"


namespace Graphics::D3D::Conversion {
constexpr D3D11_USAGE BufferBinding(Graphics::Generic::BufferBinding bb) {
	switch (bb)
	{
	case Graphics::Generic::BufferBinding::Immutable:
		return D3D11_USAGE_IMMUTABLE;

	case Graphics::Generic::BufferBinding::Dynamic:
		return D3D11_USAGE_DYNAMIC;

	case Graphics::Generic::BufferBinding::Default:
		return D3D11_USAGE_DEFAULT;

	case Graphics::Generic::BufferBinding::Staging:
		return D3D11_USAGE_STAGING;
	}
	}

constexpr unsigned int BufferBindingToCPUAccess(Graphics::Generic::BufferBinding bb) {
	switch (bb)
	{
	case Graphics::Generic::BufferBinding::Dynamic:
		return D3D11_CPU_ACCESS_WRITE;
	case Graphics::Generic::BufferBinding::Staging:
		return D3D11_CPU_ACCESS_READ;
	default:
		return 0;
	}
}

constexpr DXGI_FORMAT IndexFormat(Graphics::Generic::IndexBufferFormat fmt) {
	switch(fmt) {
	case Graphics::Generic::IndexBufferFormat::UINT_16:
		return DXGI_FORMAT_R16_UINT;
	case Graphics::Generic::IndexBufferFormat::UINT_32:
		return DXGI_FORMAT_R32_UINT;
	}
}
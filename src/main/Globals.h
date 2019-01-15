
#pragma once

#define USE_D3D11_RENDER

#if defined(USE_D3D11_RENDER)

#include ""
inline Graphics::D3D11::Render & GetRender();

#endif
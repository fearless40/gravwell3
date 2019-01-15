
#pragma once

#define USE_D3D11_RENDER

#if defined(USE_D3D11_RENDER)

#include "Graphics/D3D11/Render.h"
inline Graphics::D3D11::Render & GetRender();

#endif
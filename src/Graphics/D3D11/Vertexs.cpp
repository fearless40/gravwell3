#include "../../stdafx.h"
#include "../Shared.h"
#include <d3d11.h>
#include <D3DCompiler.h>
#include "Vertexs.h"
#include "Driver.h"

namespace Graphics::D3D11
{
	void test() {
		VertexDescription<Graphics::Generic::VertexTypes::Position> vpos;
		auto d = Driver::CreateDevice(0, {}, {});
		d->registerVertexDescription(vpos);
	}
}
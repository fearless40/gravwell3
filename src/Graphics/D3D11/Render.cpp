#include "../../stdafx.h"
#include <d3d11.h>
#include "../DXGI/dxgi.h"
#include "../Shared.h"
#include "Driver.h"
#include "Render.h"
#include "../Generic/Color.h"
#include "Types.h"

struct Monkey {};
struct Turkey {};
struct Horse {};

void test() {
	Graphics::D3D11::Render<Monkey, Turkey, Horse> r;
	auto k = r.getCbBuffer<Horse>();
}
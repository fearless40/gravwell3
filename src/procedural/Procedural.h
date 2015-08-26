
#pragma once

#include "../render/MeshBuilder.h"
#include "../d3d11/VertexTypeManager.h"

namespace Procedural {

	typedef MeshBuilder<VertexTypeManager::VertexPosNormalUV, __int32> MeshBuilderType;
	typedef DirectX::XMFLOAT3 Vector3;

	typedef VertexTypeManager::VertexPosNormalUV vertex;

	extern RefCountPointer<MeshCPU> MakeBox();
	extern RefCountPointer<MeshCPU> MakePlane();

};

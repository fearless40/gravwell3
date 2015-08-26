
#pragma once

#include "../util/GenericHandle.h"


typedef GenericHandle<12, 20> DefaultHandle;


typedef GenericHandle3<2,11, 19> VertexBufferHandle;
typedef GenericHandle3<2,11, 19> IndexBufferHandle;
typedef GenericHandle3<2,11, 19> ConstantBufferHandle; 

typedef DefaultHandle  ConstantBufferArrayHandle;


typedef DefaultHandle BufferArrayHandle;
typedef DefaultHandle MeshHandle;
typedef DefaultHandle ModelHandle;
typedef DefaultHandle MaterialHandle;
typedef DefaultHandle VertexShaderHandle;
typedef DefaultHandle PixelShaderHandle;
typedef DefaultHandle MaterialHandle;

typedef void (*HandleInvalidationCallback)(void);
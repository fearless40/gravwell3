// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include "util\InvasiveRefCount.h"
#include "util\RefCountPointer.h"


#include <DirectXMath.h>
#include <vector>
#include <functional>

#include <winrt/base.h>

#include <d3d11.h>

template<typename T> using comptr = winrt::com_ptr<T>;

// TODO: reference additional headers your program requires here

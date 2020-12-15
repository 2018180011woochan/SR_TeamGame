#pragma once

#ifndef __ENGINE_INCLUDE_H__
#define __ENGINE_INCLUDE_H__
#pragma warning(disable : 4251)

#pragma region STANDARD
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <tchar.h>
#include <Shlwapi.h>//파일경로 관련 메크로 쓸려고
using namespace std;

#pragma endregion

#pragma region DIRECT3D
#include <d3d9.h>
#include <d3dx9.h>
#pragma endregion

#pragma region USER
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include "EngineEnum.h"
#include "EngineFunction.h"
#include "EngineStruct.h"
#include "EngineExtern.h"
#include "FileInfo.h"
#pragma endregion


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG



#endif // !__ENGINE_INCLUDE_H__

// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <fstream>
#include <d3d9.h>

#include <random>
#include <ctime>
#include <iostream>
#include <functional>


#include <fstream>
#include <process.h>

// 소리 ///////////////////////////////////////////////////////////////////
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
///////////////////////////////////////////////////////////////////////////

// 동영상 /////////////////////////////////////////////////////////////////
#include <Vfw.h>
#pragma comment (lib, "vfw32.lib")
///////////////////////////////////////////////////////////////////////////

#include "Include.h"
#include "MgrInclude.h"
#include "SoundMgr.h"

using namespace std;
using namespace std::tr1;

// 메모리 누수
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG


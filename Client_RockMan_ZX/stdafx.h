// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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

// �Ҹ� ///////////////////////////////////////////////////////////////////
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
///////////////////////////////////////////////////////////////////////////

// ������ /////////////////////////////////////////////////////////////////
#include <Vfw.h>
#pragma comment (lib, "vfw32.lib")
///////////////////////////////////////////////////////////////////////////

#include "Include.h"
#include "MgrInclude.h"
#include "SoundMgr.h"

using namespace std;
using namespace std::tr1;

// �޸� ����
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG


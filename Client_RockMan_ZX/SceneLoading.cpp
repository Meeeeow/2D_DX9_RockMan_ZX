#include "stdafx.h"
#include "SceneLoading.h"
#include "AquaField.h"
#include "OXIntro.h"

CScene * CSceneLoading::Create_Scene()
{
	CScene* pInstance = new CSceneLoading;
	if (FAILED(pInstance->Init_Scene()))
	{
		MSG_BOX(L"로딩 실패");
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}

unsigned CSceneLoading::ThreadFunc(LPVOID pVoid)
{
	CRITICAL_SECTION* pCri = (CRITICAL_SECTION*)pVoid;

	EnterCriticalSection(pCri);

	if (FAILED(CTextureMgr::Get_Instance()->ReadPathFile(L"../Data/PathInfo.txt")))
	{
		MSG_BOX(L"ThreadFunc 실패");
		return E_FAIL;
	}
	CTextureMgr::Get_Instance()->Set_String(L"로딩 완료!");
	LeaveCriticalSection(pCri);
	return 0;
}

CSceneLoading::CSceneLoading()
{
}


CSceneLoading::~CSceneLoading()
{
}

HRESULT CSceneLoading::Init_Scene()
{
	CSoundMgr::Get_Instance()->Initialize();
	CEffectMgr::Get_Instance()->Insert_Effect(CAquaField::Create_AquaField(), CEffectMgr::AQUAFIELD);
	CEffectMgr::Get_Instance()->Insert_Effect(COXIntro::Create_Intro(), CEffectMgr::INTRO);
	m_hVideo = MCIWndCreate(g_hWND, NULL, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR
		, L"../Single_Texture/Loading/Loading.wmv");
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, &cri, 0, nullptr);
	InitializeCriticalSection(&cri);
	return S_OK;
}

void CSceneLoading::Update_Scene()
{
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		MCIWndPlay(m_hVideo);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		MCIWndClose(m_hVideo);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::TRAINROOM);
	}
}

void CSceneLoading::Late_Update_Scene()
{
}

void CSceneLoading::Render_Scene()
{

}

void CSceneLoading::Release_Scene()
{
}

#include "stdafx.h"
#include "MainApplication.h"
#include "Player.h"

CMainApplication * CMainApplication::Create_Game()
{
	//·£´ý MT19937
	CMainApplication* pApp = new CMainApplication;
	if (FAILED(pApp->Init_Game()))
		SAFE_DELETE(pApp);
	return pApp;
}

HRESULT CMainApplication::Init_Game()
{
	CGraphicDeviceMgr::Get_Instance()->Init_Graphic();
	CTextureMgr::Get_Instance()->Add_Texure();
	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::LOADING);
	CTimeMgr::Get_Instance()->Ready_Time_Manager();
	return S_OK;
}

void CMainApplication::Update_Game()
{
	CTimeMgr::Get_Instance()->Update_Time_Manager();
	CSceneMgr::Get_Instance()->Update_SceneMgr();
	CEffectMgr::Get_Instance()->Update_EffectMgr();
}

void CMainApplication::Late_Update_Game()
{
	CSceneMgr::Get_Instance()->Late_Update_SceneMgr();
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CEffectMgr::Get_Instance()->Late_Update_EffectMgr();
}

void CMainApplication::Render_Game()
{
	if (CSceneMgr::Get_Instance()->Get_Scene() != CSceneMgr::LOADING)
	{
		CGraphicDeviceMgr::Get_Instance()->Render_Begin();
		CSceneMgr::Get_Instance()->Render_SceneMgr();
		CEffectMgr::Get_Instance()->Render_EffectMgr();
		CGraphicDeviceMgr::Get_Instance()->Render_Stop();
	}
}

void CMainApplication::Release_Game()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CFrameMgr::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
	CObjectMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTextureMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CGraphicDeviceMgr::Destroy_Instance();
	CEffectMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CInterfaceMgr::Destroy_Instance();
	CCollisionMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
}

CMainApplication::CMainApplication()
{
}


CMainApplication::~CMainApplication()
{
	Release_Game();
}

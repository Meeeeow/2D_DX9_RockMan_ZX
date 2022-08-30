#include "stdafx.h"
#include "AreaD5.h"
#include "Player.h"

CScene * CAreaD5::Create_AreaD5()
{
	CScene* pScene = new CAreaD5;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);
	return pScene;
}

CAreaD5::CAreaD5()
{
}


CAreaD5::~CAreaD5()
{
}

HRESULT CAreaD5::Init_Scene()
{
	Get_TileArray(L"AreaD5_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoAreaD5.dat", m_vecTileArray.x, m_vecTileArray.y);
	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_AreaD5.wav");
	return S_OK;
}

void CAreaD5::Update_Scene()
{
	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::AREA_D5);
	CScrollMgr::Get_Instance()->Set_ScrollLock(11200, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();
}

void CAreaD5::Late_Update_Scene()
{
	
	CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::AREA_D5);
}

void CAreaD5::Render_Scene()
{
	Render_Scene_Key(L"AreaD5_Tile",1.f,1.f);
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::AREA_D5);
	CObjectMgr::Get_Instance()->Render_Player();
}

void CAreaD5::Release_Scene()
{
}

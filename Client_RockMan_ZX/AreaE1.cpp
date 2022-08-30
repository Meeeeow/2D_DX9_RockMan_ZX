#include "stdafx.h"
#include "AreaE1.h"
#include "SeedRoid.h"
#include "Hivok.h"
CScene * CAreaE1::Create_AreaE1()
{
	CScene* pScene = new CAreaE1;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);
	return pScene;
}

CAreaE1::CAreaE1()
{
}


CAreaE1::~CAreaE1()
{
}

HRESULT CAreaE1::Init_Scene()
{
	Get_TileArray(L"AreaE1_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoAreaE1.dat", m_vecTileArray.x, m_vecTileArray.y);
	if (CObjectMgr::Get_Instance()->Get_Type(CObjectMgr::AREA_E1).empty())
	{
		//CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(950.f, 300.f), CObjectMgr::AREA_E1);
		//CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(1527.f, 300.f), CObjectMgr::AREA_E1);
		//CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(2100.f, 250.f), CObjectMgr::AREA_E1);
		CObjectMgr::Get_Instance()->Add_Object(CHivok::Create_Hivok(), CObjectMgr::AREA_E1);
	}

	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Hivolt.wav");

	return S_OK;
}

void CAreaE1::Update_Scene()
{
	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::AREA_E1);
	CScrollMgr::Get_Instance()->Set_ScrollLock(4220, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();

}

void CAreaE1::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::AREA_E1);
}

void CAreaE1::Render_Scene()
{
	Render_Scene_Key(L"AreaE1_Tile", 1.f , 1.f);
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::AREA_E1);
	CObjectMgr::Get_Instance()->Render_Player();

}

void CAreaE1::Release_Scene()
{
}

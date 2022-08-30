#include "stdafx.h"
#include "BossOX.h"
#include "Player.h"
#include "CopyX.h"
CScene * CBossOX::Create_Boss_OX()
{
	CScene* pScene = new CBossOX;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);
	return pScene;
}

CBossOX::CBossOX()
{
}


CBossOX::~CBossOX()
{
}

HRESULT CBossOX::Init_Scene()
{
	
	Get_TileArray(L"OX_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoOX.dat", m_vecTileArray.x, m_vecTileArray.y);
	CObjectMgr::Get_Instance()->Add_Object(CCopyX::Create_X(), CObjectMgr::BOSS);
	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	return S_OK;
}

void CBossOX::Update_Scene()
{
	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::BOSS);
	CScrollMgr::Get_Instance()->Set_ScrollLock(1600, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();

}

void CBossOX::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_ObjectMgr();
}

void CBossOX::Render_Scene()
{
	Render_Scene_Key(L"OX_Background", 1.1f, 1.1f);
	Render_Scene_Key_No_Quake(L"OX_Tile");
	//CTileMgr::Get_Instance()->Render_TileMgr();
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::BOSS);
	CObjectMgr::Get_Instance()->Render_Player();
	
}

void CBossOX::Release_Scene()
{
}

#include "stdafx.h"
#include "HivokArea.h"


CScene * CHivokArea::Create_HivokStage()
{
	CScene* pScene = new CHivokArea;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);
	return pScene;
}

CHivokArea::CHivokArea()
{
}


CHivokArea::~CHivokArea()
{
}

HRESULT CHivokArea::Init_Scene()
{
	Get_TileArray(L"Hivok_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoTrainRoom.dat", m_vecTileArray.x, m_vecTileArray.y);
	return S_OK;
}

void CHivokArea::Update_Scene()
{
	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::HIVOK_AREA);
	CScrollMgr::Get_Instance()->Set_ScrollLock(800, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();
}

void CHivokArea::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_ObjectMgr();

}

void CHivokArea::Render_Scene()
{
	Render_Scene_Key(L"Hivok_Background", 1.2f, 1.2f);
	Render_Scene_Key_No_Quake(L"Hivok_Tile");
	//CTileMgr::Get_Instance()->Render_TileMgr();
	CObjectMgr::Get_Instance()->Render_Player();
}

void CHivokArea::Release_Scene()
{
}

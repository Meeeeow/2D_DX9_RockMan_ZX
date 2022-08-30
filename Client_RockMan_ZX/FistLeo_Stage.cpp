#include "stdafx.h"
#include "FistLeo_Stage.h"
#include "FistLeo.h"
#include "Player.h"

CScene * CFistLeo_Stage::Create_Magma()
{
	CScene* pScene = new CFistLeo_Stage;
	pScene->Init_Scene();
	return pScene;
}

CFistLeo_Stage::CFistLeo_Stage()
{
}


CFistLeo_Stage::~CFistLeo_Stage()
{
}

HRESULT CFistLeo_Stage::Init_Scene()
{
	Get_TileArray(L"Magma_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoMagma.dat", m_vecTileArray.x, m_vecTileArray.y);
	CObjectMgr::Get_Instance()->Add_Object(CFistLeo::Create_Leo(), CObjectMgr::MAGMA);
	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_FistLeo.wav");
	return S_OK;
}

void CFistLeo_Stage::Update_Scene()
{
	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::MAGMA);
	CScrollMgr::Get_Instance()->Set_ScrollLock(1600, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();
}

void CFistLeo_Stage::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::MAGMA);
}

void CFistLeo_Stage::Render_Scene()
{
	Render_Scene_No_Scroll(L"OXIntro");
	Render_Scene_Key(L"Magma_Tile",1.f,1.f);
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::MAGMA);
	CObjectMgr::Get_Instance()->Render_Player();
}

void CFistLeo_Stage::Release_Scene()
{
}

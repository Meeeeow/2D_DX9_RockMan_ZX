#include "stdafx.h"
#include "Scraper.h"
#include "Serpant.h"

CScene * CScraper::Create_Scraper()
{
	CScene* pScene = new CScraper;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);
	return pScene;
}

CScraper::CScraper()
{
}


CScraper::~CScraper()
{
}

HRESULT CScraper::Init_Scene()
{
	Get_TileArray(L"Scraper_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoScraper.dat", m_vecTileArray.x, m_vecTileArray.y);
	CObjectMgr::Get_Instance()->Add_Object(CSerpant::Create_Serpant(), CObjectMgr::SCRAPER);
	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Scraper.wav");
	return S_OK;
}

void CScraper::Update_Scene()
{
	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::SCRAPER);
	CScrollMgr::Get_Instance()->Set_ScrollLock(1600, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();
}

void CScraper::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::SCRAPER);
}

void CScraper::Render_Scene()
{
	Render_Scene_Key(L"Scraper_Background", 1.2f, 1.2f);
	Render_Scene_Key(L"Scraper_Tile", 1.f, 1.f);
	//CTileMgr::Get_Instance()->Render_TileMgr();
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::SCRAPER);
	CObjectMgr::Get_Instance()->Render_Player();
}

void CScraper::Release_Scene()
{
}

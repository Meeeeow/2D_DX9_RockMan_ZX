#include "stdafx.h"
#include "AreaD2.h"
#include "SeedHelmet.h"
#include "SeedAcrobat.h"
#include "SeedRoid.h"
#include "ObjectBase.h"
#include "Player.h"

CScene * CAreaD2::Create_AreaD2()
{
	CScene* pScene = new CAreaD2;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);
	return pScene;
}

CAreaD2::CAreaD2()
{
}


CAreaD2::~CAreaD2()
{
}

HRESULT CAreaD2::Init_Scene()
{
	Get_TileArray(L"AreaD2_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoAreaD2.dat", m_vecTileArray.x, m_vecTileArray.y);
	if (CObjectMgr::Get_Instance()->Get_Type(CObjectMgr::AREA_D2).empty())
	{
		CObjectMgr::Get_Instance()->Add_Object(CSeedHelmet::Create_Helmet(650.f, 400.f), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedHelmet::Create_Helmet(1630.f, 350.f), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedAcrobat::Create_Acrobat(9000.f, 300.f), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(7700.f, 400), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(7150, 350), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(6620, 400), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedAcrobat::Create_Acrobat(5872.f, 300.f), CObjectMgr::AREA_D2);
		CObjectMgr::Get_Instance()->Add_Object(CSeedAcrobat::Create_Acrobat(6000.f, 300.f), CObjectMgr::AREA_D2);


	}
	rc.left = 8868;
	rc.right = 9020;
	rc.top = 447;
	rc.bottom = 553;
	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_AreaD2.wav");
	return S_OK;
}

void CAreaD2::Update_Scene()
{
	RECT temp = {};
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
		if (IntersectRect(&temp, &rc, &iter->Get_Rect()) && CKeyMgr::Get_Instance()->Key_Down(VK_UP))
		{
			static_cast<CPlayer*>(iter)->Scene_Change_True();
			static_cast<CPlayer*>(iter)->Player_CheckLine();
			iter->Pos_SettingYPlus(400.f, 300.f);
			CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::FISTLEO_STAGE);
		}

	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::AREA_D2);
	CScrollMgr::Get_Instance()->Set_ScrollLock(10000, 0);
	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();
}

void CAreaD2::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::AREA_D2);
}

void CAreaD2::Render_Scene()
{
	Render_Scene_Key(L"AreaD2_Tile",1.f,1.f);
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::AREA_D2);
	CObjectMgr::Get_Instance()->Render_Player();
}

void CAreaD2::Release_Scene()
{
}

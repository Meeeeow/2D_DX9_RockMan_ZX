#include "stdafx.h"
#include "TrainRoom.h"
#include "Player.h"
#include "SeedAcrobat.h"
#include "SeedRoid.h"
#include "SeedHelmet.h"
#include "GravityBall.h"

CScene * CTrainRoom::Create_Train_Room()
{
	CScene* pScene = new CTrainRoom;
	if (FAILED(pScene->Init_Scene()))
		SAFE_DELETE(pScene);

	return pScene;
}

CTrainRoom::CTrainRoom()
{
}


CTrainRoom::~CTrainRoom()
{
}

HRESULT CTrainRoom::Init_Scene()
{
	
	Get_TileArray(L"TrainRoom_Tile");
	CTileMgr::Get_Instance()->Add_Tile_Path(L"../Data/TileInfoTrainRoom.dat", m_vecTileArray.x, m_vecTileArray.y);

	if (CObjectMgr::Get_Instance()->Get_Player().empty())
	{
		UNITINFO tempUnit;
		ZeroMemory(&tempUnit, sizeof(UNITINFO));
		tempUnit.vPos = { 300.f, 400.f };
		tempUnit.vSize = { 0.f , 0.f };
		tempUnit.iHP = 6;
		tempUnit.iCurHP = 5;
		tempUnit.iMP = 6;
		tempUnit.iCurMP = 5;
		tempUnit.iDMG = 1;
		tempUnit.iDEF = 1;
		tempUnit.fSpeed = 50.f;
		tempUnit.fAngle = 0.f;
		tempUnit.bPlayer = true;
		CObjectMgr::Get_Instance()->Add_Object(CPlayer::Create_Player(tempUnit), CObjectMgr::PLAYER);
	}
	CEffectMgr::Get_Instance()->Intro_Render_Start();
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_TrainRoom.wav");
	return S_OK;
}

void CTrainRoom::Update_Scene()
{
	if (CKeyMgr::Get_Instance()->Key_Down('2'))
	{
		CObjectMgr::Get_Instance()->Add_Object(CSeedAcrobat::Create_Acrobat(700.f, 450.f), CObjectMgr::TRAININGROOM);
	}
	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		CObjectMgr::Get_Instance()->Add_Object(CSeedRoid::Create_Roid(700.f, 300.f), CObjectMgr::TRAININGROOM);
	}
	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		CObjectMgr::Get_Instance()->Add_Object(CSeedHelmet::Create_Helmet(250.f, 300.f), CObjectMgr::TRAININGROOM);
	}

	CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Update_Type(CObjectMgr::TRAININGROOM);

	CScrollMgr::Get_Instance()->Set_ScrollLock(800, 0);
	if(!CObjectMgr::Get_Instance()->Using_SpecialAttack())
		CScrollMgr::Get_Instance()->Set_Quake();
}

void CTrainRoom::Late_Update_Scene()
{
	CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::PLAYER);
	if (CObjectMgr::Get_Instance()->Get_Can_Play())
		CObjectMgr::Get_Instance()->Late_Update_Type(CObjectMgr::TRAININGROOM);
}

void CTrainRoom::Render_Scene()
{
	Render_Scene_Key(L"TrainRoom_Background", 1.2f, 1.2f);
	Render_Scene_Key(L"TrainRoom_Tile",1.f,1.f);
	//CTileMgr::Get_Instance()->Render_TileMgr();
	CObjectMgr::Get_Instance()->Render_Type(CObjectMgr::TRAININGROOM);
	CObjectMgr::Get_Instance()->Render_Player();
}

void CTrainRoom::Release_Scene()
{
}

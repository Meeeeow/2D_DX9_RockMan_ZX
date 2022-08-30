#include "stdafx.h"
#include "SceneMgr.h"
#include "SceneLoading.h"
#include "TrainRoom.h"
#include "AreaE1.h"
#include "AreaD2.h"
#include "AreaD5.h"
#include "Scraper.h"
#include "BossOX.h"
#include "FistLeo_Stage.h"

HRESULT CSceneMgr::Scene_Change(ID _eID)
{
	m_ePreScene = _eID;
	if (m_eCurScene != m_ePreScene)
	{
		SAFE_DELETE(m_pScene);
		switch (m_ePreScene)
		{
		case CSceneMgr::LOADING:
			m_pScene = CSceneLoading::Create_Scene();
			break;
		case CSceneMgr::TRAINROOM:
			m_pScene = CTrainRoom::Create_Train_Room();
			break;
		case CSceneMgr::AREA_E1:
			m_pScene = CAreaE1::Create_AreaE1();
			break;
		case CSceneMgr::AREA_D2:
			m_pScene = CAreaD2::Create_AreaD2();
			break;
		case CSceneMgr::FISTLEO_STAGE:
			m_pScene = CFistLeo_Stage::Create_Magma();
			break;
		case CSceneMgr::AREA_D5:
			m_pScene = CAreaD5::Create_AreaD5();
			break;
		case CSceneMgr::SCRAPER:
			m_pScene = CScraper::Create_Scraper();
			break;
		case CSceneMgr::BOSS:
			m_pScene = CBossOX::Create_Boss_OX();
			break;
		case CSceneMgr::END:
			break;
		default:
			break;
		}
		m_eCurScene = m_ePreScene;
		CEffectMgr::Get_Instance()->Release_Effect_NoPaper();
	}
	return S_OK;
}

void CSceneMgr::Update_SceneMgr()
{
	m_pScene->Update_Scene();
}

void CSceneMgr::Late_Update_SceneMgr()
{
	m_pScene->Late_Update_Scene();
}

void CSceneMgr::Render_SceneMgr()
{
	m_pScene->Render_Scene();
}

void CSceneMgr::Release_SceneMgr()
{
	SAFE_DELETE(m_pScene);
}

_vec2 CSceneMgr::Get_TileArray()
{
	return m_pScene->Get_Vector();
}

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr)
	, m_eCurScene(END)
	, m_ePreScene(END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release_SceneMgr();
}

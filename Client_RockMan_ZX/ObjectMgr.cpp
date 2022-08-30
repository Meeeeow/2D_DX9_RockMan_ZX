#include "stdafx.h"
#include "ObjectMgr.h"
#include "Player.h"


void CObjectMgr::Load_ObjectData(ID _eID, wstring pFilePath)
{
}

void CObjectMgr::Update_ObjectMgr()
{
	for (auto i = 0; i < END; ++i)
	{
		auto iter = m_ListObject[i].begin();
		for (; iter != m_ListObject[i].end() ;)
		{
			int iEvent = (*iter)->Update_Object();
			if (OBJ_DEAD == iEvent)
			{
				SAFE_DELETE(*iter);
				iter = m_ListObject[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjectMgr::Update_Type(ID _eID)
{
	auto iter = m_ListObject[_eID].begin();
	for (; iter != m_ListObject[_eID].end();)
	{
		int iEvent = (*iter)->Update_Object();
		if (OBJ_DEAD == iEvent)
		{
			SAFE_DELETE(*iter);
			iter = m_ListObject[_eID].erase(iter);
		}
		else
			++iter;
	}
}

void CObjectMgr::Late_Update_ObjectMgr()
{
	for (int i = 0; i < END; ++i)
		for (auto& pObj : m_ListObject[i])
			pObj->Late_Update_Object();
}

void CObjectMgr::Late_Update_Type(ID _eID)
{
	for (auto& pObj : m_ListObject[_eID])
		pObj->Late_Update_Object();
}

void CObjectMgr::Render_ObjectMgr()
{
	for (int i = 0; i < END; ++i)
		for (auto& pObj : m_ListObject[i])
			pObj->Render_Object();
}

void CObjectMgr::Render_Player()
{
	for (auto& pObj : m_ListObject[PLAYER])
		pObj->Render_Object();
}

void CObjectMgr::Render_Type(ID _eID)
{
	for (auto& pObj : m_ListObject[_eID])
		pObj->Render_Object();
}

void CObjectMgr::Release_ObjectMgr()
{
	for (int i = 0; i < END; ++i)
	{
		for_each(m_ListObject[i].begin(), m_ListObject[i].end(), Safe_Delete<CObjectBase*>);
		m_ListObject[i].clear();
	}
}

bool CObjectMgr::Using_SpecialAttack()
{
	if (m_ListObject[PLAYER].empty())
		return false;
	auto pPlayer = m_ListObject[PLAYER].front();
	return static_cast<CPlayer*>(pPlayer)->Get_SpecialAttack();
}

bool CObjectMgr::Using_AquaField()
{
	if (m_ListObject[PLAYER].empty())
		return false;
	auto pPlayer = m_ListObject[PLAYER].front();
	return static_cast<CPlayer*>(pPlayer)->Get_Aqua();
}

CObjectMgr::ID CObjectMgr::Get_Current_ID()
{
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::TRAINROOM)
		return CObjectMgr::TRAININGROOM;
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::AREA_E1)
		return CObjectMgr::AREA_E1;
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::AREA_D2)
		return CObjectMgr::AREA_D2;
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::AREA_D5)
		return CObjectMgr::AREA_D5;
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::SCRAPER)
		return CObjectMgr::SCRAPER;
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::FISTLEO_STAGE)
		return CObjectMgr::MAGMA;
	if (CSceneMgr::Get_Instance()->Get_Scene() == CSceneMgr::BOSS)
		return CObjectMgr::BOSS;
}

void CObjectMgr::Hit_Check(ID _eID, int iDmg, RECT rc)
{
	for (auto iter : m_ListObject[_eID])
		iter->Default_Hit(iDmg, rc);
}

void CObjectMgr::Hurricane_Check(ID _eID, int iDmg, RECT rc,float fY)
{
	for (auto iter : m_ListObject[_eID])
		iter->Hurricane_Hit(fY,iDmg, rc);
}

void CObjectMgr::FX_Attack(ID _eID, int iDmg, RECT rc,float fX)
{
	for (auto iter : m_ListObject[_eID])
		iter->FX_Hit(fX,iDmg, rc);
}

bool CObjectMgr::Get_Can_Play()
{
	if (m_ListObject[PLAYER].empty())
		return true;
	auto pPlayer = m_ListObject[PLAYER].front();
	return static_cast<CPlayer*>(pPlayer)->Get_CanPlay();
}

UNITINFO CObjectMgr::Get_PlayerInfo()
{
	auto pPlayer = m_ListObject[PLAYER].front();
	return pPlayer->Get_Info();
}

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release_ObjectMgr();
}

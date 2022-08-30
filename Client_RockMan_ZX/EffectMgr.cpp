#include "stdafx.h"
#include "EffectMgr.h"
#include "Effect.h"
#include "OXIntro.h"
#include "LeoWall.h"

CEffectMgr::CEffectMgr()
{
}


CEffectMgr::~CEffectMgr()
{
	Release_EffectMgr();
}

void CEffectMgr::Update_EffectMgr()
{
	for (auto i = 0; i < END; ++i)
	{
		auto iter = m_ListEffect[i].begin();
		for (; iter != m_ListEffect[i].end();)
		{
			int iEvent = (*iter)->Update_Effect();
			if (OBJ_DEAD == iEvent)
			{
				SAFE_DELETE(*iter);
				iter = m_ListEffect[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CEffectMgr::Late_Update_EffectMgr()
{
	for (int i = 0; i < END; ++i)
		for (auto iter : m_ListEffect[i])
			iter->Late_Update_Effect();
}

void CEffectMgr::Render_EffectMgr()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto iter : m_ListEffect[i])
			iter->Render_Effect();
	}
}

void CEffectMgr::Release_EffectMgr()
{
	for (int i = 0; i < END; ++i)
	{
		for_each(m_ListEffect[i].begin(), m_ListEffect[i].end(), Safe_Delete<CEffect*>);
		m_ListEffect[i].clear();
	}
}

void CEffectMgr::Release_Effect_NoPaper()
{
	for (int i = 0; i < AQUAFIELD; ++i)
	{
		for_each(m_ListEffect[i].begin(), m_ListEffect[i].end(), Safe_Delete<CEffect*>);
		m_ListEffect[i].clear();
	}
}
void CEffectMgr::Intro_Render_Start()
{
	for (auto iter : m_ListEffect[INTRO])
		static_cast<COXIntro*>(iter)->Render_True();
}

void CEffectMgr::Wall_Move_Start()
{
	for (auto iter : m_ListEffect[WALL])
		static_cast<CLeoWall*>(iter)->Move_Start();
}

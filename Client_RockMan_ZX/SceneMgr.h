#pragma once
#ifndef __SCENE_MGR_H__
#define __SCENE_MGR_H__

#include "Singleton.h"
class CScene;
class CSceneMgr :
	public CSingleton<CSceneMgr>
{
public:
	enum ID { LOADING, TRAINROOM ,AREA_E1, AREA_D2, FISTLEO_STAGE, AREA_D5, SCRAPER, BOSS, END};
public:
	HRESULT Scene_Change(ID _eID);
	void	Update_SceneMgr();
	void	Late_Update_SceneMgr();
	void	Render_SceneMgr();
	void	Release_SceneMgr();
	_vec2	Get_TileArray();
	ID		Get_Scene() { return m_eCurScene; }
private:
	CScene*	m_pScene;
	ID		m_eCurScene;
	ID		m_ePreScene;
public:
	CSceneMgr();
	virtual ~CSceneMgr();
};


#endif // !__SCENE_MGR_H__

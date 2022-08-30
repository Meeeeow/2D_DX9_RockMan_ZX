#pragma once
#ifndef __EFFECT_MGR_H__
#define __EFFECT_MGR_H__

#include "Singleton.h"
class CEffect;
class CEffectMgr :
	public CSingleton<CEffectMgr>
{
public:
	enum ID { 
		/*Monster */DESTROY, BULLET, 
		/*Hivok   */HIVOK_GUARD, HIVOK_LASER,
		/*Fist Leo*/WALL,
		/*Serpant */GONG, GRAVITYBALL, WAVE, SERPANT_LASER,
		/*Copy   X*/X_SHOT, X_DIAGSHOT, X_ELECT, X_FIRE, X_LASER,
		/*Model ZX*/ZX_TEMP,
		/*Model HX*/HX_BALL,
		/*Model FX*/FX_FIREWALL,
		/*Model LX*/AQUAFIELD,
		/*Model PX*/KUNAI, SHURIKEN , CLOAK,
		/*Model OX*/INTRO, SLASH ,END};
public:
	CEffectMgr();
	virtual ~CEffectMgr();

public:
	void		Insert_Effect(CEffect* pEffect, CEffectMgr::ID _eID) { m_ListEffect[_eID].emplace_back(pEffect); }
	void		Update_EffectMgr();
	void		Late_Update_EffectMgr();
	void		Render_EffectMgr();
	void		Release_EffectMgr();
	void		Release_Effect_NoPaper();
	void		Intro_Render_Start();
	void		Wall_Move_Start();
private:
	list<CEffect*> m_ListEffect[END];
};


#endif // !__EFFECT_MGR_H__

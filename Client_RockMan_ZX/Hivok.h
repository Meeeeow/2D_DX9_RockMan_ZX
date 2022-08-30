#pragma once
#ifndef __HIVOK_H__
#define __HIVOK_H__

#include "ObjectBase.h"
class CHivok :
	public CObjectBase
{
public:
	enum HivokMotion { INTRO, IDLE, DASH, LANDATTACK, SUMMON, DEAD, END};
public:
	static CObjectBase* Create_Hivok();
private:
	CHivok();
public:
	virtual ~CHivok();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;
public:
	void Hit_Recovery();
	void Hivok_Collision();
	void Hivok_Dash();
	void Hivok_LandAttack();
	void Hivok_Summon();
	void Frame_Check();
	void Frame_Update();
	void HitBox_Update();
	void Action();
private:
	bool	 m_bCanHit;
	bool	 m_bHit;
	int		 m_iHitRecovery;
	int		 m_iAlpha;
	int		 m_iDirection;
	bool	 m_bIdle;
	bool	 m_bDash;
	int		 m_iDashCount;
	bool	 m_bLandAttack;
	int		 m_iLandAttackCount;
	bool	 m_bSummon;
	int		 m_iSummonCount;
	bool	 m_bDeadMotion;
	bool	 m_bMotion;
	bool	 m_bAction;
	bool	 m_bIntro;
	_vec2	 m_vTarget;

	HivokMotion m_eCurMotion;
	HivokMotion m_ePreMotion;
};


#endif // !__HIVOK_H__

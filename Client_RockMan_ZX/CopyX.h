#pragma once
#ifndef __COPY_X_H__
#define __COPY_X_H__

#include "ObjectBase.h"
class CCopyX :
	public CObjectBase
{
public:
	enum CopyXMotion {
		SIT, INTRO, IDLE, SHOT, OVERSHOT, JUMPSHOT
		, TACKLE, JUMPDASH, DOOM, HOVER,FALL,DEAD, END };
public:
	static CObjectBase* Create_X();
private:
	CCopyX();
public:
	virtual ~CCopyX();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;

	void Hit_Recovery();
	void CopyX_Collision();
	void CopyX_Shot();
	void CopyX_JumpShot();
	void CopyX_OverShot();
	void CopyX_Tackle();
	void CopyX_JumpDash();
	void CopyX_Doom();
	void CopyX_Hover();
	
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
	bool	 m_bAction;
	bool	 m_bMotion;
	bool	 m_bSit;
	int		 m_iSitCount;
	bool	 m_bIntro;

	bool	 m_bShot;
	int		 m_iShotCount;
	bool	 m_bJumpShot;
	int		 m_iJumpShotCount;
	bool	 m_bOverShot;
	int		 m_iOverShotCount;
	bool	 m_bTackle;
	int		 m_iTackleCount;
	bool	 m_bJumpDash;
	int		 m_iJumpDashCount;
	bool	 m_bDoom;
	int		 m_iDoomCount;
	int		 m_iDoomMax;
	bool	 m_bHover;
	int		 m_iHoverCount;
	int		 m_iHoverMax;
	bool	 m_bFall;
	int		 m_bDeadMotion;
	_vec2	 m_vTarget;

	CopyXMotion m_eCurMotion;
	CopyXMotion m_ePreMotion;
	int		 m_i;

};


#endif // !__COPY_X_H__

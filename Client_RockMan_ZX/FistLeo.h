#pragma once
#ifndef __FISTLEO_H__
#define __FISTLEO_H__

#include "ObjectBase.h"
class CFistLeo :
	public CObjectBase
{
public:
	enum FistLeoMotion { INTRO, IDLE, ATTACK, DASHATTACK, JUMPATTACk, WALLATTACK, FORGE, FINALATTACK, END};
public:
	static CObjectBase* Create_Leo();
private:
	CFistLeo();
public:
	virtual ~CFistLeo();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;

	void Hit_Recovery();
	void Leo_Collision();
	void Leo_DashAttack();
	void Leo_Attack();
	void Leo_JumpAttack();
	void Leo_WallAttack();
	void Leo_FinalAttack();
	void Leo_Forge();
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
	bool	 m_bAttack;
	int		 m_iAttackCount;
	bool	 m_bJumpAttack;
	int		 m_iJumpCount;
	bool	 m_bForge;
	bool	 m_bWallAttack;
	int		 m_iWallCount;
	bool	 m_bFinalAttack;
	int		 m_iFinalCount;
	bool	 m_bMotion;
	bool	 m_bAction;
	_vec2	 m_vTarget;

	FistLeoMotion m_eCurMotion;
	FistLeoMotion m_ePreMotion;

};


#endif // !__FISTLEO_H__

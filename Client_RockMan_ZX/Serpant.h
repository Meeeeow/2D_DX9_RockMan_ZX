#pragma once
#ifndef __SERPANT_H__
#define __SERPANT_H__

#include "ObjectBase.h"
class CSerpant :
	public CObjectBase
{
public:
	enum SerpantMotion { INTRO, IDLE, BLINKATTACK, UPPERCUT, SLIDE, FIRE, JUMPATTACK, END};
public:
	static CObjectBase* Create_Serpant();
private:
	CSerpant();
public:
	virtual ~CSerpant();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;

	void Hit_Recovery();
	void Serpant_Collision();
	void Serpant_Slide();
	void Serpant_BlinkAttack();
	void Serpant_JumpAttack();
	void Serpant_Fire();
	void Serpant_Uppercut();
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
	bool	 m_bBlink;
	int		 m_iBlinkCount;
	bool	 m_bUpperCut;
	int		 m_iUpperCount;
	bool	 m_bSlide;
	int		 m_iSlideCount;
	bool	 m_bFire;
	int		 m_iFireCount;
	bool	 m_bJump;
	int		 m_iJumpCount;
	bool	 m_bAction;
	bool	 m_bMotion;
	_vec2	 m_vTarget;
	int		 m_iCount;
	int		 m_iTime;
	SerpantMotion m_eCurMotion;
	SerpantMotion m_ePreMotion;
};


#endif // !__SERPANT_H__

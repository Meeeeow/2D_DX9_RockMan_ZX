#pragma once
#ifndef __ROID_H__
#define __ROID_H__


#include "ObjectBase.h"
class CSeedRoid :
	public CObjectBase
{
public:
	enum RoidMotion { IDLE, WALK, FIRE, END };
public:
	static CObjectBase* Create_Roid(float fX, float fY);
private:
	CSeedRoid();
public:
	virtual ~CSeedRoid();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;
	
	void Hit_Recovery();
	void Roid_Collision();
	void Roid_Fire();
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
	int 	 m_iRenderX;
	bool	 m_bIdle;
	bool	 m_bWalk;
	int      m_iCoolWalk;
	bool	 m_bFire;
	int		 m_iCoolFire;
	bool	 m_bMotion;
	float	 m_fFall;

	RoidMotion m_eCurMotion;
	RoidMotion m_ePreMotion;
};


#endif // !__ROID_H__

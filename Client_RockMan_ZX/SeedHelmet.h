#pragma once
#include "ObjectBase.h"
class CSeedHelmet :
	public CObjectBase
{
public:
	enum HelmetMotion { IDLE, WAKE, FIRE, END};
public:
	static CObjectBase* Create_Helmet(float fX, float fY);
private:
	CSeedHelmet();
public:
	virtual ~CSeedHelmet();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;

	void Hit_Recovery();
	void Helmet_Collision();
	void Helmet_Fire();
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
	bool	 m_bWake;
	int      m_iCoolWake;
	bool	 m_bFire;
	int		 m_iCoolFire;
	bool	 m_bMotion;
	float	 m_fFall;
	HelmetMotion m_eCurMotion;
	HelmetMotion m_ePreMotion;
};


#pragma once
#ifndef __ACROBAT_H__
#define __ACROBAT_H__

#include "ObjectBase.h"


class CSeedAcrobat :
	public CObjectBase
{
public:
	static CObjectBase* Create_Acrobat(float fX, float fY);
private:
	CSeedAcrobat();
public:
	virtual ~CSeedAcrobat();

	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Default_Hit(int iDmg, RECT HitBox) override;
	void HitBox_Update();
	void Attack_Acrobat();
	void Hit_Recovery();
	void Frame_Update();

private:
	bool	 m_bCanHit;
	bool	 m_bHit;
	int		 m_iHitRecovery;
	int		 m_iAlpha;
	int		 m_iDirection;
	int 	 m_iRenderX;
	_vec2	 m_vTarget;
};


#endif // !__ACROBAT_H__

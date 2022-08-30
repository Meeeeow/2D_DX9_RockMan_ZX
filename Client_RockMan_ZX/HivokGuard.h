#pragma once
#ifndef __HIVOK_GUARD_H__
#define __HIVOK_GUARD_H__

#include "Effect.h"
class CHivokGuard :
	public CEffect
{
public:
	static CEffect* Create_Guard(float fX, float fY,CObjectBase* pObj);
	void Setting_Pos(float fX, float fY,CObjectBase* pObj);
private:
	CHivokGuard();
public:
	virtual ~CHivokGuard();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

private:
	float m_fAngle;
	float m_fRotAngle;
	int   m_iShot;
	_vec2 m_vTarget;
	_vec2 m_vLook;
	_vec2 Real_Pos;
	CObjectBase* m_pParent;
};

#endif // !__HIVOK_GUARD_H__


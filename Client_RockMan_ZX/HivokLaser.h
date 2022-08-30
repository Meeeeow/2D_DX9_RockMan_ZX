#pragma once
#ifndef __HIVOK_LASER_H__
#define __HIVOK_LASER_H__

#include "Effect.h"
class CHivokLaser :
	public CEffect
{
public:
	static CEffect* Create_Laser(_vec2 Target, float fX, float fY);
	void Setting(_vec2 Target, float fX, float fY);
private:
	CHivokLaser();
public:
	virtual ~CHivokLaser();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;
	void Hit_Check();
private:
	_vec2 m_vTargetPos;
	float m_fAngle;
};


#endif // !__HIVOK_LASER_H__

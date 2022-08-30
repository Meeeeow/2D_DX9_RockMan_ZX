#pragma once
#ifndef __GRAVITY_BALL_H__
#define __GRAVITY_BALL_H__

#include "Effect.h"
class CGravityBall :
	public CEffect
{
public:
	static CEffect* Create_Ball(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:
	CGravityBall();
public:
	virtual ~CGravityBall();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

private:
	_vec2 m_vTarget;
	_vec2 m_vDir;
	int	  m_iCount;
};


#endif // !__GRAVITY_BALL_H__

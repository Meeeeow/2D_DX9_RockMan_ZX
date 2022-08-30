#pragma once
#ifndef __HURRICANE_BALL_H__
#define __HURRICANE_BALL_H__

#include "Effect.h"
class CHurricaneBall :
	public CEffect
{
public:
	static CEffect* Create_Hurricane(float fX, float fY, int iDirection);
	void Setting(float fX, float fY, int iDirection);
private:
	CHurricaneBall();
public:
	virtual ~CHurricaneBall();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

private:
	int		m_iCount;
};


#endif // !__HURRICANE_BALL_H__

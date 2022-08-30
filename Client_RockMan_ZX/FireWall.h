#pragma once
#ifndef __FIREWALL_H__
#define __FIREWALL_H__


#include "Effect.h"
class CFireWall :
	public CEffect
{
public:
	static CEffect* Create_Fire(_vec2 vPos, int iDirection);
	void Setting_Fire(_vec2 vPos, int iDirection);
private:
	CFireWall();
public:
	virtual ~CFireWall();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Hit_Check();

private:
	_vec2 m_vecRect[5];
};


#endif // !__FIREWALL_H__

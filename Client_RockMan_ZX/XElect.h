#pragma once
#ifndef __X_ELECT_H__
#define __X_ELECT_H__

#include "Effect.h"
class CXElect :
	public CEffect
{
public:
	static CEffect* Create_Elect(float fX, float fY, int iDirection, _vec2 TargetPos);
	void Setting(float fX, float fY, int iDirection,_vec2 TargetPos);
private:
	CXElect();
public:
	virtual ~CXElect();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

private:
	_vec2 m_vTargetPos;
	int m_iCount;
};


#endif // !__X_ELECT_H__

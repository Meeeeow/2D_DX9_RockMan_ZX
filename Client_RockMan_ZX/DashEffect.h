#pragma once
#ifndef __DASH_H__
#define __DASH_H__

#include "Effect.h"
class CDashEffect :
	public CEffect
{
public:
	static CEffect* Create_Dash();
private:
	CDashEffect();
public:
	virtual ~CDashEffect();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Render_True() { m_bRender = true; }
	void Render_False() { m_bRender = false; }
	void Start_False() { m_bStart = false; }
	void Set_Pos(_vec2 vPos, int iDirect) { m_tInfo.vecPos = vPos; m_tInfo.iDirection = iDirect; }
private:
	bool m_bRender;
	bool m_bStart;
};


#endif // !__DASH_H__

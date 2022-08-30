#pragma once
#include "Effect.h"
class CCharge :
	public CEffect
{
public:
	static CEffect* Create_Charge();

public:
	CCharge();
	virtual ~CCharge();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Set_Charge(_vec2 vPos, int iDirection,int iPower);

	void Render_True() { m_bRender = true; }
	void Render_False() { m_bRender = false; }
private:
	bool m_bRender;
	int  m_iPower;
};


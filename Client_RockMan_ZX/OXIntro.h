#pragma once
#ifndef __OX_INTRO_H__
#define __OX_INTRO_H__

#include "Effect.h"
class COXIntro :
	public CEffect
{
public:
	static CEffect* Create_Intro();
public:
	COXIntro();
	virtual ~COXIntro();

	// CEffect을(를) 통해 상속됨
	virtual HRESULT Init_Effect() override;
	virtual int Update_Effect() override;
	virtual void Late_Update_Effect() override;
	virtual void Render_Effect() override;
	virtual void HitBox_Update() override;

	void Render_True() { m_bRender = true; }
private:
	bool m_bRender;
	int  m_iAlpha;
};


#endif // !__OX_INTRO_H__

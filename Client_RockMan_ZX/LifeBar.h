#pragma once
#ifndef __LIFE_BAR_H__
#define __LIFE_BAR_H__

#include "UserInterface.h"
class CLifeBar :
	public CUserInterface
{
public:
	CLifeBar();
	virtual ~CLifeBar();

	// CUserInterface을(를) 통해 상속됨
	virtual HRESULT Init_UI() override;
	virtual void Update_UI() override;
	virtual void Late_Update_UI() override;
	virtual void Render_UI() override;
	void Get_Info(UNITINFO _tInfo);
	void Get_Form(int i) { m_iForm = i; }

	void Render_Human();
	void Render_ZX();
	void Render_HX();
	void Render_FX();
	void Render_LX();
	void Render_PX();
	void Render_MP(const wstring& pModel);
	void Render_Life();
	void Render_Magic(const wstring& pModel);
private:
	_vec2	m_vPos;
	int		m_iForm;
	int		m_iMaxHp;
	int		m_iCurHp;
	int		m_iMaxMp;
	int		m_iCurMp;
};


#endif // !__LIFE_BAR_H__

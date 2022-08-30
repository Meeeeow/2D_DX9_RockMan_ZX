#pragma once
#ifndef __BIOMETAL_H__
#define __BIOMETAL_H__

#include "UserInterface.h"
class CBioMetal :
	public CUserInterface
{
public:
	enum ID { HUMAN, MODEL_ZX, MODEL_HX, MODEL_FX, MODEL_LX , MODEL_PX , END};
public:
	CBioMetal();
	virtual ~CBioMetal();

	// CUserInterface을(를) 통해 상속됨
	virtual HRESULT Init_UI() override;
	virtual void Update_UI() override;
	virtual void Late_Update_UI() override;
	virtual void Render_UI() override;

	void Set_Human();
	void Set_ZX();
	void Set_HX();
	void Set_FX();
	void Set_LX();
	void Set_PX();

	void Render_Human();
	void Render_ZX();
	void Render_HX();
	void Render_FX();
	void Render_LX();
	void Render_PX();
private:
	list<UIINFO> m_ListBioMetal[END];

	UIINFO m_tHuman;
	UIINFO m_tZX;
	UIINFO m_tHX;
	UIINFO m_tFX;
	UIINFO m_tLX;
	UIINFO m_tPX;
};


#endif // !__BIOMETAL_H__

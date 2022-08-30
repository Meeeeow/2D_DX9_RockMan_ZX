#pragma once
class CUserInterface abstract
{
public:
	CUserInterface();
	virtual ~CUserInterface();

	virtual HRESULT Init_UI() PURE;
	virtual void Update_UI() PURE;
	virtual void Late_Update_UI() PURE;
	virtual void Render_UI() PURE;

};


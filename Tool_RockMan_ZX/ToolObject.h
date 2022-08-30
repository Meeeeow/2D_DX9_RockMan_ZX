#pragma once
class CToolObject
{
public:
	CToolObject();
	~CToolObject();

	void Init_Object();
	void Set_Info_Object(UNITINFO& _pInfo);
	void Set_Frame_Object(UNITINFO& _pFrame);
	void Update_Object();
	void Render_Object();

private:
	UNITINFO	m_tInfo;
	UNITFRAME	m_tFrame;
};
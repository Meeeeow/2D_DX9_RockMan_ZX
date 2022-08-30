#pragma once
#ifndef __TOOL_SCENE_H__
#define __TOOL_SCENE_H__
class CTool_RockMan_ZXView;
class CToolScene
{
public:
	void Render_Scene();
	void Change_Scene();
	void Init_Scene();
	void Set_View(CTool_RockMan_ZXView* pView) { m_pView = pView; }
	void BackGround_Change(const wstring& pPathFile);
	int Get_X() { return m_iX; }
	int Get_Y() { return m_iY; }
public:
	CToolScene();
	~CToolScene();

private:
	int m_iX;
	int m_iY;
	wstring m_strName;
	CTool_RockMan_ZXView*	m_pView;

};


#endif // !__TOOL_SCENE_H__

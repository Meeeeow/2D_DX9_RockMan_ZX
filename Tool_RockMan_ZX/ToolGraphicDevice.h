#pragma once
#include "ToolSingleton.h"
class CToolGraphicDevice :
	public CToolSingleton<CToolGraphicDevice>
{
public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font() { return m_pFont; }

public:
	HRESULT Init_Graphic();
	void Release_Graphic();
	void Render_Begin();
	void Render_Stop(HWND hWnd = nullptr);


private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;

public:
	CToolGraphicDevice();
	virtual ~CToolGraphicDevice();
};


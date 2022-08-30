#include "stdafx.h"
#include "ToolGraphicDevice.h"


HRESULT CToolGraphicDevice::Init_Graphic()
{
	D3DCAPS9 d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// ��ġ�� ������ �����ϴ� �� ��ü �����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
	{
		MSG_BOX(L"Failed Get Device Caps");
		return E_FAIL;
	}

	DWORD vp = 0;
	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	// �� ���� ����
	d3dpp.BackBufferWidth = WINCX;
	// �� ���� ����
	d3dpp.BackBufferHeight = WINCY;
	// �� ���� ���� - Alpha 8bit, Red 8bit, Green 8bit, Blue 8bit
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	// �� ���� ���� - �⺻ 1�� ����  =1 �� +1
	d3dpp.BackBufferCount = 0;

	// ��Ƽ ���ø� Ÿ��
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	// ��Ƽ ���ø� ��� �����̴� ����Ƽ 0
	d3dpp.MultiSampleQuality = 0;

	// ���� ü�� ���
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	d3dpp.Windowed = TRUE;

	// ���ٽ� ��� (�ſ￡ ���߱�)
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		MSG_BOX(L"Failed Create Device");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX(L"Failed Create Sprite");
		return E_FAIL;
	}


	D3DXFONT_DESCW font;
	ZeroMemory(&font, sizeof(D3DXFONT_DESCW));
	font.Height = 15;
	font.Width = 15;
	font.Weight = FW_HEAVY;
	font.CharSet = HANGUL_CHARSET;
	lstrcpy(font.FaceName, L"���� ���");

	if (FAILED(D3DXCreateFontIndirectW(m_pDevice, &font, &m_pFont)))
	{
		MSG_BOX(L"Failed Create Font");
		return E_FAIL;
	}

	return S_OK;
}

void CToolGraphicDevice::Release_Graphic()
{
	if (m_pFont)
		m_pFont->Release();
	if (m_pSprite)
		m_pSprite->Release();
	if (m_pDevice)
		m_pDevice->Release();
	if (m_pSDK)
		m_pSDK->Release();
}

void CToolGraphicDevice::Render_Begin()
{
	CToolGraphicDevice::Get_Instance()->Get_Device()->Clear(0, nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 150, 150, 150),
		1.f, 0);
	CToolGraphicDevice::Get_Instance()->Get_Device()->BeginScene();
	CToolGraphicDevice::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CToolGraphicDevice::Render_Stop(HWND hWnd)
{
	CToolGraphicDevice::Get_Instance()->Get_Sprite()->End();
	CToolGraphicDevice::Get_Instance()->Get_Device()->EndScene();
	CToolGraphicDevice::Get_Instance()->Get_Device()->Present(nullptr, nullptr, hWnd, nullptr);
}

CToolGraphicDevice::CToolGraphicDevice()
{
}


CToolGraphicDevice::~CToolGraphicDevice()
{
	Release_Graphic();
}

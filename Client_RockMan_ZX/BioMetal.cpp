#include "stdafx.h"
#include "BioMetal.h"
#include "Player.h"

CBioMetal::CBioMetal()
{
}


CBioMetal::~CBioMetal()
{
}

HRESULT CBioMetal::Init_UI()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tHuman.bBehind = false;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tZX.bBehind = false;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tHX.bBehind = false;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tFX.bBehind = false;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tLX.bBehind = false;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tPX.bBehind = false;
	}
	return S_OK;
}

void CBioMetal::Update_UI()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 0)
			Set_Human();
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 1)
			Set_ZX();
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 2)
			Set_HX();
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 3)
			Set_FX();
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 4)
			Set_LX();
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 5)
			Set_PX();
	}
}

void CBioMetal::Late_Update_UI()
{
}

void CBioMetal::Render_UI()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 0)
		{
			// 맨 뒤
			Render_FX();
			// 좌 상
			Render_LX();
			// 우 상
			Render_HX();
			// 좌 하
			Render_PX();
			// 우 하
			Render_ZX();
			// 맨 앞
			Render_Human();
		}
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 1)
		{
			// 맨 뒤
			Render_LX();
			// 좌 상
			Render_PX();
			// 우 상
			Render_FX();
			// 좌 하
			Render_Human();
			// 우 하
			Render_HX();
			// 맨 앞
			Render_ZX();
		}
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 2)
		{
			// 맨 뒤
			Render_PX();
			// 좌 상
			Render_Human();
			// 우 상
			Render_LX();
			// 좌 하
			Render_ZX();
			// 우 하
			Render_FX();
			// 맨 앞
			Render_HX();
		}
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 3)
		{
			// 맨 뒤
			Render_Human();
			// 좌 상
			Render_ZX();
			// 우 상
			Render_PX();
			// 좌 하
			Render_HX();
			// 우 하
			Render_LX();
			// 맨 앞
			Render_FX();
		}
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 4)
		{
			// 맨 뒤
			Render_ZX();
			// 좌 상
			Render_HX();
			// 우 상
			Render_Human();
			// 좌 하
			Render_FX();
			// 우 하
			Render_PX();
			// 맨 앞
			Render_LX();
		}
		else if (static_cast<CPlayer*>(pPlayer)->Get_FormNumber() == 5)
		{
			// 맨 뒤
			Render_HX();
			// 좌 상
			Render_FX();
			// 우 상
			Render_ZX();
			// 좌 하
			Render_LX();
			// 우 하
			Render_Human();
			// 맨 앞
			Render_PX();
		}
	}
}

void CBioMetal::Set_Human()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tHuman.bBehind = false;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x + 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tZX.bBehind = false;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x + 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tHX.bBehind = false;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y - 15 };
		m_tFX.bBehind = true;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x - 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tLX.bBehind = false;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x - 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tPX.bBehind = false;
	}
}

void CBioMetal::Set_ZX()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x - 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tHuman.bBehind = false;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tZX.bBehind = false;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x + 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tHX.bBehind = false;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x + 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tFX.bBehind = false;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y - 15 };
		m_tLX.bBehind = true;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x - 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tPX.bBehind = false;
	}
}

void CBioMetal::Set_HX()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x - 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tHuman.bBehind = false;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x - 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tZX.bBehind = false;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tHX.bBehind = false;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x + 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tFX.bBehind = false;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x + 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tLX.bBehind = false;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y - 15 };
		m_tPX.bBehind = true;
	}
}

void CBioMetal::Set_FX()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y - 15 };
		m_tHuman.bBehind = true;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x - 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tZX.bBehind = false;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x - 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tHX.bBehind = false;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tFX.bBehind = false;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x + 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tLX.bBehind = false;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x + 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tPX.bBehind = false;
	}
}

void CBioMetal::Set_LX()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x + 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tHuman.bBehind = false;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y - 15 };
		m_tZX.bBehind = true;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x - 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tHX.bBehind = false;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x - 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tFX.bBehind = false;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tLX.bBehind = false;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x + 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tPX.bBehind = false;
	}
}

void CBioMetal::Set_PX()
{
	for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
	{
		m_tHuman.vecPos = { pPlayer->Get_Info().vPos.x + 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tHuman.bBehind = false;
		m_tZX.vecPos = { pPlayer->Get_Info().vPos.x + 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tZX.bBehind = false;
		m_tHX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y - 15 };
		m_tHX.bBehind = true;
		m_tFX.vecPos = { pPlayer->Get_Info().vPos.x - 30, pPlayer->Get_Info().vPos.y - 10 };
		m_tFX.bBehind = false;
		m_tLX.vecPos = { pPlayer->Get_Info().vPos.x - 20, pPlayer->Get_Info().vPos.y + 5 };
		m_tLX.bBehind = false;
		m_tPX.vecPos = { pPlayer->Get_Info().vPos.x , pPlayer->Get_Info().vPos.y + 10 };
		m_tPX.bBehind = false;
	}
}

void CBioMetal::Render_Human()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BIOMETAL_Human");
	if (pTexInfo == nullptr)
		return;
	if (m_tHuman.bBehind)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tHuman.vecPos.x +vScroll.x, m_tHuman.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBioMetal::Render_ZX()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BIOMETAL_ZX");
	if (pTexInfo == nullptr)
		return;
	if (m_tZX.bBehind)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tZX.vecPos.x + vScroll.x, m_tZX.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBioMetal::Render_HX()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BIOMETAL_HX");
	if (pTexInfo == nullptr)
		return;
	if (m_tHX.bBehind)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tHX.vecPos.x + vScroll.x, m_tHX.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBioMetal::Render_FX()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BIOMETAL_FX");
	if (pTexInfo == nullptr)
		return;
	if (m_tFX.bBehind)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tFX.vecPos.x + vScroll.x, m_tFX.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBioMetal::Render_LX()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BIOMETAL_LX");
	if (pTexInfo == nullptr)
		return;
	if (m_tLX.bBehind)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tLX.vecPos.x + vScroll.x, m_tLX.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBioMetal::Render_PX()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BIOMETAL_PX");
	if (pTexInfo == nullptr)
		return;
	if (m_tPX.bBehind)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tPX.vecPos.x + vScroll.x, m_tPX.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

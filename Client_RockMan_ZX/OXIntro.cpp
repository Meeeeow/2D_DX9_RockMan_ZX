#include "stdafx.h"
#include "OXIntro.h"


CEffect * COXIntro::Create_Intro()
{
	CEffect* pEffect = new COXIntro;
	pEffect->Init_Effect();
	return pEffect;
}

COXIntro::COXIntro()
{
}


COXIntro::~COXIntro()
{
}

HRESULT COXIntro::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"OXIntro";
	m_tInfo.dwDelay = 0;
	m_tInfo.dwTime = 0;
	m_tInfo.tRect = {};
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 0;
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey);
	m_tInfo.vecPos = { float(pTexInfo->tImageInfo.Width >> 1) ,	float(pTexInfo->tImageInfo.Height >> 1) };
	m_iAlpha = 255.f;
	m_bRender = false;
	return S_OK;
}

int COXIntro::Update_Effect()
{
	if (!m_bRender)
		m_iAlpha = 255;
	if (m_bRender)
		m_iAlpha -= 2;

	if (m_iAlpha < 0)
		m_bRender = false;
	return OBJ_NOEVENT;
}

void COXIntro::Late_Update_Effect()
{
}

void COXIntro::Render_Effect()
{
	if (m_bRender)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey);

		if (pTexInfo == nullptr)
			return;

		_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

		_matrix matScale, matTrans;

		D3DXMatrixScaling(&matScale, 1, 1, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x, m_tInfo.vecPos.y, 0.f);
		matScale *= matTrans;

		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, &_vec3(m_tInfo.vecPos.x, m_tInfo.vecPos.y, 0), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
}

void COXIntro::HitBox_Update()
{
}

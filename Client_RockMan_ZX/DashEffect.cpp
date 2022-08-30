#include "stdafx.h"
#include "DashEffect.h"


CEffect * CDashEffect::Create_Dash()
{
	CEffect* pEffect = new CDashEffect;
	pEffect->Init_Effect();
	return pEffect;
}

CDashEffect::CDashEffect()
{
}


CDashEffect::~CDashEffect()
{
}

HRESULT CDashEffect::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Dash";
	m_tInfo.iDirection = 0;
	m_tInfo.fSpeed = 0;
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 4;
	m_tInfo.dwDelay = 100;
	m_tInfo.vecPos = {};
	m_tInfo.tRect = {};
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.bDead = false;
	m_bRender = false;
	m_bStart = true;
	return S_OK;
}

int CDashEffect::Update_Effect()
{
	return 0;
}

void CDashEffect::Late_Update_Effect()
{
	if (!m_bRender && m_bStart)
		m_tInfo.iStartFrame = 0;

	if (m_bRender && !m_bStart)
	{
		if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
		{
			++m_tInfo.iStartFrame;
			m_tInfo.dwTime = GetTickCount();
			if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			{
				m_tInfo.iStartFrame = 0;
				m_bStart = true;
			}
		}
	}
}

void CDashEffect::Render_Effect()
{
	if (m_bRender && !m_bStart)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"Dash", m_tInfo.iStartFrame);
		if (pTexInfo == nullptr)
			return;

		_matrix matTrans, matScale;
		_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
		D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.5f, 1.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (25 * m_tInfo.iDirection)+ (int)vScroll.x, m_tInfo.vecPos.y+(int)vScroll.y, 0);
		float fCenterX = pTexInfo->tImageInfo.Width >> 1;
		float fCenterY = pTexInfo->tImageInfo.Height >> 1;
		matScale *= matTrans;
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CDashEffect::HitBox_Update()
{
}

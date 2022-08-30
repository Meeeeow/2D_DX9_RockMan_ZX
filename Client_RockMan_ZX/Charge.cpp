#include "stdafx.h"
#include "Charge.h"


CEffect * CCharge::Create_Charge()
{
	CEffect* pEffect = new CCharge;
	pEffect->Init_Effect();
	return pEffect;
}

CCharge::CCharge()
{
}


CCharge::~CCharge()
{
}

HRESULT CCharge::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Charge";
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.iDirection = 0;
	m_tInfo.bDead = false;
	m_tInfo.fSpeed = 0;
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 11;
	m_iPower = 0;
	m_tInfo.vecPos = {};
	m_tInfo.tRect = {};
	m_bRender = false;
	return S_OK;
}

int CCharge::Update_Effect()
{
	return OBJ_NOEVENT;
}

void CCharge::Late_Update_Effect()
{

	if (!m_bRender)
	{
		m_tInfo.iStartFrame = 0;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHARGE);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHARGEMAX);
	}
	if (m_bRender)
	{
		if (m_tInfo.iStartFrame == 0)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHARGEMAX);
			CSoundMgr::Get_Instance()->PlaySound(L"ChargeMin.wav", CSoundMgr::CHARGE);
		}
		if (m_tInfo.iStartFrame == 6)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHARGE);
			CSoundMgr::Get_Instance()->PlaySound(L"ChargeMax.wav", CSoundMgr::CHARGEMAX);
		}
	}
	if (m_bRender)
	{
		if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
		{
			++m_tInfo.iStartFrame;
			m_tInfo.dwTime = GetTickCount();
			if (m_tInfo.iStartFrame > 5 && m_iPower < 3000)
			{
				m_tInfo.iStartFrame = 0;
				return;
			}
			if (m_tInfo.iStartFrame < 6 && m_iPower > 3001)
			{
				m_tInfo.iStartFrame = 6;
				return;
			}
			if (m_tInfo.iStartFrame > m_tInfo.iEndFrame && m_iPower > 3001)
			{
				m_tInfo.iStartFrame = 6;
				return;
			}
		}
	}
}

void CCharge::Render_Effect()
{
	if (m_bRender)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"Charge", m_tInfo.iStartFrame);
		if (pTexInfo == nullptr)
			return;

		_matrix matTrans, matScale;
		_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
		D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.5f, 1.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + vScroll.x, m_tInfo.vecPos.y, 0);
		float fCenterX = pTexInfo->tImageInfo.Width >> 1;
		float fCenterY = pTexInfo->tImageInfo.Height >> 1;
		matScale *= matTrans;
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CCharge::HitBox_Update()
{
}

void CCharge::Set_Charge(_vec2 vPos, int iDirection,int iPower)
{
	m_tInfo.vecPos = vPos;
	m_tInfo.iDirection = iDirection;
	m_iPower = iPower;
}

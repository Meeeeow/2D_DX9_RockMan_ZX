#include "stdafx.h"
#include "XLaser.h"
#include "ObjectBase.h"

CEffect * CXLaser::Create_Laser(float fX)
{
	CEffect* pEffect = new CXLaser;
	pEffect->Init_Effect();
	static_cast<CXLaser*>(pEffect)->Setting(fX);
	return pEffect;
}

void CXLaser::Setting(float fX)
{
	m_tInfo.vecPos = { fX, 300.f };
}

CXLaser::CXLaser()
{
}


CXLaser::~CXLaser()
{
}

HRESULT CXLaser::Init_Effect()
{
	m_tInfo.bDead = false;
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"XLaser";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 17;
	m_tInfo.fSpeed = 195.f;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	return S_OK;
}

int CXLaser::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tInfo.tRect, &iter->Get_Rect()))
		{
			iter->Default_Hit(3, m_tInfo.tRect);
		}
	}
	HitBox_Update();

	return OBJ_NOEVENT;
}

void CXLaser::Late_Update_Effect()
{
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			m_tInfo.bDead = true;
			m_tInfo.iStartFrame = 17;
		}
	}
}

void CXLaser::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 3.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CXLaser::HitBox_Update()
{
	if (m_tInfo.iStartFrame < 5 || m_tInfo.iStartFrame > 15)
	{
		m_tInfo.tRect = { 0,0,0,0 };
	}
	else
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - (1 * m_tInfo.iStartFrame - 4);
		m_tInfo.tRect.right = m_tInfo.vecPos.x + (1 * m_tInfo.iStartFrame - 4);
		m_tInfo.tRect.top = 0;
		m_tInfo.tRect.bottom = 600;
	}
}

#include "stdafx.h"
#include "HurricaneBall.h"
#include "ObjectBase.h"

CEffect * CHurricaneBall::Create_Hurricane(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CHurricaneBall;
	pEffect->Init_Effect();
	static_cast<CHurricaneBall*>(pEffect)->Setting(fX, fY, iDirection);
	return pEffect;
}

void CHurricaneBall::Setting(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
}

CHurricaneBall::CHurricaneBall()
{
}


CHurricaneBall::~CHurricaneBall()
{
}

HRESULT CHurricaneBall::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"HX_Special";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 11;
	m_tInfo.dwDelay = 180;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 150.f;
	m_tInfo.bDead = false;
	m_iCount = 0;
	return S_OK;
}

int CHurricaneBall::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	HitBox_Update();
	if (m_tInfo.iStartFrame >= 8)
	{
		m_tInfo.vecPos.y = 300.f;
		CObjectMgr::Get_Instance()->Hurricane_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), 2, m_tInfo.tRect, 11.1f);
	}


	return OBJ_NOEVENT;
}

void CHurricaneBall::Late_Update_Effect()
{
	
	if (m_tInfo.iStartFrame < 8)
	{
		m_tInfo.vecPos.x += (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
		RECT rc = {};
		for (auto iter : CObjectMgr::Get_Instance()->Get_Type(CObjectMgr::Get_Instance()->Get_Current_ID()))
		{
			if (IntersectRect(&rc, &m_tInfo.tRect, &iter->Get_Rect()))
				m_tInfo.iStartFrame = 8;
		}
	}

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			++m_iCount;
			m_tInfo.iStartFrame = 8;
			if (m_iCount > 3)
				m_tInfo.bDead = true;
		}
	}
}

void CHurricaneBall::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"HX_Special", m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	if(m_tInfo.iStartFrame>=8)
		D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.5f, 3.5f, 0.f);
	else
		D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, (m_tInfo.vecPos.y), 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CHurricaneBall::HitBox_Update()
{
	if (m_tInfo.iStartFrame < 8)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 10;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 10;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 10;
	}
	else
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 20;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 20;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 223.5f;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 223.5f;
	}

}

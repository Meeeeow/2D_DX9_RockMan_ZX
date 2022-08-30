#include "stdafx.h"
#include "LeoWall.h"
#include "ObjectBase.h"

CEffect * CLeoWall::Create_Wall(float fX, int iDirection)
{
	CEffect* pEffect = new CLeoWall;
	static_cast<CLeoWall*>(pEffect)->Setting(fX, iDirection);
	pEffect->Init_Effect();
	return pEffect;
}

void CLeoWall::Setting(float fX, int iDirection)
{
	m_tInfo.vecPos.y = 600;
	m_tInfo.vecPos.x = fX;
	m_tInfo.iDirection = iDirection;
}

CLeoWall::CLeoWall()
{
}


CLeoWall::~CLeoWall()
{
}

HRESULT CLeoWall::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Wall";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 3;
	m_tInfo.dwDelay = 70;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 350.f;
	m_tInfo.bDead = false;
	m_bMove = false;
	return S_OK;
}

int CLeoWall::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	if (m_tInfo.vecPos.y > 500)
		m_tInfo.vecPos.y -= 22.f;
	if (m_tInfo.vecPos.y < 500)
		m_tInfo.vecPos.y = 500;
	
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tInfo.tRect, &iter->Get_Rect()))
		{
			iter->Default_Hit(3, m_tInfo.tRect);
			m_tInfo.bDead = true;
		}
	}
	HitBox_Update();

	return OBJ_NOEVENT;
}

void CLeoWall::Late_Update_Effect()
{
	if (m_bMove)
	{
		m_tInfo.vecPos.x -= (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
		if (m_tInfo.vecPos.x > 1500 || m_tInfo.vecPos.x < 100)
			m_tInfo.bDead = true;
	}
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CLeoWall::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLeoWall::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 25;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 25;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 30;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 30;
}

#include "stdafx.h"
#include "Gong.h"
#include "ObjectBase.h"
#include "Tile.h"

CEffect * CGong::Create_Gong(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CGong;
	pEffect->Init_Effect();
	static_cast<CGong*>(pEffect)->Setting(fX, fY, iDirection);
	return pEffect;
}

void CGong::Setting(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
}

CGong::CGong()
{
}


CGong::~CGong()
{
}

HRESULT CGong::Init_Effect()
{
	m_tInfo.bDead = false;
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Gong";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 6;
	m_tInfo.fSpeed = 20.f;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();

	return S_OK;
}

int CGong::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		RECT rc = {};
		if ((iter->Get_TileInfo().iDrawID == 2)
			&& IntersectRect(&rc, &iter->Get_TileRect(), &m_tInfo.tRect))
		{
			m_tInfo.bDead = true;
		}
	}
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

void CGong::Late_Update_Effect()
{
	++m_tInfo.fSpeed;

	m_tInfo.vecPos.x -= (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CGong::Render_Effect()
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

void CGong::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 30;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 30;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 30;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 30;
}

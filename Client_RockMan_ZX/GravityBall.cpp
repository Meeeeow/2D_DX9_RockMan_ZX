#include "stdafx.h"
#include "GravityBall.h"
#include "Tile.h"
#include "ObjectBase.h"

CEffect * CGravityBall::Create_Ball(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CGravityBall;
	pEffect->Init_Effect();
	static_cast<CGravityBall*>(pEffect)->Setting(fX, fY, iDirection);
	return pEffect;
}

void CGravityBall::Setting(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
}

CGravityBall::CGravityBall()
{
}


CGravityBall::~CGravityBall()
{
}

HRESULT CGravityBall::Init_Effect()
{
	m_tInfo.bDead = false;
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Gravity";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 27;
	m_tInfo.fSpeed = 35.f;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_iCount = 0;
	return S_OK;
}

int CGravityBall::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	
	m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		RECT rc = {};
		if ((iter->Get_TileInfo().iDrawID == 2 || iter->Get_TileInfo().iDrawID == 1)
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

void CGravityBall::Late_Update_Effect()
{
	++m_iCount;

	if (m_iCount > 500)
		m_tInfo.bDead = true;

	m_vDir = m_tInfo.vecPos - m_vTarget;
	D3DXVec2Normalize(&m_vDir, &m_vDir);
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
		iter->Set_PosX(m_vDir.x * m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime());

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CGravityBall::Render_Effect()
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

void CGravityBall::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 5;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 5;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 5;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 5;

}

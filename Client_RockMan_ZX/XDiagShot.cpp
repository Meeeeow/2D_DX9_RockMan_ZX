#include "stdafx.h"
#include "XDiagShot.h"
#include "Player.h"
#include "Tile.h"

CEffect * CXDiagShot::Create_DiagShot(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CXDiagShot;
	pEffect->Init_Effect();
	static_cast<CXDiagShot*>(pEffect)->Setting(fX, fY, iDirection);
	return pEffect;
}

void CXDiagShot::Setting(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
}

CXDiagShot::CXDiagShot()
{
}


CXDiagShot::~CXDiagShot()
{
}

HRESULT CXDiagShot::Init_Effect()
{
	m_tInfo.bDead = false;
	m_tInfo.wstrObjectKey = L"CopyX";
	m_tInfo.wstrStateKey = L"ChargeDiag";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 3;
	m_tInfo.fSpeed = 195.f;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	return S_OK;
}

int CXDiagShot::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;


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
		}
	}
	HitBox_Update();

	return OBJ_NOEVENT;
}

void CXDiagShot::Late_Update_Effect()
{
	m_tInfo.vecPos.x -= m_tInfo.fSpeed * m_tInfo.iDirection * CTimeMgr::Get_Instance()->Get_DeltaTime();
	m_tInfo.vecPos.y += m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CXDiagShot::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_tInfo.iDirection*1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CXDiagShot::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 15;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 15;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 15;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 15;
}

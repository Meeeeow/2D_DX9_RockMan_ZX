#include "stdafx.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "ObjectBase.h"
#include "Tile.h"

CEffect * CEnemyBullet::Create_EnemyBullet(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CEnemyBullet;
	pEffect->Init_Effect();
	static_cast<CEnemyBullet*>(pEffect)->Set_Pos(fX, fY,iDirection);
	return pEffect;
}

void CEnemyBullet::Set_Pos(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos.x = fX;
	m_tInfo.vecPos.y = fY;
	m_tInfo.iDirection = iDirection;
}

CEnemyBullet::CEnemyBullet()
{
}


CEnemyBullet::~CEnemyBullet()
{
}

HRESULT CEnemyBullet::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Bullet";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 3;
	m_tInfo.dwDelay = 30;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 190.f;
	m_tInfo.bDead = false;

	return S_OK;
}

int CEnemyBullet::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	HitBox_Update();
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tInfo.tRect, &iter->Get_Rect()))
			iter->Default_Hit(2, m_tInfo.tRect);
	}
	m_tInfo.vecPos.x -= (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();

	return OBJ_NOEVENT;
}

void CEnemyBullet::Late_Update_Effect()
{
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		RECT rc = {};
		if ((iter->Get_TileInfo().iDrawID == 2 || iter->Get_TileInfo().iDrawID == 1)
			&& IntersectRect(&rc, &iter->Get_TileRect(), &m_tInfo.tRect))
		{
			m_tInfo.bDead = true;
		}
	}
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CEnemyBullet::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, (m_tInfo.iDirection) * 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y + (int)vScroll.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEnemyBullet::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 6;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 6;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 6;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 6;



}

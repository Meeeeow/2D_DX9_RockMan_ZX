#include "stdafx.h"
#include "Suriken.h"
#include "Tile.h"

CEffect * CSuriken::Create_Suriken(_vec2 vPos, int iDirection)
{
	CEffect* pEffect = new CSuriken;
	pEffect->Init_Effect();
	static_cast<CSuriken*>(pEffect)->Setting_Suriken(vPos, iDirection);
	return pEffect;
}

void CSuriken::Setting_Suriken(_vec2 vPos, int iDirection)
{
	m_tInfo.iDirection = iDirection;
	m_tInfo.vecPos = vPos;
}

CSuriken::CSuriken()
{
}


CSuriken::~CSuriken()
{
}

HRESULT CSuriken::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"PX_Suriken";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 3;
	m_tInfo.dwDelay = 30;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 388.f;
	m_tInfo.bDead = false;

	return S_OK;
}

int CSuriken::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	HitBox_Update();
	Hit_Check();


	return OBJ_NOEVENT;
}

void CSuriken::Late_Update_Effect()
{
	if (CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x + 700 < m_tInfo.vecPos.x
		&& CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x - 700 > m_tInfo.vecPos.x)
		m_tInfo.bDead = true;

	m_tInfo.vecPos.x += (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CSuriken::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y + (int)vScroll.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CSuriken::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 19;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 19;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 8;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 8;
}

void CSuriken::Hit_Check()
{
	// 벽에 닿았을 때
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		RECT rc = {};
		if ((iter->Get_TileInfo().iDrawID == 2 || iter->Get_TileInfo().iDrawID == 1)
			&& IntersectRect(&rc, &iter->Get_TileRect(), &m_tInfo.tRect))
		{
			m_tInfo.bDead = true;
		}
	}
	CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), 3, m_tInfo.tRect);

}

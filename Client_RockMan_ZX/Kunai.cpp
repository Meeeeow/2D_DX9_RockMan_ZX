#include "stdafx.h"
#include "Kunai.h"
#include "Tile.h"

CEffect * CKunai::Create_Kunai(_vec2 vPos, int _iDirection)
{
	CEffect* pEffect = new CKunai;
	pEffect->Init_Effect();
	static_cast<CKunai*>(pEffect)->Setting_Kunai(vPos, _iDirection);

	return pEffect;
}

void CKunai::Setting_Kunai(_vec2 vPos, int _iDirection)
{
	m_tInfo.iDirection = _iDirection;
	m_tInfo.vecPos = vPos;
}

CKunai::CKunai()
{
}


CKunai::~CKunai()
{
}

HRESULT CKunai::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"PX_Kunai";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 1;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = 0;
	m_tInfo.fSpeed = 377.f;
	m_tInfo.bDead = false;
	return S_OK;
}

int CKunai::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	HitBox_Update();
	Hit_Check();


	return OBJ_NOEVENT;
}

void CKunai::Late_Update_Effect()
{
	if (CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x + 700 < m_tInfo.vecPos.x
		&& CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x - 700 > m_tInfo.vecPos.x)
		m_tInfo.bDead = true;
	m_tInfo.vecPos.x += (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();

}

void CKunai::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + vScroll.x, m_tInfo.vecPos.y + vScroll.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CKunai::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 8;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 8;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 4;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 4;
}

void CKunai::Hit_Check()
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
	CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), 1, m_tInfo.tRect);

}

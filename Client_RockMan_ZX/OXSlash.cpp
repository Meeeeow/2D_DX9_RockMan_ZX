#include "stdafx.h"
#include "OXSlash.h"


CEffect * COXSlash::Create_Slash(_vec2 vPos)
{
	CEffect* pEffect = new COXSlash;
	static_cast<COXSlash*>(pEffect)->Setting_Slash(vPos);
	pEffect->Init_Effect();
	return pEffect;
}

void COXSlash::Setting_Slash(_vec2 vPos)
{
	m_tInfo.vecPos = vPos;
	m_tInfo.iDirection = 0;
}

COXSlash::COXSlash()
{
}


COXSlash::~COXSlash()
{
}

HRESULT COXSlash::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"OX_Special";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 2;
	m_tInfo.dwDelay = 120;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 0;
	m_tInfo.bDead = false;

	return S_OK;
}

int COXSlash::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	m_tInfo.tRect.left = m_tInfo.vecPos.x - (pTexInfo->tImageInfo.Width >> 1);
	m_tInfo.tRect.right = m_tInfo.vecPos.x + (pTexInfo->tImageInfo.Width >> 1);
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 100;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 100;
	CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), 7, m_tInfo.tRect);

	return OBJ_NOEVENT;
}

void COXSlash::Late_Update_Effect()
{
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			m_tInfo.bDead = true;
			m_tInfo.iStartFrame = 2;
		}
	}
}

void COXSlash::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.8f, 1.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void COXSlash::HitBox_Update()
{
}

void COXSlash::Attack()
{

}

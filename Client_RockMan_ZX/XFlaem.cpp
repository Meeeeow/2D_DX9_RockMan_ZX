#include "stdafx.h"
#include "XFlaem.h"
#include "Player.h"

CEffect * CXFlaem::Create_Flame(float fX, float fY, int iDirection)
{
	CEffect* pEffect = new CXFlaem;
	pEffect->Init_Effect();
	static_cast<CXFlaem*>(pEffect)->Setting(fX, fY, iDirection);
	return pEffect;
}

void CXFlaem::Setting(float fX, float fY, int iDirection)
{
	m_tInfo.vecPos = { fX,400 };
	m_tInfo.iDirection = iDirection;
}

CXFlaem::CXFlaem()
{
}


CXFlaem::~CXFlaem()
{
}

HRESULT CXFlaem::Init_Effect()
{
	m_tInfo.bDead = false;
	m_tInfo.wstrObjectKey = L"CopyX";
	m_tInfo.wstrStateKey = L"Flame";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 7;
	m_tInfo.fSpeed = 195.f;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::COPYX_FLAME);
	CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Flame.wav", CSoundMgr::COPYX_FLAME);
	return S_OK;
}

int CXFlaem::Update_Effect()
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

void CXFlaem::Late_Update_Effect()
{
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			m_tInfo.iStartFrame = 0;
			m_tInfo.bDead = true;
		}
	}
}

void CXFlaem::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey, m_tInfo.wstrStateKey, m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.5f, 3.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CXFlaem::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 15;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 15;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 25;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 25;
}

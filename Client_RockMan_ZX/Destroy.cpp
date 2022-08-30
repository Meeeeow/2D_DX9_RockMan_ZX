#include "stdafx.h"
#include "Destroy.h"


CEffect * CDestroy::Create_Destroy(_vec2 vPos)
{
	CEffect* pEffect = new CDestroy;
	static_cast<CDestroy*>(pEffect)->Setting_Pos(vPos);
	pEffect->Init_Effect();
	return pEffect;
}

void CDestroy::Setting_Pos(_vec2 vPos)
{
	m_tInfo.vecPos = vPos;
}

CDestroy::CDestroy()
{
}


CDestroy::~CDestroy()
{
}

HRESULT CDestroy::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"Destroy";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 8;
	m_tInfo.dwDelay = 70;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 0;
	m_tInfo.bDead = false;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DESTROY);
	CSoundMgr::Get_Instance()->PlaySound(L"Destroy.wav", CSoundMgr::DESTROY);
	return S_OK;
}

int CDestroy::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CDestroy::Late_Update_Effect()
{
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			m_tInfo.bDead = true;
			m_tInfo.iStartFrame = 0;
		}
	}
}

void CDestroy::Render_Effect()
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

void CDestroy::HitBox_Update()
{
}

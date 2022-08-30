#include "stdafx.h"
#include "XElect.h"
#include "Player.h"

CEffect * CXElect::Create_Elect(float fX, float fY, int iDirection, _vec2 TargetPos)
{
	CEffect* pEffect = new CXElect;
	pEffect->Init_Effect();
	static_cast<CXElect*>(pEffect)->Setting(fX, fY, iDirection, TargetPos);
	return pEffect;
}

void CXElect::Setting(float fX, float fY, int iDirection, _vec2 TargetPos)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
	m_vTargetPos = TargetPos;
	m_tInfo.vDir = m_vTargetPos - m_tInfo.vecPos;
	D3DXVec2Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

CXElect::CXElect()
{
}


CXElect::~CXElect()
{
}

HRESULT CXElect::Init_Effect()
{
	m_tInfo.bDead = false;
	m_tInfo.wstrObjectKey = L"CopyX";
	m_tInfo.wstrStateKey = L"Elect";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 3;
	m_tInfo.fSpeed = 195.f;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_iCount = 0;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::COPYX_ELECT);
	CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Elect.wav", CSoundMgr::COPYX_ELECT);
	return S_OK;
}

int CXElect::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	if (m_tInfo.vecPos.x < 0 || m_tInfo.vecPos.x > 1600)
		m_tInfo.bDead = true;
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

void CXElect::Late_Update_Effect()
{
	m_tInfo.vecPos += (m_tInfo.vDir * m_tInfo.fSpeed) * CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			m_tInfo.iStartFrame = 1;
		}
	}
}

void CXElect::Render_Effect()
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

void CXElect::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 10;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 10;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 10;
}

#include "stdafx.h"
#include "HivokLaser.h"
#include "ObjectBase.h"

CEffect * CHivokLaser::Create_Laser(_vec2 Target, float fX, float fY)
{
	CEffect* pEffect = new CHivokLaser;
	pEffect->Init_Effect();
	static_cast<CHivokLaser*>(pEffect)->Setting(Target, fX, fY);
	return pEffect;
}

void CHivokLaser::Setting(_vec2 Target, float fX, float fY)
{
	m_vTargetPos = Target;
	m_tInfo.vecPos.x = fX;
	m_tInfo.vecPos.y = fY;
	m_tInfo.vDir = m_vTargetPos - m_tInfo.vecPos;
	D3DXVec2Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	_vec2 vLook = { 1.f , 0.f };
	float fCosTheta = D3DXVec2Dot(&m_tInfo.vDir, &vLook);
	m_fAngle = acosf(fCosTheta);
}

CHivokLaser::CHivokLaser()
{
}


CHivokLaser::~CHivokLaser()
{
}

HRESULT CHivokLaser::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"HivokLaser";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 1;
	m_tInfo.tRect = {};
	m_tInfo.bDead = false;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 300;
	return S_OK;
}

int CHivokLaser::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	HitBox_Update();
	Hit_Check();
	return OBJ_NOEVENT;
}

void CHivokLaser::Late_Update_Effect()
{
	m_tInfo.vecPos += (m_tInfo.vDir*m_tInfo.fSpeed) * CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tInfo.vecPos.y > 600)
		m_tInfo.bDead = true;
	if (m_tInfo.vecPos.y < 0)
		m_tInfo.bDead = true;

	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CHivokLaser::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"HivokLaser", m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale,matRotZ,matWorld;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x, m_tInfo.vecPos.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matWorld = matScale * matRotZ* matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CHivokLaser::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 5;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 5;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 5;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 5;
}

void CHivokLaser::Hit_Check()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tInfo.tRect, &iter->Get_Rect()))
			iter->Default_Hit(2, m_tInfo.tRect);
	}
}

#include "stdafx.h"
#include "HivokGuard.h"
#include "ObjectBase.h"
#include "HivokLaser.h"
CEffect * CHivokGuard::Create_Guard(float fX, float fY, CObjectBase* pObj)
{
	CEffect* pEffect = new CHivokGuard;
	pEffect->Init_Effect();
	static_cast<CHivokGuard*>(pEffect)->Setting_Pos(fX, fY,pObj);
	return pEffect;
}

void CHivokGuard::Setting_Pos(float fX, float fY, CObjectBase* pObj)
{
	m_pParent = pObj;
	
	m_tInfo.vecPos.x = 40;
	m_tInfo.vecPos.y = 40;
}

CHivokGuard::CHivokGuard()
{
}


CHivokGuard::~CHivokGuard()
{
}

HRESULT CHivokGuard::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"HivokGuard";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 3;
	m_tInfo.tRect = {};
	m_tInfo.bDead = false;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 300;
	m_fAngle = 0.f;
	m_fRotAngle = 0.f;
	m_vLook = { 1.f, 1.f };
	m_pParent = nullptr;
	Real_Pos.x = 0.f;
	Real_Pos.y = 0.f;
	m_iShot = 0;
	return S_OK;
}

int CHivokGuard::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;



	return OBJ_NOEVENT;
}

void CHivokGuard::Late_Update_Effect()
{
	m_fRotAngle += 1.5f;

	++m_iShot;
	if (m_iShot > 225)
	{
		CEffectMgr::Get_Instance()->Insert_Effect(CHivokLaser::Create_Laser(m_vTarget, Real_Pos.x, Real_Pos.y), CEffectMgr::HIVOK_LASER);
		m_iShot = 0;
	}

	if (m_pParent == nullptr)
	{
		m_tInfo.bDead = true;
		return;
	}

	if (m_pParent->Get_Info().vPos.x < m_vTarget.x)
		m_tInfo.iDirection = ENEMY_RIGHT;
	else
		m_tInfo.iDirection = ENEMY_LEFT;
		
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
}

void CHivokGuard::Render_Effect()
{
	if (m_pParent == nullptr)
		return;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"HivokGuard", m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale, matRotZ, matParent, matRotaZ,matWorld,mat2D;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x, m_tInfo.vecPos.y, 0);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRotAngle));
	D3DXMatrixRotationZ(&matRotaZ, D3DXToRadian(-m_fRotAngle));
	D3DXMatrixTranslation(&matParent, m_pParent->Get_Info().vPos.x + vScroll.x, m_pParent->Get_Info().vPos.y, 0.f);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	matWorld = matScale *matRotZ * matTrans * matRotaZ * matParent;
	Real_Pos.x = m_pParent->Get_Info().vPos.x - (m_tInfo.vecPos.x * cosf(-m_fRotAngle));
	Real_Pos.y = m_pParent->Get_Info().vPos.y + (m_tInfo.vecPos.y * sinf(-m_fRotAngle));

	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
}

void CHivokGuard::HitBox_Update()
{
}

#include "stdafx.h"
#include "SeedHelmet.h"
#include "Destroy.h"
#include "EnemyBullet.h"

CObjectBase * CSeedHelmet::Create_Helmet(float fX, float fY)
{
	CObjectBase* pObj = new CSeedHelmet;
	pObj->Init_Object();
	pObj->Pos_Setting(fX, fY);	
	return pObj;
}

CSeedHelmet::CSeedHelmet()
{
}


CSeedHelmet::~CSeedHelmet()
{
}

HRESULT CSeedHelmet::Init_Object()
{
	m_ePreMotion = END;
	m_eCurMotion = IDLE;
	m_tFrame.wstrObjectKey = L"Helmet";
	m_tFrame.wstrStateKey = L"Idle";
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 1;
	m_tFrame.dwDelay = 100;
	m_tFrame.dwTime = GetTickCount();
	m_tInfo.bPlayer = false;
	m_tInfo.fSpeed = 90.f;
	m_tInfo.iHP = 8;
	m_tInfo.iMP = 0;
	m_tInfo.iCurHP = 8;
	m_tInfo.iCurMP = 0;
	m_tInfo.iDEF = 0;
	m_tInfo.iDMG = 1;
	m_bCanHit = true;
	m_bHit = false;
	m_bDead = false;
	m_iAlpha = 255;
	m_iHitRecovery = 0;
	m_iRenderX = 0;
	m_tInfo.vDir = { 0,0 };
	m_iDirection = ENEMY_LEFT;
	m_iCoolFire = 0;
	m_iCoolWake = 0;
	m_bFire = false;
	m_bWake = false;
	m_bIdle = true;
	m_bMotion = false;
	m_fFall = 0.f;
	return S_OK;
}

int CSeedHelmet::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_iRenderX = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x;
	if (m_iRenderX + 700 > m_tInfo.vPos.x
		&& m_iRenderX - 700 < m_tInfo.vPos.x)
	{
		Action();
		HitBox_Update();

		Helmet_Collision();
		Helmet_Fire();
		Frame_Check();
		Frame_Update();
	}
	return OBJ_NOEVENT;
}

void CSeedHelmet::Late_Update_Object()
{
	if (m_iRenderX > m_tInfo.vPos.x)
		m_iDirection = ENEMY_RIGHT;
	if (m_iRenderX < m_tInfo.vPos.x)
		m_iDirection = ENEMY_LEFT;
	if (m_tInfo.iCurHP < 0)
	{
		CEffectMgr::Get_Instance()->Insert_Effect(CDestroy::Create_Destroy(m_tInfo.vPos), CEffectMgr::DESTROY);
		m_bDead = true;
	}
}

void CSeedHelmet::Render_Object()
{
	Hit_Recovery();

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tFrame.wstrObjectKey, m_tFrame.wstrStateKey, m_tFrame.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_iDirection* 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + (int)vScroll.x, m_tInfo.vPos.y + (int)vScroll.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, m_iAlpha));
}

void CSeedHelmet::Release_Object()
{
}

void CSeedHelmet::Default_Hit(int iDmg, RECT HitBox)
{
	if (m_bCanHit && !m_bIdle)
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &HitBox))
		{
			m_bCanHit = false;
			m_bHit = true;
			int iDamege = iDmg - m_tInfo.iDEF;
			if (iDamege < 1)
				iDamege = 1;
			m_tInfo.iCurHP -= iDamege;
			CSoundMgr::Get_Instance()->PlaySound(L"Monster_Hit.wav", CSoundMgr::HELMET_HIT);

		}
	}
}

void CSeedHelmet::Hit_Recovery()
{
	if (!m_bCanHit)
	{
		m_iAlpha = 10;
		++m_iHitRecovery;
		m_iAlpha = 180;
		if (m_iHitRecovery > 50)
		{
			m_iAlpha = 255;
			m_bCanHit = true;
			m_iHitRecovery = 0;
		}
	}
}

void CSeedHelmet::Helmet_Collision()
{

	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(m_tInfo.iDMG, m_tHitBox);
	}
}

void CSeedHelmet::Helmet_Fire()
{
	if (m_tFrame.wstrStateKey == L"Attack" && m_tFrame.iStartFrame == 1)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CEffectMgr::Get_Instance()->Insert_Effect(CEnemyBullet::Create_EnemyBullet((m_tInfo.vPos.x - (5 * m_iDirection)), m_tInfo.vPos.y, m_iDirection)
				, CEffectMgr::BULLET);
		}
	}
	if (m_tFrame.wstrStateKey == L"Attack" && m_tFrame.iStartFrame > 1)
	{
		m_bMotion = false;
	}
}

void CSeedHelmet::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		switch (m_eCurMotion)
		{
		case CSeedHelmet::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 1;
			m_tFrame.dwDelay = 100;
			break;
		case CSeedHelmet::WAKE:
			m_tFrame.wstrStateKey = L"Wake";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 3;
			m_tFrame.dwDelay = 250;

			break;
		case CSeedHelmet::FIRE:
			m_tFrame.wstrStateKey = L"Attack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 2;
			break;
		case CSeedHelmet::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CSeedHelmet::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_bWake)
				m_bWake = false;
			if (m_bFire)
			{
				m_bFire = false;
				m_bWake = false;
			}
			if (!m_bIdle)
				m_bIdle = true;
			m_tFrame.iStartFrame = 0;
		}
	}
}

void CSeedHelmet::HitBox_Update()
{
	if (m_bIdle)
	{
		m_tHitBox.left = m_tInfo.vPos.x - 15;
		m_tHitBox.top = m_tInfo.vPos.y-5;
		m_tHitBox.right = m_tInfo.vPos.x + 15;
		m_tHitBox.bottom = m_tInfo.vPos.y+15;
	}
	if (m_bFire)
	{
		m_tHitBox.left = m_tInfo.vPos.x - 15;
		m_tHitBox.top = m_tInfo.vPos.y - 15;
		m_tHitBox.right = m_tInfo.vPos.x + 15;
		m_tHitBox.bottom = m_tInfo.vPos.y+15;
	}
	if (m_bWake)
	{
		m_tHitBox.left = m_tInfo.vPos.x - 15;
		m_tHitBox.top = m_tInfo.vPos.y-15;
		m_tHitBox.right = m_tInfo.vPos.x + 15;
		m_tHitBox.bottom = m_tInfo.vPos.y+15;
	}
}

void CSeedHelmet::Action()
{
	float fY = 0.f;
	bool TileCollision = CCollisionMgr::Get_Instance()->TileCollision(this, &fY);
	if (TileCollision)
	{
		m_tInfo.vPos.y = fY;
		m_fFall = 0.f;
	}
	else
	{
		m_fFall += 0.4f;
		m_tInfo.vPos.y += m_fFall;
	}
	CCollisionMgr::Get_Instance()->WallTileCollision(this);
	if (m_tInfo.vPos.y > 600)
		m_bDead = true;
	if (m_bIdle)
		m_eCurMotion = IDLE;
	if (m_bFire)
		m_eCurMotion = FIRE;
	if (m_bWake)
		m_eCurMotion = WAKE;
	if (!m_bFire && m_bWake)
	{
		++m_iCoolFire;
		if (m_iCoolFire > 70)
		{
			m_bIdle = false;
			m_iCoolFire = 0;
			m_bFire = true;
			m_bWake = false;
		}
	}
	if (!m_bWake && !m_bFire)
	{
		++m_iCoolWake;
		if (m_iCoolWake> 300)
		{
			m_bIdle = false;
			m_iCoolWake = 0;
			m_bWake = true;
		}
	}
}

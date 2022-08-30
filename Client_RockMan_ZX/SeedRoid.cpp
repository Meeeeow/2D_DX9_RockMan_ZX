#include "stdafx.h"
#include "SeedRoid.h"
#include "EnemyBullet.h"
#include "Destroy.h"

CObjectBase * CSeedRoid::Create_Roid(float fX, float fY)
{
	CObjectBase* pObj = new CSeedRoid;
	pObj->Init_Object();
	pObj->Pos_Setting(fX, fY);
	return pObj;
}

CSeedRoid::CSeedRoid()
{
}


CSeedRoid::~CSeedRoid()
{
}

HRESULT CSeedRoid::Init_Object()
{
	m_ePreMotion = END;
	m_eCurMotion = IDLE;
	m_tFrame.wstrObjectKey = L"Roid";
	m_tFrame.wstrStateKey = L"Idle";
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 1;
	m_tFrame.dwDelay = 100;
	m_tFrame.dwTime = GetTickCount();
	m_tInfo.bPlayer = false;
	m_tInfo.fSpeed = 90.f;
	m_tInfo.iHP = 5;
	m_tInfo.iMP = 0;
	m_tInfo.iCurHP = 5;
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
	m_iCoolWalk = 0;
	m_bFire = false;
	m_bWalk = false;
	m_bIdle = true;
	m_bMotion = false;
	m_fFall = 0.f;
	return S_OK;
}

int CSeedRoid::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_iRenderX = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x;
	if (m_iRenderX + 700 > m_tInfo.vPos.x
		&& m_iRenderX - 700 < m_tInfo.vPos.x)
	{
		Action();
		HitBox_Update();

		Roid_Collision();
		Roid_Fire();
		Frame_Check();
		Frame_Update();
	}
	return OBJ_NOEVENT;
}

void CSeedRoid::Late_Update_Object()
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

void CSeedRoid::Render_Object()
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

void CSeedRoid::Release_Object()
{
}

void CSeedRoid::Default_Hit(int iDmg, RECT HitBox)
{
	if (m_bCanHit)
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

void CSeedRoid::Hit_Recovery()
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

void CSeedRoid::Roid_Collision()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(m_tInfo.iDMG, m_tHitBox);
	}
}

void CSeedRoid::Roid_Fire()
{
	if (m_tFrame.wstrStateKey == L"Fire" && m_tFrame.iStartFrame == 1)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CEffectMgr::Get_Instance()->Insert_Effect(CEnemyBullet::Create_EnemyBullet((m_tInfo.vPos.x - (5 * m_iDirection)), m_tInfo.vPos.y, m_iDirection)
				,CEffectMgr::BULLET);
		}
	}
	if (m_tFrame.wstrStateKey == L"Fire" && m_tFrame.iStartFrame > 1)
	{
		m_bMotion = false;
	}
}

void CSeedRoid::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		switch (m_eCurMotion)
		{
		case CSeedRoid::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 1;
			m_tFrame.dwDelay = 100;
			break;
		case CSeedRoid::WALK:
			m_tFrame.wstrStateKey = L"Walk";
			m_tFrame.iStartFrame = 0;
			m_tFrame.dwDelay = 250;
			m_tFrame.iEndFrame = 3;
			break;
		case CSeedRoid::FIRE:
			m_tFrame.wstrStateKey = L"Fire";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 2;
			m_tFrame.dwDelay = 100;
			break;
		case CSeedRoid::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CSeedRoid::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_bWalk)
				m_bWalk = false;
			if (m_bFire)
				m_bFire = false;
			if (!m_bIdle)
				m_bIdle = true;
			m_tFrame.iStartFrame = 0;
		}
	}
}

void CSeedRoid::HitBox_Update()
{
	m_tHitBox.left = m_tInfo.vPos.x - 21;
	m_tHitBox.top = m_tInfo.vPos.y - 25;
	m_tHitBox.right = m_tInfo.vPos.x + 21;
	m_tHitBox.bottom = m_tInfo.vPos.y + 25;
}

void CSeedRoid::Action()
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
	if (m_bWalk)
	{
		m_eCurMotion = WALK;
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (!m_bFire && !m_bWalk)
	{
		++m_iCoolFire;
		if (m_iCoolFire > 100)
		{
			m_bIdle = false;
			m_iCoolFire = 0;
			m_bFire = true;
		}
	}
	if (!m_bWalk && !m_bFire)
	{
		++m_iCoolWalk;
		if (m_iCoolWalk > 50)
		{
			m_bIdle = false;
			m_iCoolWalk = 0;
			m_bWalk = true;
		}
	}
}

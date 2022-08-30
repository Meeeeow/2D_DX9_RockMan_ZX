#include "stdafx.h"
#include "Hivok.h"
#include "Destroy.h"
#include "HivokGuard.h"

CObjectBase * CHivok::Create_Hivok()
{
	CObjectBase* pObj = new CHivok;
	pObj->Init_Object();
	return pObj;
}

CHivok::CHivok()
{
}


CHivok::~CHivok()
{
}

HRESULT CHivok::Init_Object()
{
	m_tInfo.vPos = { 3820.f , 350.f };
	m_tFrame.wstrObjectKey = L"Hivok";
	m_tFrame.wstrStateKey = L"Intro";
	m_eCurMotion = INTRO;
	m_ePreMotion = END;
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 11;
	m_tFrame.dwDelay = 100;
	m_tFrame.dwTime = GetTickCount();
	m_tInfo.bPlayer = false;
	m_tInfo.fSpeed = 111.f;
	m_tInfo.iHP = 15;
	m_tInfo.iMP = 0;
	m_tInfo.iCurHP = 15;
	m_tInfo.iCurMP = 0;
	m_tInfo.iDEF = 0;
	m_tInfo.iDMG = 1;
	m_bCanHit = true;
	m_bHit = false;
	m_bDead = false;
	m_iAlpha = 255;
	m_iHitRecovery = 0;
	m_tInfo.vDir = { 0,0 };
	m_iDirection = ENEMY_LEFT;

	m_bAction = false;
	m_bDeadMotion = false;

	m_bIdle = false;
	m_bDash = false;
	m_bLandAttack = false;
	m_bSummon = false;

	m_iDashCount = 0;
	m_iLandAttackCount = 0;
	m_iSummonCount = 0;

	m_bIntro = false;
	return S_OK;
}

int CHivok::Update_Object()
{
	
	if (m_bDead)
		return OBJ_DEAD;
	m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;
	if (!m_bAction&&m_tInfo.vPos.x - 150.f < m_vTarget.x)
	{
		m_bAction = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Warning.wav", CSoundMgr::WARNING);
		CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_Intro.wav", CSoundMgr::HI_INTRO);
	}
	
	if (m_bAction)
	{
		Action();
		Hivok_Collision();
		HitBox_Update();
		Frame_Check();
		Frame_Update();
	}

	return OBJ_NOEVENT;
}

void CHivok::Late_Update_Object()
{
	if (m_tFrame.iStartFrame < 2)
	{
		if (m_vTarget.x < m_tInfo.vPos.x)
			m_iDirection = ENEMY_LEFT;
		else if (m_vTarget.x > m_tInfo.vPos.x)
			m_iDirection = ENEMY_RIGHT;
	}
	
}

void CHivok::Render_Object()
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

void CHivok::Release_Object()
{

}

void CHivok::Default_Hit(int iDmg, RECT HitBox)
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
			CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_Hit.wav", CSoundMgr::HI_HIT);
		}
	}
}

void CHivok::Hit_Recovery()
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

void CHivok::Hivok_Collision()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(2, m_tHitBox);
	}
}

void CHivok::Hivok_Dash()
{
	CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_Dash.wav", CSoundMgr::HI_DASH);
	m_tInfo.vDir = m_vTarget - m_tInfo.vPos;
	D3DXVec2Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_tInfo.vPos.x += m_tInfo.vDir.x * m_tInfo.fSpeed * 4.5f  * CTimeMgr::Get_Instance()->Get_DeltaTime();
	m_tInfo.vPos.y += m_tInfo.vDir.y * m_tInfo.fSpeed * 2.5f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(4, m_tHitBox);
	}
}

void CHivok::Hivok_LandAttack()
{
	if(m_tFrame.iStartFrame == 11)
		CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_First.wav", CSoundMgr::HI_F);
	if (m_tFrame.iStartFrame == 20)
		CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_Second.wav", CSoundMgr::HI_S);
	if(m_tFrame.iStartFrame == 31)
		CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_Third.wav", CSoundMgr::HI_T);

	if (m_tFrame.iStartFrame == 11 || m_tFrame.iStartFrame == 12 ||
		m_tFrame.iStartFrame == 20 || m_tFrame.iStartFrame == 21 ||
		m_tFrame.iStartFrame == 31 || m_tFrame.iStartFrame == 32)
	{
		m_tInfo.vPos.x -= (m_tInfo.fSpeed / 2.f) * m_iDirection * CTimeMgr::Get_Instance()->Get_DeltaTime();
		RECT rc = {};
		rc.top = m_tInfo.vPos.y - (30 * 1.5f);
		rc.bottom = m_tInfo.vPos.y + 30;
		if (m_iDirection == ENEMY_LEFT)
		{
			rc.left = m_tInfo.vPos.x - (65 * 1.5f);
			rc.right = m_tInfo.vPos.x + 10;
		}
		else
		{
			rc.right = m_tInfo.vPos.x + (65 * 1.5f);
			rc.left = m_tInfo.vPos.x - 10;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::PLAYER, 3, rc);
	
	}
}

void CHivok::Hivok_Summon()
{
	if(m_tFrame.iStartFrame == 1)
		CSoundMgr::Get_Instance()->PlaySound(L"Hivolt_Summon.wav", CSoundMgr::HI_SUMMON);
}

void CHivok::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		switch (m_eCurMotion)
		{
		case CHivok::INTRO:
			m_tFrame.wstrStateKey = L"Intro";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 11;
			break;
		case CHivok::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 2;
			break;
		case CHivok::DASH:
			m_tFrame.wstrStateKey = L"Dash";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 7;
			break;
		case CHivok::LANDATTACK:
			m_tFrame.wstrStateKey = L"LandAttack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 41;
			break;
		case CHivok::SUMMON:
			m_tFrame.wstrStateKey = L"Summon";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 15;
			break;
		case CHivok::DEAD:
			m_tFrame.wstrStateKey = L"Dead";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 3;
			break;
		case CHivok::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CHivok::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_ePreMotion == INTRO)
			{
				m_eCurMotion = IDLE;
				m_bIntro = true;
			}

			if (m_bDeadMotion)
				m_bDead = true;

			if (m_bDash)
				m_bDash = false;

			if (m_bLandAttack && !m_bIdle)
				m_bLandAttack = false;

			if (m_bSummon)
				m_bSummon = false;

			if (!m_bIdle && !m_bDeadMotion)
				m_bIdle = true;

		
			m_tFrame.iStartFrame = 0;
		}
	}
}

void CHivok::HitBox_Update()
{
	m_tHitBox.left = m_tInfo.vPos.x - 60;
	m_tHitBox.top = m_tInfo.vPos.y - 45;
	m_tHitBox.right = m_tInfo.vPos.x + 60;
	m_tHitBox.bottom = m_tInfo.vPos.y + 40;
}

void CHivok::Action()
{
	if (m_tInfo.iCurHP < 0)
	{
		CRandomMgr::Range(-2, 2);
		_vec2 Dest = { (float)CRandomMgr::Random(),(float)CRandomMgr::Random() };
		CEffectMgr::Get_Instance()->Insert_Effect(CDestroy::Create_Destroy(m_tInfo.vPos - Dest), CEffectMgr::DESTROY);

		m_bDeadMotion = true;
	}
	CCollisionMgr::Get_Instance()->WallTileCollision(this);
	if (m_bAction)
	{
		float fY = 0.f;
		if (m_bLandAttack && CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
		{
			m_bIdle = false;
			m_eCurMotion = LANDATTACK;
			m_tInfo.vPos.y = fY;
			Hivok_LandAttack();
		}
		else if (m_bLandAttack && !CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
		{
			m_eCurMotion = IDLE;
			m_tInfo.vPos.y += 2.1f;
		}

		if (m_bIntro&&!m_bLandAttack && !m_bDash && m_tInfo.vPos.y > m_vTarget.y - 70)
			m_tInfo.vPos.y -= 2.f;
		if (m_bIntro&&!m_bLandAttack && !m_bDash && m_tInfo.vPos.y < m_vTarget.y - 80)
			m_tInfo.vPos.y += 1.f;
		if (m_bIdle && !m_bDeadMotion)
		{
			m_eCurMotion = IDLE;
			if ((m_tInfo.vPos.x > m_vTarget.x + 10 &&
				m_tInfo.vPos.x > m_vTarget.x - 10))
				m_tInfo.vPos.x -= (m_tInfo.fSpeed / 2.f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
		}
		if (m_bDeadMotion)
			m_eCurMotion = DEAD;
		if (m_bDash)
		{
			m_eCurMotion = DASH;
			Hivok_Dash();
		}
		if (m_bSummon)
		{
			m_eCurMotion = SUMMON;
			Hivok_Summon();
		}

		if (!m_bDash && !m_bLandAttack && !m_bSummon)
		{
			++m_iDashCount;
			if (m_iDashCount > 449)
			{
				m_iDashCount = 0;
				m_bDash = true;
				m_bIdle = false;
			}
		}
		if (!m_bDash && !m_bLandAttack && !m_bSummon)
		{
			++m_iLandAttackCount;
			if (m_iLandAttackCount > 740)
			{
				m_iLandAttackCount = 0;
				m_bLandAttack = true;
			}
		}
		if (!m_bDash && !m_bLandAttack && !m_bSummon)
		{
			++m_iSummonCount;
			if (m_iSummonCount > 1200)
			{
				CEffectMgr::Get_Instance()->Insert_Effect(CHivokGuard::Create_Guard(m_tInfo.vPos.x,m_tInfo.vPos.y, this),CEffectMgr::HIVOK_GUARD);
				m_iSummonCount = 0;
				m_bSummon = true;
				m_bIdle = false;
			}
		}
	}
}

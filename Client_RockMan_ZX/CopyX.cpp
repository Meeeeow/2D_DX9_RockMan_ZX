#include "stdafx.h"
#include "CopyX.h"
#include "XDiagShot.h"
#include "XShot.h"
#include "XElect.h"
#include "XFlaem.h"
#include "XLaser.h"

CObjectBase * CCopyX::Create_X()
{
	CObjectBase* pObj = new CCopyX;
	pObj->Init_Object();
	return pObj;
}

CCopyX::CCopyX()
{
}


CCopyX::~CCopyX()
{
}

HRESULT CCopyX::Init_Object()
{
	m_tInfo.vPos = { 800.f , 539.f };
	m_tFrame.wstrObjectKey = L"CopyX";
	m_tFrame.wstrStateKey = L"Sit";
	m_eCurMotion = SIT;
	m_ePreMotion = END;
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 2;
	m_tFrame.dwDelay = 100;
	m_tFrame.dwTime = GetTickCount();
	m_tInfo.bPlayer = false;
	m_tInfo.fSpeed = 111.f;
	m_tInfo.iHP = 10;
	m_tInfo.iMP = 0;
	m_tInfo.iCurHP = 10;
	m_tInfo.iCurMP = 0;
	m_tInfo.iDEF = 0;
	m_tInfo.iDMG = 3;
	m_bCanHit = true;
	m_bHit = false;
	m_bDead = false;
	m_iAlpha = 255;
	m_iHitRecovery = 0;
	m_tInfo.vDir = { 0,0 };
	m_iDirection = ENEMY_LEFT;

	m_bAction = false;
	m_bMotion = false;
	m_bIdle = false;

	m_bSit = false;
	m_bIntro = false;
	m_bShot = false;
	m_iShotCount = 0;
	m_bJumpShot = false;
	m_iJumpShotCount = 0;
	m_bOverShot = false;
	m_iOverShotCount = 0;
	m_bJumpDash = false;
	m_iJumpDashCount = 0;
	m_bTackle = false;
	m_iTackleCount = 0;
	m_bHover = false;
	m_iHoverCount = 0;
	m_iHoverMax = 0;
	m_bDoom = false;
	m_iDoomCount = 0;
	m_iDoomMax = 0;
	m_iSitCount = 0;
	m_bFall = false;
	m_i = 0;
	m_bDeadMotion = false;
	return S_OK;
}

int CCopyX::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (!m_bTackle && !m_bFall)
	{
		m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;
	}

	if (!m_bAction && m_tInfo.vPos.x - 200.f < CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x)
	{
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_OX.wav");
		CSoundMgr::Get_Instance()->PlaySound(L"Warning.wav", CSoundMgr::WARNING);
		CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Intro.wav", CSoundMgr::COPYX_INTRO);
		m_bSit = true;
		m_bAction = true;
	}
	if (m_bAction)
	{
		Action();
		CopyX_Collision();
		HitBox_Update();
		Frame_Check();
		Frame_Update();
	}

	return OBJ_NOEVENT;
}

void CCopyX::Late_Update_Object()
{
	if (m_tFrame.iStartFrame < 2)
	{
		if (m_vTarget.x < m_tInfo.vPos.x)
			m_iDirection = ENEMY_LEFT;
		else if (m_vTarget.x > m_tInfo.vPos.x)
			m_iDirection = ENEMY_RIGHT;
	}
}

void CCopyX::Render_Object()
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

void CCopyX::Release_Object()
{
}

void CCopyX::Default_Hit(int iDmg, RECT HitBox)
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
			CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Hit.wav", CSoundMgr::COPYX_HIT);
			if (m_tInfo.iCurHP < 0)
			{
				m_bIdle = false;
				m_bDeadMotion = true;
				m_bDoom = false;
				m_bFall = false;
				m_bJumpDash = false;
				m_bJumpShot = false;
				m_bShot = false;
				m_bOverShot = false;
				m_bTackle = false;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = DEAD;
				Frame_Check();
			}
		}
	}
}

void CCopyX::Hit_Recovery()
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

void CCopyX::CopyX_Collision()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(m_tInfo.iDMG, m_tHitBox);
	}
}

void CCopyX::CopyX_Shot()
{
	if (m_tFrame.iStartFrame == 3)
	{
		m_tFrame.dwDelay = 400;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::COPYX_SHOT);
		CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Shot.wav", CSoundMgr::COPYX_SHOT);
	}
	if (m_tFrame.iStartFrame == 5)
	{
		if (!m_bMotion)
		{

			CEffectMgr::Get_Instance()->Insert_Effect(CXShot::Create_Shot(m_tInfo.vPos.x, m_tInfo.vPos.y, m_iDirection), CEffectMgr::X_SHOT);
			m_bMotion = true;
		}
		m_tInfo.vPos.x += (5 * m_iDirection);
		m_tFrame.dwDelay = 100;
	}
}

void CCopyX::CopyX_JumpShot()
{
	if (m_tFrame.iStartFrame == 2)
	{
		m_tFrame.dwDelay = 400;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::COPYX_SHOT);
		CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Shot.wav", CSoundMgr::COPYX_SHOT);

	}
	if (m_tFrame.iStartFrame == 4)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CXDiagShot::Create_DiagShot(m_tInfo.vPos.x, m_tInfo.vPos.y, m_iDirection), CEffectMgr::X_DIAGSHOT);
			m_bMotion = true;
		}
		m_tInfo.vPos.x += (5 * m_iDirection);
		m_tFrame.dwDelay = 100;
	}
}

void CCopyX::CopyX_OverShot()
{
	if (m_tFrame.iStartFrame == 3)
		m_tFrame.dwDelay = 500;

	if (m_tFrame.iStartFrame == 4)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x + 50), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x - 50), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x + 100), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x - 100), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x + 150), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x - 150), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x + 200), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x - 200), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x + 250), CEffectMgr::X_LASER);
			CEffectMgr::Get_Instance()->Insert_Effect(CXLaser::Create_Laser(m_tInfo.vPos.x - 250), CEffectMgr::X_LASER);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 5)
		m_bMotion = true;
}

void CCopyX::CopyX_Tackle()
{
	if(m_tFrame.iStartFrame >= 1 && m_tFrame.iStartFrame <= 3)
		m_tInfo.vPos.x -= (m_tInfo.fSpeed*8.5f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();

}

void CCopyX::CopyX_JumpDash()
{
	if (m_tInfo.vPos.y > m_vTarget.y && m_tFrame.iStartFrame <= 3)
		m_tInfo.vPos.y -= 12.f;
	if (m_tFrame.iStartFrame >= 4 && m_tFrame.iStartFrame <= 14)
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * 2.5f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.iStartFrame == 9)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CXElect::Create_Elect(m_tInfo.vPos.x, m_tInfo.vPos.y, m_iDirection, m_vTarget),
				CEffectMgr::X_ELECT);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 10)
		m_bMotion = false;
	if (m_tFrame.iStartFrame == 11)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CXElect::Create_Elect(m_tInfo.vPos.x, m_tInfo.vPos.y, m_iDirection, m_vTarget),
				CEffectMgr::X_ELECT);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 12)
		m_bMotion = false;
}

void CCopyX::CopyX_Doom()
{
	CScrollMgr::Get_Instance()->Set_Quake();
	if (m_tFrame.iStartFrame == 8)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CXFlaem::Create_Flame(m_tInfo.vPos.x + (60 * m_i),m_tInfo.vPos.y,m_iDirection), CEffectMgr::X_FIRE);
			CEffectMgr::Get_Instance()->Insert_Effect(CXFlaem::Create_Flame(m_tInfo.vPos.x - (60 * m_i), m_tInfo.vPos.y, -m_iDirection), CEffectMgr::X_FIRE);
			++m_i;
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 9)
		m_bMotion = false;
}

void CCopyX::CopyX_Hover()
{
	if (m_tInfo.vPos.y > 300)
		m_tInfo.vPos.y -= 12.f;

	m_tInfo.vDir = m_vTarget - m_tInfo.vPos;
	D3DXVec2Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos.x += (m_tInfo.vDir.x * m_tInfo.fSpeed * 2.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
}

void CCopyX::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		m_tFrame.iStartFrame = 0;

		switch (m_eCurMotion)
		{
		case CCopyX::SIT:
			m_tFrame.wstrStateKey = L"Sit";
			m_tFrame.iEndFrame = 2;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::INTRO:
			m_tFrame.wstrStateKey = L"Intro";
			m_tFrame.iEndFrame = 27;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.iEndFrame = 1;
			m_tFrame.dwDelay = 400;
			break;
		case CCopyX::SHOT:
			m_tFrame.wstrStateKey = L"Shot";
			m_tFrame.iEndFrame = 5;
			m_tFrame.dwDelay = 150;
			break;
		case CCopyX::OVERSHOT:
			m_tFrame.wstrStateKey = L"Upper";
			m_tFrame.iEndFrame = 5;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::JUMPSHOT:
			m_tFrame.wstrStateKey = L"JumpShot";
			m_tFrame.iEndFrame = 4;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::TACKLE:
			m_tFrame.wstrStateKey = L"Tackle";
			m_tFrame.iEndFrame = 4;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::JUMPDASH:
			m_tFrame.wstrStateKey = L"JumpDash";
			m_tFrame.iEndFrame = 15;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::FALL:
			m_tFrame.wstrStateKey = L"Fall";
			m_tFrame.iEndFrame = 2;
			m_tFrame.dwDelay = 100;
			break;
		case CCopyX::DOOM:
			m_tFrame.wstrStateKey = L"Doom";
			m_tFrame.iEndFrame = 9;
			m_tFrame.dwDelay = 250;
			break;
		case CCopyX::HOVER:
			m_tFrame.wstrStateKey = L"Hover";
			m_tFrame.iEndFrame = 2;
			m_tFrame.dwDelay = 350;
			break;
		case CCopyX::DEAD:
			m_tFrame.wstrStateKey = L"Dead";
			m_tFrame.iEndFrame = 5;
			m_tFrame.dwDelay = 350;
		case CCopyX::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CCopyX::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_ePreMotion == DEAD)
			{
				m_tFrame.iStartFrame = 2;
				m_bDead = true;
				return;
			}
			if (m_ePreMotion == SIT)
			{
				++m_iSitCount;
				m_tFrame.iStartFrame = 0;
				if (m_iSitCount > 3)
				{
					m_eCurMotion = INTRO;
					m_tFrame.iStartFrame = 0;
					m_iSitCount = 0;
				}
				return;
			}
			if (m_ePreMotion == INTRO)
			{
				m_eCurMotion = IDLE;
				m_tFrame.iStartFrame = 0;
				m_bIntro = true;
				return;
			}
			if (m_bHover)
			{
				++m_iHoverMax;
				m_tFrame.iStartFrame = 0;
				if (m_iHoverMax > 6)
				{
					m_iHoverMax = 0;
					m_bHover = false;
					m_bIdle = true;
				}
				return;
			}
			if (m_bDoom)
			{
				++m_iDoomMax;
				m_tFrame.iStartFrame = 8;
				if (m_iDoomMax > 10)
				{
					m_iDoomMax = 0;
					m_bDoom = false;
					m_bIdle = true;
				}
				return;
			}

			if (!m_bIdle)
				m_bIdle = true;

			if (m_bJumpDash)
				m_bJumpDash = false;
			if (m_bTackle)
				m_bTackle = false;
			
			if (m_bJumpShot)
				m_bJumpShot = false;
			if (m_bShot)
				m_bShot = false;
			if (m_bOverShot)
				m_bOverShot = false;
			m_tFrame.iStartFrame = 0;

		}
	}
}

void CCopyX::HitBox_Update()
{
	if (m_bIntro)
	{
		m_tHitBox.left = m_tInfo.vPos.x - 25;
		m_tHitBox.right = m_tInfo.vPos.x + 25;
		m_tHitBox.top = m_tInfo.vPos.y - 25;
		m_tHitBox.bottom = m_tInfo.vPos.y + 20;
	}
}

void CCopyX::Action()
{
	CCollisionMgr::Get_Instance()->WallTileCollision(this);
	float fY = 0.f;
	if (m_bIntro&&!m_bJumpShot&&!m_bJumpDash&&!m_bHover&&CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
	{
		m_bFall = false;
		m_tInfo.vPos.y = fY;
	}
	else if (m_bIntro&&!m_bJumpShot && !m_bJumpDash && !m_bHover&&!CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
	{
		m_bFall = true;
		m_tInfo.vPos.y += 3.3f;
		m_tInfo.vPos.x += (m_tInfo.vDir.x * m_tInfo.fSpeed * 1.2f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_bFall && m_bIntro)
		m_eCurMotion = FALL;
	if (m_bIdle && !m_bFall && !m_bDeadMotion)
	{
		m_bMotion = false;
		m_eCurMotion = IDLE;
		m_i = 1;
	}
	if (m_bDead)
		m_eCurMotion = DEAD;
	if (m_bHover)
	{
		m_eCurMotion = HOVER;
		CopyX_Hover(); 
	}
	if (m_bDoom)
	{
		m_eCurMotion = DOOM;
		CopyX_Doom();
	}
	if (m_bJumpShot)
	{
		m_eCurMotion = JUMPSHOT;
		CopyX_JumpShot();
	}
	if (m_bJumpDash)
	{
		m_eCurMotion = JUMPDASH;
		CopyX_JumpDash();
	}
	if (m_bOverShot)
	{
		m_eCurMotion = OVERSHOT;
		CopyX_OverShot();
	}
	if (m_bTackle)
	{
		m_eCurMotion = TACKLE;
		CopyX_Tackle();
	}
	if (m_bShot)
	{
		m_eCurMotion = SHOT;
		CopyX_Shot();
	}

	if (m_bIntro && !m_bFall && !m_bDeadMotion)
	{
		if (!m_bDoom && !m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle)
		{
			++m_iDoomCount;
			if (m_iDoomCount > 1000)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Doom.wav", CSoundMgr::COPYX_DOOM);
				m_bDoom = true;
				m_bIdle = false;
				m_iDoomCount = 0;
			}
		}

		if (!m_bDoom && !m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle)
		{
			++m_iJumpDashCount;
			if (m_iJumpDashCount > 700)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"CopyX_JumpDash.wav", CSoundMgr::COPYX_JD);

				m_bJumpDash = true;
				m_bIdle = false;
				m_iJumpDashCount = 0;
			}
		}
		if (!m_bDoom && !m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle && !m_bTackle)
		{
			++m_iShotCount;
			if (m_iShotCount > 257)
			{

				m_bShot = true;
				m_bIdle = false;
				m_iShotCount = 0;
			}
		}
		if (!m_bDoom && !m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle)
		{
			++m_iOverShotCount;
			if (m_iOverShotCount > 1000)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Over.wav", CSoundMgr::COPYX_OVER);

				m_bOverShot = true;
				m_bIdle = false;
				m_iOverShotCount = 0;
			}
		}
		if (!m_bDoom && m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle)
		{
			++m_iJumpShotCount;
			if (m_iJumpShotCount > 750)
			{

				m_bJumpShot = true;
				m_bHover = false;
				m_bIdle = false;
				m_iJumpShotCount = 0;
			}
		}
		if (!m_bDoom && !m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle )
		{
			++m_iHoverCount;
			if (m_iHoverCount > 700)
			{
				m_bHover = true;
				m_bIdle = false;
				m_iHoverCount = 0;
			}
		}
		if (!m_bDoom && !m_bHover && !m_bJumpDash && !m_bShot && !m_bOverShot && !m_bJumpShot && !m_bTackle && !m_bFall)
		{
			++m_iTackleCount;
			if (m_iTackleCount > 320)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"CopyX_Tackle.wav", CSoundMgr::COPYX_TACKLE);
				m_bTackle = true;
				m_bIdle = false;
				m_iTackleCount = 0;
			}
		}
	}
}

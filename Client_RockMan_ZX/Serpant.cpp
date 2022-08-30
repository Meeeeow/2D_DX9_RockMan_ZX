#include "stdafx.h"
#include "Serpant.h"
#include "Gong.h"
#include "GravityBall.h"
#include "SerpantLaser.h"
#include "SerpantWave.h"

CObjectBase * CSerpant::Create_Serpant()
{
	CObjectBase* pObj = new CSerpant;
	pObj->Init_Object();
	return pObj;
}

CSerpant::CSerpant()
{
}


CSerpant::~CSerpant()
{
}

HRESULT CSerpant::Init_Object()
{
	m_tInfo.vPos = { 1050 , 470.f };
	m_tFrame.wstrObjectKey = L"Serpant";
	m_tFrame.wstrStateKey = L"Intro";
	m_eCurMotion = INTRO;
	m_ePreMotion = END;
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 18;
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

	m_bIdle = false;
	m_bBlink = false;
	m_iBlinkCount = 0;
	
	m_bFire = false;
	m_iFireCount = 0;
	
	m_bJump = false;
	m_iJumpCount = 0;

	m_bSlide = false;
	m_iSlideCount = 0;

	m_bUpperCut = false;
	m_iUpperCount = 0;

	m_bMotion = false;
	CRandomMgr::Range(0, 2);
	m_iCount = 0;
	m_iTime = 501;
	return S_OK;
}

int CSerpant::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	if(!m_bBlink)
		m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;
	if (!m_bAction&&m_tInfo.vPos.x - 200.f < m_vTarget.x)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Intro.wav", CSoundMgr::SERPANT_INTRO);
		CSoundMgr::Get_Instance()->PlaySound(L"Warning.wav", CSoundMgr::WARNING);

		m_bAction = true;
	}
	if (m_bAction)
	{

		Action();
		Serpant_Collision();
		HitBox_Update();
		Frame_Check();
		Frame_Update();
	}

	return OBJ_NOEVENT;
}

void CSerpant::Late_Update_Object()
{
	if (m_tFrame.iStartFrame < 2)
	{
		if (m_vTarget.x < m_tInfo.vPos.x)
			m_iDirection = ENEMY_LEFT;
		else if (m_vTarget.x > m_tInfo.vPos.x)
			m_iDirection = ENEMY_RIGHT;
	}
}

void CSerpant::Render_Object()
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

void CSerpant::Release_Object()
{
}

void CSerpant::Default_Hit(int iDmg, RECT HitBox)
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
			CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Hit.wav", CSoundMgr::SERPANT_HIT);
		}
	}
}

void CSerpant::Hit_Recovery()
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

void CSerpant::Serpant_Collision()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(m_tInfo.iDMG, m_tHitBox);
	}
}

void CSerpant::Serpant_Slide()
{
	if (m_tFrame.iStartFrame >= 1 && m_tFrame.iStartFrame <= 7)
	{
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * 3.f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
}

void CSerpant::Serpant_BlinkAttack()
{
	if (m_tFrame.iStartFrame == 0)
	{
		m_tInfo.vPos.x = m_vTarget.x;
		m_tInfo.vPos.y = 300.f;
	}

	if (m_tFrame.iStartFrame > 2 && m_tFrame.iStartFrame < 6)
	{
		m_tInfo.vPos.y += m_tInfo.fSpeed*5.f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
}

void CSerpant::Serpant_JumpAttack()
{
	if (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3)
	{
		m_tInfo.vPos.x -= m_iDirection* m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.y -= m_tInfo.fSpeed*3.f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame == 4 || m_tFrame.iStartFrame == 5)
	{
		m_tInfo.vPos.x -= m_iDirection* m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.y += m_tInfo.fSpeed*3.f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame >= 7 && m_tFrame.iStartFrame <= 10)
	{
		m_tInfo.vPos.x -= m_iDirection* m_tInfo.fSpeed*4 * CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.y -= m_tInfo.fSpeed*3.f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame == 11)
	{
		m_tInfo.vPos.x -= m_iDirection* m_tInfo.fSpeed*4 * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame >= 12 && m_tFrame.iStartFrame <= 15)
	{
		m_tInfo.vPos.x -= m_iDirection* m_tInfo.fSpeed*2 * CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.y += m_tInfo.fSpeed*3.f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
}

void CSerpant::Serpant_Fire()
{
	if (m_tFrame.iStartFrame == 2)
	{
		if (!m_bMotion)
		{
			++m_iCount;
			
			if (m_iCount%2 == 0)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Fire.wav", CSoundMgr::SERPANT_FIRE);
				CEffectMgr::Get_Instance()->Insert_Effect(CGravityBall::Create_Ball(m_tInfo.vPos.x - (40 * m_iDirection)
					, m_tInfo.vPos.y, m_iDirection), CEffectMgr::GRAVITYBALL);
			}
			else if (m_iCount %2 == 1)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Fire.wav", CSoundMgr::SERPANT_FIRE);
				CEffectMgr::Get_Instance()->Insert_Effect(CGong::Create_Gong(m_tInfo.vPos.x - (40 * m_iDirection)
					, m_tInfo.vPos.y, m_iDirection), CEffectMgr::GONG);
			}
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame > 2)
		m_bMotion = false;
}

void CSerpant::Serpant_Uppercut()
{
	if (m_tFrame.iStartFrame >= 2 && m_tFrame.iStartFrame <= 8)
	{
		m_tInfo.vPos.y -= m_tInfo.fSpeed*5.f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame == 4)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CSerpantLaser::Create_Laser(m_tInfo.vPos.x - (40 * m_iDirection), 300, m_iDirection)
				,CEffectMgr::Get_Instance()->SERPANT_LASER);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 5)
	{
		if (m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CSerpantLaser::Create_Laser(m_tInfo.vPos.x - (80 * m_iDirection), 300, m_iDirection)
				, CEffectMgr::Get_Instance()->SERPANT_LASER);
			m_bMotion = false;
		}
	}
	if (m_tFrame.iStartFrame == 6)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CSerpantLaser::Create_Laser(m_tInfo.vPos.x - (120 * m_iDirection), 300, m_iDirection)
				, CEffectMgr::Get_Instance()->SERPANT_LASER);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 7)
	{
		if (m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CSerpantLaser::Create_Laser(m_tInfo.vPos.x - (160 * m_iDirection), 300, m_iDirection)
				, CEffectMgr::Get_Instance()->SERPANT_LASER);
			m_bMotion = false;
		}
	}
	if (m_tFrame.iStartFrame == 9)
	{
		m_tInfo.vPos.x += (m_tInfo.fSpeed * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame >= 10 && m_tFrame.iStartFrame <= 12)
	{
		m_tInfo.vPos.y += m_tInfo.fSpeed * 11.5f * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
}

void CSerpant::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		m_tFrame.iStartFrame = 0;

		switch (m_eCurMotion)
		{
		case CSerpant::INTRO:
			m_tFrame.wstrStateKey = L"Intro";
			m_tFrame.iEndFrame = 18;
			m_tFrame.dwDelay = 100;
			break;
		case CSerpant::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.iEndFrame = 1;
			m_tFrame.dwDelay = 300;
			break;
		case CSerpant::BLINKATTACK:
			m_tFrame.wstrStateKey = L"BlinkAttack";
			m_tFrame.iEndFrame = 8;
			m_tFrame.dwDelay = 120;
			break;
		case CSerpant::UPPERCUT:
			m_tFrame.wstrStateKey = L"Uppercut";
			m_tFrame.iEndFrame = 14;
			m_tFrame.dwDelay = 100;
			break;
		case CSerpant::SLIDE:
			m_tFrame.wstrStateKey = L"Slide";
			m_tFrame.iEndFrame = 9;
			m_tFrame.dwDelay = 100;
			break;
		case CSerpant::FIRE:
			m_tFrame.wstrStateKey = L"Fire";
			m_tFrame.iEndFrame = 11;
			m_tFrame.dwDelay = 100;
			break;
		case CSerpant::JUMPATTACK:
			m_tFrame.wstrStateKey = L"JumpAttack";
			m_tFrame.iEndFrame = 15;
			m_tFrame.dwDelay = 100;
			break;
		case CSerpant::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CSerpant::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_ePreMotion == INTRO)
			{
				m_bBlink = true;
				m_tFrame.wstrStateKey = L"BlinkAttack";
				m_tFrame.iStartFrame = 0;
				return;
			}
			if (!m_bIdle)
				m_bIdle = true;
			if (m_ePreMotion == BLINKATTACK)
				m_bBlink = false;
			if (m_bJump)
				m_bJump = false;
			if (m_bSlide)
				m_bSlide = false;
			if (m_bFire)
				m_bFire = false;
			if (m_bUpperCut)
				m_bUpperCut = false;
			m_tFrame.iStartFrame = 0;
		}
	}
}

void CSerpant::HitBox_Update()
{
	m_tHitBox.left = (LONG)m_tInfo.vPos.x - (25 * 1.5f);
	m_tHitBox.right = (LONG)m_tInfo.vPos.x + (25 * 1.5f);
	m_tHitBox.top = (LONG)m_tInfo.vPos.y - (25 * 1.5f);
	m_tHitBox.bottom = (LONG)m_tInfo.vPos.y + (30 * 1.5f);
}

void CSerpant::Action()
{
	CCollisionMgr::Get_Instance()->WallTileCollision(this);
	float fY = 0.f;
	if (!m_bBlink && !m_bJump && !m_bUpperCut&&CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
	{
		m_tInfo.vPos.y = fY;
	}
	if (!m_bBlink && !m_bJump && !m_bUpperCut&& !CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
	{
		m_tInfo.vPos.y += 10.f;
	}

	if (m_bIdle)
		m_eCurMotion = IDLE;
	if (m_bBlink)
	{
		m_eCurMotion = BLINKATTACK;
		Serpant_BlinkAttack();
	}
	if (m_bFire)
	{
		m_eCurMotion = FIRE;
		Serpant_Fire();
	}
	if (m_bJump)
	{
		m_eCurMotion = JUMPATTACK;
		Serpant_JumpAttack();
	}
	if (m_bSlide)
	{
		m_eCurMotion = SLIDE;
		Serpant_Slide();
	}
	if (m_bUpperCut)
	{
		m_eCurMotion = UPPERCUT;
		Serpant_Uppercut();
	}

	if (!m_bBlink && !m_bFire && !m_bSlide && !m_bJump && !m_bUpperCut)
	{
		++m_iBlinkCount;
		if (m_iBlinkCount > m_iTime)
		{
			m_bIdle = false;
			m_bBlink = true;
			m_bFire = false;
			m_bUpperCut = false;
			m_iBlinkCount = 0;
			m_iTime *= 3.f;
			CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Blink.wav", CSoundMgr::SERPANT_BLINK);
		}
	}
	if (!m_bBlink && !m_bFire && !m_bSlide && !m_bJump && !m_bUpperCut)
	{
		++m_iFireCount;
		if (m_iFireCount > 333)
		{
			m_bIdle = false;
			m_bFire = true;
			m_iFireCount = 0;
		}
	}
	if (!m_bBlink && !m_bFire && !m_bSlide && !m_bJump && !m_bUpperCut)
	{
		++m_iJumpCount;
		if(m_iJumpCount > 1180)
		{
			m_bIdle = false;
			m_bJump = true;
			m_iJumpCount = 0;
			CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Jump.wav", CSoundMgr::SERPANT_JUMP);
		}
	}
	if (!m_bBlink && !m_bFire && !m_bSlide && !m_bJump && !m_bUpperCut)
	{
		++m_iSlideCount;
		if (m_iSlideCount > 277)
		{
			m_bIdle = false;
			m_bSlide = true;
			m_iSlideCount = 0;
			CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Slide.wav", CSoundMgr::SERPANT_SLIDE);

		}
	}
	if (!m_bBlink && !m_bFire && !m_bSlide && !m_bJump && !m_bUpperCut)
	{
		++m_iUpperCount;
		if (m_iUpperCount > 789)
		{
			m_bIdle = false;
			m_bUpperCut = true;
			m_iUpperCount = 0;
			CSoundMgr::Get_Instance()->PlaySound(L"Serpant_Upper.wav", CSoundMgr::SERPANT_UPPER);
		}
	}
}

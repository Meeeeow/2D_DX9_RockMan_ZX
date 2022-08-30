#include "stdafx.h"
#include "FistLeo.h"
#include "Destroy.h"
#include "LeoWall.h"

CObjectBase * CFistLeo::Create_Leo()
{
	CObjectBase* pObj = new CFistLeo;
	pObj->Init_Object();
	return pObj;
}

CFistLeo::CFistLeo()
{
}


CFistLeo::~CFistLeo()
{
}

HRESULT CFistLeo::Init_Object()
{
	m_tInfo.vPos = { 1050 , 470.f };
	m_tFrame.wstrObjectKey = L"Fistleo";
	m_tFrame.wstrStateKey = L"Intro";
	m_eCurMotion = INTRO;
	m_ePreMotion = END;
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 19;
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

	m_bIdle = false;
	m_bDash = false;
	m_iDashCount = 0;

	m_bAttack = false;
	m_iAttackCount = 0;

	m_bFinalAttack = false;
	m_iFinalCount = 0;

	m_bForge = false;

	m_bJumpAttack = false;
	m_iJumpCount = 0;

	m_bWallAttack = false;
	m_iWallCount = 0;

	return S_OK;
}

int CFistLeo::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;
	if (!m_bAction &&m_tInfo.vPos.x - 150.f < m_vTarget.x)
	{
		m_bAction = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Warning.wav", CSoundMgr::WARNING);
		CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Intro.wav", CSoundMgr::FL_INTRO);
	}


	if (m_bAction)
	{
		Action();
		Leo_Collision();
		HitBox_Update();
		Frame_Check();
		Frame_Update();
	}

	return OBJ_NOEVENT;
}

void CFistLeo::Late_Update_Object()
{
	if (!m_bFinalAttack && m_tFrame.iStartFrame < 2)
	{
		if (m_vTarget.x < m_tInfo.vPos.x)
			m_iDirection = ENEMY_LEFT;
		else if (m_vTarget.x > m_tInfo.vPos.x)
			m_iDirection = ENEMY_RIGHT;
	}
}

void CFistLeo::Render_Object()
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

void CFistLeo::Release_Object()
{
}

void CFistLeo::Default_Hit(int iDmg, RECT HitBox)
{
	if (m_bCanHit && !m_bForge)
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &HitBox))
		{
			m_bCanHit = false;
			m_bHit = true;
			int iDamege = iDmg - m_tInfo.iDEF;
			if (iDamege < 1)
				iDamege = 1;
			CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Hit.wav", CSoundMgr::FL_HIT);
			m_tInfo.iCurHP -= iDamege;
			if (m_tInfo.iCurHP < 10)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FL_HIT);
				m_bIdle = false;
				m_bAttack = false;
				m_bDash = false;
				m_bJumpAttack = false;
				m_bWallAttack = false;

				m_bForge = true;
				CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Forge.wav", CSoundMgr::FL_FORGE);
				m_eCurMotion = FORGE;
			}
		}
	}
}

void CFistLeo::Hit_Recovery()
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

void CFistLeo::Leo_Collision()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(m_tInfo.iDMG, m_tHitBox);
	}
}

void CFistLeo::Leo_DashAttack()
{
	if (m_tFrame.iStartFrame > 2 && m_tFrame.iStartFrame < 5)
	{
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * 8.f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
}

void CFistLeo::Leo_Attack()
{
	if (m_tFrame.iStartFrame == 1)
	{
		m_tInfo.vPos.y -= (m_tInfo.fSpeed)*CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * -m_iDirection)*CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame == 2)
	{
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * -m_iDirection)*CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame == 3)
	{
		m_tInfo.vPos.y += (m_tInfo.fSpeed)*CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * -m_iDirection)*CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame > 3)
	{
		m_tInfo.vPos.x -= (m_tInfo.fSpeed*5.f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
}

void CFistLeo::Leo_JumpAttack()
{
	if (m_tFrame.iStartFrame > 2 && m_tFrame.iStartFrame < 10)
	{
		m_tInfo.vPos.y -= (m_tInfo.fSpeed*1.8f)*CTimeMgr::Get_Instance()->Get_DeltaTime();
		m_tInfo.vPos.x -= (m_tInfo.fSpeed * m_iDirection)*CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tFrame.iStartFrame == 10)
		m_tInfo.vPos.y += 2.5f;
}

void CFistLeo::Leo_WallAttack()
{
	if (m_tFrame.iStartFrame == 0)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CLeoWall::Create_Wall(m_tInfo.vPos.x - (25 * m_iDirection), m_iDirection), CEffectMgr::WALL);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 2)
	{
		if (m_bMotion)
		{
			CEffectMgr::Get_Instance()->Wall_Move_Start();
			m_bMotion = false;
		}
	}
	if (m_tFrame.iStartFrame == 3)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CLeoWall::Create_Wall(m_tInfo.vPos.x - (25 * m_iDirection), m_iDirection), CEffectMgr::WALL);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 5)
	{
		if (m_bMotion)
		{
			CEffectMgr::Get_Instance()->Wall_Move_Start();
			m_bMotion = false;
		}
	}
	if (m_tFrame.iStartFrame == 9)
	{
		if (!m_bMotion)
		{
			CEffectMgr::Get_Instance()->Insert_Effect(CLeoWall::Create_Wall(m_tInfo.vPos.x - (25 * m_iDirection), m_iDirection), CEffectMgr::WALL);
			m_bMotion = true;
		}
	}
	if (m_tFrame.iStartFrame == 12)
	{
		if (m_bMotion)
		{
			CEffectMgr::Get_Instance()->Wall_Move_Start();
			m_bMotion = false;
		}
	}
}

void CFistLeo::Leo_FinalAttack()
{
	m_tInfo.vPos.x -= (m_tInfo.fSpeed*3.5f * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
}

void CFistLeo::Leo_Forge()
{
}

void CFistLeo::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		switch (m_eCurMotion)
		{
		case CFistLeo::INTRO:
			m_tFrame.wstrStateKey = L"Intro";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 19;
			m_tFrame.dwDelay = 150;

			break;
		case CFistLeo::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 1;
			m_tFrame.dwDelay = 100;

			break;
		case CFistLeo::ATTACK:
			m_tFrame.wstrStateKey = L"Attack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 9;
			m_tFrame.dwDelay = 130;

			break;
		case CFistLeo::DASHATTACK:
			m_tFrame.wstrStateKey = L"DashAttack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 7;
			m_tFrame.dwDelay = 100;
			break;
		case CFistLeo::JUMPATTACk:
			m_tFrame.wstrStateKey = L"JumpAttack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 10;
			m_tFrame.dwDelay = 130;

			break;
		case CFistLeo::WALLATTACK:
			m_tFrame.wstrStateKey = L"WallAttack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 14;
			m_tFrame.dwDelay = 150;

			break;
		case CFistLeo::FORGE:
			m_tFrame.wstrStateKey = L"Forge";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 23;
			m_tFrame.dwDelay = 150;

			break;
		case CFistLeo::FINALATTACK:
			m_tFrame.wstrStateKey = L"FinalAttack";
			m_tFrame.iStartFrame = 0;
			m_tFrame.iEndFrame = 10;
			m_tFrame.dwDelay = 100;

			break;
		case CFistLeo::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CFistLeo::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_ePreMotion == INTRO)
				m_eCurMotion = IDLE;
			if (m_ePreMotion == FORGE)
				m_eCurMotion = IDLE;
			if (!m_bIdle)
				m_bIdle = true;
			if (m_bAttack)
				m_bAttack = false;
			if (m_bDash)
				m_bDash = false;
			if (m_bJumpAttack)
				m_bJumpAttack = false;
			if (m_bFinalAttack)
			{
				m_bFinalAttack = false;
				m_bForge = false;
			}
			if (m_bWallAttack)
				m_bWallAttack = false;
			m_tFrame.iStartFrame = 0;
		}
	}
}

void CFistLeo::HitBox_Update()
{
	m_tHitBox.left = m_tInfo.vPos.x - 60;
	m_tHitBox.top = m_tInfo.vPos.y - 45;
	m_tHitBox.right = m_tInfo.vPos.x + 60;
	m_tHitBox.bottom = m_tInfo.vPos.y + 40;
}

void CFistLeo::Action()
{
	
	if (m_tInfo.iCurHP < 0)
	{
		CRandomMgr::Range(-2, 2);
		_vec2 Dest = { (float)CRandomMgr::Random(),(float)CRandomMgr::Random() };
		CEffectMgr::Get_Instance()->Insert_Effect(CDestroy::Create_Destroy(m_tInfo.vPos + Dest), CEffectMgr::DESTROY);
		m_bDead = true;
	}
	float fY = 0.f;
	if (!m_bJumpAttack && !m_bAttack && CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
	{
		m_tInfo.vPos.y = fY;
	}
	if (!m_bJumpAttack && !m_bAttack &&!CCollisionMgr::Get_Instance()->TileCollision(this, &fY))
	{
		m_tInfo.vPos.y += 10.0f;
	}
	

	if (m_bIdle && !m_bForge)
		m_eCurMotion = IDLE;
	if (m_bAttack)
	{
		m_eCurMotion = ATTACK;
		Leo_Attack();
	}
	if (m_bDash)
	{
		m_eCurMotion = DASHATTACK;
		Leo_DashAttack();
	}
	if (m_bJumpAttack)
	{
		m_eCurMotion = JUMPATTACk;
		Leo_JumpAttack();
	}
	if (m_bFinalAttack)
	{
		m_eCurMotion = FINALATTACK;
		Leo_FinalAttack();
	}
	if (m_bWallAttack)
	{
		m_eCurMotion = WALLATTACK;
		Leo_WallAttack();
	}

	if (!m_bAttack && !m_bDash && !m_bJumpAttack &&  !m_bFinalAttack && !m_bWallAttack && !m_bForge)
	{
		++m_iAttackCount;
		if (m_iAttackCount > 300)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Attack.wav", CSoundMgr::FL_ATT);
			m_bAttack = true;
			m_bIdle = false;
			m_iAttackCount = 0;
		}
	}
	if (!m_bAttack && !m_bDash && !m_bJumpAttack && !m_bFinalAttack && !m_bWallAttack && !m_bForge)
	{
		++m_iDashCount;
		if (m_iDashCount > 550)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Dash.wav", CSoundMgr::FL_DASH);
			m_bDash = true;
			m_bIdle = false;
			m_iDashCount = 0;
		}
	}
	if (!m_bAttack && !m_bDash && !m_bJumpAttack && !m_bFinalAttack && !m_bWallAttack && !m_bForge)
	{
		++m_iJumpCount;
		if (m_iJumpCount > 900)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Jump.wav", CSoundMgr::FL_JUMP);
			m_bJumpAttack = true;
			m_bIdle = false;
			m_iJumpCount = 0;
		}
	}
	if (!m_bAttack && !m_bDash && !m_bJumpAttack && !m_bFinalAttack && !m_bWallAttack && m_bForge)
	{
		++m_iFinalCount;
		if (m_iFinalCount > 300)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Final.wav", CSoundMgr::FL_FINAL);
			m_bFinalAttack = true;
			m_bIdle = false;
			m_iFinalCount = 0;
		}
	}
	if (!m_bAttack && !m_bDash && !m_bJumpAttack && !m_bFinalAttack && !m_bWallAttack && !m_bForge)
	{
		++m_iWallCount;
		if (m_iWallCount > 1100)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FistLeo_Wall.wav", CSoundMgr::FL_WALL);
			m_bWallAttack = true;
			m_bIdle = false;
			m_iWallCount = 0;
		}
	}


}

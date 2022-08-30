#include "stdafx.h"
#include "Player.h"
// UI
#include "UserInterface.h"
#include "BioMetal.h"
#include "LifeBar.h"

// EFFECT - DEFAULT
#include "DashEffect.h"
#include "DashDust.h"

// EFFECT - ZX
#include "Charge.h"

// EFFECT - HX
#include "HurricaneBall.h"

// EFFECT - FX
#include "FireWall.h"

// EFFECT - PX
#include "Kunai.h"
#include "Suriken.h"
#include "PXCloak.h"

// EFFECT - OX
#include "OXSlash.h"

CObjectBase * CPlayer::Create_Player(UNITINFO _pUnitInfo)
{
	CObjectBase* pObj = new CPlayer;
	static_cast<CPlayer*>(pObj)->Setting_Player(_pUnitInfo);
	pObj->Init_Object();
	return pObj;
}

void CPlayer::Setting_Player(UNITINFO _pUnitInfo)
{
	m_tInfo = _pUnitInfo;
}

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release_Object();
}

HRESULT CPlayer::Init_Object()
{
	m_eCurMotion = IDLE;
	m_ePreMotion = END;

	m_tFrame.wstrObjectKey = L"Aile";
	m_tFrame.wstrStateKey = L"Idle";
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 2;
	m_tFrame.dwDelay = 100;
	m_tFrame.dwTime = GetTickCount();

	m_bSceneChange = false;

	m_bAttack = false;
	m_bMagicUse = false;
	m_bDead = false;
	ZeroMemory(&m_tStatus, sizeof(PLAYERSTATUS));
	m_tStatus.m_bCanAct = true;
	m_tStatus.m_iForm = 0;
	m_iTempForm = 0;
	m_tStatus.m_iMaxFormCount = 5;
	m_tStatus.m_bForm[m_tStatus.m_iForm] = true;
	m_tStatus.m_fJumpPower = 50.f;
	m_tStatus.m_fClimbFallPower = 0.5f;
	m_fOmegaZeroScale = 1.6f;

	m_bMotion = false;
	m_tStatus.m_bCanHit = true;
	m_tStatus.m_bCanPlay = true;

	// UI
	m_pLifeBar = new CLifeBar;
	m_pLifeBar->Init_UI();
	m_pBioMetal = new CBioMetal;
	m_pBioMetal->Init_UI();

	// EFFECT
	m_pCharge = CCharge::Create_Charge();
	m_pDashEffect = CDashEffect::Create_Dash();
	m_pDashDust = CDashDust::Create_Dust();

	m_tTime = 0;
	m_iAlpha = 255;
	m_tGenTime = GetTickCount();
	Form_Setting();
	CRandomMgr::Range(0, 4);
	return S_OK;
}

int CPlayer::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if(!m_bSceneChange)
		Player_CheckLine();

	if(!m_tStatus.m_bFormSelect && !m_tStatus.m_bFormChange && !m_tStatus.m_bHit)
		KeyboardAction();
	HitBbox_Update();
	OffSet();
	Hit_Recovery();
	Debug_Sync();
	static_cast<CLifeBar*>(m_pLifeBar)->Get_Info(m_tInfo);
	if (m_tStatus.m_bFormSelect)
	{
		Form_Select();
		m_pBioMetal->Update_UI();
	}

	Form_Check();

	if (m_tStatus.m_bCanAct)
	{
		Player_Regen();
		Form_Update();
		PlayerAction();
	}
	Frame_Check();
	Frame_Update();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_Object()
{

	if (m_tInfo.vPos.y < 25)
		m_tInfo.vPos.y = 25;

	Player_Motion();
	if(m_tStatus.m_iChargePower > 1000)
		m_pCharge->Late_Update_Effect();

	if (m_tStatus.m_bDash || m_tStatus.m_bAirDash)
	{
		static_cast<CDashEffect*>(m_pDashEffect)->Render_True();
		m_pDashEffect->Late_Update_Effect();
		static_cast<CDashEffect*>(m_pDashEffect)->Set_Pos(m_tInfo.vPos, m_iDirection);
	}
	if (m_tStatus.m_bDash)
	{
		static_cast<CDashDust*>(m_pDashDust)->Render_True();
		m_pDashDust->Late_Update_Effect();
		static_cast<CDashDust*>(m_pDashDust)->Set_Pos(m_tInfo.vPos, m_iDirection);
	}
	if (!m_tStatus.m_bDash && !m_tStatus.m_bAirDash)
	{
		static_cast<CDashEffect*>(m_pDashEffect)->Render_False();
		static_cast<CDashEffect*>(m_pDashEffect)->Start_False();
	}
	if (!m_tStatus.m_bDash)
	{
		static_cast<CDashDust*>(m_pDashDust)->Render_False();
		static_cast<CDashDust*>(m_pDashDust)->Start_False();
	}

}

void CPlayer::Render_Object()
{
	if (m_tStatus.m_bOX)
		Omega_Zero_Render();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tFrame.wstrObjectKey, m_tFrame.wstrStateKey, m_tFrame.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans,matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_iDirection* 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + (int)vScroll.x , m_tInfo.vPos.y +(int)vScroll.y , 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, m_iAlpha));

	/*CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->End();

	CGraphicDeviceMgr::Get_Instance()->Get_Line()->Draw(m_vecRect, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/

	if (m_tStatus.m_bFormSelect)
		m_pBioMetal->Render_UI();
	m_pCharge->Render_Effect();
	m_pDashDust->Render_Effect();
	m_pDashEffect->Render_Effect();

	m_pLifeBar->Render_UI();
}

void CPlayer::Release_Object()
{
	SAFE_DELETE(m_pBioMetal);
	SAFE_DELETE(m_pLifeBar);
	SAFE_DELETE(m_pCharge);
	SAFE_DELETE(m_pDashEffect);
	SAFE_DELETE(m_pDashDust);
}

void CPlayer::Debug_Sync()
{
	if (m_tInfo.iCurHP < 0)
		m_tInfo.iCurHP = 0;
	if (m_tInfo.iCurHP > m_tInfo.iHP)
		m_tInfo.iCurHP = m_tInfo.iHP;
	if (m_tInfo.iCurMP < 0)
		m_tInfo.iCurMP = 0;
	if (m_tInfo.iCurMP > m_tInfo.iMP)
		m_tInfo.iCurMP = m_tInfo.iMP;
}

void CPlayer::Keyboard_Debug_Test()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		++m_tInfo.iHP;
		if (m_tInfo.iHP > 20)
			m_tInfo.iHP = 20;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		++m_tInfo.iMP;
		if (m_tInfo.iMP > 20)
			m_tInfo.iMP = 20;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
	{
		++m_tInfo.iCurHP;
		if (m_tInfo.iCurHP > m_tInfo.iHP)
			m_tInfo.iCurHP = m_tInfo.iHP;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F4))
	{
		++m_tInfo.iCurMP;
		if (m_tInfo.iCurMP > m_tInfo.iMP)
			m_tInfo.iCurMP = m_tInfo.iMP;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD1))
	{
		--m_tInfo.iHP;
		if (m_tInfo.iHP < 0)
			m_tInfo.iHP = 0;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD2))
	{
		--m_tInfo.iMP;
		if (m_tInfo.iMP < 0)
			m_tInfo.iMP = 0;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD3))
	{
		--m_tInfo.iCurHP;
		if (m_tInfo.iCurHP < 0)
			m_tInfo.iCurHP = 0;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD4))
	{
		--m_tInfo.iCurMP;
		if (m_tInfo.iCurMP < 0)
			m_tInfo.iCurMP = 0;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		RECT rc = m_tHitBox;
		Default_Hit(1,rc);
	}
}

void CPlayer::Player_Regen()
{
	
	if (m_tGenTime + 10000 < GetTickCount())
	{
		m_tGenTime = GetTickCount();
		if (m_tStatus.m_bOX)
			return;
		if (m_tInfo.iCurHP == m_tInfo.iHP && m_tInfo.iCurMP == m_tInfo.iMP)
			return;

		++m_tInfo.iCurHP;
		++m_tInfo.iCurMP;
		if (m_tInfo.iCurHP > m_tInfo.iHP)
			m_tInfo.iCurHP = m_tInfo.iHP;
		if (m_tInfo.iCurMP > m_tInfo.iMP)
			m_tInfo.iCurMP = m_tInfo.iMP;
	}
}

bool CPlayer::Map_Change()
{

	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD0))
	{
		m_tInfo.vPos.x = 400.f;
		m_tInfo.vPos.y = 100.f;
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::BOSS);
		return true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD5))
	{
		Pos_Setting(300.f, 100.f);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::AREA_E1);
		return true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD6))
	{
		Pos_Setting(200.f, 400.f);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::AREA_D2);
		return true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD7))
	{
		Pos_Setting(300.f, 100.f);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::FISTLEO_STAGE);
		return true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD8))
	{
		m_tInfo.vPos = { 300.f , 300.f };
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::AREA_D5);
		return true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD9))
	{
		m_tInfo.vPos = { 300.f , 100.f };
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCRAPER);
		return true;
	}

	return false;
}

void CPlayer::Player_CheckLine()
{


	float fY = 0.f;
	bool bTileCollision = CCollisionMgr::Get_Instance()->TileCollision(this, &fY);
	if (bTileCollision && !m_bSceneChange)
	{
		if (m_tStatus.m_bAqua && m_tStatus.m_bLX)
			return;
		Keyboard_Debug_Test();
		if (Map_Change() || m_bSceneChange)
			m_tInfo.vPos.y = 100.f;
		else if (!Map_Change())
			m_tInfo.vPos.y = fY;

		m_tStatus.m_bCanJump = true;
		m_tStatus.m_bJump = false;
		m_tStatus.m_bJumpAttack = false;
		m_tStatus.m_fJumpHeight = m_tInfo.vPos.y;
		m_tStatus.m_fJumpTime = 0.f;
		m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
		m_tStatus.m_fFallTime = 0.f;
		m_tStatus.m_bFall = false;

		m_tStatus.m_bCanAirDash = false;
		m_tStatus.m_bCanHover = false;
		m_tStatus.m_bCanVertDash = false;
		m_tStatus.m_bAirDash = false;

		WallClimbInActive();
		WallClimbLeftInActive();
		WallClimbRightInActive();
	}
	else if (!bTileCollision && !m_bSceneChange)
	{
		if (m_tStatus.m_bAqua && m_tStatus.m_bLX)
		{
			m_tStatus.m_bFall = false;
			m_tStatus.m_bJump = false;
			return;
		}
		m_tStatus.m_bFall = true;
	}
	else if (bTileCollision && m_bSceneChange)
	{
		m_tInfo.vPos.y = 100.f;
	}
	else if (!bTileCollision && m_bSceneChange)
		m_tInfo.vPos.y = 100.f;
	if (!m_tStatus.m_bJump)
		CCollisionMgr::Get_Instance()->WallTileCollision(this);
}

void CPlayer::KeyboardAction()
{
	if(!m_tStatus.m_bBeForm)
		Default_Keyboard();

	if (m_tStatus.m_bHuman)
		Model_Human_Keyboard();
	else if (m_tStatus.m_bZX)
		Model_ZX_Keyboard();
	else if (m_tStatus.m_bHX)
		Model_HX_Keyboard();
	else if (m_tStatus.m_bFX)
		Model_FX_Keyboard();
	else if (m_tStatus.m_bLX)
		Model_LX_Keyboard();
	else if (m_tStatus.m_bPX)
		Model_PX_Keyboard();
}

void CPlayer::Default_Keyboard()
{
	if (!m_tStatus.m_bWallClimb && !(m_tStatus.m_bAqua && m_tStatus.m_bLX) && !m_tStatus.m_bSpecialAttack)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		{
			m_tStatus.m_bIdle = false;
			m_tStatus.m_bWalk = true;
			m_iDirection = OBJ_LEFT;

			if (!m_tStatus.m_bJump && !m_tStatus.m_bDash)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Walk.wav", CSoundMgr::WALK);
				m_eCurMotion = WALK;
			}
			if (m_tStatus.m_bAttackFirst)
				m_tStatus.m_bAttackFirst = false;
			if (m_tStatus.m_bAttackSecond)
				m_tStatus.m_bAttackSecond = false;
			if (m_tStatus.m_bAttackThird)
				m_tStatus.m_bAttackThird = false;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		{
			m_tStatus.m_bIdle = false;
			m_tStatus.m_bWalk = true;
			m_iDirection = OBJ_RIGHT;

			if (!m_tStatus.m_bJump && !m_tStatus.m_bDash)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Walk.wav", CSoundMgr::WALK);
				m_eCurMotion = WALK;
			}
			if (m_tStatus.m_bAttackFirst)
				m_tStatus.m_bAttackFirst = false;
			if (m_tStatus.m_bAttackSecond)
				m_tStatus.m_bAttackSecond = false;
			if (m_tStatus.m_bAttackThird)
				m_tStatus.m_bAttackThird = false;
		}
	}
	if ((CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)) && 
		!(m_tStatus.m_bAqua && m_tStatus.m_bLX))
	{
		m_tStatus.m_bIdle = true;
		m_tStatus.m_bWalk = false;
		m_eCurMotion = IDLE;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S') && !m_tStatus.m_bOX)
	{
		m_tFrame.iStartFrame = 0;
		m_iTempForm = m_tStatus.m_iForm;
		m_tStatus.m_bCanPlay = false;
		m_tStatus.m_bIdle = false;
		m_tStatus.m_bFormSelect = true;
		m_tStatus.m_bCanAct = false;
		CSoundMgr::Get_Instance()->PlaySound(L"FormSelect.wav", CSoundMgr::FORMSELECT);
		m_eCurMotion = FORMSELECT;
	}
	if (!m_tStatus.m_bHuman && !(m_tStatus.m_bAqua && m_tStatus.m_bLX) && !m_tStatus.m_bSpecialAttack)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('Z') && m_tStatus.m_bCanDash && m_tStatus.m_bCanAct && !m_tStatus.m_bJump)
		{
			m_tStatus.m_bIdle = false;
			m_tStatus.m_bWalk = false;
			m_tStatus.m_bDash = true;
			if (!m_tStatus.m_bDashAttack)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WALK);
				CSoundMgr::Get_Instance()->PlaySound(L"Dash.wav", CSoundMgr::DASH);
				m_eCurMotion = DASH;
			}
			if (m_tStatus.m_bDashAttack)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WALK);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH);
				CSoundMgr::Get_Instance()->PlaySound(L"ZX_Third.wav", CSoundMgr::DASH_ATTACK);

				m_eCurMotion = DASH_ATT;
			}

			m_tStatus.m_bAttackFirst = false;
			m_tStatus.m_bAttackSecond = false;
			m_tStatus.m_bAttackThird = false;
		}
		if (CKeyMgr::Get_Instance()->Key_Up('Z'))
		{
			m_tStatus.m_bIdle = true;
			m_tStatus.m_bCanDash = true;
			m_tStatus.m_bDash = false;
			m_tStatus.m_bDashAttack = false;
			m_eCurMotion = IDLE;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH);
			m_tStatus.m_bAttackFirst = false;
			m_tStatus.m_bAttackSecond = false;
			m_tStatus.m_bAttackThird = false;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('X') && m_tStatus.m_bCanJump)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::JUMP);
			CSoundMgr::Get_Instance()->PlaySound(L"Jump.wav", CSoundMgr::JUMP);
			m_tStatus.m_bIdle = false;
			m_tStatus.m_bCanJump = false;
			m_tStatus.m_bJump = true;
		}
	}
}

void CPlayer::Model_Human_Keyboard()
{

}

void CPlayer::Model_ZX_Keyboard()
{
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (m_tStatus.m_bWalk && !m_tStatus.m_bJump)
			return;
		if (m_tStatus.m_bJump && (m_tStatus.m_bDash || !m_tStatus.m_bDash))
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::JUMP_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"ZX_First.wav", CSoundMgr::JUMP_ATTACK);
			m_tStatus.m_bJumpAttack = true;
			return;
		}
		if (m_tStatus.m_bDash && !m_tStatus.m_bJump)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"ZX_First.wav", CSoundMgr::DASH_ATTACK);
			m_tStatus.m_bDashAttack = true;
			return;
		}

		if (!m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
		{
			m_tStatus.m_bAttackFirst = true;
			CSoundMgr::Get_Instance()->PlaySound(L"ZX_First.wav", CSoundMgr::SABER_FIRST);
			m_eCurMotion = ATT_FIRST;
		}
		else if (m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
		{
			m_tStatus.m_bAttackSecond = true;

		}
		else if (m_tStatus.m_bAttackFirst && m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
		{
			m_tStatus.m_bAttackThird = true;
		}

	}
	

	if (CKeyMgr::Get_Instance()->Key_Pressing('F') && m_tStatus.m_bCharging)
		m_tStatus.m_iChargePower += 20;
	if (CKeyMgr::Get_Instance()->Key_Up('F') && m_tStatus.m_iChargePower < 3000)
	{
		m_tStatus.m_iChargePower = 0;
		static_cast<CCharge*>(m_pCharge)->Render_False();
		static_cast<CCharge*>(m_pCharge)->Set_Charge(m_tInfo.vPos, m_iDirection, m_tStatus.m_iChargePower);
	}
	if (CKeyMgr::Get_Instance()->Key_Down('V') && m_tStatus.m_bCharging)
	{
		m_tStatus.m_bCharging = false;
		static_cast<CCharge*>(m_pCharge)->Render_False();
		static_cast<CCharge*>(m_pCharge)->Set_Charge(m_tInfo.vPos, m_iDirection, 0);
		if (m_tStatus.m_iChargePower > 1000)
		{
			m_tStatus.m_bCharge = true;
			m_tStatus.m_iChargePower = 0;
			CSoundMgr::Get_Instance()->PlaySound(L"Charge.wav", CSoundMgr::CHARGE);
			m_eCurMotion = CHARGEATTACK;
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing('F') && !m_tStatus.m_bCharging)
		m_tStatus.m_bCharging = true;
	
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		if (!m_tStatus.m_bOX)
		{
			m_tStatus.m_bOX = true;
			m_tStatus.m_bBeForm = true;
			m_tStatus.m_bCanPlay = false;
			m_tFrame.wstrObjectKey = L"Model OX";
			m_tFrame.iStartFrame = 0;
			m_tFrame.wstrStateKey = L"BeForm";
			m_eCurMotion = BEFORM;
			CSoundMgr::Get_Instance()->PlaySound(L"OX_BeForm.wav", CSoundMgr::OX_BEFORM);
			CEffectMgr::Get_Instance()->Intro_Render_Start();
			return;
		}
		else if (m_tStatus.m_bOX)
		{
			m_tInfo.iCurMP -= 1;
			if (m_tInfo.iCurMP < 0)
			{
				m_tInfo.iCurMP = 0;
				OMEGA_ZERO_Off();
				return;
			}
			m_tStatus.m_bSpecialAttack = true;
			CSoundMgr::Get_Instance()->PlaySound(L"OX_Special.wav", CSoundMgr::OX_SPECIAL);
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down('G') && m_tStatus.m_bOX)
	{
		m_tStatus.m_bOX = false;
		m_tStatus.m_bBeForm = true;
		m_tFrame.wstrObjectKey = L"Model ZX";
		m_tFrame.iStartFrame = 0;
		m_eCurMotion = BEFORM;
	}
}

void CPlayer::Model_HX_Keyboard()
{
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (m_tStatus.m_bWalk && !m_tStatus.m_bJump)
			return;
		if (m_tStatus.m_bJump && !m_tStatus.m_bAirDash)
		{
			m_tStatus.m_bJumpAttack = true;
			return;
		}
		if (m_tStatus.m_bDash && !m_tStatus.m_bJump)
		{
			m_tStatus.m_bDashAttack = true;
			return;
		}
		if (m_tStatus.m_bAirDash)
		{
			m_tStatus.m_bAirDashAttack = true;
			return;
		}

		if (!m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
		{
			m_tStatus.m_bAttackFirst = true;
			CSoundMgr::Get_Instance()->PlaySound(L"ZX_First.wav", CSoundMgr::SABER_FIRST);
			m_eCurMotion = ATT_FIRST;
		}
		else if (m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
			m_tStatus.m_bAttackSecond = true;
		else if (m_tStatus.m_bAttackFirst && m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
			m_tStatus.m_bAttackThird = true;
	}
	if (m_tStatus.m_bJump && !m_tStatus.m_bDash && !m_tStatus.m_bWallClimb)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP) //&& CKeyMgr::Get_Instance()->Key_Pressing('Z')
			&& m_tStatus.m_bCanVertDash && !m_tStatus.m_bAirDash && !m_tStatus.m_bHover)
		{
			m_tStatus.m_bCanAirDash = false;
			m_tStatus.m_bCanHover = false;
			m_tStatus.m_bVertDash = true;
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing('Z') && m_tStatus.m_bCanAirDash
			&& !m_tStatus.m_bHover && !m_tStatus.m_bVertDash)
		{
			m_tStatus.m_bCanHover = false;
			m_tStatus.m_bCanVertDash = false;
			m_tStatus.m_bDash = false;
			m_tStatus.m_bAirDash = true;
			m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
		}
		if (CKeyMgr::Get_Instance()->Key_Up('Z'))
		{
			m_tStatus.m_bAirDash = false;
			m_tStatus.m_bVertDash = false;
			m_tStatus.m_bCanAirDash = false;
			m_tStatus.m_bJump = false;
			m_tStatus.m_bFall = true;
			m_eCurMotion = FALL;
			
		}


		// 질문하기
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN) && m_tStatus.m_bCanHover)
		{
			m_tStatus.m_bCanAirDash = false;
			m_tStatus.m_bCanVertDash = false;
			m_tStatus.m_bHover = true;
			m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
		}
		if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN) && m_tStatus.m_bHover)
		{
			m_tStatus.m_iHoverCount = 0;
			m_tStatus.m_bCanHover = false;
			m_tStatus.m_bHover = false;
			if (!m_tStatus.m_bWallClimb)
			{
				m_tStatus.m_bJump = false;
				m_tStatus.m_bFall = true;
				m_eCurMotion = FALL;
			}
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		m_tInfo.iCurMP -= 1;
		if (m_tInfo.iCurMP < 0)
		{
			m_tInfo.iCurMP = 0;
			return;
		}
		m_tStatus.m_bSpecialAttack = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Special.wav", CSoundMgr::HX_SPECIAL);

		CEffectMgr::Get_Instance()->Insert_Effect(CHurricaneBall::Create_Hurricane(m_tInfo.vPos.x + (50 * m_iDirection)
			, m_tInfo.vPos.y - 5, m_iDirection), CEffectMgr::HX_BALL);
	}
}

void CPlayer::Model_FX_Keyboard()
{
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (m_tStatus.m_bWalk && !m_tStatus.m_bJump)
			return;
		if (m_tStatus.m_bDash)
			return;
		if (m_tStatus.m_bJump)
		{
			m_tStatus.m_bJumpAttack = true;
			return;
		}


		if (!m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FX_First.wav", CSoundMgr::SABER_FIRST);
			m_tStatus.m_bAttackFirst = true;
			m_eCurMotion = ATT_FIRST;
		}
		else if (m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
			m_tStatus.m_bAttackSecond = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		m_tInfo.iCurMP -= 1;
		if (m_tInfo.iCurMP < 0)
		{
			m_tInfo.iCurMP = 0;
			return;
		}
		m_tStatus.m_bSpecialAttack = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Special.wav", CSoundMgr::FX_SPECIAL);

	}
}

void CPlayer::Model_LX_Keyboard()
{
	if (!m_tStatus.m_bAqua)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('C'))
		{
			if (m_tStatus.m_bDash)
				return;
			if (m_tStatus.m_bWalk && !m_tStatus.m_bJump)
				return;
			if (m_tStatus.m_bJump)
			{
				m_tStatus.m_bJumpAttack = true;
				return;
			}
			if (!m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
			{
				m_tStatus.m_bAttackFirst = true;
				CSoundMgr::Get_Instance()->PlaySound(L"ZX_First.wav", CSoundMgr::SABER_FIRST);
				m_eCurMotion = ATT_FIRST;
			}
		}
		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			m_tStatus.m_bSpecialAttack = true;
			CSoundMgr::Get_Instance()->PlaySound(L"Special.wav", CSoundMgr::LX_SPECIAL);

		}
	}
	else if (m_tStatus.m_bAqua)
	{
		// Aqua Moving
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
			{
				m_eCurMotion = AQUA_DASH_VERT;
				m_tStatus.m_bAquaDash = true;
				m_tInfo.vPos.y -= (m_tInfo.fSpeed + 50.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
			}
			if (CKeyMgr::Get_Instance()->Key_Up('Z'))
			{
				m_tStatus.m_bAquaDash = false;
			}
			if (!m_tStatus.m_bAquaDash)
			{
				m_eCurMotion = AQUA_WALK_VERT;
				m_tStatus.m_bAquaWalk = true;
				m_tInfo.vPos.y -= (m_tInfo.fSpeed) * CTimeMgr::Get_Instance()->Get_DeltaTime();
			}
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
			{
				m_eCurMotion = AQUA_DASH_VERT_REV;
				m_tStatus.m_bAquaDash = true;
				m_tInfo.vPos.y += (m_tInfo.fSpeed + 50.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
			}
			if (CKeyMgr::Get_Instance()->Key_Up('Z'))
			{
				m_tStatus.m_bAquaDash = false;
			}
			if (!m_tStatus.m_bAquaDash)
			{
				m_eCurMotion = AQUA_WALK_VERT_REV;
				m_tStatus.m_bAquaWalk = true;
				m_tInfo.vPos.y += (m_tInfo.fSpeed) * CTimeMgr::Get_Instance()->Get_DeltaTime();
			}
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		{
			m_iDirection = OBJ_LEFT;
			m_tStatus.m_bAquaWalk = true;
			if (!m_tStatus.m_bAquaDash)
			{
				m_tInfo.vPos.x += (m_tInfo.fSpeed * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
				m_eCurMotion = AQUA_WALK;
			}

			if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
			{
				m_eCurMotion = AQUA_DASH;
				m_tStatus.m_bAquaDash = true;
				m_tInfo.vPos.x += ((m_tInfo.fSpeed + 100.f) * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
			}
			if (CKeyMgr::Get_Instance()->Key_Up('Z'))
			{
				m_tStatus.m_bAquaDash = false;
			}

			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
				{
					m_eCurMotion = AQUA_DIAG_DASH;
					m_tStatus.m_bAquaDash = true;
					m_tInfo.vPos.y -= (m_tInfo.fSpeed + 50.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				if (CKeyMgr::Get_Instance()->Key_Up('Z'))
				{
					m_tStatus.m_bAquaDash = false;
				}

				if (!m_tStatus.m_bAquaDash)
				{
					m_eCurMotion = AQUA_WALK_DIAG;
					m_tInfo.vPos.y -= m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				return;
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
				{
					m_eCurMotion = AQUA_DIAG_DASH_REV;
					m_tStatus.m_bAquaDash = true;
					m_tInfo.vPos.y += (m_tInfo.fSpeed + 50.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				if (CKeyMgr::Get_Instance()->Key_Up('Z'))
				{
					m_tStatus.m_bAquaDash = false;
				}

				if (!m_tStatus.m_bAquaDash)
				{
					m_eCurMotion = AQUA_WALK_DIAG_REV;
					m_tInfo.vPos.y += m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				return;
			}
		}
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		{
			m_iDirection = OBJ_RIGHT;
			m_tStatus.m_bAquaWalk = true;
			if (!m_tStatus.m_bAquaDash)
			{
				m_tInfo.vPos.x += (m_tInfo.fSpeed * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
				m_eCurMotion = AQUA_WALK;
			}

			if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
			{
				m_eCurMotion = AQUA_DASH;
				m_tStatus.m_bAquaDash = true;
				m_tInfo.vPos.x += ((m_tInfo.fSpeed + 100.f) * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
			}
			if (CKeyMgr::Get_Instance()->Key_Up('Z'))
			{
				m_tStatus.m_bAquaDash = false;
			}

			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
				{
					m_eCurMotion = AQUA_DIAG_DASH;
					m_tStatus.m_bAquaDash = true;
					m_tInfo.vPos.y -= (m_tInfo.fSpeed + 50.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				if (CKeyMgr::Get_Instance()->Key_Up('Z'))
				{
					m_tStatus.m_bAquaDash = false;
				}

				if (!m_tStatus.m_bAquaDash)
				{
					m_eCurMotion = AQUA_WALK_DIAG;
					m_tInfo.vPos.y -= m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				return;
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
				{
					m_eCurMotion = AQUA_DIAG_DASH_REV;
					m_tStatus.m_bAquaDash = true;
					m_tInfo.vPos.y += (m_tInfo.fSpeed + 50.f) * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				if (CKeyMgr::Get_Instance()->Key_Up('Z'))
				{
					m_tStatus.m_bAquaDash = false;
				}

				if (!m_tStatus.m_bAquaDash)
				{
					m_eCurMotion = AQUA_WALK_DIAG_REV;
					m_tInfo.vPos.y += m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
				}
				return;
			}
		}
		if ((CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)
			|| CKeyMgr::Get_Instance()->Key_Up(VK_UP) || CKeyMgr::Get_Instance()->Key_Up(VK_DOWN)))
		{
			m_tStatus.m_bAquaWalk = false;
			m_eCurMotion = AQUA_IDLE;
		}
			
		if (CKeyMgr::Get_Instance()->Key_Down('C'))
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
			{

			}
		}


	}
}

void CPlayer::Model_PX_Keyboard()
{
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (m_tStatus.m_bWalk && !m_tStatus.m_bJump)
			return;
		if (m_tStatus.m_bJump)
		{
			m_tStatus.m_bJumpAttack = true;
			return;
		}
		if (m_tStatus.m_bDash && !m_tStatus.m_bJump)
		{
			m_tStatus.m_bDashAttack = true;
			return;
		}

		if (!m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird)
		{
			m_tStatus.m_bAttackFirst = true;
			++m_tStatus.m_iAttackCount;

			m_eCurMotion = ATT_FIRST;
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		m_tStatus.m_bSpecialAttack = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Special.wav", CSoundMgr::PX_SPECIAL);

		CEffectMgr::Get_Instance()->Insert_Effect(CPXCloak::Create_Cloak(m_tInfo.vPos.x, m_tInfo.vPos.y, m_iDirection,
			this),CEffectMgr::CLOAK);
	}
}

void CPlayer::Player_Motion()
{
	if (m_tStatus.m_bZX)
		ZX_Motion();
	else if (m_tStatus.m_bHX)
		HX_Motion();
	else if (m_tStatus.m_bFX)
		FX_Motion();
	else if (m_tStatus.m_bLX)
		LX_Motion();
	else if (m_tStatus.m_bPX)
		PX_Motion();
}

void CPlayer::ZX_Motion()
{
	if (m_tStatus.m_iChargePower > 1000)
	{
		static_cast<CCharge*>(m_pCharge)->Set_Charge(m_tInfo.vPos, m_iDirection, m_tStatus.m_iChargePower);
		static_cast<CCharge*>(m_pCharge)->Render_True();
	}
	if (m_tStatus.m_bOX)
	{
		if (m_tFrame.wstrStateKey == L"SaberFirst" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG*3, rc);
		}
		if (m_tFrame.wstrStateKey == L"SaberSecond" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG*3, rc);
		}
		if (m_tFrame.wstrStateKey == L"SaberThird" && (m_tFrame.iStartFrame == 4 || m_tFrame.iStartFrame == 5))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 20;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG*3, rc);
		}
		if (m_tFrame.wstrStateKey == L"DashAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ZX_First.wav", CSoundMgr::DASH_ATTACK);
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG*3, rc);
		}
		if (m_tFrame.wstrStateKey == L"JumpAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{

			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG*3, rc);
		}
		if (m_tFrame.wstrStateKey == L"CA" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 30;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG * 5, rc);
		}

		if (m_tFrame.wstrStateKey == L"SpecialAttack" && m_tFrame.iStartFrame == 2)
		{
			if (!m_bMotion)
			{
				m_bMotion = true;
				CEffectMgr::Get_Instance()->Insert_Effect(COXSlash::Create_Slash(m_tInfo.vPos), CEffectMgr::SLASH);
			}
		}
		if (m_tFrame.wstrStateKey == L"SpecialAttack" && m_tFrame.iStartFrame > 2)
			m_bMotion = false;
	}
	if (!m_tStatus.m_bOX)
	{
		if (m_tFrame.wstrStateKey == L"SaberFirst" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right =(LONG) m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right =(LONG) m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
		}
		if (m_tFrame.wstrStateKey == L"SaberSecond" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
		}
		if (m_tFrame.wstrStateKey == L"SaberThird" && (m_tFrame.iStartFrame == 4 || m_tFrame.iStartFrame == 5))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 20;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
		}
		if (m_tFrame.wstrStateKey == L"DashAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
		}
		if (m_tFrame.wstrStateKey == L"JumpAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 10;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
		}
		if (m_tFrame.wstrStateKey == L"CA" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
		{
			RECT rc = {};
			rc.top = (LONG)m_tInfo.vPos.y - 30;
			rc.bottom = (LONG)m_tInfo.vPos.y + 30;
			if (m_iDirection == OBJ_RIGHT)
			{
				rc.left = (LONG)m_tInfo.vPos.x - 10;
				rc.right = (LONG)m_tInfo.vPos.x + 80;
			}
			else
			{
				rc.right = (LONG)m_tInfo.vPos.x + 10;
				rc.left = (LONG)m_tInfo.vPos.x - 80;
			}
			CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG*3, rc);
		}
	}
	if (m_tStatus.m_bOX)
	{
		if (m_tInfo.iCurMP < 0)
		{

		}
	}
}

void CPlayer::HX_Motion()
{
	if (m_tFrame.wstrStateKey == L"SaberFirst" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 10;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 70;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 70;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"SaberSecond" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 10;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 70;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 70;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"SaberThird" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 20;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"DashAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 10;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"AirDashAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 10;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"JumpAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 10;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"CA" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 30;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG * 3, rc);
	}
	if (m_tFrame.wstrStateKey == L"AirDash" && m_tFrame.iStartFrame == 2)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"HX_AirDash.wav", CSoundMgr::AIRDASH);

		if (!m_bMagicUse)
		{
			m_bMagicUse = true;
			m_tInfo.iCurMP -= 1;
			if (m_tInfo.iCurMP < 0)
			{
				m_tInfo.iCurMP = 0;
				m_tStatus.m_bAirDash = false;
				m_tStatus.m_bJump = false;
				m_tStatus.m_bFall = true;
				m_bMagicUse = false;
			}
		}
	}
	if (m_tFrame.wstrStateKey == L"AirDash" && m_tFrame.iStartFrame > 2)
		m_bMagicUse = false;

	if (m_tFrame.wstrStateKey == L"Hover" && m_tFrame.iStartFrame == 1)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"HX_Hover.wav", CSoundMgr::HOVER);
		if (!m_bMagicUse)
		{
			m_bMagicUse = true;
			m_tInfo.iCurMP -= 1;
			if (m_tInfo.iCurMP < 0)
			{
				m_tInfo.iCurMP = 0;
				m_tStatus.m_bHover = false;
				m_tStatus.m_bJump = false;
				m_tStatus.m_bFall = true;
				m_bMagicUse = false;
			}
		}
	}
	if (m_tFrame.wstrStateKey == L"Hover" && m_tFrame.iStartFrame > 1)
		m_bMagicUse = false;

	if (m_tFrame.wstrStateKey == L"VerticalAirDash" && m_tFrame.iStartFrame == 2)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"HX_VertDash.wav", CSoundMgr::VERTDASH);

		if (!m_bMagicUse)
		{
			m_bMagicUse = true;
			m_tInfo.iCurMP -= 1;
			if (m_tInfo.iCurMP < 0)
			{
				m_tInfo.iCurMP = 0;
				m_tStatus.m_bVertDash = false;
				m_tStatus.m_bJump = false;
				m_tStatus.m_bFall = true;
				m_bMagicUse = false;
			}
		}
	}
	if (m_tFrame.wstrStateKey == L"VerticalAirDash" && m_tFrame.iStartFrame > 2)
		m_bMagicUse = false;
}

void CPlayer::FX_Motion()
{
	if (m_tFrame.wstrStateKey == L"SaberFirst" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 60;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 60;
		}
		CObjectMgr::Get_Instance()->FX_Attack(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc, (m_iDirection * 5));
	}
	if (m_tFrame.wstrStateKey == L"SaberSecond" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->FX_Attack(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc, (m_iDirection * 10));
	}

	
	if (m_tFrame.wstrStateKey == L"JumpAttack" && (m_tFrame.iStartFrame == 1 || m_tFrame.iStartFrame == 2))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y - 10;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->FX_Attack(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc, (m_iDirection * 10));
	}
	

	if (m_tFrame.wstrStateKey == L"SpecialAttack" && m_bMotion)
		m_bMotion = false;
	if (m_tFrame.wstrStateKey == L"SpecialAttack" && m_tFrame.iStartFrame == 2)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CEffectMgr::Get_Instance()->Insert_Effect(CFireWall::Create_Fire(m_tInfo.vPos, m_iDirection), CEffectMgr::FX_FIREWALL);
		}
	}
	if (m_tFrame.wstrStateKey == L"SpecialAttack" && m_tFrame.iStartFrame == 9)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CEffectMgr::Get_Instance()->Insert_Effect(CFireWall::Create_Fire(m_tInfo.vPos, -m_iDirection), CEffectMgr::FX_FIREWALL);
		}
	}
}

void CPlayer::LX_Motion()
{
	if (m_tFrame.wstrStateKey == L"SaberFirst" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tFrame.wstrStateKey == L"JumpAttack" && (m_tFrame.iStartFrame == 2 || m_tFrame.iStartFrame == 3))
	{
		RECT rc = {};
		rc.top = (LONG)m_tInfo.vPos.y;
		rc.bottom = (LONG)m_tInfo.vPos.y + 30;
		if (m_iDirection == OBJ_RIGHT)
		{
			rc.left = (LONG)m_tInfo.vPos.x - 10;
			rc.right = (LONG)m_tInfo.vPos.x + 80;
		}
		else
		{
			rc.right = (LONG)m_tInfo.vPos.x + 10;
			rc.left = (LONG)m_tInfo.vPos.x - 80;
		}
		CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(), m_tInfo.iDMG, rc);
	}
	if (m_tStatus.m_bAqua)
	{
		if (m_tTime + 5000 < GetTickCount())
		{
			m_tTime = GetTickCount();
			m_tInfo.iCurMP -= 1;
		}
	}
}

void CPlayer::PX_Motion()
{

	if (m_tFrame.wstrStateKey == L"JumpAttack" && m_tFrame.iStartFrame == 2)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::JUMP_ATTACK);

			CSoundMgr::Get_Instance()->PlaySound(L"PX_First.wav", CSoundMgr::JUMP_ATTACK);
			CEffectMgr::Get_Instance()->Insert_Effect(CKunai::Create_Kunai(m_tInfo.vPos, m_iDirection), CEffectMgr::KUNAI);
		}
	}
	else if (m_tFrame.wstrStateKey == L"JumpAttack" && m_tFrame.iStartFrame > 2)
		m_bMotion = false;

	if (m_tFrame.wstrStateKey == L"DashAttack" && m_tFrame.iStartFrame == 2)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"PX_First.wav", CSoundMgr::DASH_ATTACK);
			CEffectMgr::Get_Instance()->Insert_Effect(CKunai::Create_Kunai(m_tInfo.vPos, m_iDirection), CEffectMgr::KUNAI);
		}
	}
	else if (m_tFrame.wstrStateKey == L"DashAttack" && m_tFrame.iStartFrame > 2)
		m_bMotion = false;

	if (m_tFrame.wstrStateKey == L"SaberFirst" && m_tFrame.iStartFrame == 2 && m_tStatus.m_iAttackCount < 3)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SABER_FIRST);
			CSoundMgr::Get_Instance()->PlaySound(L"PX_First.wav", CSoundMgr::SABER_FIRST);
			CEffectMgr::Get_Instance()->Insert_Effect(CKunai::Create_Kunai(m_tInfo.vPos, m_iDirection), CEffectMgr::KUNAI);
		}
	}
	else if (m_tFrame.wstrStateKey == L"SaberFirst" && m_tFrame.iStartFrame > 2)
		m_bMotion = false;

	if (m_tFrame.wstrStateKey == L"SaberFirst" && m_tFrame.iStartFrame == 2 && m_tStatus.m_iAttackCount >= 3)
	{
		if (!m_bMotion)
		{
			m_bMotion = true;
			m_tStatus.m_iAttackCount = 0;
			m_tInfo.iCurMP -= 1;
			if (m_tInfo.iCurMP < 0)
			{
				m_tInfo.iCurMP = 0;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SABER_FIRST);
				CSoundMgr::Get_Instance()->PlaySound(L"PX_First.wav", CSoundMgr::SABER_FIRST);
				CEffectMgr::Get_Instance()->Insert_Effect(CKunai::Create_Kunai(m_tInfo.vPos, m_iDirection), CEffectMgr::KUNAI);
			}
			else
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SABER_FIRST);
				CSoundMgr::Get_Instance()->PlaySound(L"PX_First.wav", CSoundMgr::SABER_FIRST);
				CEffectMgr::Get_Instance()->Insert_Effect(CSuriken::Create_Suriken(m_tInfo.vPos, m_iDirection), CEffectMgr::SHURIKEN);
			}
		}
	}
	else if (m_tFrame.wstrStateKey == L"SaberFirst" && m_tFrame.iStartFrame > 2)
		m_bMotion = false;

}



void CPlayer::PlayerAction()
{
	if (!m_tStatus.m_bJump && !m_tStatus.m_bJumpAttack && !m_tStatus.m_bDash && !m_tStatus.m_bDashAttack && !m_tStatus.m_bWalk 
		&& !m_tStatus.m_bFormChange && !m_tStatus.m_bFormSelect && !m_tStatus.m_bBeForm 
		&& !m_tStatus.m_bAttackFirst && !m_tStatus.m_bAttackSecond && !m_tStatus.m_bAttackThird && !m_tStatus.m_bFall
		&& !m_tStatus.m_bSpecialAttack && !m_tStatus.m_bAqua && !m_tStatus.m_bCharge && !m_tStatus.m_bHit)
		m_eCurMotion = IDLE;

	if (m_tStatus.m_bHit)
	{
		m_eCurMotion = HIT;
		m_tInfo.vPos.x += (10 * -m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}

	if (m_tStatus.m_bCharge && !m_tStatus.m_bHit)
		m_eCurMotion = CHARGEATTACK;

	if (m_tStatus.m_bLX && m_tStatus.m_bAqua && !m_tStatus.m_bAquaWalk && !m_tStatus.m_bAquaDash)
		m_eCurMotion = AQUA_IDLE;

	if (!m_tStatus.m_bWallClimb && !m_tStatus.m_bSpecialAttack && !m_tStatus.m_bHit)
	{
		if (m_tStatus.m_bWalk && !m_tStatus.m_bDash && !m_tStatus.m_bAirDash)
			m_tInfo.vPos.x += (m_tInfo.fSpeed * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
		else if (m_tStatus.m_bWalk && m_tStatus.m_bDash && !m_tStatus.m_bAirDash)
			m_tInfo.vPos.x += ((m_tInfo.fSpeed + 100.f) * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
		else if (!m_tStatus.m_bWalk && m_tStatus.m_bDash && !m_tStatus.m_bAirDash)
			m_tInfo.vPos.x += ((m_tInfo.fSpeed + 100.f) * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
		else if (m_tStatus.m_bAirDash)
			m_tInfo.vPos.x += ((m_tInfo.fSpeed + 150.f) * m_iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	}
	if (m_tStatus.m_bSpecialAttack)
	{
		CScrollMgr::Get_Instance()->Set_StartQuake();
		m_tStatus.m_bCanPlay = false;
		m_eCurMotion = SPECIALATTACK;
	}
	if (!m_tStatus.m_bSpecialAttack && !m_tStatus.m_bHit)
	{
		if (m_tStatus.m_bJump)
			Jump();
		if (m_tStatus.m_bFall)
			Fall();
		if (m_tStatus.m_bWallClimb)
			WallClimb();
	}
}

void CPlayer::Jump()
{
	float fX = 0.f;
	m_tStatus.m_bFall = false;
	CCollisionMgr::Get_Instance()->WallClimbCollision(this);
	CCollisionMgr::Get_Instance()->FalseWallCollision(this);
	if (!m_tStatus.m_bWallClimb)
	{
		if (!m_tStatus.m_bHover && !m_tStatus.m_bAirDash && !m_tStatus.m_bVertDash)
		{
			if (!m_tStatus.m_bAqua)
			{
				m_tStatus.m_fJumpTime += 0.2f;
				m_tInfo.vPos.y = m_tStatus.m_fJumpHeight - m_tStatus.m_fJumpPower * m_tStatus.m_fJumpTime + 0.5f * GRAVITY * m_tStatus.m_fJumpTime * m_tStatus.m_fJumpTime;
			}
			if (m_tStatus.m_bAqua)
			{
				m_tStatus.m_fJumpTime += 0.05f;
				m_tInfo.vPos.y = m_tStatus.m_fJumpHeight - 70.f * m_tStatus.m_fJumpTime + 0.5f * GRAVITY * m_tStatus.m_fJumpTime * m_tStatus.m_fJumpTime;
			}
		}

		if (m_tStatus.m_bVertDash)
		{
			m_eCurMotion = VERTDASH;
			m_tStatus.m_bWalk = false;
			m_tStatus.m_bDash = false;
			m_tInfo.vPos.y -= 4.5f;
			m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
		}
		
		if (m_tStatus.m_bAirDash && !m_tStatus.m_bAirDashAttack)
			m_eCurMotion = AIRDASH;
		if (m_tStatus.m_bAirDashAttack)
			m_eCurMotion = AIRDASHATT;
		if (m_tStatus.m_bHover)
			m_eCurMotion = HOVER;
		if (m_tStatus.m_bJumpAttack)
			m_eCurMotion = JUMP_ATT;

		if (m_tStatus.m_bJump && !m_tStatus.m_bAirDash && !m_tStatus.m_bAirDashAttack
			&& !m_tStatus.m_bVertDash && !m_tStatus.m_bHover && !m_tStatus.m_bJumpAttack)
			m_eCurMotion = JUMP;

		if (m_tStatus.m_bHX)
		{
			m_tStatus.m_bCanAirDash = true;
			m_tStatus.m_bCanHover = true;
			m_tStatus.m_bCanVertDash = true;
		}
	}
	else
	{
		m_tStatus.m_fJumpTime = 0.f;
		m_tStatus.m_fJumpHeight = m_tInfo.vPos.y;
	}

}

void CPlayer::Fall()
{
	CCollisionMgr::Get_Instance()->WallClimbCollision(this);
	CCollisionMgr::Get_Instance()->FalseWallCollision(this);
	m_tStatus.m_fJumpHeight = m_tInfo.vPos.y;
	if (!m_tStatus.m_bWallClimb)
	{
		m_eCurMotion = FALL;
		m_tStatus.m_bJump = false;
		if (!m_tStatus.m_bAqua)
		{
			m_tStatus.m_fFallTime += 0.2f;
			m_tInfo.vPos.y = m_tStatus.m_fFallHeight - 0 * m_tStatus.m_fFallTime + 0.5f * GRAVITY * m_tStatus.m_fFallTime * m_tStatus.m_fFallTime;
		}
		else if (m_tStatus.m_bAqua)
		{
			m_tStatus.m_fFallTime += 0.05f;
			m_tInfo.vPos.y = m_tStatus.m_fFallHeight - 0 * m_tStatus.m_fFallTime + 0.5f * GRAVITY * m_tStatus.m_fFallTime * m_tStatus.m_fFallTime;
		}
	}
	else
	{
		m_tStatus.m_fFallTime = 0;
		m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
	}
}

void CPlayer::WallClimb()
{
	m_eCurMotion = WALLCLIMB;
	m_tStatus.m_bJump = false;
	m_tStatus.m_bCanJump = true;
	m_tInfo.vPos.y += 0.2f;
	m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
	m_tStatus.m_fJumpHeight = m_tInfo.vPos.y;
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) && m_tStatus.m_bWallClimbRight)
	{
		Set_PosX(-10);
		m_tStatus.m_bJump = false;
		m_tStatus.m_bFall = true;
		WallClimbRightInActive();
		WallClimbLeftInActive();
		WallClimbInActive();
		HitBbox_Update();
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT) && m_tStatus.m_bWallClimbRight)
	{
		Set_PosX(0);
		return;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT) && m_tStatus.m_bWallClimbLeft && !m_tStatus.m_bWallClimbRight)
	{
		Set_PosX(10);
		m_tStatus.m_bJump = false;
		m_tStatus.m_bFall = true;
		WallClimbRightInActive();
		WallClimbLeftInActive();
		WallClimbInActive();
		HitBbox_Update();
		return;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('X') && m_tStatus.m_bWallClimbLeft)
	{
		Set_PosX(10);
		m_tStatus.m_bFall = false;
		m_tStatus.m_bJump = true;
		WallClimbRightInActive();
		WallClimbLeftInActive();
		WallClimbInActive();
		HitBbox_Update();
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('X') && m_tStatus.m_bWallClimbRight)
	{
		Set_PosX(-10);
		m_tStatus.m_bFall = false;
		m_tStatus.m_bJump = true;
		WallClimbRightInActive();
		WallClimbLeftInActive();
		WallClimbInActive();
		HitBbox_Update();
		return;
	}

}

void CPlayer::OffSet()
{
	/*int iOffSetX = (WINCX / 2);
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_Scroll().x;

	if (iOffSetX > (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetX - (m_tInfo.vPos.x + iScrollX));
	if (iOffSetX < (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetX - (m_tInfo.vPos.x + iScrollX));*/
	
	int iOffSetXMin = (WINCX / 2) - 200;
	int iOffSetXMax = (WINCX / 2) + 200;
	int iOffSetX = (WINCX / 2);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffSetXMin > (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetXMin - (m_tInfo.vPos.x + iScrollX));
	if (iOffSetXMax < (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetXMax - (m_tInfo.vPos.x + iScrollX));
	ShowScrollSetting();
}

void CPlayer::ShowScrollSetting()
{
	int iOffSetXMin = (WINCX / 2) - 200;
	int iOffSetXMax = (WINCX / 2) + 200;
	int iOffSetX = (WINCX / 2);
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ShowScroll().x;

	if (iOffSetX > (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ShowScrollX(iOffSetX - (m_tInfo.vPos.x + iScrollX));
	if (iOffSetX < (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ShowScrollX(iOffSetX - (m_tInfo.vPos.x + iScrollX));
	

	/*int iOffSetXMin = (WINCX / 2) - 200;
	int iOffSetXMax = (WINCX / 2) + 200;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ShowScroll().x;

	if (iOffSetXMax > (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetXMax - (m_tInfo.vPos.x + iScrollX));
	if (iOffSetXMin < (int)m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetXMin - (m_tInfo.vPos.x + iScrollX));*/
}

void CPlayer::OMEGA_ZERO_Off()
{
	m_tStatus.m_bOX = false;
	m_tStatus.m_bBeForm = true;
	m_tFrame.iStartFrame = 0;
	m_eCurMotion = BEFORM;
	m_tFrame.wstrObjectKey = L"Model ZX";
	
}

void CPlayer::Default_Hit(int iDmg, RECT HitBox)
{
	if (m_tStatus.m_bCanHit)
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &HitBox))
		{
			m_tStatus.m_bCanHit = false;
			m_tStatus.m_bHit = true;
			int iDamege = iDmg - m_tInfo.iDEF;
			if (iDamege < 1)
				iDamege = 1;
			m_tInfo.iCurHP -= iDamege;
			CSoundMgr::Get_Instance()->PlaySound(L"Hit.wav", CSoundMgr::HIT);
		}
	}
}

void CPlayer::Hit_Recovery()
{
	if (!m_tStatus.m_bCanHit)
	{
		m_iAlpha = 10;
		++m_tStatus.m_iHitCount;
		m_iAlpha = 180;
		if (m_tStatus.m_iHitCount > 150)
		{
			m_iAlpha = 255;
			m_tStatus.m_bCanHit = true;
			m_tStatus.m_iHitCount = 0;
		}
	}
}

void CPlayer::Form_Check()
{
	if (m_tStatus.m_bHuman)
	{
		m_tInfo.fSpeed = 70.f;
		m_tStatus.m_fJumpPower = 50.f;

		if (m_tFrame.wstrStateKey == L"BeForm")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Idle")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"Walk")
			m_tFrame.iEndFrame = 9;
		if (m_tFrame.wstrStateKey == L"Hit")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"FormSelect")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"FormChange")
			m_tFrame.iEndFrame = 11;
	}
	else if (m_tStatus.m_bZX)
	{
		if (!m_tStatus.m_bOX)
		{
			m_tInfo.fSpeed = 85;
			m_tStatus.m_fJumpPower = 51.f;
		}
		else if (m_tStatus.m_bOX)
		{
			m_tInfo.fSpeed = 130;
			m_tStatus.m_fJumpPower = 54.f;
		}


		if (m_tFrame.wstrStateKey == L"BeForm")
		{
			if (!m_tStatus.m_bOX)
			{
				m_tFrame.dwDelay = 50;
				m_tFrame.iEndFrame = 8;
			}
			else
			{
				m_tFrame.dwDelay = 50;
				m_tFrame.iEndFrame = 15;
			}
		}
		if (m_tFrame.wstrStateKey == L"Idle")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Walk")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Hit")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"FormSelect")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"FormChange")
			m_tFrame.iEndFrame = 7;

		if (m_tFrame.wstrStateKey == L"Dash")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"DashAttack")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Jump")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"JumpAttack")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"Fall")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"WallClimb")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"WallClimbJump")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"CA")
			m_tFrame.iEndFrame = 11;

		if (m_tFrame.wstrStateKey == L"SaberFirst")
		{
			m_tFrame.dwDelay = 40;
			m_tFrame.iEndFrame = 7;
		}
		if (m_tFrame.wstrStateKey == L"SaberSecond")
		{
			if (!m_tStatus.m_bOX)
			{
				m_tFrame.dwDelay = 40;
				m_tFrame.iEndFrame = 10;
			}
			else
			{
				m_tFrame.dwDelay = 30;
				m_tFrame.iEndFrame = 6;
			}
		}
		if (m_tFrame.wstrStateKey == L"SaberThird")
		{
			m_tFrame.dwDelay = 50;
			m_tFrame.iEndFrame = 12;
		}
		if (m_tFrame.wstrObjectKey == L"SpecialAttack" && m_tStatus.m_bOX)
		{
			m_tFrame.dwDelay = 50;
			m_tFrame.iEndFrame = 10;
		}

	}
	else if (m_tStatus.m_bHX)
	{
		m_tInfo.fSpeed = 120;
		m_tStatus.m_fJumpPower = 55.f;

		if (m_tFrame.wstrStateKey == L"BeForm")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Idle")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"Walk")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Hit")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"FormSelect")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"FormChange")
			m_tFrame.iEndFrame = 7;

		if (m_tFrame.wstrStateKey == L"Dash")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"DashAttack")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Jump")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"JumpAttack")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"Fall")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"WallClimb")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"WallClimbJump")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"SpecialAttack")
			m_tFrame.iEndFrame = 5;

		if (m_tFrame.wstrStateKey == L"SaberFirst")
		{
			m_tFrame.dwDelay = 30;
			m_tFrame.iEndFrame = 11;
		}
		if (m_tFrame.wstrStateKey == L"SaberSecond")
		{
			m_tFrame.dwDelay = 40;
			m_tFrame.iEndFrame = 11;
		}
		if (m_tFrame.wstrStateKey == L"SaberThird")
		{
			m_tFrame.dwDelay = 50;
			m_tFrame.iEndFrame = 12;
		}

		/* Model HX */
		if (m_tFrame.wstrStateKey == L"AirDash")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"AirDashAttack")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"Hover")
			m_tFrame.iEndFrame = 4;
		if (m_tFrame.wstrStateKey == L"VerticalAirDash")
			m_tFrame.iEndFrame = 9;
	}
	else if (m_tStatus.m_bFX)
	{
		m_tInfo.fSpeed = 80;
		m_tStatus.m_fJumpPower = 48.f;

		if (m_tFrame.wstrStateKey == L"BeForm")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Idle")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"Walk")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Hit")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"FormSelect")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"FormChange")
			m_tFrame.iEndFrame = 7;

		if (m_tFrame.wstrStateKey == L"Dash")
			m_tFrame.iEndFrame = 4;
		if (m_tFrame.wstrStateKey == L"Jump")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"JumpAttack")
			m_tFrame.iEndFrame = 3;
		if (m_tFrame.wstrStateKey == L"Fall")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"WallClimb")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"WallClimbJump")
			m_tFrame.iEndFrame = 2;

		if (m_tFrame.wstrStateKey == L"SaberFirst")
		{
			m_tFrame.dwDelay = 100;
			m_tFrame.iEndFrame = 7;
		}
		if (m_tFrame.wstrStateKey == L"SaberSecond")
		{
			m_tFrame.dwDelay = 150;
			m_tFrame.iEndFrame = 6;
		}

		if (m_tFrame.wstrStateKey == L"SpecialAttack")
		{
			m_tFrame.dwDelay = 150;
			m_tFrame.iEndFrame = 13;
		}

	}
	else if (m_tStatus.m_bLX)
	{
		m_tInfo.fSpeed = 99;
		m_tStatus.m_fJumpPower = 51.5f;

		if (m_tFrame.wstrStateKey == L"BeForm")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Idle")
			m_tFrame.iEndFrame = 4;
		if (m_tFrame.wstrStateKey == L"Walk")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Hit")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"FormSelect")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"FormChange")
			m_tFrame.iEndFrame = 7;

		if (m_tFrame.wstrStateKey == L"Dash")
			m_tFrame.iEndFrame = 4;
		if (m_tFrame.wstrStateKey == L"Jump")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"JumpAttack")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Fall")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"WallClimb")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"WallClimbJump")
			m_tFrame.iEndFrame = 2;

		if (m_tFrame.wstrStateKey == L"SaberFirst")
		{
			m_tFrame.dwDelay = 100;
			m_tFrame.iEndFrame = 9;
		}
		if (m_tFrame.wstrStateKey == L"SpecialAttack")
		{
			m_tFrame.dwDelay = 150;
			m_tFrame.iEndFrame = 10;
		}

		// Aqua Attack
		if (m_tFrame.wstrStateKey == L"AA")
			m_tFrame.iEndFrame = 10;

		// Aqua Idle
		if (m_tFrame.wstrStateKey == L"AI")
			m_tFrame.iEndFrame = 10;

		// Aqua Walk
		if (m_tFrame.wstrStateKey == L"AW")
			m_tFrame.iEndFrame = 5;
		// Aqua Dash
		if (m_tFrame.wstrStateKey == L"AD")
			m_tFrame.iEndFrame = 4;

		// Aqua Walk Diagonal
		if (m_tFrame.wstrStateKey == L"AWD")
			m_tFrame.iEndFrame = 5;
		// Aqua Dash Diagonal
		if (m_tFrame.wstrStateKey == L"ADD")
			m_tFrame.iEndFrame = 4;

		// Aqua Walk Reverse Diagonal
		if (m_tFrame.wstrStateKey == L"AWRD")
			m_tFrame.iEndFrame = 5;
		// Aqua Dash Reverse Diagonal
		if (m_tFrame.wstrStateKey == L"ADRD")
			m_tFrame.iEndFrame = 4;

		// Aqua Walk Vert
		if (m_tFrame.wstrStateKey == L"AWV")
			m_tFrame.iEndFrame = 5;
		// Aqua Vert Dash
		if (m_tFrame.wstrStateKey == L"AVD")
			m_tFrame.iEndFrame = 4;

		// Aqua Walk Reverse Vert
		if (m_tFrame.wstrStateKey == L"AWRV")
			m_tFrame.iEndFrame = 5;
		// Aqua Reverse Vert Dash
		if (m_tFrame.wstrStateKey == L"ARVD")
			m_tFrame.iEndFrame = 4;

	}
	else if (m_tStatus.m_bPX)
	{
		m_tInfo.fSpeed = 130;
		m_tStatus.m_fJumpPower = 47.5f;

		if (m_tFrame.wstrStateKey == L"BeForm")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Idle")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"Walk")
			m_tFrame.iEndFrame = 10;
		if (m_tFrame.wstrStateKey == L"Hit")
			m_tFrame.iEndFrame = 2;
		if (m_tFrame.wstrStateKey == L"FormSelect")
			m_tFrame.iEndFrame = 8;
		if (m_tFrame.wstrStateKey == L"FormChange")
			m_tFrame.iEndFrame = 7;

		if (m_tFrame.wstrStateKey == L"Dash")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"DashAttack")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"Jump")
			m_tFrame.iEndFrame = 6;
		if (m_tFrame.wstrStateKey == L"JumpAttack")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"Fall")
			m_tFrame.iEndFrame = 5;
		if (m_tFrame.wstrStateKey == L"WallClimb")
			m_tFrame.iEndFrame = 7;
		if (m_tFrame.wstrStateKey == L"WallClimbJump")
			m_tFrame.iEndFrame = 2;

		if (m_tFrame.wstrStateKey == L"SaberFirst")
		{
			m_tFrame.dwDelay = 70;
			m_tFrame.iEndFrame = 6;
		}

		if (m_tFrame.wstrStateKey == L"SpecialAttack")
		{
			m_tFrame.dwDelay = 150;
			m_tFrame.iEndFrame = 8;
		}
	}
}

void CPlayer::Form_Update()
{
}

void CPlayer::Form_Setting()
{
	if (m_tStatus.m_bForm[0])
	{
		//Alie
		static_cast<CLifeBar*>(m_pLifeBar)->Get_Form(0);
		m_tFrame.wstrObjectKey = L"Aile";

		m_tStatus.m_bHuman = true;
		m_tStatus.m_bZX = false;
		m_tStatus.m_bHX = false;
		m_tStatus.m_bFX = false;
		m_tStatus.m_bLX = false;
		m_tStatus.m_bPX = false;
		

		m_tStatus.m_bCanAirDash = false;
		m_tStatus.m_bCanHover = false;
		m_tStatus.m_bCanVertDash = false;
	}
	else if (m_tStatus.m_bForm[1])
	{
		//Model ZX
	
		static_cast<CLifeBar*>(m_pLifeBar)->Get_Form(1);
		if (!m_tStatus.m_bOX)
			m_tFrame.wstrObjectKey = L"Model ZX";
		else if (m_tStatus.m_bOX)
			m_tFrame.wstrObjectKey = L"Model OX";

		m_tStatus.m_bHuman = false;
		m_tStatus.m_bZX = true;
		m_tStatus.m_bHX = false;
		m_tStatus.m_bFX = false;
		m_tStatus.m_bLX = false;
		m_tStatus.m_bPX = false;

	}
	else if (m_tStatus.m_bForm[2])
	{
		//Model HX
		static_cast<CLifeBar*>(m_pLifeBar)->Get_Form(2);
		m_tFrame.wstrObjectKey = L"Model HX";

		m_tStatus.m_bHuman = false;
		m_tStatus.m_bZX = false;
		m_tStatus.m_bHX = true;
		m_tStatus.m_bFX = false;
		m_tStatus.m_bLX = false;
		m_tStatus.m_bPX = false;
		

	}
	else if (m_tStatus.m_bForm[3])
	{
		//Model FX
		static_cast<CLifeBar*>(m_pLifeBar)->Get_Form(3);
		m_tFrame.wstrObjectKey = L"Model FX";

		m_tStatus.m_bHuman = false;
		m_tStatus.m_bZX = false;
		m_tStatus.m_bHX = false;
		m_tStatus.m_bFX = true;
		m_tStatus.m_bLX = false;
		m_tStatus.m_bPX = false;


	}
	else if (m_tStatus.m_bForm[4])
	{
		//Model LX
		static_cast<CLifeBar*>(m_pLifeBar)->Get_Form(4);
		m_tFrame.wstrObjectKey = L"Model LX";

		m_tStatus.m_bHuman = false;
		m_tStatus.m_bZX = false;
		m_tStatus.m_bHX = false;
		m_tStatus.m_bFX = false;
		m_tStatus.m_bLX = true;
		m_tStatus.m_bPX = false;


	}
	else if (m_tStatus.m_bForm[5])
	{
		//Model PX
		static_cast<CLifeBar*>(m_pLifeBar)->Get_Form(5);
		m_tFrame.wstrObjectKey = L"Model PX";

		m_tStatus.m_bHuman = false;
		m_tStatus.m_bZX = false;
		m_tStatus.m_bHX = false;
		m_tStatus.m_bFX = false;
		m_tStatus.m_bLX = false;
		m_tStatus.m_bPX = true;


	}
}

void CPlayer::Form_Select()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FORMMOVE);
		CSoundMgr::Get_Instance()->PlaySound(L"FormMove.wav", CSoundMgr::FORMMOVE);
		++m_tStatus.m_iForm;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FORMMOVE);
		CSoundMgr::Get_Instance()->PlaySound(L"FormMove.wav", CSoundMgr::FORMMOVE);
		--m_tStatus.m_iForm;
	}

	if (m_tStatus.m_iForm > 5)
		m_tStatus.m_iForm = 0;
	if (m_tStatus.m_iForm < 0)
		m_tStatus.m_iForm = 5;

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		m_tFrame.iStartFrame = 0;
		m_tStatus.m_iForm = m_iTempForm;
		m_tStatus.m_bCanPlay = true;
		m_tStatus.m_bFormSelect = false;
		m_tStatus.m_bIdle = true;
		m_tStatus.m_bCanAct = true;
		m_eCurMotion = IDLE;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		m_tFrame.iStartFrame = 0;
		if (m_tStatus.m_iForm == m_iTempForm)
		{
			m_tStatus.m_bCanPlay = true;
			m_tStatus.m_bFormSelect = false;
			m_tStatus.m_bIdle = true;
			m_tStatus.m_bCanAct = true;
			return;
		}
		m_tStatus.m_bFormSelect = false;
		m_tStatus.m_bFormChange = true;
		m_eCurMotion = FORMCHANGE;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BEFORM);
		CSoundMgr::Get_Instance()->PlaySound(L"LockOn.wav", CSoundMgr::BEFORM);
	}

}

void CPlayer::Frame_Check()
{
	if (m_eCurMotion != m_ePreMotion)
	{
		switch (m_eCurMotion)
		{
		case CPlayer::IDLE:
			m_tFrame.wstrStateKey = L"Idle";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::WALK:
			m_tFrame.wstrStateKey = L"Walk";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::DASH:
			m_tFrame.wstrStateKey = L"Dash";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::JUMP:
			m_tFrame.wstrStateKey = L"Jump";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::FALL:
			m_tFrame.wstrStateKey = L"Fall";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::WALLCLIMB:
			m_tFrame.wstrStateKey = L"WallClimb";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::WALLCLIMBJUMP:
			m_tFrame.wstrStateKey = L"WallClimbJump";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::HIT:
			m_tFrame.wstrStateKey = L"Hit";
			m_tFrame.dwDelay = 200;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::FORMCHANGE:
			m_tFrame.wstrStateKey = L"FormChange";
			m_tFrame.dwDelay = 50;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::FORMSELECT:
			m_tFrame.wstrStateKey = L"FormSelect";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::BEFORM:
			m_tFrame.wstrStateKey = L"BeForm";
			m_tFrame.dwDelay = 50;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::ATT_FIRST:
			m_tFrame.wstrStateKey = L"SaberFirst";
			m_tFrame.dwDelay = 20;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::ATT_SECOND:
			m_tFrame.wstrStateKey = L"SaberSecond";
			m_tFrame.dwDelay = 30;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::ATT_THIRD:
			m_tFrame.wstrStateKey = L"SaberThird";
			m_tFrame.dwDelay = 40;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::DASH_ATT:
			m_tFrame.wstrStateKey = L"DashAttack";
			m_tFrame.dwDelay = 50;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::JUMP_ATT:
			m_tFrame.wstrStateKey = L"JumpAttack";
			m_tFrame.dwDelay = 50;
			m_tFrame.iStartFrame = 0;
			break;
			/* Special Attack*/
		case CPlayer::SPECIALATTACK:
			m_tFrame.wstrStateKey = L"SpecialAttack";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::CHARGEATTACK:
			m_tFrame.wstrStateKey = L"CA";
			m_tFrame.dwDelay = 30;
			m_tFrame.iStartFrame = 0;
			break;
			/* Model HX */
		case CPlayer::AIRDASH:
			m_tFrame.wstrStateKey = L"AirDash";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AIRDASHATT:
			m_tFrame.wstrStateKey = L"AirDashAttack";
			m_tFrame.dwDelay = 50;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::VERTDASH:
			m_tFrame.wstrStateKey = L"VerticalAirDash";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::HOVER:
			m_tFrame.wstrStateKey = L"Hover";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
			/* Model FX */
			//
			/* Model LX */
		case CPlayer::AQUA_IDLE:
			m_tFrame.wstrStateKey = L"AI";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_WALK:
			m_tFrame.wstrStateKey = L"AW";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_WALK_VERT:
			m_tFrame.wstrStateKey = L"AWV";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_WALK_VERT_REV:
			m_tFrame.wstrStateKey = L"AWRV";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_DASH:
			m_tFrame.wstrStateKey = L"AD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_DASH_VERT:
			m_tFrame.wstrStateKey = L"AVD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_DASH_VERT_REV:
			m_tFrame.wstrStateKey = L"ARVD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_WALK_DIAG:
			m_tFrame.wstrStateKey = L"AWD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_DIAG_DASH:
			m_tFrame.wstrStateKey = L"ADD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_WALK_DIAG_REV:
			m_tFrame.wstrStateKey = L"AWRD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_DIAG_DASH_REV:
			m_tFrame.wstrStateKey = L"ADRD";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
		case CPlayer::AQUA_ATT:
			m_tFrame.wstrStateKey = L"AA";
			m_tFrame.dwDelay = 100;
			m_tFrame.iStartFrame = 0;
			break;
			/* Model PX */
			//
			/* END */
		case CPlayer::END:
			break;
		default:
			break;
		}
		m_ePreMotion = m_eCurMotion;
	}
}

void CPlayer::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			if (m_tStatus.m_bFormChange)
			{
				m_tStatus.m_bFormChange = false;
				m_tStatus.m_bForm[m_tStatus.m_iForm] = true;
				m_tStatus.m_bForm[m_iTempForm] = false;
				Form_Setting();
				m_tFrame.iStartFrame = 0;
				m_tStatus.m_bCanAct = true;
				if (!m_tStatus.m_bJump && !m_tStatus.m_bFall)
				{
					m_eCurMotion = BEFORM;
					m_tStatus.m_bBeForm = true;
					return;
				}
				if (m_tStatus.m_bFall && !m_tStatus.m_bJump)
				{
					m_eCurMotion = FALL;
					m_tStatus.m_bCanPlay = true;
					return;
				}
				else if (m_tStatus.m_bJump && !m_tStatus.m_bFall)
				{
					m_tStatus.m_bCanPlay = true;
					m_eCurMotion = JUMP;
					return;
				}
				else if (m_tStatus.m_bJump && m_tStatus.m_bFall)
				{
					m_tStatus.m_bCanPlay = true;
					m_eCurMotion = FALL;
					return;
				}
			}
			if (m_tStatus.m_bHit)
			{
				m_tStatus.m_bHit = false;
				m_tFrame.iStartFrame = 0;
				if (m_tStatus.m_bJump)
				{
					m_tStatus.m_fFallHeight = m_tInfo.vPos.y;
					m_tStatus.m_bJump = false;
					m_tStatus.m_bFall = true;
					m_eCurMotion = FALL;
					return;
				}
				if (m_tStatus.m_bFall)
				{
					m_eCurMotion = FALL;
					return;
				}
				m_eCurMotion = IDLE;
				return;
			}
			if (m_tStatus.m_bWallClimb)
			{
				m_tFrame.iStartFrame = 4;
				return;
			}
			if (m_tStatus.m_bCharge)
			{
				m_tStatus.m_bCharge = false;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = IDLE;
				return;
			}
			if (m_tStatus.m_bSpecialAttack)
			{
				m_tStatus.m_bSpecialAttack = false;
				m_tStatus.m_bCanPlay = true;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = IDLE;
				if (m_tStatus.m_bLX)
				{
					m_tStatus.m_bAqua = true;
					m_tTime = GetTickCount();
				}
				return;
			}
			if (m_tStatus.m_bAirDashAttack)
			{
				m_tStatus.m_bAirDash = false;
				m_tStatus.m_bAirDashAttack = false;
				m_tFrame.iStartFrame = 0;
				if (!m_tStatus.m_bWallClimb)
				{
					m_tStatus.m_bJump = false;
					m_tStatus.m_bFall = true;
					m_eCurMotion = FALL;
				}
				return;
			}
			if (m_tStatus.m_bVertDash)
			{
				m_tStatus.m_bCanVertDash = false;
				m_tStatus.m_bVertDash = false;
				m_tFrame.iStartFrame = 0;
				if (!m_tStatus.m_bWallClimb)
				{
					m_tStatus.m_bJump = false;
					m_tStatus.m_bFall = true;
					m_eCurMotion = FALL;
				}
				return;
			}
			if (m_tStatus.m_bHover)
			{
				m_tFrame.iStartFrame = 1;
				++m_tStatus.m_iHoverCount;
				if (m_tStatus.m_iHoverCount > 4)
				{
					m_tFrame.iStartFrame = 0;
					m_tStatus.m_iHoverCount = 0;
					m_tStatus.m_bCanHover = false;
					m_tStatus.m_bHover = false;
					if (!m_tStatus.m_bWallClimb)
					{
						m_tStatus.m_bJump = false;
						m_tStatus.m_bFall = true;
						m_eCurMotion = FALL;
					}
				}
				return;
			}
			if (m_tStatus.m_bAirDash)
			{
				m_tStatus.m_bCanAirDash = false;
				m_tStatus.m_bAirDash = false;
				m_tStatus.m_bJump = false;
				m_tStatus.m_bFall = true;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = FALL;
				return;
			}
			if (m_ePreMotion == JUMP_ATT && m_tStatus.m_bJumpAttack)
			{
				m_tFrame.iStartFrame = 5;
				return;
			}
			if (m_ePreMotion == DASH_ATT)
			{
				m_tStatus.m_bDash = false;
				m_tStatus.m_bDashAttack = false;
				m_tStatus.m_bCanDash = false;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = IDLE;
				return;
			}
			if (m_ePreMotion == ATT_FIRST && m_tStatus.m_bAttackSecond)
			{
				if(!m_tStatus.m_bFX)
					CSoundMgr::Get_Instance()->PlaySound(L"ZX_Second.wav", CSoundMgr::SABER_SECOND);
				if(m_tStatus.m_bFX)
					CSoundMgr::Get_Instance()->PlaySound(L"FX_Second.wav", CSoundMgr::SABER_SECOND);

				m_tFrame.iStartFrame = 0;
				m_eCurMotion = ATT_SECOND;
				return;
			}
			else if (m_ePreMotion == ATT_FIRST && !m_tStatus.m_bAttackSecond)
			{
				m_tFrame.iStartFrame = 0;
				m_tStatus.m_bAttackFirst = false;
				m_eCurMotion = IDLE;
				return;
			}

			if (m_ePreMotion == ATT_SECOND && m_tStatus.m_bAttackThird)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"ZX_Third.wav", CSoundMgr::SABER_THIRD);
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = ATT_THIRD;
				return;
			}
			else if (m_ePreMotion == ATT_SECOND && !m_tStatus.m_bAttackThird)
			{
				m_tFrame.iStartFrame = 0;
				m_tStatus.m_bAttackFirst = false;
				m_tStatus.m_bAttackSecond = false;
				m_eCurMotion = IDLE;
				return;
			}

			if (m_ePreMotion == ATT_THIRD)
			{
				m_tStatus.m_bAttackFirst = false;
				m_tStatus.m_bAttackSecond = false;
				m_tStatus.m_bAttackThird = false;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = IDLE;
				return;
			}

			if (m_tStatus.m_bFormSelect)
			{
				m_tFrame.iStartFrame = 4;
				return;
			}
			if (m_tStatus.m_bFall)
			{
				m_tFrame.iStartFrame = 2;
				return;
			}
			if (m_tStatus.m_bBeForm)
			{
				m_tStatus.m_bBeForm = false;
				m_tStatus.m_bCanPlay = true;
				m_tStatus.m_bIdle = true;
				m_tFrame.iStartFrame = 0;
				m_eCurMotion = IDLE;
			}
		
			
			if (m_tStatus.m_bDash)
			{
				m_eCurMotion = IDLE;
				m_tStatus.m_bCanDash = false;
				m_tStatus.m_bDash = false;
			}
			
			if (m_tStatus.m_bJump)
			{
				m_tFrame.iStartFrame = 2;
				return;
			}
			m_tFrame.iStartFrame = 0;

		
		}
	}
}

void CPlayer::Omega_Zero_Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"OXShadow", m_tFrame.wstrStateKey, m_tFrame.iStartFrame);
	if (pTexInfo == nullptr)
		return;
	if (m_fOmegaZeroScale > 2.0f)
		m_fOmegaZeroScale = 1.6f;
	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_iDirection* m_fOmegaZeroScale, m_fOmegaZeroScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + (int)vScroll.x, m_tInfo.vPos.y - 8 + (int)vScroll.y, 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	m_fOmegaZeroScale += 0.1f;
}

void CPlayer::HitBbox_Update()
{
	_vec2 vecScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	if (m_tStatus.m_bDash || m_tStatus.m_bDashAttack || m_tStatus.m_bAirDash || m_tStatus.m_bAirDashAttack)
	{
		m_tHitBox.left = m_tInfo.vPos.x - 20;
		m_tHitBox.right = m_tInfo.vPos.x + 20;
		m_tHitBox.top = m_tInfo.vPos.y - 10;
		m_tHitBox.bottom = m_tInfo.vPos.y + 10;
	}
	else
	{
		m_tHitBox.left = m_tInfo.vPos.x - 10;
		m_tHitBox.right = m_tInfo.vPos.x + 10;
		m_tHitBox.top = m_tInfo.vPos.y - 20;
		m_tHitBox.bottom = m_tInfo.vPos.y + 20;
	}
	m_vecRect[0] = { (float)m_tHitBox.left + vecScroll.x,(float)m_tHitBox.top + vecScroll.y };
	m_vecRect[1] = { (float)m_tHitBox.right + vecScroll.x,(float)m_tHitBox.top + vecScroll.y };
	m_vecRect[2] = { (float)m_tHitBox.right + vecScroll.x,(float)m_tHitBox.bottom + vecScroll.y };
	m_vecRect[3] = { (float)m_tHitBox.left + vecScroll.x,(float)m_tHitBox.bottom + vecScroll.y };
	m_vecRect[4] = m_vecRect[0];
}

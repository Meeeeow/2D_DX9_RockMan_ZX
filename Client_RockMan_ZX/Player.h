#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ObjectBase.h"
class CUserInterface;
class CBioMetal;
class CPlayer :
	public CObjectBase
{
public:
	enum PlayerMotion { 
		/*Default 1*/ IDLE, WALK,
		/*Default 2*/ DASH, JUMP, FALL ,WALLCLIMB, WALLCLIMBJUMP,HIT,FORMCHANGE, FORMSELECT, BEFORM,ATT_FIRST, ATT_SECOND, ATT_THIRD , DASH_ATT, JUMP_ATT,
		/*Specail A*/ SPECIALATTACK, CHARGEATTACK,
		/*Model  HX*/ AIRDASH, AIRDASHATT,VERTDASH, HOVER,
		/*Model  FX*/ 
		/*Model  LX*/ AQUA_IDLE, AQUA_WALK, AQUA_WALK_VERT, AQUA_WALK_VERT_REV,  AQUA_DASH, AQUA_DASH_VERT, AQUA_DASH_VERT_REV,
					  AQUA_WALK_DIAG, AQUA_DIAG_DASH, AQUA_WALK_DIAG_REV, AQUA_DIAG_DASH_REV, AQUA_ATT,
		/*MODEL	 PX*/ 
		END
	};
public:
	static CObjectBase* Create_Player(UNITINFO _pUnitInfo);
	void Setting_Player(UNITINFO _pUnitInfo);

private:
	explicit CPlayer();
	virtual ~CPlayer();

public:
	// CObjectBase을(를) 통해 상속됨
	virtual HRESULT Init_Object() override;
	virtual int Update_Object() override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

	void Debug_Sync();
	void Keyboard_Debug_Test();
	void Player_Regen();
	bool Map_Change();
public:
	void Player_CheckLine();
	void KeyboardAction();

	void Default_Keyboard();
	void Model_Human_Keyboard();
	void Model_ZX_Keyboard();
	void Model_HX_Keyboard();
	void Model_FX_Keyboard();
	void Model_LX_Keyboard();
	void Model_PX_Keyboard();

	void Player_Motion();
	void ZX_Motion();
	void HX_Motion();
	void FX_Motion();
	void LX_Motion();
	void PX_Motion();

	void PlayerAction();
	void Jump();
	void Fall();
	void WallClimb();
	void OffSet();
	void ShowScrollSetting();

	void OMEGA_ZERO_Off();

	void Default_Hit(int iDmg, RECT HitBox);
	void Hit_Recovery();
public:
	void Form_Check();
	void Form_Update();
	void Form_Setting();

public:
	void Form_Select();
	void Frame_Check();
	void Frame_Update();


public: 
	void Omega_Zero_Render();
	void HitBbox_Update();	


	// Inline ////////////////////////////////////////////////////////////
	void Set_Motion(PlayerMotion _pMotion) { m_eCurMotion = _pMotion; }
	PLAYERSTATUS Get_PlayerStatus() { return m_tStatus; }
	bool Get_FormSelect() { return m_tStatus.m_bFormSelect; }
	int  Get_FormNumber() { return m_tStatus.m_iForm; }
	bool Get_CanPlay() { return m_tStatus.m_bCanPlay; }
	//////////////////////////////////////////////////////////////////////

	// WallClimb /////////////////////////////////////////////////////////
	void WallClimbActive() { m_tStatus.m_bWallClimb = true; }
	void WallClimbInActive() { m_tStatus.m_bWallClimb = false; }
	void Set_Direction(int iDirection) { m_iDirection = iDirection; }
	//////////////////////////////////////////////////////////////////////

	// WallClimb LR //////////////////////////////////////////////////////
	void WallClimbLeftActive() { m_tStatus.m_bWallClimbLeft = true; }
	void WallClimbLeftInActive() { m_tStatus.m_bWallClimbLeft = false; }
	void WallClimbRightActive() { m_tStatus.m_bWallClimbRight = true; }
	void WallClimbRightInActive() { m_tStatus.m_bWallClimbRight = false; }
	//////////////////////////////////////////////////////////////////////

	// Default ///////////////////////////////////////////////////////////
	void JumpInActive() { m_tStatus.m_bJump = false; }
	bool Get_SpecialAttack() { return m_tStatus.m_bSpecialAttack; }
	//////////////////////////////////////////////////////////////////////

	// Model HX WallClimb Inline //////////////////////////////////////////
	void CanAirDashInActive() { m_tStatus.m_bCanAirDash = false; }
	void AirDashInActive() { m_tStatus.m_bAirDash = false; }
	void AirDashAttackInActive() { m_tStatus.m_bAirDashAttack = false; }
	void WalkInActive() { m_tStatus.m_bWalk = false; }
	void DashInActive() { m_tStatus.m_bDash = false; }
	void HoverInActive() { m_tStatus.m_bHover = false; }
	//////////////////////////////////////////////////////////////////////

	// Modle LX Special //////////////////////////////////////////////////
	bool Get_Aqua() { return m_tStatus.m_bAqua; }
	void AquaInActive() { m_tStatus.m_bAqua = false; }
	//////////////////////////////////////////////////////////////////////

	// SceneChange Option ////////////////////////////////////////////////
	void Scene_Change_True() { m_bSceneChange = true; }
	void Scene_Change_False() { m_bSceneChange = false; }
	//////////////////////////////////////////////////////////////////////


private:
	// Player
	PLAYERSTATUS		m_tStatus;
	bool				m_bSceneChange;
	// Motion
	PlayerMotion		m_eCurMotion;
	PlayerMotion		m_ePreMotion;

	// UI
	CUserInterface*		m_pBioMetal;
	CUserInterface*		m_pLifeBar;

	// Effect
	CEffect*			m_pCharge;
	CEffect*			m_pDashEffect;
	CEffect*			m_pDashDust;

	bool				m_bMotion;
	bool				m_bAttack;
	bool				m_bMagicUse;
	int					m_iTempForm;
	DWORD				m_tTime;
	DWORD				m_tGenTime;

	int					m_iAlpha;
	float				m_fOmegaZeroScale;
};


#endif // !__PLAYER_H__

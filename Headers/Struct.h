#pragma once
#ifndef __STRUCT_H__
#define __STRUCT_H__


typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		tImageInfo;
}TEXINFO;

typedef struct tagPathInfo
{
	wstring wstrObjectKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrFilePath = L"";
	int		dwCount = 0;
}PATHINFO;

typedef struct tagTileInfo
{
	_vec2	vPos;
	_vec2	vSize;
	int		iDrawID;
	int		iOptionID;
}TILEINFO;

typedef struct tagUnitInfo
{
	// 유닛 스탯 정보
	_vec2	vPos;
	_vec2	vSize;
	_vec2	vDir;
	int		iHP;
	int		iCurHP;
	int		iMP;
	int		iCurMP;
	int		iDMG;
	int		iDEF;
	float	fSpeed;
	float	fAngle;
	bool	bPlayer;
}UNITINFO;

typedef struct tagUnitFrame
{
	// 유닛 렌더 정보
	wstring wstrObjectKey;
	wstring wstrStateKey;
	DWORD	dwDelay;
	DWORD	dwTime;
	int		iStartFrame;
	int		iEndFrame;
}UNITFRAME;

typedef struct tagPlayerStatus
{
	// 키 입력 가능 한가?
	bool	m_bCanPlay;
	bool	m_bCanAct;

	bool	m_bHuman;

	bool	m_bWalk;
	bool	m_bIdle;

	bool	m_bCanJump;
	bool	m_bJump;
	bool	m_bJumpAttack;
	float	m_fJumpTime;
	float	m_fJumpPower;
	float	m_fJumpHeight;

	bool	m_bWallClimb;
	bool	m_bWallClimbJump;
	bool	m_bWallClimbLeft;
	bool	m_bWallClimbRight;
	float	m_fClimbFallPower;

	bool	m_bFall;
	float	m_fFallHeight;
	float	m_fFallTime;

	bool	m_bCanDash;
	bool	m_bDash;
	bool	m_bDashAttack;

	bool	m_bBeForm;
	bool	m_bFormSelect;
	bool	m_bFormChange;
	bool	m_bForm[6];
	int		m_iForm;
	int		m_iMaxFormCount;
	
	bool	m_bCanHit;
	bool	m_bHit;
	int 	m_iHitCount;
	// Model 공용
	bool	m_bZX;
	bool	m_bOX;
	bool	m_bAttackFirst;
	bool	m_bAttackSecond;
	bool	m_bAttackThird;
	bool	m_bSpecialAttack;
	bool	m_bCharge;
	bool	m_bCharging;
	int		m_iChargePower;

	// Model HX 전용
	bool	m_bHX;
	bool	m_bCanAirDash;
	bool	m_bAirDash;
	bool	m_bAirDashAttack;
	bool	m_bCanVertDash;
	bool	m_bVertDash;
	bool	m_bCanHover;
	bool	m_bHover;
	int		m_iHoverCount;

	// Model FX 전용
	bool	m_bFX;

	// Model LX 전용
	bool	m_bLX;
	bool	m_bAqua;
	bool	m_bAquaAttackRoop;
	bool	m_bAquaWalk;
	bool	m_bAquaDash;

	// Model PX 전용
	bool	m_bPX;
	int		m_iAttackCount;

}PLAYERSTATUS;

typedef struct tagEffectInfo
{
	wstring wstrObjectKey;
	wstring wstrStateKey;
	DWORD	dwDelay;
	DWORD	dwTime;
	_vec2	vecPos;
	_vec2	vDir;
	RECT	tRect;
	int		iDirection;
	float	fSpeed;
	int		iStartFrame;
	int		iEndFrame;
	bool	bDead;
}EFFECTINFO;

typedef struct tagInterfaceInfo
{
	_vec2	vecPos;
	bool	bBehind;
}UIINFO;

#endif // !__STRUCT_H__

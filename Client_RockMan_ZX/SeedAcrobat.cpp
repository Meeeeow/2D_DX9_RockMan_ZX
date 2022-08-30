#include "stdafx.h"
#include "SeedAcrobat.h"
#include "Destroy.h"

CObjectBase * CSeedAcrobat::Create_Acrobat(float fX, float fY)
{
	CObjectBase* pObject = new CSeedAcrobat;
	pObject->Pos_Setting(fX,fY);
	pObject->Init_Object();
	return pObject;
}

CSeedAcrobat::CSeedAcrobat()
{
}


CSeedAcrobat::~CSeedAcrobat()
{
}

HRESULT CSeedAcrobat::Init_Object()
{
	m_tFrame.wstrObjectKey = L"Acrobat";
	m_tFrame.wstrStateKey = L"Flying";
	m_tFrame.iStartFrame = 0;
	m_tFrame.iEndFrame = 9;
	m_tFrame.dwDelay = 150;
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
	m_tInfo.vDir = {0,0};
	m_iDirection = ENEMY_LEFT;
	return S_OK;
}

int CSeedAcrobat::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_iRenderX = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x;
	m_vTarget = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos;
	if ( m_iRenderX + 700 > m_tInfo.vPos.x
		&& m_iRenderX -700 < m_tInfo.vPos.x)
	{
		D3DXVec2Normalize(&m_tInfo.vDir,&(m_vTarget - m_tInfo.vPos));
		m_tInfo.vPos += (m_tInfo.vDir) * m_tInfo.fSpeed * CTimeMgr::Get_Instance()->Get_DeltaTime();
		CCollisionMgr::Get_Instance()->WallTileCollision(this);

		Attack_Acrobat();
		HitBox_Update();
		Frame_Update();
	}
	return OBJ_NOEVENT;
}

void CSeedAcrobat::Late_Update_Object()
{
	if (m_iRenderX < m_tInfo.vPos.x)
		m_iDirection = ENEMY_LEFT;
	else if (m_iRenderX > m_tInfo.vPos.x)
		m_iDirection = ENEMY_RIGHT;
	if (m_tInfo.iCurHP < 0)
	{
		CEffectMgr::Get_Instance()->Insert_Effect(CDestroy::Create_Destroy(m_tInfo.vPos), CEffectMgr::DESTROY);
		m_bDead = true;
	}
}

void CSeedAcrobat::Render_Object()
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

void CSeedAcrobat::Release_Object()
{
}

void CSeedAcrobat::Default_Hit(int iDmg, RECT HitBox)
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

void CSeedAcrobat::HitBox_Update()
{
	m_tHitBox.left = m_tInfo.vPos.x - 25;
	m_tHitBox.top = m_tInfo.vPos.y - 5;
	m_tHitBox.right = m_tInfo.vPos.x + 25;
	m_tHitBox.bottom = m_tInfo.vPos.y + 5;
}

void CSeedAcrobat::Attack_Acrobat()
{
	for (auto iter : CObjectMgr::Get_Instance()->Get_Player())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tHitBox, &iter->Get_Rect()))
			iter->Default_Hit(m_tInfo.iDMG, m_tHitBox);
	}
}

void CSeedAcrobat::Hit_Recovery()
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

void CSeedAcrobat::Frame_Update()
{
	if (m_tFrame.dwDelay + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iStartFrame;
		m_tFrame.dwTime = GetTickCount();
		if (m_tFrame.iStartFrame > m_tFrame.iEndFrame)
		{
			m_tFrame.iStartFrame = 0;
		
		}
	}
}

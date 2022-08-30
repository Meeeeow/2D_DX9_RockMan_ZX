#include "stdafx.h"
#include "PXCloak.h"
#include "ObjectBase.h"

CEffect * CPXCloak::Create_Cloak(float fX, float fY, int iDirection, CObjectBase* pObj)
{
	CEffect* pEffect = new CPXCloak;
	pEffect->Init_Effect();
	static_cast<CPXCloak*>(pEffect)->Setting(fX, fY, iDirection,pObj);
	return pEffect;
}

void CPXCloak::Setting(float fX, float fY, int iDirection, CObjectBase* pObj)
{
	m_tInfo.vecPos = { fX,fY };
	m_tInfo.iDirection = iDirection;
	m_pParent = pObj;
}

CPXCloak::CPXCloak()
{
}


CPXCloak::~CPXCloak()
{
}

HRESULT CPXCloak::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"PX_Special";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 18;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 388.f;
	m_tInfo.bDead = false;
	m_fAngle = 0.f;
	m_iCount = 0;
	return S_OK;
}

int CPXCloak::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;
	HitBox_Update();
	for(auto iter : CObjectMgr::Get_Instance()->Get_Type(CObjectMgr::Get_Instance()->Get_Current_ID()))
		CObjectMgr::Get_Instance()->FX_Attack(CObjectMgr::Get_Instance()->Get_Current_ID(), 1, m_tInfo.tRect,iter->Get_Direction() * 5.f);

	m_tInfo.vecPos = m_pParent->Get_Info().vPos;

	return OBJ_NOEVENT;
}

void CPXCloak::Late_Update_Effect()
{
	m_fAngle += 5.1f;
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
		{
			m_tInfo.iStartFrame = 0;
			++m_iCount;
			if (m_iCount > 10)
			{
				m_tInfo.bDead = true;
			}
		}
	}
}

void CPXCloak::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"PX_Special", m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale, matRotZ, matWorld;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToDegree(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_pParent->Get_Info().vPos.x + (int)vScroll.x, m_pParent->Get_Info().vPos.y , 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matWorld = matScale * matRotZ * matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
}

void CPXCloak::HitBox_Update()
{
	m_tInfo.tRect.left = m_tInfo.vecPos.x - 30;
	m_tInfo.tRect.right = m_tInfo.vecPos.x + 30;
	m_tInfo.tRect.top = m_tInfo.vecPos.y - 30;
	m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 30;
}

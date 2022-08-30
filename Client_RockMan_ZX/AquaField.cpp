#include "stdafx.h"
#include "AquaField.h"
#include "Player.h"

CEffect * CAquaField::Create_AquaField()
{
	CEffect* pEffect = new CAquaField;
	pEffect->Init_Effect();
	return pEffect;
}

CAquaField::CAquaField()
{
}


CAquaField::~CAquaField()
{
}

HRESULT CAquaField::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"AquaField";
	m_tInfo.dwDelay = 15000;
	m_tInfo.dwTime = 0;
	m_tInfo.tRect = {};
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 0;
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey);
	m_tInfo.vecPos = { float(pTexInfo->tImageInfo.Width >> 1) ,	float(pTexInfo->tImageInfo.Height >> 1) };
	AquaTime = false;
	return S_OK;
}

int CAquaField::Update_Effect()
{
	return OBJ_NOEVENT;
}

void CAquaField::Late_Update_Effect()
{
}

void CAquaField::Render_Effect()
{
	if (CObjectMgr::Get_Instance()->Using_AquaField())
	{
		if (!AquaTime)
		{
			m_tInfo.dwTime = GetTickCount();
			AquaTime = true;
		}
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_tInfo.wstrObjectKey);

		if (pTexInfo == nullptr)
			return;

		_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

		_matrix matScale, matTrans;

		D3DXMatrixScaling(&matScale, 1, 1, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x, m_tInfo.vecPos.y, 0.f);
		matScale *= matTrans;

		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, &_vec3(m_tInfo.vecPos.x, m_tInfo.vecPos.y, 0), nullptr, D3DCOLOR_ARGB(170, 255, 255, 255));

		if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
		{
			for (auto pPlayer : CObjectMgr::Get_Instance()->Get_Player())
				static_cast<CPlayer*>(pPlayer)->AquaInActive();
			AquaTime = false;
		}
	}

}

void CAquaField::HitBox_Update()
{
}

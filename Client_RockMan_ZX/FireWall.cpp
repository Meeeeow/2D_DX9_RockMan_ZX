#include "stdafx.h"
#include "FireWall.h"
#include "Tile.h"

CEffect * CFireWall::Create_Fire(_vec2 vPos, int iDirection)
{
	CEffect* pEffect = new CFireWall;
	static_cast<CFireWall*>(pEffect)->Setting_Fire(vPos, iDirection);
	pEffect->Init_Effect();
	return pEffect;
}

void CFireWall::Setting_Fire(_vec2 vPos, int iDirection)
{
	m_tInfo.vecPos = vPos;
	m_tInfo.iDirection = iDirection;
}

CFireWall::CFireWall()
{
}


CFireWall::~CFireWall()
{
}

HRESULT CFireWall::Init_Effect()
{
	m_tInfo.wstrObjectKey = L"Effect";
	m_tInfo.wstrStateKey = L"FX_Special";
	m_tInfo.iStartFrame = 0;
	m_tInfo.iEndFrame = 20;
	m_tInfo.tRect = {};
	m_tInfo.bDead = false;
	m_tInfo.dwDelay = 100;
	m_tInfo.dwTime = GetTickCount();
	m_tInfo.fSpeed = 300;

	return S_OK;
}

int CFireWall::Update_Effect()
{
	if (m_tInfo.bDead)
		return OBJ_DEAD;

	HitBox_Update();
	Hit_Check();
	m_tInfo.vecPos.x += (m_tInfo.fSpeed * m_tInfo.iDirection) * CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tInfo.dwDelay + m_tInfo.dwTime < GetTickCount())
	{
		++m_tInfo.iStartFrame;
		m_tInfo.dwTime = GetTickCount();
		if (m_tInfo.iStartFrame > m_tInfo.iEndFrame)
			m_tInfo.iStartFrame = 0;
	}
	return OBJ_NOEVENT;
}

void CFireWall::Late_Update_Effect()
{

	if (CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x + 500 < m_tInfo.vecPos.x
		&& CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x - 500 > m_tInfo.vecPos.x)
		m_tInfo.bDead = true;

	
}

void CFireWall::Render_Effect()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Effect", L"FX_Special", m_tInfo.iStartFrame);
	if (pTexInfo == nullptr)
		return;

	_matrix matTrans, matScale;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	D3DXMatrixScaling(&matScale, m_tInfo.iDirection * 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vecPos.x + (int)vScroll.x , (m_tInfo.vecPos.y -10), 0);
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	matScale *= matTrans;
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	/*CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->End();

	CGraphicDeviceMgr::Get_Instance()->Get_Line()->Draw(m_vecRect, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/
}

void CFireWall::HitBox_Update()
{
	_vec2 vecScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	if (m_tInfo.iStartFrame == 0)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 45;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 45;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 1)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 40;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 2)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 3)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x -50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 4)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 5)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 5;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 6)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x +10;
		m_tInfo.tRect.top = m_tInfo.vecPos.y - 10;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 7)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x +10;
		m_tInfo.tRect.top = m_tInfo.vecPos.y;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 8)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 25;
		m_tInfo.tRect.top = m_tInfo.vecPos.y;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 9)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x -50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 25;
		m_tInfo.tRect.top = m_tInfo.vecPos.y;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 10)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x +25;
		m_tInfo.tRect.top = m_tInfo.vecPos.y;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 11)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 12)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 20;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 13)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 30;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 14)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 30;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 15)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x -50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 20;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 30;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 16)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x + 5;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 30;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 17)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 10;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 30;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 18)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 15;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 30;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 19)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50 ;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 25;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 35;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}
	else if (m_tInfo.iStartFrame == 20)
	{
		m_tInfo.tRect.left = m_tInfo.vecPos.x - 50;
		m_tInfo.tRect.right = m_tInfo.vecPos.x - 40;
		m_tInfo.tRect.top = m_tInfo.vecPos.y + 40;
		m_tInfo.tRect.bottom = m_tInfo.vecPos.y + 50;
	}

	if (m_tInfo.iDirection == OBJ_RIGHT)
	{
		m_vecRect[0] = { (float)m_tInfo.tRect.left + vecScroll.x,(float)m_tInfo.tRect.top + vecScroll.y };
		m_vecRect[1] = { (float)m_tInfo.tRect.right + vecScroll.x,(float)m_tInfo.tRect.top + vecScroll.y };
		m_vecRect[2] = { (float)m_tInfo.tRect.right + vecScroll.x,(float)m_tInfo.tRect.bottom + vecScroll.y };
		m_vecRect[3] = { (float)m_tInfo.tRect.left + vecScroll.x,(float)m_tInfo.tRect.bottom + vecScroll.y };
		m_vecRect[4] = m_vecRect[0];
	}
	else if (m_tInfo.iDirection == OBJ_LEFT)
	{
		m_vecRect[0] = { (float)m_tInfo.tRect.right + vecScroll.x,(float)m_tInfo.tRect.top + vecScroll.y };
		m_vecRect[1] = { (float)m_tInfo.tRect.left + vecScroll.x,(float)m_tInfo.tRect.top + vecScroll.y };
		m_vecRect[2] = { (float)m_tInfo.tRect.left + vecScroll.x,(float)m_tInfo.tRect.bottom + vecScroll.y };
		m_vecRect[3] = { (float)m_tInfo.tRect.right + vecScroll.x,(float)m_tInfo.tRect.bottom + vecScroll.y };
		m_vecRect[4] = m_vecRect[0];
	}
}

void CFireWall::Hit_Check()
{
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		RECT rc = {};
		if ((iter->Get_TileInfo().iDrawID == 2 || iter->Get_TileInfo().iDrawID == 3)
			&& IntersectRect(&rc, &iter->Get_TileRect(), &m_tInfo.tRect))
		{
			m_tInfo.bDead = true;
		}
	}
	CObjectMgr::Get_Instance()->Hit_Check(CObjectMgr::Get_Instance()->Get_Current_ID(),5, m_tInfo.tRect);

}

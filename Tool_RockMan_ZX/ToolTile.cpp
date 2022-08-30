#include "stdafx.h"
#include "ToolTile.h"
#include "ToolMgrInclude.h"
#include "MainFrm.h"
#include "Tool_RockMan_ZXView.h"

CToolTile * CToolTile::Create(float fX, float fY)
{
	CToolTile* pTile = new CToolTile;
	pTile->Set_Pos(fX, fY);
	return pTile;
}

CToolTile * CToolTile::Create_Tile(TILEINFO tTile)
{
	CToolTile* pTile = new CToolTile;
	pTile->Set_All(tTile);
	return pTile;
}

CToolTile::CToolTile()
{
}


CToolTile::~CToolTile()
{
}

void CToolTile::Set_Pos(float fX, float fY)
{
	m_tTileInfo.vPos = { fX,fY };
	m_tTileInfo.vSize = { 0,0 };
	m_tTileInfo.iDrawID = 0;
	m_tTileInfo.iOptionID = 0;
}

void CToolTile::Render()
{
	const TEXINFO* pTexInfo = CToolTextureMgr::Get_Instance()->Get_TexInfo(L"Tile",L"TileType",m_tTileInfo.iDrawID);

	if (pTexInfo == nullptr)
		return;

	CMainFrame* pMain1 = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTool_RockMan_ZXView* pSub = dynamic_cast<CTool_RockMan_ZXView*>(pMain1->m_wMainSplitter.GetPane(0, 1));
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, 1, 1, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tTileInfo.vPos.x - pSub->GetScrollPos(SB_HORZ), m_tTileInfo.vPos.y - pSub->GetScrollPos(SB_VERT), 0.f);
	matScale *= matTrans;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	CToolGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CToolGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CToolTile::Change(int iDrawID, int iOptionID)
{
	m_tTileInfo.iDrawID = iDrawID;
	m_tTileInfo.iOptionID = iOptionID;
}

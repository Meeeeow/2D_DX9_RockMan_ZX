#include "stdafx.h"
#include "Tile.h"


CTile * CTile::Create_Tile(TILEINFO _pTile)
{
	CTile* pTile = new CTile;
	pTile->Set_Tile(_pTile);
	pTile->Init_Tile();
	return pTile;
}

void CTile::Set_Tile(TILEINFO _pTile)
{
	m_tTile.vPos = { _pTile.vPos.x , _pTile.vPos.y };
	m_tTile.vSize = { _pTile.vSize.x, _pTile.vSize.y };
	m_tTile.iDrawID = _pTile.iDrawID;
	m_tTile.iOptionID = _pTile.iOptionID;
}

void CTile::Init_Tile()
{
	Update_Tile_Rect();
}

void CTile::Update_Tile()
{

}

void CTile::Update_Tile_Rect()
{
	m_tRect.left = (double)(m_tTile.vPos.x - 10);
	m_tRect.top = (double)(m_tTile.vPos.y - 10);
	m_tRect.right = (double)(m_tTile.vPos.x + 10);
	m_tRect.bottom = (double)(m_tTile.vPos.y + 10);
}

void CTile::Late_Update_Tile()
{
}

void CTile::Render_Tile()
{
	int iScrollX = CScrollMgr::Get_Instance()->Get_Scroll().x;
	int iScrollY = CScrollMgr::Get_Instance()->Get_Scroll().y;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Tile", L"TileType", m_tTile.iDrawID);
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tTile.vPos.x + iScrollX, m_tTile.vPos.y + iScrollY, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&_vec3(pTexInfo->tImageInfo.Width >> 1, pTexInfo->tImageInfo.Height >> 1, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CTile::Release_Tile()
{
}

CTile::CTile()
{
}


CTile::~CTile()
{
}

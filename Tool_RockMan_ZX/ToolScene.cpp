#include "stdafx.h"
#include "ToolScene.h"
#include "Tool_RockMan_ZXView.h"

void CToolScene::Render_Scene()
{
	if (m_iX == 0 && m_iY == 0)
		return;

	const TEXINFO* pTexInfo = CToolTextureMgr::Get_Instance()->Get_TexInfo(m_strName);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, 1, 1, 0.f);
	D3DXMatrixTranslation(&matTrans, fCenterX - m_pView->GetScrollPos(SB_HORZ), fCenterY - m_pView->GetScrollPos(SB_VERT), 0.f);
	matScale *= matTrans;

	CToolGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CToolGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CToolScene::Change_Scene()
{
}

void CToolScene::Init_Scene()
{

}

void CToolScene::BackGround_Change(const wstring & pPathFile)
{
	const TEXINFO* pTexInfo = CToolTextureMgr::Get_Instance()->Get_TexInfo(pPathFile);
	m_strName = pPathFile;
	if (nullptr == pTexInfo)
		return;
	m_iX = (pTexInfo->tImageInfo.Width / 20);
	m_iY = (pTexInfo->tImageInfo.Height / 20);
	CToolTileMgr::Get_Instance()->TileSetting(m_iX, m_iY);
}

CToolScene::CToolScene()
	:m_iX(0),m_iY(0),m_pView(nullptr)
{

}


CToolScene::~CToolScene()
{
}

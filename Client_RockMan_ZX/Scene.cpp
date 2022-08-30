#include "stdafx.h"
#include "Scene.h"
#include "ObjectBase.h"
#include "Player.h"

CScene::CScene()
{
	ZeroMemory(&m_vecTileArray, sizeof(_vec2));
}


CScene::~CScene()
{
}

void CScene::Render_Scene_Key(const wstring & _FilePath, float fSclaex, float fSclaey)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(_FilePath);
	float fX = 0;
	if (!CObjectMgr::Get_Instance()->Get_Player().empty())
		fX = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x;
	if (pTexInfo == nullptr)
		return;

	if (!CObjectMgr::Get_Instance()->Using_SpecialAttack())
	{
		_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

		_matrix matScale, matTrans;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, fSclaex, fSclaey, 0.f);
		D3DXMatrixTranslation(&matTrans, fCenterX , fCenterY, 0.f);

		matScale *= matTrans;

		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, &_vec3(fCenterX , fCenterY, 0), &_vec3((vScroll.x),0.f,0.f), D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		_vec2 vScroll = CScrollMgr::Get_Instance()->Get_QuakeScroll();
		
		_matrix matScale, matTrans;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, fSclaex, fSclaey, 0.f);
		D3DXMatrixTranslation(&matTrans, fCenterX + (vScroll.x), fCenterY, 0.f);

		matScale *= matTrans;
		
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
		CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CScene::Render_Scene_Key_No_Quake(const wstring & _FilePath)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(_FilePath);
	float fX = 0;
	if (!CObjectMgr::Get_Instance()->Get_Player().empty())
		fX = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x;
	
	if (pTexInfo == nullptr)
		return;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	_matrix matScale, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1, 1, 0.f);
	D3DXMatrixTranslation(&matTrans, fCenterX , fCenterY, 0.f);
	matScale *= matTrans;

	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX , fCenterY, 0), &_vec3((vScroll.x), 0.f, 0.f), D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CScene::Render_Scene_No_Scroll(const wstring & _FilePath)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(_FilePath);
	float fX = 0;
	if (!CObjectMgr::Get_Instance()->Get_Player().empty())
		fX = CObjectMgr::Get_Instance()->Get_PlayerInfo().vPos.x;

	if (pTexInfo == nullptr)
		return;
	_vec2 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	_matrix matScale, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1, 1, 0.f);
	D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
	matScale *= matTrans;

	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matScale);
	CGraphicDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, &_vec3(fCenterX, fCenterY, 0), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CScene::Get_TileArray(const wstring & _FileKey)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(_FileKey);
	m_vecTileArray.x = (float)(pTexInfo->tImageInfo.Width / 20);
	m_vecTileArray.y = (float)(pTexInfo->tImageInfo.Height / 20);
}

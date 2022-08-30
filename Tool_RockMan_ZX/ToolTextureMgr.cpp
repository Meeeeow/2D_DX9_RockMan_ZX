#include "stdafx.h"
#include "ToolTextureMgr.h"
#include "ToolTextrue.h"
#include "ToolSingleTexture.h"
#include "ToolMultiTexture.h"

HRESULT CToolTextureMgr::Insert_TextureMgr(ID eID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iCount)
{
	auto& iter = m_mapTexture.find(wstrObjectKey);
	if (iter == m_mapTexture.end())
	{
		CToolTextrue* pTexture = nullptr;
		switch (eID)
		{
		case CToolTextureMgr::SINGLE:
			pTexture = new CToolSingleTexture;
			break;
		case CToolTextureMgr::MULTI:
			pTexture = new CToolMultiTexture;
			break;
		case CToolTextureMgr::END:
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, iCount)))
		{
			TCHAR ERR[MAX_PATH];
			swprintf_s(ERR, L"FAILED %s Insert", eID);
			MSG_BOX(ERR);
			return E_FAIL;
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (MULTI == eID)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, iCount)))
		{
			MSG_BOX(L"FAILED else MULTI INSERT");
			return E_FAIL;
		}
	}
	return S_OK;
}

const TEXINFO * CToolTextureMgr::Get_TexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iIndex)
{
	auto& iter = m_mapTexture.find(wstrObjectKey);
	if (iter == m_mapTexture.end())
	{
		TCHAR ERR[MAX_PATH];
		swprintf_s(ERR, L"FAILED %s INFO", wstrObjectKey.c_str());
		MSG_BOX(ERR);
		return nullptr;
	}
	return iter->second->Get_TexInfo(wstrStateKey, iIndex);
}

void CToolTextureMgr::Release_TextureMgr()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear();
}

void CToolTextureMgr::Add_Texture()
{
	m_pInstance->Insert_TextureMgr(MULTI, L"../Texture/Tile/TileType/TILE%d.png", L"Tile", L"TileType", 8);
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/Loading.png", L"Loading");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/OX_Tile.png", L"OX_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/TrainRoom_Tile.png", L"TrainRoom_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/Scraper_Tile.png", L"Scraper_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/AreaD5_Tile.png", L"AreaD5_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/AreaD2_Tile.png", L"AreaD2_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/FistLeo_Tile.png", L"FistLeo_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/AreaE1_Tile.png", L"AreaE1_Tile");

}

CToolTextureMgr::CToolTextureMgr()
{
}


CToolTextureMgr::~CToolTextureMgr()
{
	Release_TextureMgr();
}

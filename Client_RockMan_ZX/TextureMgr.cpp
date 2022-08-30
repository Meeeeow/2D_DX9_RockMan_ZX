#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexutre.h"
#include "MultiTexture.h"

HRESULT CTextureMgr::Insert_TextureMgr(ID eID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iCount)
{
	auto& iter = m_mapTexture.find(wstrObjectKey);
	if (iter == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;
		switch (eID)
		{
		case CTextureMgr::SINGLE:
			pTexture = new CSingleTexutre;
			break;
		case CTextureMgr::MULTI:
			pTexture = new CMultiTexture;
			break;
		case CTextureMgr::END:
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, iCount)))
		{
			TCHAR ERR[MAX_PATH];
			swprintf_s(ERR, L"FAILED %s Insert in Mgr", eID);
			MSG_BOX(ERR);
			return E_FAIL;
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (MULTI == eID)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, iCount)))
		{
			TCHAR szBuf[MAX_PATH] = {};
			swprintf_s(szBuf, L"%s %s %d", wstrStateKey, wstrObjectKey, iCount);
			MSG_BOX(szBuf);
			return E_FAIL;
		}
	}
	return S_OK;
}

const TEXINFO * CTextureMgr::Get_TexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iIndex)
{
	auto& iter = m_mapTexture.find(wstrObjectKey);
	if (iter == m_mapTexture.end())
	{
		TCHAR ERR[MAX_PATH];
		swprintf_s(ERR, L"FAILED %s , %s ,  %d INFO", wstrObjectKey.c_str(),wstrStateKey.c_str(),iIndex);
		MSG_BOX(ERR);
		return nullptr;
	}
	return iter->second->Get_TexInfo(wstrStateKey, iIndex);
}

void CTextureMgr::Release_TextureMgr()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear();
}

HRESULT CTextureMgr::ReadPathFile(const wstring & wstrPath)
{
	wifstream fin;
	fin.open(L"../Data/PathInfo.txt");
	if (!fin.fail())
	{
		TCHAR szPath[MAX_PATH]{};
		TCHAR szObjectKey[MAX_PATH]{};
		TCHAR szStateKey[MAX_PATH]{};
		TCHAR szCount[MAX_PATH]{};

		while (true)
		{
			fin.getline(szPath, MAX_PATH, L'|');
			fin.getline(szObjectKey, MAX_PATH, L'|');
			fin.getline(szStateKey, MAX_PATH, L'|');
			fin.getline(szCount, MAX_PATH);

			if (fin.eof())
			{
				break;
			}
			int iCount = _wtoi(szCount);
			if (FAILED(Insert_TextureMgr(MULTI, szPath, szObjectKey, szStateKey, iCount)))
				return E_FAIL;
		}
		fin.close();

	}
	return S_OK;
}

void CTextureMgr::Add_Texure()
{
	// Single Texture ¼öµ¿ »ðÀÔ
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/Loading.png", L"Loading");

	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Background/OX_Background.png", L"OX_Background");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/OX_Tile.png", L"OX_Tile");

	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Background/TrainRoom_Background.png", L"TrainRoom_Background");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/TrainRoom_Tile.png", L"TrainRoom_Tile");

	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/FistLeo_Tile.png", L"Magma_Tile");


	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/AreaD2_Tile.png", L"AreaD2_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/AreaD5_Tile.png", L"AreaD5_Tile");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/AreaE1_Tile.png", L"AreaE1_Tile");

	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Background/Scraper_Background.png", L"Scraper_Background");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/Map/Scraper_Tile.png", L"Scraper_Tile");


	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/MapEffect/Aqua.png", L"AquaField");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/MapEffect/OXIntro.png", L"OXIntro");

	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/BioMetal/Model_Human.png", L"BIOMETAL_Human");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/BioMetal/Model_ZX.png", L"BIOMETAL_ZX");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/BioMetal/Model_HX.png", L"BIOMETAL_HX");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/BioMetal/Model_FX.png", L"BIOMETAL_FX");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/BioMetal/Model_LX.png", L"BIOMETAL_LX");
	m_pInstance->Insert_TextureMgr(SINGLE, L"../Single_Texture/BioMetal/Model_PX.png", L"BIOMETAL_PX");



}

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release_TextureMgr();
}

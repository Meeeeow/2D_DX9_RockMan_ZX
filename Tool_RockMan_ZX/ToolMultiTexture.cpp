#include "stdafx.h"
#include "ToolMultiTexture.h"
#include "ToolGraphicDevice.h"

CToolMultiTexture::CToolMultiTexture()
{
}


CToolMultiTexture::~CToolMultiTexture()
{
}

HRESULT CToolMultiTexture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey , const int & iCount )
{
	auto& iter = m_mapMultiTexInfo.find(wstrStateKey);
	if (iter == m_mapMultiTexInfo.end())
	{
		TCHAR szFilePath[MAX_PATH]{};
		TEXINFO* pTexInfo = nullptr;
		for (int i = 0; i < iCount; ++i)
		{
			pTexInfo = new TEXINFO;
			swprintf_s(szFilePath, wstrFilePath.c_str(), i);
			if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImageInfo)))
			{
				MSG_BOX(L"FAILED Multi Texture Get ImageInfo");
				return E_FAIL;
			}
			if (FAILED(D3DXCreateTextureFromFileEx(CToolGraphicDevice::Get_Instance()->Get_Device(),
				szFilePath,
				pTexInfo->tImageInfo.Width,
				pTexInfo->tImageInfo.Height,
				pTexInfo->tImageInfo.MipLevels,
				0,
				pTexInfo->tImageInfo.Format,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				&pTexInfo->tImageInfo,
				nullptr,
				&pTexInfo->pTexture)))
			{
				MSG_BOX(L"FAILED Multi Texture Create Texture");
				return E_FAIL;
			}
			m_mapMultiTexInfo[wstrStateKey].emplace_back(pTexInfo);
		}
	}
	return S_OK;
}

const TEXINFO * CToolMultiTexture::Get_TexInfo(const wstring & wstrStateKey, const int & iIndex)
{
	auto& iter = m_mapMultiTexInfo.find(wstrStateKey);
	if (iter == m_mapMultiTexInfo.end())
	{
		MSG_BOX(L"FAILED Get Multi Texture Info");
		return nullptr;
	}
	return iter->second[iIndex];
}

void CToolMultiTexture::Release_Texture()
{
	for (auto& rPair : m_mapMultiTexInfo)
	{
		for (auto& pTexInfo : rPair.second)
			Safe_Delete(pTexInfo);
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	m_mapMultiTexInfo.clear();
}

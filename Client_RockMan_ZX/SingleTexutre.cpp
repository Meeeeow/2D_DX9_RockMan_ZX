#include "stdafx.h"
#include "SingleTexutre.h"
#include "GraphicDeviceMgr.h"

CSingleTexutre::CSingleTexutre()
{
}


CSingleTexutre::~CSingleTexutre()
{
}

HRESULT CSingleTexutre::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey , const int & iCount )
{
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tImageInfo)))
	{
		MSG_BOX(L"FAILED Single Texture Get ImageInfo");
		return E_NOTIMPL;
	}
	if (FAILED(D3DXCreateTextureFromFileEx(CGraphicDeviceMgr::Get_Instance()->Get_Device(),
		wstrFilePath.c_str(),
		m_tTexInfo.tImageInfo.Width,
		m_tTexInfo.tImageInfo.Height,
		m_tTexInfo.tImageInfo.MipLevels,
		0,
		m_tTexInfo.tImageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		&m_tTexInfo.tImageInfo,
		nullptr,
		&m_tTexInfo.pTexture)))
	{
		MSG_BOX(L"FAILED Single Texture Create Texture");
		return E_NOTIMPL;
	}
	return S_OK;
}

const TEXINFO * CSingleTexutre::Get_TexInfo(const wstring & wstrStateKey , const int & iIndex )
{
	return &m_tTexInfo;
}

void CSingleTexutre::Release_Texture()
{
	if (m_tTexInfo.pTexture)
		m_tTexInfo.pTexture->Release();

}

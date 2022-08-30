#include "stdafx.h"
#include "ToolSingleTexture.h"
#include "ToolGraphicDevice.h"

CToolSingleTexture::CToolSingleTexture()
{
}


CToolSingleTexture::~CToolSingleTexture()
{
}

HRESULT CToolSingleTexture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey , const int & iCount )
{
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tImageInfo)))
	{
		MSG_BOX(L"FAILED Single Texture Get ImageInfo");
		return E_NOTIMPL;
	}
	if (FAILED(D3DXCreateTextureFromFileEx(CToolGraphicDevice::Get_Instance()->Get_Device(),
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

const TEXINFO * CToolSingleTexture::Get_TexInfo(const wstring & wstrStateKey , const int & iIndex)
{
	return &m_tTexInfo;
}

void CToolSingleTexture::Release_Texture()
{
	if (m_tTexInfo.pTexture)
		m_tTexInfo.pTexture->Release();
}

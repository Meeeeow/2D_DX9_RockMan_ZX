#pragma once
#ifndef __SINGLE_TEXTURE_H__
#define __SINGLE_TEXTURE_H__

#include "Texture.h"
class CSingleTexutre final :
	public CTexture
{
public:
	CSingleTexutre();
	virtual ~CSingleTexutre();



	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const int & iCount = 0) override;

	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const int & iIndex = 0) override;

	virtual void Release_Texture() override;

private:
	TEXINFO m_tTexInfo;
};


#endif // !__SINGLE_TEXTURE_H__

#pragma once
#include "ToolTextrue.h"
class CToolSingleTexture :
	public CToolTextrue
{
public:
	CToolSingleTexture();
	virtual ~CToolSingleTexture();

	// CToolTextrue��(��) ���� ��ӵ�
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const int & iCount = 0) override;
	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const int & iIndex = 0) override;
	virtual void Release_Texture() override;

private:
	TEXINFO m_tTexInfo;
};


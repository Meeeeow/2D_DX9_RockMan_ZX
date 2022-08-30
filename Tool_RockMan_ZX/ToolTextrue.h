#pragma once
class CToolTextrue abstract
{
public:
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath
		, const wstring& wstrStateKey = L""
		, const int& iCount = 0) PURE;
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"",
		const int& iIndex = 0) PURE;
	virtual void Release_Texture() PURE;

public:
	CToolTextrue();
	virtual ~CToolTextrue();
};


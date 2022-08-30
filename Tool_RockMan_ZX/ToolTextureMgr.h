#pragma once

#include "ToolSingleton.h"
class CToolTextrue;
class CToolTextureMgr :
	public CToolSingleton<CToolTextureMgr>
{
public:
	enum ID { SINGLE, MULTI, END };

public:
	HRESULT Insert_TextureMgr(
		ID eID,
		const wstring& wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		const int& iCount = 0);
	const TEXINFO* Get_TexInfo(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		const int& iIndex = 0);
	void Release_TextureMgr();
	void Add_Texture();
private:
	map<wstring, CToolTextrue*> m_mapTexture;

public:
	CToolTextureMgr();
	virtual ~CToolTextureMgr();
};


#pragma once
#ifndef __TEXTURE_MGR_H__
#define __TEXUTRE_MGR_H__

#include "Singleton.h"
class CTexture;
class CTextureMgr :
	public CSingleton<CTextureMgr>
{
public: 
	enum ID { SINGLE, MULTI, END};

public:
	const wstring& Get_String() { return m_wstrString; }
	void Set_String(const wstring& wstrString) { m_wstrString = wstrString; }

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

	// ¼±»ý´Ô ²¨
	HRESULT ReadPathFile(const wstring& wstrPath);

public:
	void Add_Texure();

private:
	map<wstring, CTexture*> m_mapTexture;
	wstring m_wstrString;
public:
	CTextureMgr();
	virtual ~CTextureMgr();
};



#endif // !__TEXTURE_MGR_H__

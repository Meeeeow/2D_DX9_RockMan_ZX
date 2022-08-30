#pragma once
#ifndef __FILEINFO_H__
#define __FILEINFO_H__



class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();
public:
	static CString ConvertRelativePath(const CString& wstrFilePath);
	static void PathInfoExtraction(const wstring wstrPath, list<PATHINFO*>& rlistPathInfo);
	static int FileCountFunc(const wstring& wstrPath);
};


#endif // !__FILEINFO_H__

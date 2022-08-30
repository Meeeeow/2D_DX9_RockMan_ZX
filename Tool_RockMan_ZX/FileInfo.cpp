#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString & wstrFilePath)
{
	TCHAR szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	TCHAR szRelativePath[MAX_PATH]{};
	PathRelativePathTo(szRelativePath, szCurDir, FILE_ATTRIBUTE_DIRECTORY, wstrFilePath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::PathInfoExtraction(const wstring wstrPath, list<PATHINFO*>& rlistPathInfo)
{
	CFileFind find;  // 파일 경로 제어하는 객체

	wstring wstrFilePath = wstrPath + L"\\*.*";

	// 파일 존재 유무 파악. 
	BOOL bContinue = find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// 동일한 경로 내 파일들 찾아냄 
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			PathInfoExtraction(find.GetFilePath().GetString(), rlistPathInfo);
		else
		{
			// 내가 작업해야 하는 환경. 

			//시스템 파일일 경우. 
			if (find.IsSystem())
				continue;

			PATHINFO* pPathInfo = new PATHINFO;

			TCHAR szPath[MAX_PATH] = L"";

			//현재 경로 얻어옴
			lstrcpy(szPath, find.GetFilePath().GetString());

			//파일명 잘라낼거임
			PathRemoveFileSpec(szPath);

			// 파일 갯수 셀거임 
			pPathInfo->dwCount = FileCountFunc(szPath);

			//파일명 얻어옴 확장자 제외한. 
			wstring wstrTextureName = find.GetFileTitle().GetString();
			// 만약 Tile0이라면 -> Tile + %d.png = Tile%d.png 로 만드는 거임. 
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d.png";

			//q59번째 줄에서 잘라낸 경로 + 파일명 합치면 ? 절대경로 완성/. 
			PathCombine(szPath, szPath, wstrTextureName.c_str());

			pPathInfo->wstrFilePath = CFileInfo::ConvertRelativePath(szPath);

			// 경로에서 한번 짤라내면? 스테이트키 지금 수업소스 그렇게 규칙 잡혀 있음. 
			PathRemoveFileSpec(szPath);
			pPathInfo->wstrStateKey = PathFindFileName(szPath);

			// 한번더 잘라내면? 그게 오브젝트 키 

			PathRemoveFileSpec(szPath);
			pPathInfo->wstrObjectKey = PathFindFileName(szPath);

			rlistPathInfo.emplace_back(pPathInfo);
			bContinue = FALSE;

		}

	}

}

int CFileInfo::FileCountFunc(const wstring & wstrPath)
{
	CFileFind find;  // 파일 경로 제어하는 객체

	wstring wstrFilePath = wstrPath + L"\\*.*";

	// 파일 존재 유무 파악. 
	BOOL bContinue = find.FindFile(wstrFilePath.c_str());
	int iCnt = 0;
	while (bContinue)
	{
		// 동일한 경로 내 파일들 찾아냄 
		bContinue = find.FindNextFile();
		// 갯수 셈
		if (find.IsSystem() || find.IsDots())
			continue;;
		++iCnt;
	}
	return iCnt;
}

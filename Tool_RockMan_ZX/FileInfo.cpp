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
	CFileFind find;  // ���� ��� �����ϴ� ��ü

	wstring wstrFilePath = wstrPath + L"\\*.*";

	// ���� ���� ���� �ľ�. 
	BOOL bContinue = find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// ������ ��� �� ���ϵ� ã�Ƴ� 
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			PathInfoExtraction(find.GetFilePath().GetString(), rlistPathInfo);
		else
		{
			// ���� �۾��ؾ� �ϴ� ȯ��. 

			//�ý��� ������ ���. 
			if (find.IsSystem())
				continue;

			PATHINFO* pPathInfo = new PATHINFO;

			TCHAR szPath[MAX_PATH] = L"";

			//���� ��� ����
			lstrcpy(szPath, find.GetFilePath().GetString());

			//���ϸ� �߶󳾰���
			PathRemoveFileSpec(szPath);

			// ���� ���� ������ 
			pPathInfo->dwCount = FileCountFunc(szPath);

			//���ϸ� ���� Ȯ���� ������. 
			wstring wstrTextureName = find.GetFileTitle().GetString();
			// ���� Tile0�̶�� -> Tile + %d.png = Tile%d.png �� ����� ����. 
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d.png";

			//q59��° �ٿ��� �߶� ��� + ���ϸ� ��ġ�� ? ������ �ϼ�/. 
			PathCombine(szPath, szPath, wstrTextureName.c_str());

			pPathInfo->wstrFilePath = CFileInfo::ConvertRelativePath(szPath);

			// ��ο��� �ѹ� ©�󳻸�? ������ƮŰ ���� �����ҽ� �׷��� ��Ģ ���� ����. 
			PathRemoveFileSpec(szPath);
			pPathInfo->wstrStateKey = PathFindFileName(szPath);

			// �ѹ��� �߶󳻸�? �װ� ������Ʈ Ű 

			PathRemoveFileSpec(szPath);
			pPathInfo->wstrObjectKey = PathFindFileName(szPath);

			rlistPathInfo.emplace_back(pPathInfo);
			bContinue = FALSE;

		}

	}

}

int CFileInfo::FileCountFunc(const wstring & wstrPath)
{
	CFileFind find;  // ���� ��� �����ϴ� ��ü

	wstring wstrFilePath = wstrPath + L"\\*.*";

	// ���� ���� ���� �ľ�. 
	BOOL bContinue = find.FindFile(wstrFilePath.c_str());
	int iCnt = 0;
	while (bContinue)
	{
		// ������ ��� �� ���ϵ� ã�Ƴ� 
		bContinue = find.FindNextFile();
		// ���� ��
		if (find.IsSystem() || find.IsDots())
			continue;;
		++iCnt;
	}
	return iCnt;
}

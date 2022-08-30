// TileTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool_RockMan_ZX.h"
#include "TileTool.h"
#include "afxdialogex.h"


// CTileTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTileTool, CDialog)

CTileTool::CTileTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TILETOOL, pParent)
{

}

CTileTool::~CTileTool()
{
}

void CTileTool::AutoFindFileFromFolder()
{
	CString strFindPath, strReadPath, strDirName, strFileName;
	BOOL bContinue;

	strFindPath = L"../Texture/Stage/Terrain/Tile/*.png";

	CFileFind filefind;
	bContinue = filefind.FindFile(strFindPath);

	if (!bContinue)
	{
		filefind.Close();
		return;
	}

	while (bContinue)
	{
		bContinue = filefind.FindNextFile();
		strReadPath = filefind.GetFileName();

		strReadPath = filefind.GetFileName();
		strFileName = strReadPath;
		m_ListTileBox.AddString(strFileName);
	}
	filefind.Close();
}

void CTileTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListTileBox);
}


BEGIN_MESSAGE_MAP(CTileTool, CDialog)
END_MESSAGE_MAP()


// CTileTool �޽��� ó�����Դϴ�.

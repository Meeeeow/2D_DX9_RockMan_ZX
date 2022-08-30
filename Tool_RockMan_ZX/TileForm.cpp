// TileForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool_RockMan_ZX.h"
#include "TileForm.h"
#include "ToolTile.h"

// CTileForm

IMPLEMENT_DYNCREATE(CTileForm, CFormView)

CTileForm::CTileForm()
	: CFormView(IDD_TILEFORM)
	, m_iDrawID(0)
	, m_iOptionID(0)
{

}

CTileForm::~CTileForm()
{
	for (auto& iter : m_mapTileList)
		Safe_Delete(iter.second);
	m_mapTileList.clear();
}

void CTileForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListTile);
	DDX_Control(pDX, IDC_LIST1, m_ListScene);
	DDX_Control(pDX, IDC_RADIO1, m_RenderType[0]);
	DDX_Control(pDX, IDC_RADIO2, m_RenderType[1]);
	DDX_Text(pDX, IDC_EDIT1, m_iDrawID);
	DDX_Text(pDX, IDC_EDIT2, m_iOptionID);
	DDX_Control(pDX, IDC_PICTURE1, m_PictureTile);

}

void CTileForm::AutoFindTileFromFolder()
{
	CString strFindPath, strReadPath, strDirName, strFileName;
	BOOL bContinue;

	strFindPath = L"../Texture/Tile/TileType/*.png";

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
		int nPos = strFileName.ReverseFind(L'.png');
		CString strRealName = strFileName.Left(nPos);
		m_ListTile.AddString(strRealName);
	}
	filefind.Close();
}

void CTileForm::AutoFindSceneFromFolder()
{
	CString strFindPath, strReadPath, strDirName, strFileName;
	BOOL bContinue;

	strFindPath = L"../Single_Texture/Map/*.png";

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
		strFileName = strReadPath;
		int nPos = strFileName.ReverseFind(L'.png');
		CString strRealName = strFileName.Left(nPos);
		m_ListScene.AddString(strRealName);
	}
	filefind.Close();
}

BEGIN_MESSAGE_MAP(CTileForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CTileForm::FindFile)
	ON_BN_CLICKED(IDC_BUTTON3, &CTileForm::OnBnClickedButton3)
	ON_LBN_SELCHANGE(IDC_LIST2, &CTileForm::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTileForm::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTileForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CTileForm::OnBnClickedButton4)
END_MESSAGE_MAP()


// CTileForm �����Դϴ�.

#ifdef _DEBUG
void CTileForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTileForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTileForm �޽��� ó�����Դϴ�.


void CTileForm::FindFile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	AutoFindSceneFromFolder();
	AutoFindTileFromFolder();
	CToolTextureMgr::Get_Instance()->Add_Texture();
}




//Save
void CTileForm::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L"dat", L".dat");
	TCHAR szBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		vector<CToolTile*> vecTile = CToolTileMgr::Get_Instance()->Get_vecTile();
		DWORD dwByte = 0;

		for (auto& pTile : vecTile)
			WriteFile(hFile, &pTile->Get_Tile(), sizeof(TILEINFO), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}

// Ÿ�� ����Ʈ Ŭ��
void CTileForm::OnLbnSelchangeList2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// �� ����Ʈ Ŭ��
void CTileForm::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTileForm::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CTileForm::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CToolTileMgr::Get_Instance()->LoadTile(L"../Data/TileInfoAreaD5.dat");
}

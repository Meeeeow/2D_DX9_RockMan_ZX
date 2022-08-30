// PathExtraction.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool_RockMan_ZX.h"
#include "PathExtraction.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CPathExtraction ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathExtraction, CDialog)

CPathExtraction::CPathExtraction(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHEXTRACTION, pParent)
{

}

CPathExtraction::~CPathExtraction()
{
	for (auto& pPath : m_listPathInfo)
		SAFE_DELETE(pPath);
	m_listPathInfo.clear();
}

void CPathExtraction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listPathBox);
}


BEGIN_MESSAGE_MAP(CPathExtraction, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathExtraction::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathExtraction::OnBnClickedButton2)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathExtraction �޽��� ó�����Դϴ�.



// Save
void CPathExtraction::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	wofstream fout;

	fout.open(L"../Data/PathInfo.txt");

	if (!fout.fail())
	{
		for (auto& pPathInfo : m_listPathInfo)
		{
			fout << pPathInfo->wstrFilePath << L"|" << pPathInfo->wstrObjectKey << L"|" << pPathInfo->wstrStateKey << L"|" << pPathInfo->dwCount << endl;
		}
		fout.close();
	}
}

// Load
void CPathExtraction::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	wifstream fin;
	fin.open(L"../Data/PathInfo.txt");
	if (!fin.fail())
	{
		for (auto& pPathInfo : m_listPathInfo)
			SAFE_DELETE(pPathInfo);
		m_listPathInfo.clear();
		m_listPathBox.ResetContent();

		TCHAR szPath[MAX_PATH]{};
		TCHAR szObjectKey[MAX_PATH]{};
		TCHAR szStateKey[MAX_PATH]{};
		TCHAR szCount[MAX_PATH]{};

		while (true)
		{
			fin.getline(szPath, MAX_PATH, L'|');
			fin.getline(szObjectKey, MAX_PATH, L'|');
			fin.getline(szStateKey, MAX_PATH, L'|');
			fin.getline(szCount, MAX_PATH);

			if (fin.eof())
			{
				break;
			}

			wstring wstrCombine = L"";


			wstrCombine = wstring(szPath) + L"|" + szObjectKey + L"|" + szStateKey + L"|" + szCount;
			m_listPathBox.AddString(wstrCombine.c_str());

		}
		fin.close();
	}
	m_listPathBox.SetHorizontalExtent(800);
}




void CPathExtraction::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);
	TCHAR szBuf[MAX_PATH]{};

	for (auto& pPath : m_listPathInfo)
		SAFE_DELETE(pPath);
	m_listPathInfo.clear();

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szBuf, MAX_PATH);
		CFileInfo::PathInfoExtraction(szBuf, m_listPathInfo);
	}


	m_listPathBox.ResetContent();
	TCHAR szCount[MAX_PATH]{};
	wstring wstrCombine = L"";

	for (auto& pPathInfo : m_listPathInfo)
	{
		_itow_s(pPathInfo->dwCount, szCount, 10);
		wstrCombine = pPathInfo->wstrFilePath + L"|" + pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|" + szCount;
		m_listPathBox.AddString(wstrCombine.c_str());
	}
	m_listPathBox.SetHorizontalExtent(1600);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

// PathExtraction.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool_RockMan_ZX.h"
#include "PathExtraction.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CPathExtraction 대화 상자입니다.

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


// CPathExtraction 메시지 처리기입니다.



// Save
void CPathExtraction::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

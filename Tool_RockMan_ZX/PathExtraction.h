#pragma once
#include "afxwin.h"


// CPathExtraction 대화 상자입니다.

class CPathExtraction : public CDialog
{
	DECLARE_DYNAMIC(CPathExtraction)

public:
	CPathExtraction(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathExtraction();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CListBox m_listPathBox;
	list<PATHINFO*> m_listPathInfo;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

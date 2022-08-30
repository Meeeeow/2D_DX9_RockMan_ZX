#pragma once
#include "afxwin.h"


// CPathExtraction ��ȭ �����Դϴ�.

class CPathExtraction : public CDialog
{
	DECLARE_DYNAMIC(CPathExtraction)

public:
	CPathExtraction(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathExtraction();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	CListBox m_listPathBox;
	list<PATHINFO*> m_listPathInfo;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

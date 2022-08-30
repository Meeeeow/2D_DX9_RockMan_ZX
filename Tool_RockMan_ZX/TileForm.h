#pragma once



// CTileForm 폼 뷰입니다.
#include "TileTool.h"
#include "afxwin.h"

class CTileForm : public CFormView
{
	DECLARE_DYNCREATE(CTileForm)

protected:
	CTileForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTileForm();
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILEFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListTile;
	CListBox m_ListScene;
	void AutoFindTileFromFolder();
	void AutoFindSceneFromFolder();

	CButton m_RenderType[2];
	map<CString, TILEINFO*> m_mapTileList;
	TILEINFO m_tTile;

	CStatic m_PicScene;
	afx_msg void FindFile();
	// DrawID
	int m_iDrawID;
	// OptionID
	int m_iOptionID;
	afx_msg void OnBnClickedButton3();
	CStatic m_PictureTile;
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};



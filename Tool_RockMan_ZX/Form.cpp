// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool_RockMan_ZX.h"
#include "Form.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CForm::ClickUnitTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CForm::OnBnClickedButton2)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.

// Unit
void CForm::ClickUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dialUnitTool.GetSafeHwnd() == nullptr)
		m_dialUnitTool.Create(IDD_UNITTOOL);
	m_dialUnitTool.ShowWindow(SW_SHOW);
}


//Path Extraction
void CForm::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dialPathExtraction.GetSafeHwnd() == nullptr)
		m_dialPathExtraction.Create(IDD_PATHEXTRACTION);
	m_dialPathExtraction.ShowWindow(SW_SHOW);
}

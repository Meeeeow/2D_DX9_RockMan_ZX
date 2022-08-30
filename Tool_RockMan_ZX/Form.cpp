// Form.cpp : ���� �����Դϴ�.
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


// CForm �����Դϴ�.

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


// CForm �޽��� ó�����Դϴ�.

// Unit
void CForm::ClickUnitTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_dialUnitTool.GetSafeHwnd() == nullptr)
		m_dialUnitTool.Create(IDD_UNITTOOL);
	m_dialUnitTool.ShowWindow(SW_SHOW);
}


//Path Extraction
void CForm::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_dialPathExtraction.GetSafeHwnd() == nullptr)
		m_dialPathExtraction.Create(IDD_PATHEXTRACTION);
	m_dialPathExtraction.ShowWindow(SW_SHOW);
}

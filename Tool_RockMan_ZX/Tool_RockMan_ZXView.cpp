
// Tool_RockMan_ZXView.cpp : CTool_RockMan_ZXView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool_RockMan_ZX.h"
#endif

#include "MainFrm.h"
#include "TileForm.h"
#include "Form.h"
#include "ToolScene.h"
#include "Tool_RockMan_ZXDoc.h"
#include "Tool_RockMan_ZXView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWND;
// CTool_RockMan_ZXView

IMPLEMENT_DYNCREATE(CTool_RockMan_ZXView, CScrollView)

BEGIN_MESSAGE_MAP(CTool_RockMan_ZXView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CTool_RockMan_ZXView ����/�Ҹ�

CTool_RockMan_ZXView::CTool_RockMan_ZXView()
	:m_pBackGround(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTool_RockMan_ZXView::~CTool_RockMan_ZXView()
{
	SAFE_DELETE(m_pBackGround);
	CToolGraphicDevice::Destroy_Instance();
	CToolTextureMgr::Destroy_Instance();
}

BOOL CTool_RockMan_ZXView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CTool_RockMan_ZXView �׸���

void CTool_RockMan_ZXView::OnDraw(CDC* /*pDC*/)
{
	CTool_RockMan_ZXDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CToolGraphicDevice::Get_Instance()->Render_Begin();
	m_pBackGround->Render_Scene();
	CToolTileMgr::Get_Instance()->RenderTile();
	CToolGraphicDevice::Get_Instance()->Render_Stop();
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CTool_RockMan_ZXView �μ�

BOOL CTool_RockMan_ZXView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTool_RockMan_ZXView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTool_RockMan_ZXView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CTool_RockMan_ZXView ����

#ifdef _DEBUG
void CTool_RockMan_ZXView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTool_RockMan_ZXView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTool_RockMan_ZXDoc* CTool_RockMan_ZXView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool_RockMan_ZXDoc)));
	return (CTool_RockMan_ZXDoc*)m_pDocument;
}
#endif //_DEBUG


// CTool_RockMan_ZXView �޽��� ó����


void CTool_RockMan_ZXView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(11200 , 1600));
	g_hWND = m_hWnd;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0 ,WINCX + iGapX, WINCY + iGapY, SWP_NOMOVE);
	if (FAILED(CToolGraphicDevice::Get_Instance()->Init_Graphic()))
	{
		MSG_BOX(L"�׷��� ����̽� Init ����");
		return;
	}

	if (!m_pBackGround)
		m_pBackGround = new CToolScene;
	m_pBackGround->Set_View(this);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void CTool_RockMan_ZXView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTileForm* pTileForm = dynamic_cast<CTileForm*>(pMain->m_wSubSplitter.GetPane(1, 0));
	int iSelect = 0;
	for (int i = 0; i < 2; i++)
	{
		if (pTileForm->m_RenderType[i].GetCheck())
			iSelect = i;
	}

	if (iSelect == 0)
	{
		int DrawID = pTileForm->m_iDrawID;
		int OptionID = pTileForm->m_iOptionID;
		D3DXVECTOR3 vMouse = { float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f };
		CToolTileMgr::Get_Instance()->Chagne(vMouse, DrawID, OptionID);
	}
	else if (iSelect == 1)
	{
		CString wstrFindName;
		int iIndex = pTileForm->m_ListScene.GetCurSel();

		pTileForm->m_ListScene.GetText(iIndex, wstrFindName);
		m_pBackGround->BackGround_Change(wstrFindName.operator LPCWSTR());
	}
	Invalidate(FALSE);
	CScrollView::OnLButtonDown(nFlags, point);
}

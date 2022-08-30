
// Tool_RockMan_ZXView.cpp : CTool_RockMan_ZXView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CTool_RockMan_ZXView 생성/소멸

CTool_RockMan_ZXView::CTool_RockMan_ZXView()
	:m_pBackGround(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTool_RockMan_ZXView::~CTool_RockMan_ZXView()
{
	SAFE_DELETE(m_pBackGround);
	CToolGraphicDevice::Destroy_Instance();
	CToolTextureMgr::Destroy_Instance();
}

BOOL CTool_RockMan_ZXView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CTool_RockMan_ZXView 그리기

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
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTool_RockMan_ZXView 인쇄

BOOL CTool_RockMan_ZXView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTool_RockMan_ZXView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTool_RockMan_ZXView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTool_RockMan_ZXView 진단

#ifdef _DEBUG
void CTool_RockMan_ZXView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTool_RockMan_ZXView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTool_RockMan_ZXDoc* CTool_RockMan_ZXView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool_RockMan_ZXDoc)));
	return (CTool_RockMan_ZXDoc*)m_pDocument;
}
#endif //_DEBUG


// CTool_RockMan_ZXView 메시지 처리기


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
		MSG_BOX(L"그래픽 디바이스 Init 실패");
		return;
	}

	if (!m_pBackGround)
		m_pBackGround = new CToolScene;
	m_pBackGround->Set_View(this);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CTool_RockMan_ZXView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

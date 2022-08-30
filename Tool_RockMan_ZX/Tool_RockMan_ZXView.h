
// Tool_RockMan_ZXView.h : CTool_RockMan_ZXView 클래스의 인터페이스
//

#pragma once
class CToolScene;
class CTool_RockMan_ZXDoc;
class CTool_RockMan_ZXView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CTool_RockMan_ZXView();
	DECLARE_DYNCREATE(CTool_RockMan_ZXView)

// 특성입니다.
public:
	CTool_RockMan_ZXDoc* GetDocument() const;

// 작업입니다.
public:
	CToolScene* m_pBackGround;
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTool_RockMan_ZXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Tool_RockMan_ZXView.cpp의 디버그 버전
inline CTool_RockMan_ZXDoc* CTool_RockMan_ZXView::GetDocument() const
   { return reinterpret_cast<CTool_RockMan_ZXDoc*>(m_pDocument); }
#endif


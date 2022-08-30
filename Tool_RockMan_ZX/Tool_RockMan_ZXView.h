
// Tool_RockMan_ZXView.h : CTool_RockMan_ZXView Ŭ������ �������̽�
//

#pragma once
class CToolScene;
class CTool_RockMan_ZXDoc;
class CTool_RockMan_ZXView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CTool_RockMan_ZXView();
	DECLARE_DYNCREATE(CTool_RockMan_ZXView)

// Ư���Դϴ�.
public:
	CTool_RockMan_ZXDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CToolScene* m_pBackGround;
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTool_RockMan_ZXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Tool_RockMan_ZXView.cpp�� ����� ����
inline CTool_RockMan_ZXDoc* CTool_RockMan_ZXView::GetDocument() const
   { return reinterpret_cast<CTool_RockMan_ZXDoc*>(m_pDocument); }
#endif


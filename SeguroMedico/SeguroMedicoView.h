
// SeguroMedicoView.h : interface of the CSeguroMedicoView class
//

#pragma once


class CSeguroMedicoView : public CView
{
protected: // create from serialization only
	CSeguroMedicoView() noexcept;
	DECLARE_DYNCREATE(CSeguroMedicoView)

// Attributes
public:
	CSeguroMedicoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSeguroMedicoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SeguroMedicoView.cpp
inline CSeguroMedicoDoc* CSeguroMedicoView::GetDocument() const
   { return reinterpret_cast<CSeguroMedicoDoc*>(m_pDocument); }
#endif


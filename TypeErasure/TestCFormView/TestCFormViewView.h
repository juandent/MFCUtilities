
// TestCFormViewView.h : interface of the CTestCFormViewView class
//

#pragma once


class CTestCFormViewView : public CView
{
protected: // create from serialization only
	CTestCFormViewView() noexcept;
	DECLARE_DYNCREATE(CTestCFormViewView)

// Attributes
public:
	CTestCFormViewDoc* GetDocument() const;

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
	virtual ~CTestCFormViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestCFormViewView.cpp
inline CTestCFormViewDoc* CTestCFormViewView::GetDocument() const
   { return reinterpret_cast<CTestCFormViewDoc*>(m_pDocument); }
#endif


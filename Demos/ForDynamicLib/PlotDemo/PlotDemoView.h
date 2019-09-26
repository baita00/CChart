// PlotDemoView.h : interface of the CPlotDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTDEMOVIEW_H__B7A9F41F_60F4_4581_B97D_E72A7B89FD6B__INCLUDED_)
#define AFX_PLOTDEMOVIEW_H__B7A9F41F_60F4_4581_B97D_E72A7B89FD6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../PreCompiled/Chart.h"
#if defined(_UNICODE) || defined(UNICODE)
#	pragma comment(lib,"../PreCompiled/CChartu.lib")
#else
#	pragma comment(lib,"../PreCompiled/CChart.lib")
#endif

using namespace NsCChart;


class CPlotDemoView : public CView
{
protected: // create from serialization only
	CPlotDemoView();
	DECLARE_DYNCREATE(CPlotDemoView)

// Attributes
public:
	CPlotDemoDoc* GetDocument();

// Operations
public:
	CChartWnd	m_ChartWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlotDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPlotDemoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PlotDemoView.cpp
inline CPlotDemoDoc* CPlotDemoView::GetDocument()
   { return (CPlotDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTDEMOVIEW_H__B7A9F41F_60F4_4581_B97D_E72A7B89FD6B__INCLUDED_)

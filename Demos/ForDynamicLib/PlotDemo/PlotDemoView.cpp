// PlotDemoView.cpp : implementation of the CPlotDemoView class
//

#include "stdafx.h"
#include "PlotDemo.h"

#include "PlotDemoDoc.h"
#include "PlotDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoView

IMPLEMENT_DYNCREATE(CPlotDemoView, CView)

BEGIN_MESSAGE_MAP(CPlotDemoView, CView)
	//{{AFX_MSG_MAP(CPlotDemoView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoView construction/destruction

CPlotDemoView::CPlotDemoView()
{
	// TODO: add construction code here

}

CPlotDemoView::~CPlotDemoView()
{
}

BOOL CPlotDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoView drawing

void CPlotDemoView::OnDraw(CDC* pDC)
{
	CPlotDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoView printing

BOOL CPlotDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPlotDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPlotDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoView diagnostics

#ifdef _DEBUG
void CPlotDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CPlotDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlotDemoDoc* CPlotDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlotDemoDoc)));
	return (CPlotDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoView message handlers

int CPlotDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	//Attach Window
	m_ChartWnd.Attach(m_hWnd, kTypeXY);
	
	//Add Data
	for(int i=0; i<10; i++)
	{
		m_ChartWnd.GetChart()->AddPoint2D(i, i*i);
	}
		
	//Modify the properties
	m_ChartWnd.GetChart()->SetTitle(_T("Test Dll"));
	m_ChartWnd.GetChart()->SetDataTitle(_T("Test data"), 0, 0 );
	
	return 0;
}

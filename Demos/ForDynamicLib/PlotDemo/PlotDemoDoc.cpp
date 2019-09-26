// PlotDemoDoc.cpp : implementation of the CPlotDemoDoc class
//

#include "stdafx.h"
#include "PlotDemo.h"

#include "PlotDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoDoc

IMPLEMENT_DYNCREATE(CPlotDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlotDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CPlotDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoDoc construction/destruction

CPlotDemoDoc::CPlotDemoDoc()
{
	// TODO: add one-time construction code here

}

CPlotDemoDoc::~CPlotDemoDoc()
{
}

BOOL CPlotDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPlotDemoDoc serialization

void CPlotDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoDoc diagnostics

#ifdef _DEBUG
void CPlotDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPlotDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoDoc commands

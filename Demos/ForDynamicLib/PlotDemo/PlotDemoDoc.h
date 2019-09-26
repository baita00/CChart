// PlotDemoDoc.h : interface of the CPlotDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTDEMODOC_H__E1DD7D9E_6950_4333_A052_5C009BAE30EC__INCLUDED_)
#define AFX_PLOTDEMODOC_H__E1DD7D9E_6950_4333_A052_5C009BAE30EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlotDemoDoc : public CDocument
{
protected: // create from serialization only
	CPlotDemoDoc();
	DECLARE_DYNCREATE(CPlotDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlotDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPlotDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTDEMODOC_H__E1DD7D9E_6950_4333_A052_5C009BAE30EC__INCLUDED_)

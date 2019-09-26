// PlotDemo.h : main header file for the PLOTDEMO application
//

#if !defined(AFX_PLOTDEMO_H__D3232A33_6264_41E8_AE1A_2077C354FD41__INCLUDED_)
#define AFX_PLOTDEMO_H__D3232A33_6264_41E8_AE1A_2077C354FD41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlotDemoApp:
// See PlotDemo.cpp for the implementation of this class
//

class CPlotDemoApp : public CWinApp
{
public:
	CPlotDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPlotDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTDEMO_H__D3232A33_6264_41E8_AE1A_2077C354FD41__INCLUDED_)

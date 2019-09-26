/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2019 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//                                  版权申明                                  //
//                         版权所有(C)2006-2019，杨国君                       //
//                                保留全部权利                                //
////////////////////////////////////////////////////////////////////////////////
/* ############################################################################################################################## */

#ifndef __GLOBAL_H_122333444455555__
#define __GLOBAL_H_122333444455555__

#include "MyString.h"
#include "CChartNameSpace.h"
#include <windows.h>
#include <prsht.h>
#include <commctrl.h>

/////////////////////////////////////////////////////////////////////////////////////
// For the additional dialogs
#include <vector>

Declare_Namespace_CChart

typedef struct stPagePara
{
	unsigned int ID;
	DLGPROC DlgProc;
	tstring title;
}PagePara;
typedef vector<PagePara> vPagePara;
//
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// For the langurage
enum
{
	kLangEnglish,
	kLangChinese,
	kLangCount
};
//
/////////////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////////////

const int	nMaxString = 1024;

class CGlobal
{
public:
	// Global information
	static	tstring		GetStrMyVersion();
	static	tstring		GetStrAuthorInfo();
	static	tstring		GetStrCopyright();
	static	tstring		GetStrLicense();
	static	tstring		GetStrStatement();
	static	tstring		GetStrContact();

	//
	static	tstring		GetStrUntitle();
	
	// For the loading of prepositioned patterns
	static	bool		m_bLoadDefPatterns;

	// For default axes
	static	bool		m_bAddDefAxes;
	
	// For compilation of dll file
	static	bool		m_bInDll;
	static	tstring		m_strDllName;
	static	HMODULE		MyGetModuleHandle();
	
	// for the data processing menu
	static	bool		m_bShowDataProcessingMenu;

	// langurage choosing
	static	int			m_nLangurage;

	// user init gdi+
	static	void		InitGdiPlus();
	static	void		FreeGdiPlus();
	//static	void		FinalizeGdiPlus();
	static	bool		IsGdiPlusInitialized();

	// tool tip
	static	bool			m_bEnableToolTip;

	// for CChartWnd
	static	bool			m_bContinuePaint, m_bContinueReact;

	// global context menu control
	static	bool			m_bImagePopupMenu;
};

Declare_Namespace_End

#endif

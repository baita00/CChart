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

#pragma once

#include <windows.h>

//#include "./Dialogs/DefPlotSettingDialogs/CopyRightDlg.h"
//#include "./Dialogs/DefPlotSettingDialogs.h"
//#include "./Dialogs/DefAxisSettingDialogs.h"
//#include "./Dialogs/DefDataSettingDialogs.h"
//#include "./ContextMenu/DefContextMenu.h"

#include "../MyString.h"
#include "../CChartNameSpace.h"

using namespace std;

#pragma comment(lib,"comctl32.lib")

Declare_Namespace_CChart

tstring	GetFontDescription( LOGFONT *font );

UINT	GetPopupCheckedFlag( BOOL how );
bool	ShowFileDialog(HWND hWnd, bool open, tstring &filename, tstring &fileext, tstring &filepath,
					   LPCTSTR filter = _TEXT("Text Files(*.txt)\0*.TXT\0All Files(*.*)\0*.*\0\0"),
					   LPCTSTR defExt = _TEXT("txt"));
bool	ShowFontDialog(HWND hWnd, LOGFONT &logfont, COLORREF &color);
bool	ShowColorDialog(HWND hWnd, COLORREF &cr);
bool	ShowPrintDialog(HWND hWnd, bool stretch);

Declare_Namespace_End

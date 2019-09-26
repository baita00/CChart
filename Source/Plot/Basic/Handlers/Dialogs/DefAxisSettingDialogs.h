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
//#include "DefAxisSettingDialogs.cpp"
//class CXYPlotImplI;

//bool DefAxisSettings( CXYPlotImplI *plot, HDC hDC, int location );

#include <tchar.h>
#include <sstream>
#include <math.h>

#include "../../../Resources/PlotResources.h"
#include "../../MyString.h"
#include "../../CChartNameSpace.h"


#include "DefAxisSettingDialogs/AxisTitleDlg.h"
#include "DefAxisSettingDialogs/AxisTickLabelDlg.h"
#include "DefAxisSettingDialogs/AxisRangeDlg.h"
#include "DefAxisSettingDialogs/AxisColorDlg.h"
#include "DefAxisSettingDialogs/AxisMajorTicksDlg.h"
#include "DefAxisSettingDialogs/AxisMinorTicksDlg.h"
#include "DefAxisSettingDialogs/AxisArrowsDlg.h"
#include "DefAxisSettingDialogs/AxisColorBarDlg.h"
#include "DefAxisSettingDialogs/AxisMiscellaneousDlg.h"

Declare_Namespace_CChart

/////////////////////////////////////////////////////////////////////////
// Axis Settings

template<class PlotImplT>
struct AxisSettingParam
{
	PlotImplT *plot;
	int location;
};


extern double tlScale;

template<class PlotImplT>
class ClassForDefAxisSettings
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK DefAxisSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
#else
	static	LRESULT CALLBACK DefAxisSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
#endif
};


template<class PlotImplT>
tstring	GetMainLabel( PlotImplT *plot, int index );

template<class PlotImplT>
void InitAllTabs(HWND *hPages, AxisSettingParam<PlotImplT> *para);

/////////////////////////////////////////////////////////////////////////

template<class PlotImplT>
bool DefAxisSettings( PlotImplT *plot, HDC hDC, int location )
{
	AxisSettingParam<PlotImplT> para;
	para.plot = plot;
	para.location = location;

	
	UINT dlgID;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		dlgID = IDD_AXISSETTINGS;
		break;
	case kLangChinese:
		dlgID = IDD_AXISSETTINGS_C;
		break;
	default:
		return false;
	}
	if(DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(dlgID), WindowFromDC(hDC), (DLGPROC)ClassForDefAxisSettings<PlotImplT>::DefAxisSettingsFNew, (LPARAM)&para))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

//bool DefAxisSettings( CXYPlotImplI *plot, HDC hDC, int location )
//{
//	return DefAxisSetting<CXYPlotImplI>(plot, hDC, location);
//}

const int nAxisPages = 10;

template<class PlotImplT>
#ifndef _WIN64
BOOL CALLBACK ClassForDefAxisSettings<PlotImplT>::DefAxisSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
LRESULT CALLBACK ClassForDefAxisSettings<PlotImplT>::DefAxisSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
{
	static PlotImplT *plot;
	static int location;
	static AxisSettingParam<PlotImplT> *para;

	static HWND hPage[nAxisPages];
	static int curpage = 0;
	TCITEM tcitem;
	RECT rt;

	LPNMHDR pnmh;

#ifndef _WIN64
	int num;
#else
	LRESULT num;
#endif
	int i;
	tstring label;
	UINT style;
	switch(message)
	{
	case WM_INITDIALOG:
		para = (AxisSettingParam<PlotImplT> *)lparam;
		plot = para->plot;
		location = para->location;
		
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_DELETEALLITEMS, 0, 0);
		// add pages
		ZeroMemory(&tcitem, sizeof(TCITEM));
		tcitem.mask = TCIF_PARAM | TCIF_TEXT;
		tcitem.iImage = -1;
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Title");
			break;
		case kLangChinese:
			tcitem.pszText = _T("标题");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 0, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Tick label");
			break;
		case kLangChinese:
			tcitem.pszText = _T("标签");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 1, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Range");
			break;
		case kLangChinese:
			tcitem.pszText = _T("范围");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 2, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Axis line");
			break;
		case kLangChinese:
			tcitem.pszText = _T("轴线");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 3, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Major ticks");
			break;
		case kLangChinese:
			tcitem.pszText = _T("主刻度");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 4, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Minor ticks");
			break;
		case kLangChinese:
			tcitem.pszText = _T("副刻度");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 5, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Arrows");
			break;
		case kLangChinese:
			tcitem.pszText = _T("箭头");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 6, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Color bar");
			break;
		case kLangChinese:
			tcitem.pszText = _T("颜色棒");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 7, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Miscellaneous");
			break;
		case kLangChinese:
			tcitem.pszText = _T("杂项");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 8, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Copyright(C)");
			break;
		case kLangChinese:
			tcitem.pszText = _T("版权声明(C)");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_AXIS_SETTINGS_TAB, TCM_INSERTITEM, 9, (LPARAM)&tcitem);

		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[0] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_TITLE), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisTitleDlg<PlotImplT>::AxisTitleDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[0] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_TITLE_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisTitleDlg<PlotImplT>::AxisTitleDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[1] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_TICKLABEL), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisTickLabelDlg<PlotImplT>::AxisTickLabelDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[1] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_TICKLABEL_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisTickLabelDlg<PlotImplT>::AxisTickLabelDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[2] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_RANGE), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisRangeDlg<PlotImplT>::AxisRangeDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[2] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_RANGE_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisRangeDlg<PlotImplT>::AxisRangeDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[3] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_AXISCOLOR), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisColorsDlg<PlotImplT>::AxisColorsDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[3] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_AXISCOLOR_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisColorsDlg<PlotImplT>::AxisColorsDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[4] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_MAJORTICKS), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisMajorTicksDlg<PlotImplT>::AxisMajorTicksDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[4] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_MAJORTICKS_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisMajorTicksDlg<PlotImplT>::AxisMajorTicksDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[5] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_MINORTICKS), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisMinorTicksDlg<PlotImplT>::AxisMinorTicksDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[5] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_MINORTICKS_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisMinorTicksDlg<PlotImplT>::AxisMinorTicksDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[6] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_ARROWS), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisArrowsDlg<PlotImplT>::AxisArrowsDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[6] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_ARROWS_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisArrowsDlg<PlotImplT>::AxisArrowsDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[7] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_COLORBAR), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisColorBarDlg<PlotImplT>::AxisColorBarDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[7] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_COLORBAR_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisColorBarDlg<PlotImplT>::AxisColorBarDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[8] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_MISCELLANEOUS), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisMiscellaneousDlg<PlotImplT>::AxisMiscellaneousDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[8] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_AXISSETTINGS_MISCELLANEOUS_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForAxisMiscellaneousDlg<PlotImplT>::AxisMiscellaneousDlgFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[9] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_XYSETTINGS_COPYRIGHT), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForPlotCopyrightDlg<PlotImplT>::PlotCopyrightDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[9] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_XYSETTINGS_COPYRIGHT_C), GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), ClassForPlotCopyrightDlg<PlotImplT>::PlotCopyrightDlgFunc, (LPARAM)para);
			break;
		}
		style = GetWindowLong(hPage[8], GWL_STYLE);
		SetWindowLong(hPage[9],  GWL_STYLE, style);

		GetClientRect(GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), &rt);
		rt.top += 20;
		rt.bottom -= 8;
		rt.left += 8;
		rt.right -= 8;

		for(i=0; i<nAxisPages; i++)
		{
			MoveWindow(hPage[i], rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, TRUE);
			if(i==curpage)
			{
				ShowWindow(hPage[i], SW_SHOW);
			}
			else
			{
				ShowWindow(hPage[i], SW_HIDE);
			}
		}
		

		//create the combo box
		//i = plot->GetAxisIndex(location);
		SendDlgItemMessage(hWnd, IDC_AS_LISTAXES, CB_RESETCONTENT, 0, 0);
		for(i=plot->CPlotImpl::GetAxesCount()-1; i>=0; i--)
		{
			label = GetMainLabel(plot, i);
			SendDlgItemMessage(hWnd, IDC_AS_LISTAXES, CB_INSERTSTRING, 0, (LPARAM)label.c_str());
		}
		SendDlgItemMessage(hWnd, IDC_AS_LISTAXES, CB_SETCURSEL, (WPARAM)plot->GetAxisIndex(location), 0);

		// check the save when switch box
		SendDlgItemMessage(hWnd, IDC_AS_SAVE_WHEN_SWITCHING, BM_SETCHECK, TRUE, 0);

		TabCtrl_SetCurSel(GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB), curpage);

		return TRUE;
	case WM_DESTROY:
		for(i=0; i<nAxisPages; i++)
		{
			if(IsWindow(hPage[i]))
				DestroyWindow(hPage[i]);
		}
		return TRUE;;
	case WM_COMMAND:
		if(HIWORD (wparam) == CBN_SELCHANGE && LOWORD(wparam) == IDC_AS_LISTAXES )
		{
			num = SendDlgItemMessage(hWnd, IDC_AS_LISTAXES, CB_GETCURSEL, 0, 0);
			if( SendDlgItemMessage(hWnd, IDC_AS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
			{
				for(i=0; i<nAxisPages; i++)
				{
					SendMessage(hPage[i],	WM_COMMAND, IDOK, 0);
				}
			}
			location = plot->CPlotImpl::GetAxis((int)num)->GetLocation();
			para->location = location;
			InitAllTabs(hPage, para);
			return TRUE;
		}
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_AXIS_PREVIOUS || LOWORD(wparam)==IDC_AXIS_NEXT)
			{
				num = SendDlgItemMessage(hWnd, IDC_AS_LISTAXES, CB_GETCURSEL, 0, 0);
				if( SendDlgItemMessage(hWnd, IDC_AS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
				{
					for(i=0; i<nAxisPages; i++)
					{
						SendMessage(hPage[i],	WM_COMMAND, IDOK, 0);
					}
				}
				if(LOWORD(wparam)==IDC_DATA_PREVIOUS)
				{
					num--;
					if (num<0 ) num = plot->CPlotImpl::GetAxesCount()-1;
				}
				else
				{
					num++;
					if( num>= plot->CPlotImpl::GetAxesCount() )num=0;
				}
				SendDlgItemMessage(hWnd, IDC_AS_LISTAXES, CB_SETCURSEL, (WPARAM)num, 0);
				location = plot->CPlotImpl::GetAxis((int)num)->GetLocation();
				para->location = location;
				InitAllTabs(hPage, para);
				return TRUE;
			}
		}
		switch(LOWORD(wparam))
		{
		case IDOK:
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB));
			SendMessage(hPage[curpage], WM_COMMAND, IDOK, 0);
			
			EndDialog(hWnd, 1);
			return TRUE;
		case IDCANCEL:
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB));
			EndDialog(hWnd, 0);
			return FALSE;
		}
		break;
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
			case TCN_SELCHANGE:
			// Apply Changes
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB));
			for(i=0; i<nAxisPages; i++)
			{
				if(i!=curpage)
					ShowWindow(hPage[i], SW_HIDE);
				else
					ShowWindow(hPage[i], SW_SHOW);
			}
			return TRUE;
			case TCN_SELCHANGING:
			{
				if( SendDlgItemMessage(hWnd, IDC_AS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
				{
					curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_AXIS_SETTINGS_TAB));
					SendMessage(hPage[curpage], WM_COMMAND, IDOK, 0);
					return TRUE;
				}
			}
		}
		break;
	}
	return FALSE;
}

template<class PlotImplT>
tstring	GetMainLabel( PlotImplT *plot, int index )
{
	if(index<0 || index >= plot->CPlotImpl::GetAxesCount())return _TEXT("");
	
	tstring		 location;
	CAxisImpl		*axis = plot->CPlotImpl::GetAxis(index);//&m_pAxes[index];
	
	if( axis != NULL )
	{
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			switch( axis->GetLocation() )
			{
			case kLocationBottom: location = _TEXT("bottom"); break;
			case kLocationLeft: location = _TEXT("left"); break;
			case kLocationRight: location = _TEXT("right"); break;
			case kLocationTop: location = _TEXT("top"); break;
			case kLocationCenterHB: location = _TEXT("center hb"); break;
			case kLocationCenterHT: location = _TEXT("center ht"); break;
			case kLocationCenterVL: location = _TEXT("center vl"); break;
			case kLocationCenterVR: location = _TEXT("center vr"); break;
			default: break;
			}
			break;
		case kLangChinese:
			switch( axis->GetLocation() )
			{
			case kLocationBottom: location = _TEXT("底轴"); break;
			case kLocationLeft: location = _TEXT("左轴"); break;
			case kLocationRight: location = _TEXT("右轴"); break;
			case kLocationTop: location = _TEXT("上轴"); break;
			case kLocationCenterHB: location = _TEXT("中轴水平偏下"); break;
			case kLocationCenterHT: location = _TEXT("中轴水平偏上"); break;
			case kLocationCenterVL: location = _TEXT("中轴竖直偏左"); break;
			case kLocationCenterVR: location = _TEXT("中轴竖直偏右"); break;
			default: break;
			}
			break;
		}
	}
	else location = _TEXT("");
	
	tostringstream ostr;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		ostr<<_TEXT("Axis ")<<index+1<<_TEXT(" of ")<<plot->CPlotImpl::GetAxesCount()<<_TEXT(", (")<<location.c_str()<<_TEXT(")");
		break;
	case kLangChinese:
		ostr<<plot->CPlotImpl::GetAxesCount()<<_TEXT("条坐标轴的第")<<index+1<<_TEXT("条, (")<<location.c_str()<<_TEXT(")");
		break;
	}
		
	return ostr.str();
}

template<class PlotImplT>
void InitAllTabs(HWND *hPages, AxisSettingParam<PlotImplT> *para)
{
	for(int i=0; i<nAxisPages; i++)
	{
		SendMessage(hPages[i], WM_INITDIALOG, 0, (LPARAM)para);
	}
}

Declare_Namespace_End

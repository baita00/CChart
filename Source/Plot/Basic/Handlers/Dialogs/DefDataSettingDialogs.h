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
//#include "DefDataSettingDialogs.cpp"

//class CXYPlotImplI;

//bool	DefDataSettings( CXYPlotImplI *plot, HDC hDC, int whichDataSet );

#pragma once
#include <tchar.h>
#include <sstream>

#include "../../../Resources/PlotResources.h"
#include "../../MyString.h"
#include "ColorControl.h"

#include "DefDataSettingDialogs/DataTitleDlg.h"
#include "DefDataSettingDialogs/DataLineDlg.h"
#include "DefDataSettingDialogs/DataChartTypeDlg.h"
#include "DefDataSettingDialogs/DataFillDlg.h"
#include "DefDataSettingDialogs/DataMarkersDlg.h"
#include "DefDataSettingDialogs/DataManagementDlg.h"

Declare_Namespace_CChart

/////////////////////////////////////////////////////////////////////////
// Data Settings

class CXYPlotImplI;

template<class PlotImplT>
struct DataSettingParam
{
	PlotImplT *plot;
	int whichDataSet;
};

template<class PlotImplT>
class ClassForDefDataSettings
{
public:
#ifndef _WIN64
	static	BOOL CALLBACK DefDataSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
#else
	static	LRESULT CALLBACK DefDataSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
#endif
};


template<class PlotImplT>
void InitAllTabs(HWND *hPages, DataSettingParam<PlotImplT> *para);


/////////////////////////////////////////////////////////////////////////

template<class PlotImplT>
bool DefDataSettings( PlotImplT *plot, HDC hDC, int whichDataSet )
{
	DataSettingParam<PlotImplT> para;
	para.plot = plot;
	para.whichDataSet = whichDataSet;
	
	UINT dlgID;
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		dlgID = IDD_DATASETTINGS;
		break;
	case kLangChinese:
		dlgID = IDD_DATASETTINGS_C;
		break;
	default:
		return false;
	}
	if(DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(dlgID), WindowFromDC(hDC), (DLGPROC)ClassForDefDataSettings<PlotImplT>::DefDataSettingsFNew, (LPARAM)&para))
		return true;
	else
		return false;
	
}

//bool	DefDataSettings( CXYPlotImplI *plot, HDC hDC, int whichDataSet )
//{
//	return DefDataSetting<CXYPlotImplI>(plot, hDC, whichDataSet);
//}

const int nDataPages=7;
template<class PlotImplT>
#ifndef _WIN64
BOOL CALLBACK ClassForDefDataSettings<PlotImplT>::DefDataSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#else
LRESULT CALLBACK ClassForDefDataSettings<PlotImplT>::DefDataSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
#endif
{
	static PlotImplT *plot;
#ifndef _WIN64
	static int whichDataSet;
#else
	static LRESULT whichDataSet;
#endif
	static DataSettingParam<PlotImplT> *para;

	static HWND hPage[nDataPages];
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
	tostringstream ostr;
	tstring str;
	UINT style;
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam<PlotImplT> *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;

		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_DELETEALLITEMS, 0, 0);
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
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 0, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Line");
			break;
		case kLangChinese:
			tcitem.pszText = _T("连线");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 1, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Plot type");
			break;
		case kLangChinese:
			tcitem.pszText = _T("类型");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 2, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Fill Curve");
			break;
		case kLangChinese:
			tcitem.pszText = _T("填充");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 3, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Markers");
			break;
		case kLangChinese:
			tcitem.pszText = _T("标记");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 4, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Management");
			break;
		case kLangChinese:
			tcitem.pszText = _T("管理");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 5, (LPARAM)&tcitem);
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			tcitem.pszText = _T("Copyright(C)");
			break;
		case kLangChinese:
			tcitem.pszText = _T("版权声明(C)");
			break;
		}
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 6, (LPARAM)&tcitem);

		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[0] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_TITLE), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataTitleDlg<PlotImplT>::DataTitleSettingsFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[0] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_TITLE_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataTitleDlg<PlotImplT>::DataTitleSettingsFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[1] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_LINE), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataLingDlg<PlotImplT>::DataLineSettingsFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[1] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_LINE_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataLingDlg<PlotImplT>::DataLineSettingsFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[2] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_CHARTTYPE), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataChartTypeDlg<PlotImplT>::DataChartTypeSettingsFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[2] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_CHARTTYPE_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataChartTypeDlg<PlotImplT>::DataChartTypeSettingsFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[3] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_FILL), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataFillDlg<PlotImplT>::DataFillSettingsFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[3] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_FILL_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataFillDlg<PlotImplT>::DataFillSettingsFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[4] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_MAKERS), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataMarkersDlg<PlotImplT>::DataMarkersSettingsFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[4] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_MAKERS_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataMarkersDlg<PlotImplT>::DataMarkersSettingsFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[5] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_MANAGEMENT), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataManagementDlg<PlotImplT>::DataManagementSettingsFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[5] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_MANAGEMENT_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForDataManagementDlg<PlotImplT>::DataManagementSettingsFunc, (LPARAM)para);
			break;
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			hPage[6] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_XYSETTINGS_COPYRIGHT), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForPlotCopyrightDlg<PlotImplT>::PlotCopyrightDlgFunc, (LPARAM)para);
			break;
		case kLangChinese:
			hPage[6] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_XYSETTINGS_COPYRIGHT_C), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), ClassForPlotCopyrightDlg<PlotImplT>::PlotCopyrightDlgFunc, (LPARAM)para);
			break;
		}
		style = GetWindowLong(hPage[5], GWL_STYLE);
		SetWindowLong(hPage[6],  GWL_STYLE, style);

		GetClientRect(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), &rt);
		rt.top += 20;
		rt.bottom -= 8;
		rt.left += 8;
		rt.right -= 8;

		for(i=0; i<nDataPages; i++)
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
		SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_RESETCONTENT, 0, 0);
		for(i=plot->GetPlotDataCount()-1; i>=0; i--)
		{
			ostr.str(_TEXT(""));
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<_TEXT("Data ")<<i+1<<_TEXT(" of ")<<plot->GetPlotDataCount()<<_TEXT(": ")<<plot->GetAbsDataTitle(i).c_str();
				break;
			case kLangChinese:
				ostr<<plot->GetPlotDataCount()<<_TEXT("条曲线的第")<<i+1<<_TEXT("条: ")<<plot->GetAbsDataTitle(i).c_str();
				break;
			}
			str=ostr.str();
			SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_INSERTSTRING, 0, (LPARAM)str.c_str());
		}
		SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_SETCURSEL, (WPARAM)whichDataSet, 0);

		// check the save when switching box
		SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_SETCHECK, TRUE, 0);

		TabCtrl_SetCurSel(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), curpage);

		return TRUE;
	case WM_DESTROY:
		for(i=0; i<nDataPages; i++)
		{
			if(IsWindow(hPage[i]))
				DestroyWindow(hPage[i]);
		}
		return TRUE;
	case WM_COMMAND:
		if(HIWORD (wparam) == CBN_SELCHANGE && LOWORD(wparam) == IDC_DS_LISTDATASETS )
		{
			if( SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
			{
				for(i=0; i<nDataPages; i++)
				{
					SendMessage(hPage[i], WM_COMMAND, IDOK, 0);
				}
//				ostr.str(_TEXT(""));
//				ostr<<_TEXT("Data ")<<whichDataSet+1<<_TEXT(" of ")<<plot->GetDataSetCount()<<_TEXT(": ")<<plot->GetDataTitle(plot->GetDataID(whichDataSet)).c_str();
//				str=ostr.str();
//				SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_INSERTSTRING, (WPARAM)whichDataSet, (LPARAM)str.c_str());
//				SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_DELETESTRING, (WPARAM)(whichDataSet+1), 0);
			}

			whichDataSet = SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_GETCURSEL, 0, 0);
			if(whichDataSet<0)return FALSE;
			para->whichDataSet = (int)whichDataSet;
			InitAllTabs(hPage, para);
			return TRUE;
		}
//		if(HIWORD (wparam) == CBN_DBLCLK && LOWORD(wparam) == IDC_DS_LISTDATASETS )
//		{
//			return TRUE;
//		}
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_DATA_PREVIOUS || LOWORD(wparam)==IDC_DATA_NEXT)
			{
				if( SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
				{
					for(i=0; i<nDataPages; i++)
					{
						SendMessage(hPage[i], WM_COMMAND, IDOK, 0);
					}
					ostr.str(_TEXT(""));
					switch(CGlobal::m_nLangurage)
					{
					case kLangEnglish:
						//ostr<<_TEXT("Data ")<<i+1<<_TEXT(" of ")<<plot->GetDataSetCount()<<_TEXT(": ")<<plot->GetDataTitle(plot->GetDataID(i)).c_str();
						ostr<<_TEXT("Data ")<<(int)whichDataSet+1<<_TEXT(" of ")<<plot->GetPlotDataCount()<<_TEXT(": ")<<plot->GetAbsDataTitle((int)whichDataSet).c_str();
						break;
					case kLangChinese:
						ostr<<plot->GetPlotDataCount()<<_TEXT("条曲线的第")<<(int)whichDataSet+1<<_TEXT("条: ")<<plot->GetAbsDataTitle((int)whichDataSet).c_str();
						break;
					}
					str=ostr.str();
					SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_INSERTSTRING, (WPARAM)whichDataSet, (LPARAM)str.c_str());
					SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_DELETESTRING, (WPARAM)(whichDataSet+1), 0);
				}

				//num = SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_GETCURSEL, 0, 0);
				num = whichDataSet;
				
				if(LOWORD(wparam)==IDC_DATA_PREVIOUS)
				{
					num--;
					if (num<0 ) num = plot->GetPlotDataCount()-1;
				}
				else
				{
					num++;
					if( num>= plot->GetPlotDataCount() )num=0;
				}
				SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_SETCURSEL, (WPARAM)num, 0);
				whichDataSet = num;
				para->whichDataSet = (int)whichDataSet;
				InitAllTabs(hPage, para);
				return TRUE;
			}
		
		}
		switch(LOWORD(wparam))
		{
		case IDOK:
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB));
			SendMessage(hPage[curpage], WM_COMMAND, IDOK, 0);		
			EndDialog(hWnd, 1);
			return TRUE;
		case IDCANCEL:
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB));
			EndDialog(hWnd, 0);
			return FALSE;
		}
		break;
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case TCN_SELCHANGE:
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB));
			for(i=0; i<nDataPages; i++)
			{
				if(i!=curpage)
					ShowWindow(hPage[i], SW_HIDE);
				else
					ShowWindow(hPage[i], SW_SHOW);
			}
			return TRUE;
		case TCN_SELCHANGING:
			{
				if( SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
				{
					curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB));
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
void InitAllTabs(HWND *hPages, DataSettingParam<PlotImplT> *para)
{
	for(int i=0; i<nDataPages; i++)
	{
		SendMessage(hPages[i], WM_INITDIALOG, 0, (LPARAM)para);
	}
}

Declare_Namespace_End
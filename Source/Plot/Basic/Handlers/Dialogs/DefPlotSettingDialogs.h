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

//bool	DefPlotSettings( CXYPlotImplI *plot, HDC hDC );

#include "../../../Resources/PlotResources.h"
//#include "../../XYPlotImpl.h"


#include "../../Global.h"

#include "DefPlotSettingDialogs/TitleDlg.h"
#include "DefPlotSettingDialogs/LegendDlg.h"
#include "DefPlotSettingDialogs/RangeDlg.h"
#include "DefPlotSettingDialogs/BkgndDlg.h"
#include "DefPlotSettingDialogs/MajorGridsDlg.h"
#include "DefPlotSettingDialogs/MinorGridsDlg.h"
#include "DefPlotSettingDialogs/AutoSettingDlg.h"
#include "DefPlotSettingDialogs/ExtraFunctionDlg.h"
#include "DefPlotSettingDialogs/CursorDlg.h"
#include "DefPlotSettingDialogs/SelZoneDlg.h"
#include "DefPlotSettingDialogs/CopyRightDlg.h"

Declare_Namespace_CChart

//class CXYPlotImplI;

/////////////////////////////////////////////////////////////////////////
// Plot Settings

template<class PlotImplT>
bool	DoPropertySheet(PlotImplT *plot, HWND   hwndOwner);

extern vPagePara g_vPP;
/////////////////////////////////////////////////////////////////////////

template<class PlotImplT>
bool DefPlotSettings( PlotImplT *plot, HDC hDC )
{
	if( DoPropertySheet(plot, WindowFromDC(hDC)) )
		return true;
	else
		return false;
}

//
//bool	DefPlotSettings( CXYPlotImplI *plot, HDC hDC )
//{
//	return DefPlotSetting<CXYPlotImplI>(plot, hDC);
//}

template<class PlotImplT>
void	InitPageParaTitle(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_TITLE;
		pp.title = _TEXT("Title");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_TITLE_C;
		pp.title = _TEXT("标题");
		break;
	}
	pp.DlgProc = ClassForPlotTitleDlg<PlotImplT>::PlotTitleDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaLegend(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_LEGEND;
		pp.title = _TEXT("Legend");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_LEGEND_C;
		pp.title = _TEXT("图例");
		break;
	}
	pp.DlgProc = ClassForPlotLegendDlg<PlotImplT>::PlotLegendDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaRanges(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_PLOTRANGES;
		pp.title = _TEXT("Ranges");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_PLOTRANGES_C;
		pp.title = _TEXT("数据范围");
		break;
	}
	pp.DlgProc = ClassForPlotRangesDlg<PlotImplT>::PlotRangesDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaBkgnd(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_BACKGROUND;
		pp.title = _TEXT("Background");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_BACKGROUND_C;
		pp.title = _TEXT("背景");
		break;
	}
	pp.DlgProc = ClassForPlotBkgndDlg<PlotImplT>::PlotBkgndDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaMajorGrids(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_MAJORGRIDS;
		pp.title = _TEXT("Major Grids");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_MAJORGRIDS_C;
		pp.title = _TEXT("主网格");
		break;
	}
	pp.DlgProc = ClassForPlotMajorGridsDlg<PlotImplT>::PlotMajorGridsDlgFunc;	
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaMinorGrids(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_MINORGRIDS;
		pp.title = _TEXT("Minor Grids");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_MINORGRIDS_C;
		pp.title = _TEXT("副网格");
		break;
	}
	pp.DlgProc = ClassForPlotMinorGridsDlg<PlotImplT>::PlotMinorGridsDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaAutoSetting(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_AUTOSETTINGS;
		pp.title = _TEXT("Plot Stauts");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_AUTOSETTINGS_C;
		pp.title = _TEXT("绘图状态");
		break;
	}
	pp.DlgProc = ClassForPlotAutoSettingDlg<PlotImplT>::PlotAutoSettingDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaCopyRight(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_COPYRIGHT;
		pp.title = _TEXT("Copyright(C)");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_COPYRIGHT_C;
		pp.title = _TEXT("版权声明(C)");
		break;
	}
	pp.DlgProc = ClassForPlotCopyrightDlg<PlotImplT>::PlotCopyrightDlgFunc;
	vPP.push_back(pp);;
}

template<class PlotImplT>
void	InitPageParaExtraFunction(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_EXTRAFUNCTION;
		pp.title = _TEXT("Extra Functions");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_EXTRAFUNCTION_C;
		pp.title = _TEXT("特别功能");
		break;
	}
	pp.DlgProc = ClassForPlotExtraFunctionDlg<PlotImplT>::PlotExtraFunctionDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaCursors(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_CURSORS;
		pp.title = _TEXT("Cursors");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_CURSORS_C;
		pp.title = _TEXT("光标");
		break;
	}
	pp.DlgProc = ClassForPlotCursorsDlg<PlotImplT>::PlotCursorsDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPageParaSelZone(PlotImplT *pPlot, vPagePara &vPP)
{
	PagePara pp;
	
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		pp.ID = IDD_XYSETTINGS_SELZONE;		
		pp.title = _TEXT("Zones");
		break;
	case kLangChinese:
		pp.ID = IDD_XYSETTINGS_SELZONE_C;		
		pp.title = _TEXT("数据选择范围");
		break;
	}
	pp.DlgProc = ClassForPlotSelZoneDlg<PlotImplT>::PlotSelZoneDlgFunc;
	vPP.push_back(pp);
}

template<class PlotImplT>
void	InitPagePara(PlotImplT *plot, vPagePara &vPP)
{
	InitPageParaTitle(plot, vPP);
	InitPageParaLegend(plot, vPP);
	InitPageParaRanges(plot, vPP);
	InitPageParaBkgnd(plot, vPP);
	InitPageParaMajorGrids(plot, vPP);
	InitPageParaMinorGrids(plot, vPP);
	InitPageParaAutoSetting(plot, vPP);
	InitPageParaCopyRight(plot, vPP);
}

template<class PlotImplT>
void	InitPageParaOscillo(PlotImplT *plot, vPagePara &vPP)
{
	InitPageParaCursors(plot, vPP);
	InitPageParaSelZone(plot, vPP);
}

template<class PlotImplT>
void	InitPageParaSpecial(PlotImplT *plot, vPagePara &vPP)
{
	InitPageParaExtraFunction<PlotImplT>(plot, vPP);
	if(plot->IsOscilloScopeMode())InitPageParaOscillo<PlotImplT>(plot, vPP);
}

template<class PlotImplT>
bool	DoPropertySheet(PlotImplT *plot, HWND hwndOwner)   
{   
	PROPSHEETPAGE   *psp;
	PROPSHEETHEADER   psh;
    
	g_vPP.clear();
	InitPagePara<PlotImplT>(plot, g_vPP);
	InitPageParaSpecial(plot, g_vPP);
	if((int)g_vPP.size()<=0)return false;

	int i;
	int nPages = (int)g_vPP.size();

	psp = new PROPSHEETPAGE[nPages];
	for(i=0; i<(int)g_vPP.size(); i++)
	{
		psp[i].dwSize   =   sizeof(PROPSHEETPAGE);
		psp[i].dwFlags   =   PSP_USEICONID   |   PSP_USETITLE;
		psp[i].hInstance   =   CGlobal::MyGetModuleHandle();
		psp[i].pszTemplate   =   MAKEINTRESOURCE(g_vPP[i].ID);
		psp[i].pszIcon   =   NULL;//MAKEINTRESOURCE(IDI_FONT);
		psp[i].pfnDlgProc   =   g_vPP[i].DlgProc;
		psp[i].pszTitle   =   g_vPP[i].title.c_str();
		psp[i].lParam   =   (LPARAM)plot;
		psp[i].pfnCallback   =   NULL;
	}
    
	psh.dwSize   =   sizeof(PROPSHEETHEADER);
	psh.dwFlags   =   PSH_USEICONID   |   PSH_PROPSHEETPAGE;
	psh.hwndParent   =   hwndOwner;
	psh.hInstance   =   CGlobal::MyGetModuleHandle();
	psh.pszIcon   =   NULL;//MAKEINTRESOURCE(IDI_CELL_PROPERTIES);
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		psh.pszCaption   =   _TEXT("Plot Settings");
		break;
	case kLangChinese:
		psh.pszCaption   =   _TEXT("绘图设置");
		break;
	}
	psh.nPages   =   nPages;//sizeof(psp)   /   sizeof(PROPSHEETPAGE);
	
	psh.nStartPage   =   0;//g_vPP.size()-1;
	psh.ppsp   =   (LPCPROPSHEETPAGE)   psp;
	psh.pfnCallback   =   NULL;
    
	bool ret;
	if( PropertySheet(&psh) > 0)
	{
		ret=true;
	}
	else
	{
		ret=false;
	}

	delete []psp;
	return ret;
}

Declare_Namespace_End


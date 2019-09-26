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

//#include "stdafx.h"
#include "DataProcessing.h"
#include "../XYPlotBasicImpl.h"
#include "../../Resources/PlotResources.h"
#include "../../Accessary/Numerical/NlFit/NlFit.h"

Declare_Namespace_CChart

double	GetDataX(MyVData2D vData, int index)
{
	return vData[index].val[0];
}

double	GetDataY(MyVData2D vData, int index)
{
	return vData[index].val[1];
}

extern void	ClearAdjoinCoincide(MyVData2D &vData);

bool lessVal(const double &d1, const double &d2)
{
	return d1 < d2;
}

double	GetMedian(std::vector<double> vVals)
{
	if(vVals.size()<=0)return 0.0;
	sort(vVals.begin(), vVals.end(), lessVal);
	int len = (int)vVals.size();
	return vVals[(len+1)/2];
}

bool	PerformNlFit(CXYPlotBasicImpl *plot, int dataID, double *pX, double *pY, int nLen, vector<double> &vCoff, vector<double> &vAbsErr, MyVData2D &vData, int cnum, double fMaxTol, int nMaxIter, int nPtsFitted, FcnFitFunction *pF)
{
	int i;

	double *pA = new double[cnum];
	double *pAbsErr = new double[cnum];
	for(i=0; i<cnum; i++)
	{
		pA[i] = vCoff[i];
	}
	
	double err;
	int iter, method, fail;
	err = fMaxTol;
	iter = nMaxIter;
	method = 1;
	nlfit(pA, pAbsErr, cnum, pX, pY, nLen, err, iter, method, fail);
	
	if(fail)return false;
	
	double xRange[2];
	plot->GetPlotData(dataID)->GetXDataRange(xRange);
	double dx = (xRange[1] - xRange[0]) / (nPtsFitted - 1);
	vData.resize(nPtsFitted);

	for(i=0; i<nPtsFitted; i++)
	{
		vData[i].val[0] = xRange[0] + i * dx;
		vData[i].val[1] = pF(pA, cnum, vData[i].val[0]);
	}
	
	vAbsErr.resize(cnum);
	for(i=0; i<cnum; i++)
	{
		vCoff[i] = pA[i];
		vAbsErr[i] = pAbsErr[i];
	}
	delete []pA;
	delete []pAbsErr;

	return true;
}

void	SetProcessedData(CXYPlotBasicImpl *plot, int nWhich, MyVData2D vData, bool reserve, tstring procName)
{
	int dataID;
	if(!reserve)
	{
		plot->GetAbsPlotData(nWhich)->GetData() = vData;
		plot->GetAbsPlotData(nWhich)->bForceUpdate =  true;
		
	}
	else
	{
		dataID = plot->AddCurve(vData);
		tostringstream ostr;
		tstring str;
		
		ostr.str(_TEXT(""));
		ostr<<procName.c_str();
		ostr<<_TEXT(" by No ")<<nWhich+1<<_TEXT(": ")<<plot->GetAbsDataTitle(nWhich);
		str=ostr.str();
		plot->SetDataTitle(dataID, str);
		plot->GetPlotData(dataID)->bCompactDraw = true;
	}
}

tstring CreateInfoString(CXYPlotBasicImpl *plot)
{
	tostringstream ostr;
	tstring str;

	vector<double> vCoff;
	vCoff = *((vector<double>*)plot->m_pSpecial);
	int i;
	
	ostr.str(_TEXT(""));
	ostr<<_TEXT("Fitting result:\r\n");
	
	if(plot->m_nSpecial == 1)
	{
		ostr<<_TEXT("Polynomial fitting\r\n");
		ostr<<_TEXT("y=...+a2*x^2+a1*x+a0\r\n");
		ostr<<_TEXT("y=");
		for(i=(int)vCoff.size()-1; i>=0; i--)
		{
			ostr<<fabs(vCoff[i]);
			if(i >= 1)ostr<<_TEXT("*x");
			if(i >= 2)ostr<<_TEXT("^")<<i;
			
			if(i > 0 )
			{
				if(vCoff[i-1]>=0)
					ostr<<_TEXT("+");
				else
					ostr<<_TEXT("-");
			}
		}
		for(i=0; i<(int)vCoff.size(); i++)
		{
			ostr<<_TEXT("\r\na")<<i<<_TEXT(" = ")<<vCoff[i];
		}
	}
	else if(plot->m_nSpecial == 2)
	{
		ostr<<_TEXT("Exponential fitting\r\n");
		ostr<<_TEXT("y=a*exp(b*x)+c\r\n");
		ostr<<_TEXT("y=");
		ostr<<vCoff[2]<<_TEXT("*exp(")<<vCoff[1];
		if(vCoff[0]>=0)
			ostr<<_TEXT("*x)+");
		else
			ostr<<_TEXT("*x)-");
		ostr<<fabs((vCoff[0]));
		
		ostr<<_TEXT("\r\na=")<<vCoff[2]<<_TEXT("\r\nb=")<<vCoff[1]<<_TEXT("\r\nc=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 3)
	{
		ostr<<_TEXT("Logarithmic fitting\r\n");
		ostr<<_TEXT("y=a*ln(x+b)+c\r\n");
		ostr<<_TEXT("y=");
		ostr<<vCoff[2];
		if(vCoff[1]>=0)
			ostr<<_TEXT("*log(x+");
		else
			ostr<<_TEXT("*log(x-");
		ostr<<fabs(vCoff[1]);
		if(vCoff[0]>=0)
			ostr<<_TEXT("*x)+");
		else
			ostr<<_TEXT("*x)-");
		ostr<<fabs(vCoff[0]);
		ostr<<_TEXT("\r\na=")<<vCoff[2]<<_TEXT("\r\nb=")<<vCoff[1]<<_TEXT("\r\nc=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 4)
	{
		ostr<<_TEXT("Gaussian fitting\r\n");
		ostr<<_TEXT("y=a*exp(-( (x-b)/c )^2/2)+d\r\n");
		ostr<<_TEXT("y=");
		ostr<<vCoff[3];
		if(vCoff[2]>=0)
			ostr<<_TEXT("*exp(-((x-");
		else
			ostr<<_TEXT("*exp(-((x+");
		ostr<<fabs(vCoff[2]);
		ostr<<_TEXT(")/");
		ostr<<fabs(vCoff[1]);
		if(vCoff[0]>=0)
			ostr<<_TEXT(")^2/2)+");
		else
			ostr<<_TEXT(")^2/2)-");
		ostr<<fabs(vCoff[0]);
		ostr<<_TEXT("\r\na="<<vCoff[3])<<_TEXT("\r\nb=")<<vCoff[2]<<_TEXT("\r\nc=")<<vCoff[1]<<_TEXT("\r\nd=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 5)
	{
		ostr<<_TEXT("Slope backgroud gaussian fitting\r\n");
		ostr<<_TEXT("y=a*exp(-( (x-b)/c )^2/2)+d*x+e\r\n");
		ostr<<_TEXT("y=");
		//ostr<<vCoff[4]<<_TEXT("*exp(-((x-")<<vCoff[3]<<_TEXT(")/")<<vCoff[2]<<_TEXT(")^2/2)+")<<vCoff[1]<<_TEXT("*x+")<<vCoff[0];
		ostr<<vCoff[4];
		if(vCoff[3]>=0)
			ostr<<_TEXT("*exp(-((x-");
		else
			ostr<<_TEXT("*exp(-((x+");
		ostr<<fabs(vCoff[3]);
		ostr<<_TEXT(")/");
		ostr<<fabs(vCoff[2]);
		if(vCoff[1]>=0)
			ostr<<_TEXT(")^2/2)+");
		else
			ostr<<_TEXT(")^2/2)-");
		ostr<<fabs(vCoff[1]);
		ostr<<_TEXT("*x");
		if(vCoff[0]>0)
			ostr<<_TEXT("+");
		else
			ostr<<_TEXT("-");
		ostr<<fabs(vCoff[0]);
		ostr<<_TEXT("\r\na=")<<vCoff[4]<<_TEXT("\r\nb=")<<vCoff[3]<<_TEXT("\r\nc=")<<vCoff[2]<<_TEXT("\r\nd=")<<vCoff[1]<<_TEXT("\r\ne=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 6)
	{
		ostr<<_TEXT("Parabolic backgroud gaussian fitting\r\n");
		ostr<<_TEXT("y=a*exp(-( (x-b)/c )^2/2)+d*x^2+e*x+f\r\n");
		ostr<<_TEXT("y=");
		//ostr<<vCoff[5]<<_TEXT("*exp(-((x-")<<vCoff[4]<<_TEXT(")/")<<vCoff[3]<<_TEXT(")^2/2)+")<<vCoff[2]<<_TEXT("*x^2+")<<vCoff[1]<<_TEXT("*x+")<<vCoff[0];
		ostr<<vCoff[5];
		if(vCoff[4]>=0)
			ostr<<_TEXT("*exp(-((x-");
		else
			ostr<<_TEXT("*exp(-((x+");
		ostr<<fabs(vCoff[4]);
		ostr<<_TEXT(")/");
		ostr<<fabs(vCoff[3]);
		if(vCoff[2]>=0)
			ostr<<_TEXT(")^2/2)+");
		else
			ostr<<_TEXT(")^2/2)-");
		ostr<<fabs(vCoff[2]);
		ostr<<_TEXT("*x^2");
		if(vCoff[1]>0)
			ostr<<_TEXT("+");
		else
			ostr<<_TEXT("-");
		ostr<<fabs(vCoff[1]);
		ostr<<_TEXT("*x");
		if(vCoff[0]>0)
			ostr<<_TEXT("+");
		else
			ostr<<_TEXT("-");
		ostr<<fabs(vCoff[0]);
		ostr<<_TEXT("\r\na=")<<vCoff[5]<<_TEXT("\r\nb=")<<vCoff[4]<<_TEXT("\r\nc=")<<vCoff[3]<<_TEXT("\r\nd=")<<vCoff[2]<<_TEXT("\r\ne=")<<vCoff[1]<<_TEXT("\r\nf=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 7)
	{
		ostr<<_TEXT("Multiple gaussian fitting\r\n");
		ostr<<_TEXT("y = ...+a1*exp(-( (x-b1)/c1 )^2/2)+d\r\n");
		ostr<<_TEXT("y=");
		int nPeaks = (int)(vCoff.size()-1)/3;
		int i;
		if(vCoff[vCoff.size()<=0])
			ostr<<_TEXT("-");
		for(i=nPeaks-1; i>=0; i--)
		{
			ostr<<fabs(vCoff[3*i+3]);
			if(vCoff[3*i+2]>=0)
				ostr<<_TEXT("*exp(-((x-");
			else
				ostr<<_TEXT("*exp(-((x+");
			ostr<<fabs(vCoff[3*i+2]);
			ostr<<_TEXT(")/");
			//ostr<<vCoff[3*i+1];
			ostr<<fabs(vCoff[3*i+1]);
			if(vCoff[3*i]>=0)
				ostr<<_TEXT(")^2/2)+");
			else
				ostr<<_TEXT(")^2/2)-");
		}
		ostr<<fabs(vCoff[0]);
		for(i=nPeaks-1; i>=0; i--)
		{
			ostr<<_TEXT("\r\na")<<i+1<<_TEXT("=")<<vCoff[3*i+3]<<_TEXT("\r\nb")<<i+1<<_TEXT("=")<<vCoff[3*i+2]<<_TEXT("\r\nc")<<i+1<<_TEXT("=")<<vCoff[3*i+1];
		}
		ostr<<_TEXT("\r\nd=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 8)
	{
		ostr<<_TEXT("Multiple slope gaussian fitting\r\n");
		ostr<<_TEXT("y = ...+a1*exp(-( (x-b1)/c1 )^2/2)+d*x+e\r\n");
		ostr<<_TEXT("y=");
		int nPeaks = (int)(vCoff.size()-2)/3;
		int i;
		if(vCoff[vCoff.size()<=0])
			ostr<<_TEXT("-");
		for(i=nPeaks-1; i>=0; i--)
		{
			ostr<<fabs(vCoff[3*i+4]);
			if(vCoff[3*i+3]>=0)
				ostr<<_TEXT("*exp(-((x-");
			else
				ostr<<_TEXT("*exp(-((x+");
			ostr<<fabs(vCoff[3*i+3]);
			ostr<<_TEXT(")/");
			//ostr<<vCoff[3*i+1];
			ostr<<fabs(vCoff[3*i+2]);
			if(vCoff[3*i+1]>=0)
				ostr<<_TEXT(")^2/2)+");
			else
				ostr<<_TEXT(")^2/2)-");
		}
		ostr<<fabs(vCoff[1]);
		if(fabs(vCoff[0])>=0)
			ostr<<_TEXT("*x+");
		else
			ostr<<_TEXT("*x-");
		ostr<<fabs(vCoff[0]);
		for(i=nPeaks-1; i>=0; i--)
		{
			ostr<<_TEXT("\r\na")<<i+1<<_TEXT("=")<<vCoff[3*i+4]<<_TEXT("\r\nb")<<i+1<<_TEXT("=")<<vCoff[3*i+3]<<_TEXT("\r\nc")<<i+1<<_TEXT("=")<<vCoff[3*i+2];
		}
		ostr<<_TEXT("\r\nd=")<<vCoff[1]<<_TEXT("\r\ne=")<<vCoff[0];
	}
	else if(plot->m_nSpecial == 9)
	{
		ostr<<_TEXT("Multiple parabolic gaussian fitting\r\n");
		ostr<<_TEXT("y = ...+a1*exp(-( (x-b1)/c1 )^2/2)+d*x^2+e*x+f\r\n");
		ostr<<_TEXT("y=");
		int nPeaks = (int)(vCoff.size()-3)/3;
		int i;
		if(vCoff[vCoff.size()<=0])
			ostr<<_TEXT("-");
		for(i=nPeaks-1; i>=0; i--)
		{
			ostr<<fabs(vCoff[3*i+5]);
			if(vCoff[3*i+4]>=0)
				ostr<<_TEXT("*exp(-((x-");
			else
				ostr<<_TEXT("*exp(-((x+");
			ostr<<fabs(vCoff[3*i+4]);
			ostr<<_TEXT(")/");
			//ostr<<vCoff[3*i+1];
			ostr<<fabs(vCoff[3*i+3]);
			if(vCoff[3*i+1]>=0)
				ostr<<_TEXT(")^2/2)+");
			else
				ostr<<_TEXT(")^2/2)-");
		}
		ostr<<fabs(vCoff[2]);
		if(fabs(vCoff[1])>=0)
			ostr<<_TEXT("*x^2+");
		else
			ostr<<_TEXT("*x^2-");
		ostr<<fabs(vCoff[1]);
		if(fabs(vCoff[0])>=0)
			ostr<<_TEXT("*x+");
		else
			ostr<<_TEXT("*x-");
		ostr<<fabs(vCoff[0]);
		for(i=nPeaks-1; i>=0; i--)
		{
			ostr<<_TEXT("\r\na")<<i+1<<_TEXT("=")<<vCoff[3*i+5]<<_TEXT("\r\nb")<<i+1<<_TEXT("=")<<vCoff[3*i+4]<<_TEXT("\r\nc")<<i+1<<_TEXT("=")<<vCoff[3*i+3];
		}
		ostr<<_TEXT("\r\nd=")<<vCoff[2]<<_TEXT("\r\ne=")<<vCoff[1]<<_TEXT("\r\nf=")<<vCoff[0];
	}
	str=ostr.str();
	
	return str;
}

BOOL	CALLBACK ShowInfomationDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		SendDlgItemMessage(hWndDlg, IDC_INFORMATION_SHOWTEXT, WM_SETTEXT, 0, (LPARAM)CreateInfoString(plot).c_str());
		SetWindowText(hWndDlg, _TEXT("Data fitting result"));
		return TRUE;
	case WM_NCDESTROY:
	case WM_DESTROY:
	case WM_CLOSE:
		DestroyWindow(hWndDlg);
		break;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcOffsetDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForOffset(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForOffset(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcOffsetData(CXYPlotBasicImpl *plot, int nWhich, double dx, double dy, bool reserve);

BOOL	CALLBACK DataProcOffsetDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForOffset(hWndDlg, plot);

		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForOffset(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForOffset(HWND hWnd, CXYPlotBasicImpl *plot)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<0;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_OFFSETX, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_OFFSETY, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForOffset(HWND hWnd, CXYPlotBasicImpl *plot)
{
	double dx, dy;

	TCHAR strtmp[nMaxString];
	double	fVal;
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_OFFSETX), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	dx = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_OFFSETY), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	dy = fVal;

	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	//plot->SetUseMajorHGrids(check==BST_CHECKED);
	return ProcOffsetData(plot, plot->m_nSpecial, dx, dy, check == BST_CHECKED);
}

bool	ProcOffsetData(CXYPlotBasicImpl *plot, int nWhich, double dx, double dy, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;

	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;

	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	for(i=0; i<(int)plot->GetAbsData(nWhich).size(); i++)
	{
		vData[i].val[0] += dx;
		vData[i].val[1] += dy;
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Offset"));

	return true;
}

bool	ShowOffsetDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;

	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_OFFSET), hWnd, (DLGPROC)DataProcOffsetDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcScaleDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForScale(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForScale(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcScaleData(CXYPlotBasicImpl *plot, int nWhich, double ctx, double scalex, double cty, double scaley, bool reserve);

BOOL	CALLBACK DataProcScaleDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForScale(hWndDlg, plot);
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForScale(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForScale(HWND hWnd, CXYPlotBasicImpl *plot)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<0;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_SCALECTX, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_SCALECTY, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<1;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_SCALEFCTX, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_SCALEFCTY, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForScale(HWND hWnd, CXYPlotBasicImpl *plot)
{
	double ctx, cty;
	double scalex, scaley;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_SCALECTX), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	ctx = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_SCALECTY), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	cty = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_SCALEFCTX), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	scalex = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_SCALEFCTY), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	scaley = fVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcScaleData(plot, plot->m_nSpecial, ctx, scalex, cty, scaley, check==BST_CHECKED);
}

bool	ProcScaleData(CXYPlotBasicImpl *plot, int nWhich, double ctx, double scalex, double cty, double scaley, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;

	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	for(i=0; i<(int)plot->GetAbsData(nWhich).size(); i++)
	{
		vData[i].val[0] = ctx + (vData[i].val[0]-ctx) * scalex;
		vData[i].val[1] = cty + (vData[i].val[1]-cty) * scaley;
	}
	
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Scaled"));
	
	return true;
}

bool	ShowScaleDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_SCALE), hWnd, (DLGPROC)DataProcScaleDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcAffineDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForAffine(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForAffine(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcAffineData(CXYPlotBasicImpl *plot, int nWhich, double a1, double b1, double c1, double a2, double b2, double c2, bool reserve);

BOOL	CALLBACK DataProcAffineDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForAffine(hWndDlg, plot);
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForAffine(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForAffine(HWND hWnd, CXYPlotBasicImpl *plot)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<1;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_AFFINEA1, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_AFFINEB2, WM_SETTEXT, 0, (LPARAM)str.c_str());
	ostr.str(_TEXT(""));
	ostr<<0;
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_AFFINEB1, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_AFFINEC1, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_AFFINEA2, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DP_TRANS_AFFINEC2, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForAffine(HWND hWnd, CXYPlotBasicImpl *plot)
{
	double a1, b1, c1;
	double a2, b2, c2;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_AFFINEA1), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	a1 = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_AFFINEB1), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	b1 = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_AFFINEC1), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	c1 = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_AFFINEA2), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	a2 = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_AFFINEB2), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	b2 = fVal;
	GetWindowText(GetDlgItem(hWnd, IDC_DP_TRANS_AFFINEC2), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	c2 = fVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcAffineData(plot, plot->m_nSpecial, a1, b1, c1, a2, b2, c2, check==BST_CHECKED);
}

bool	ProcAffineData(CXYPlotBasicImpl *plot, int nWhich, double a1, double b1, double c1, double a2, double b2, double c2, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	double x, y;

	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	for(i=0; i<(int)plot->GetAbsData(nWhich).size(); i++)
	{
		x = plot->GetAbsPlotData(nWhich)->GetData()[i].val[0];
		y = plot->GetAbsPlotData(nWhich)->GetData()[i].val[1];
		vData[i].val[0] = a1 * x + b1 * y + c1;
		vData[i].val[1] = a2 * x + b2 * y + c2;
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Affine"));
	
	return true;
}

bool	ShowAffineDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_AFFINE), hWnd, (DLGPROC)DataProcAffineDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcDifferentialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForDifferential(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForDifferential(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcDifferentialData(CXYPlotBasicImpl *plot, int nWhich, int method, bool reserve);

BOOL	CALLBACK DataProcDifferentialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForDifferential(hWndDlg, plot);
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForDifferential(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForDifferential(HWND hWnd, CXYPlotBasicImpl *plot)
{
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Backward"));
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Forward"));
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dual"));
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_SETCURSEL, (WPARAM)0, 0);

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForDifferential(HWND hWnd, CXYPlotBasicImpl *plot)
{
#ifndef _WIN64
	int method, num, check;
#else
	LRESULT method, num, check;
#endif
	
	num = SendDlgItemMessage(hWnd, IDC_DP_CALCULUS_DIFFMETHOD, CB_GETCURSEL, 0, 0);
	method = num;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcDifferentialData(plot, plot->m_nSpecial, (int)method, check==BST_CHECKED);
}

bool	ProcDifferentialData(CXYPlotBasicImpl *plot, int nWhich, int method, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	double x1, y1, x2, y2;

	MyVData2D vData, vDataCompact;
	vDataCompact = plot->GetAbsData(nWhich);
	ClearAdjoinCoincide(vDataCompact);
	vData = vDataCompact;

	for(i=0; i<(int)vDataCompact.size(); i++)
	{
		switch(method)
		{
		case 0:
			if(i==0)
			{
				x1 = GetDataX(vDataCompact, i);
				y1 = GetDataY(vDataCompact, i);
				x2 = GetDataX(vDataCompact, i+1);
				y2 = GetDataY(vDataCompact, i+1);
			}
			else
			{
				x1 = GetDataX(vDataCompact, i-1);
				y1 = GetDataY(vDataCompact, i-1);
				x2 = GetDataX(vDataCompact, i);
				y2 = GetDataY(vDataCompact, i);
			}
			
			break;
		case 1:
			if(i==(int)vDataCompact.size()-1)
			{
				x1 = GetDataX(vDataCompact, i-1);
				y1 = GetDataY(vDataCompact, i-1);
				x2 = GetDataX(vDataCompact, i);
				y2 = GetDataY(vDataCompact, i);
			}
			else
			{
				x1 = GetDataX(vDataCompact, i);
				y1 = GetDataY(vDataCompact, i);
				x2 = GetDataX(vDataCompact, i+1);
				y2 = GetDataY(vDataCompact, i+1);
			}
			
			break;
		case 2:
			if(i==0)
			{
				x1 = (GetDataX(vDataCompact, i)+GetDataX(vDataCompact, i+1))/2.0;
				y1 = (GetDataY(vDataCompact, i)+GetDataY(vDataCompact, i+1))/2.0;
				x2 = (GetDataX(vDataCompact, i+1)+GetDataX(vDataCompact, i+2))/2.0;
				y2 = (GetDataY(vDataCompact, i+1)+GetDataY(vDataCompact, i+2))/2.0;
			}
			else if(i==(int)vDataCompact.size()-1)
			{
				x1 = (GetDataX(vDataCompact, i-2)+GetDataX(vDataCompact, i-1))/2.0;
				y1 = (GetDataY(vDataCompact, i-2)+GetDataY(vDataCompact, i-1))/2.0;
				x2 = (GetDataX(vDataCompact, i-1)+GetDataX(vDataCompact, i))/2.0;
				y2 = (GetDataY(vDataCompact, i-1)+GetDataY(vDataCompact, i))/2.0;
			}
			else
			{
				x1 = (GetDataX(vDataCompact, i-1)+GetDataX(vDataCompact, i))/2.0;
				y1 = (GetDataY(vDataCompact, i-1)+GetDataY(vDataCompact, i))/2.0;
				x2 = (GetDataX(vDataCompact, i)+GetDataX(vDataCompact, i+1))/2.0;
				y2 = (GetDataY(vDataCompact, i)+GetDataY(vDataCompact, i+1))/2.0;
			}
			
			break;
		default:
			return false;
		}
		vData[i].val[1] = (y2 - y1)/(x2 - x1);
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Differential"));
	
	return true;
}

bool	ShowDiffentialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_CALCULUS), hWnd, (DLGPROC)DataProcDifferentialDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcIntegratialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForIntegratial(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForIntegratial(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcIntegratialData(CXYPlotBasicImpl *plot, int nWhich, int method, bool reserve);

BOOL	CALLBACK DataProcIntegratialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;
	
	switch(message)
	{
	case WM_INITDIALOG:
		SetWindowText(hWndDlg, _TEXT("Integration"));
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForIntegratial(hWndDlg, plot);
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForIntegratial(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForIntegratial(HWND hWnd, CXYPlotBasicImpl *plot)
{
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Left"));
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Right"));
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_ADDSTRING, 0, (LPARAM)_TEXT("Center"));
	SendDlgItemMessage(hWnd,IDC_DP_CALCULUS_DIFFMETHOD, CB_SETCURSEL, (WPARAM)0, 0);

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForIntegratial(HWND hWnd, CXYPlotBasicImpl *plot)
{
#ifndef _WIN64
	int method, num, check;
#else
	LRESULT method, num, check;
#endif
	
	num = SendDlgItemMessage(hWnd, IDC_DP_CALCULUS_DIFFMETHOD, CB_GETCURSEL, 0, 0);
	method = num;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcIntegratialData(plot, plot->m_nSpecial, (int)method, check==BST_CHECKED);
}

bool	ProcIntegratialData(CXYPlotBasicImpl *plot, int nWhich, int method, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	double x1, y1, x2, y2;

	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	double curVal = 0.0;
	for(i=0; i<(int)vData.size(); i++)
	{
		if(i==0)
		{
			x1 = GetDataX(plot->GetAbsData(nWhich), i);
			y1 = GetDataY(plot->GetAbsData(nWhich), i);
		}
		else
		{
			x1 = GetDataX(plot->GetAbsData(nWhich), i-1);
			y1 = GetDataY(plot->GetAbsData(nWhich), i-1);
		}
		x2 = GetDataX(plot->GetAbsData(nWhich), i);
		y2 = GetDataY(plot->GetAbsData(nWhich), i);

		switch(method)
		{
		case 0:
			curVal += y1*(x2-x1);
			break;
		case 1:
			curVal += y2*(x2-x1);
			break;
		case 2:
			curVal += 0.5*(y1+y2)*(x2-x1);
			break;
		default:
			return false;
		}
		vData[i].val[1] = curVal;
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Integrate"));
	
	return true;
}

bool	ShowIntegratialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_CALCULUS), hWnd, (DLGPROC)DataProcIntegratialDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcMedianFilterDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForMedianFilter(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForMedianFilter(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcMedianFilterData(CXYPlotBasicImpl *plot, int nWhich, int order, bool reserve);

BOOL	CALLBACK DataProcMedianFilterDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForMedianFilter(hWndDlg, plot);
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForMedianFilter(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForMedianFilter(HWND hWnd, CXYPlotBasicImpl *plot)
{
	tostringstream ostr;
	tstring str;
	
	for(int i=0; i<20; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_SMOOTHING_ORDER, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_SMOOTHING_ORDER, CB_SETCURSEL, (WPARAM)1, 0);

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForMedianFilter(HWND hWnd, CXYPlotBasicImpl *plot)
{
#ifndef _WIN64
	int order, num, check;
#else
	LRESULT order, num, check;
#endif
	
	num = SendDlgItemMessage(hWnd, IDC_DP_SMOOTHING_ORDER, CB_GETCURSEL, 0, 0);
	order = num;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcMedianFilterData(plot, plot->m_nSpecial, (int)order, check==BST_CHECKED);
}

bool	ProcMedianFilterData(CXYPlotBasicImpl *plot, int nWhich, int order, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nl, nr;
	int j;

	vector<double> vVal;
	for(i=0; i<(int)plot->GetAbsData(nWhich).size(); i++)
	{
		nl = order;
		if(i - nl < 0)nl = i;
		nr = order;
		if(i + nr >= (int)plot->GetAbsData(nWhich).size())nr = (int)plot->GetAbsData(nWhich).size()-1-i;
		vVal.clear();
		for(j=i-nl; j<=i+nr; j++)
		{
			vVal.push_back(plot->GetAbsData(nWhich)[j].val[1]);
		}
		vData[i].val[1] = GetMedian(vVal);
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Median filter"));
	
	return true;
}

bool	ShowMedianFilterDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FILTER), hWnd, (DLGPROC)DataProcMedianFilterDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcAverageFilterDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForAverageFilter(HWND hWnd, CXYPlotBasicImpl *plot);
bool	SaveDataForAverageFilter(HWND hWnd, CXYPlotBasicImpl *plot);
bool	ProcAverageFilterData(CXYPlotBasicImpl *plot, int nWhich, int order, bool reserve);

BOOL	CALLBACK DataProcAverageFilterDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;
	
	switch(message)
	{
	case WM_INITDIALOG:
		SetWindowText(hWndDlg, _TEXT("Average filter"));
		plot = (CXYPlotBasicImpl *)lparam;
		SetupDataForAverageFilter(hWndDlg, plot);
		
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForAverageFilter(hWndDlg, plot);
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForAverageFilter(HWND hWnd, CXYPlotBasicImpl *plot)
{
	tostringstream ostr;
	tstring str;
	
	for(int i=0; i<20; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_SMOOTHING_ORDER, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_SMOOTHING_ORDER, CB_SETCURSEL, (WPARAM)1, 0);

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForAverageFilter(HWND hWnd, CXYPlotBasicImpl *plot)
{
#ifndef _WIN64
	int order, num, check;
#else
	LRESULT order, num, check;
#endif
	
	num = SendDlgItemMessage(hWnd, IDC_DP_SMOOTHING_ORDER, CB_GETCURSEL, 0, 0);
	order = num;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcAverageFilterData(plot, plot->m_nSpecial, (int)order, check==BST_CHECKED);
}

bool	ProcAverageFilterData(CXYPlotBasicImpl *plot, int nWhich, int order, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nl, nr;
	int j;

	double val;
	for(i=0; i<(int)plot->GetAbsData(nWhich).size(); i++)
	{
		nl = order;
		if(i - nl < 0)nl = i;
		nr = order;
		if(i + nr >= (int)plot->GetAbsData(nWhich).size())nr = (int)plot->GetAbsData(nWhich).size()-1-i;
		val = 0.0;
		for(j=i-nl; j<=i+nr; j++)
		{
			val+=plot->GetAbsData(nWhich)[j].val[1];
		}
		vData[i].val[1] = val/(nl+1+nr);
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Average filter"));
	
	return true;
}

bool	ShowAverageFilterDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FILTER), hWnd, (DLGPROC)DataProcAverageFilterDlgFunc, (LPARAM)plot) )
	{
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingPolynomialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingPolynomial(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff);
bool	SaveDataForFittingPolynomial(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr);
bool	ProcFittingPolynomialData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitPolynomialCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, int nOrder);

double	PolynomialFuncVal(double *pA, int cnum, double x);
double	PolynomialRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	PolynomialDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitPolynomialCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, int nOrder)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;
	if(nOrder<0)return;

	vCoff.resize(nOrder+1);
	
	double xRange[2], yRange[2];
	plot->GetDataRanges(xRange, yRange);
	double k = (yRange[1]-yRange[0])/(xRange[1]-xRange[0]);

	vCoff[0] = yRange[0]-k*xRange[0];
	vCoff[1] = k;
}

double	PolynomialFuncVal(double *pA, int cnum, double x)
{
	double val = 0;
	int i;
	for(i=cnum-1; i>=0; i--)
	{
//		val *= x;
//		val += pA[i];
		val += pA[i] * pow(x, (double)i);
	}
	return val;
}

double	PolynomialRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - PolynomialFuncVal(pA, cnum, x);
}

void	PolynomialDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];
	
	double val = 0;
	int i;
	for(i=0; i<cnum; i++)
	{
		pDiff[i] = pow(x, (double)i);
	}
}

BOOL	CALLBACK DataProcFittingPolynomialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;
#ifndef _WIN64
	int num;
#else
	LRESULT num;
#endif

	tostringstream ostr;
	tstring str;

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitPolynomialCofficient(plot, plot->m_nSpecial, vCoff, 1);
		SetupDataForFittingPolynomial(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case  CBN_SELCHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_ORDER)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_ORDER, CB_GETCURSEL, 0, 0);
				InitPolynomialCofficient(plot, plot->m_nSpecial, vCoff, (int)num);
				SetupDataForFittingPolynomial(hWndDlg, plot, vCoff);
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_COEFFORDER)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_COEFFORDER, CB_GETCURSEL, 0, 0);
				ostr.str(_TEXT(""));
				ostr<<vCoff[num];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_COEFFICIENT, WM_SETTEXT, 0, (LPARAM)str.c_str());
			}
			
			UpdateWindow(hWndDlg);
			break;
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_COEFFICIENT)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_COEFFICIENT), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_COEFFORDER, CB_GETCURSEL, 0, 0);
				vCoff[num] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingPolynomial(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingPolynomial(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff)
{
	tostringstream ostr;
	tstring str;
	
	int i;
	int initorder = (int)vCoff.size()-1;

	SendDlgItemMessage(hWnd, IDC_DP_FITTING_ORDER, CB_RESETCONTENT, 0, 0);
	for(i=0; i<21; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_ORDER, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_ORDER, CB_SETCURSEL, (WPARAM)initorder, 0);
	
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_COEFFORDER, CB_RESETCONTENT, 0, 0);
	for(i=0; i<=initorder; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_COEFFORDER, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_COEFFORDER, CB_SETCURSEL, (WPARAM)0, 0);

	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_COEFFICIENT, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingPolynomial(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingPolynomialData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingPolynomialData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = PolynomialRemain;
	//Fcn_NlFitDerivative = PolynomialDerivative;
	initfunc(PolynomialDerivative, PolynomialRemain);

	
	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, PolynomialFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Polynomial fitting"));
	
	delete []pX;
	delete []pY;
	return true;
}

bool	ShowFittingPolynomialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_POLYNOMIAL), hWnd, (DLGPROC)DataProcFittingPolynomialDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 1;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingExponentialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingExponential(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff);
bool	SaveDataForFittingExponential(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr);
bool	ProcFittingExponentialData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitExponentialCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff);

double	ExponentialFuncVal(double *pA, int cnum, double x);
double	ExponentialRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	ExponentialDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitExponentialCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;

	vCoff.resize(3);
	
	double xRange[2], yRange[2];
	plot->GetDataRanges(xRange, yRange);

	vCoff[2] = (yRange[1] - yRange[0])/(exp(xRange[1]) - exp(xRange[0]));
	vCoff[1] = 1;
	vCoff[0] = yRange[0] - vCoff[2]*exp(xRange[0]);
}

double	ExponentialFuncVal(double *pA, int cnum, double x)
{
	return pA[2]*exp(pA[1]*x)+pA[0];
}

double	ExponentialRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - ExponentialFuncVal(pA, cnum, x);
}

void	ExponentialDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];
	
	pDiff[0] = 1;
	pDiff[1] = pA[2]*x*exp(pA[1]*x);
	pDiff[2] = exp(pA[1]*x);
}

BOOL	CALLBACK DataProcFittingExponentialDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitExponentialCofficient(plot, plot->m_nSpecial, vCoff);
		SetupDataForFittingExponential(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_A)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_A), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[2] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_B)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_B), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_C)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_C), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingExponential(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingExponential(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_C, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_B, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_A, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingExponential(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingExponentialData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingExponentialData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = ExponentialRemain;
	//Fcn_NlFitDerivative = ExponentialDerivative;
	initfunc(ExponentialDerivative, ExponentialRemain);

	
	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, ExponentialFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Exponential fitting"));
	
	delete []pX;
	delete []pY;
	return true;
}

bool	ShowFittingExponentialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_EXPONENTIAL), hWnd, (DLGPROC)DataProcFittingExponentialDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 2;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingLogarithmicDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingLogarithmic(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff);
bool	SaveDataForFittingLogarithmic(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr);
bool	ProcFittingLogarithmicData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitLogarithmicCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff);

double	LogarithmicFuncVal(double *pA, int cnum, double x);
double	LogarithmicRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	LogarithmicDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitLogarithmicCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;

	vCoff.resize(3);
	
	double xRange[2], yRange[2];
	plot->GetDataRanges(xRange, yRange);

	double b = 1 - xRange[0];

	vCoff[2] = (yRange[1] - yRange[0])/(log(xRange[1] + b) - exp(xRange[0] + b));
	vCoff[1] = b;
	vCoff[0] = yRange[0] - vCoff[2]*log(xRange[0] + b);
}

double	LogarithmicFuncVal(double *pA, int cnum, double x)
{
	return pA[2]*log(x+pA[1])+pA[0];
}

double	LogarithmicRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - LogarithmicFuncVal(pA, cnum, x);
}

void	LogarithmicDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];
	
	pDiff[0] = 1;
	pDiff[1] = pA[2]/(x + pA[1]);
	pDiff[2] = log(x + pA[1]);
}

BOOL	CALLBACK DataProcFittingLogarithmicDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitLogarithmicCofficient(plot, plot->m_nSpecial, vCoff);
		SetupDataForFittingLogarithmic(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_A)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_A), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[2] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_B)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_B), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_C)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_C), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingLogarithmic(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingLogarithmic(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_C, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_B, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_A, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingLogarithmic(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingLogarithmicData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingLogarithmicData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = LogarithmicRemain;
	//Fcn_NlFitDerivative = LogarithmicDerivative;
	initfunc(LogarithmicDerivative, LogarithmicRemain);

	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, LogarithmicFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Logarithmic fitting"));
	
	delete []pX;
	delete []pY;
	return true;
}

bool	ShowFittingLogarithmicDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_LOGARITHMIC), hWnd, (DLGPROC)DataProcFittingLogarithmicDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 3;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff);
bool	SaveDataForFittingGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr);
bool	ProcFittingGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff);

double	GaussianFuncVal(double *pA, int cnum, double x);
double	GaussianRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	GaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;

	vCoff.resize(4);
	
	double xRange[2], yRange[2];
	plot->GetDataRanges(xRange, yRange);

	vCoff[0] = yRange[0];
	vCoff[1] = (xRange[1] - xRange[0])/5.0;
	vCoff[2] = (xRange[1] + xRange[0])/2.0;
	vCoff[3] = yRange[1] - yRange[0];
}

double	GaussianFuncVal(double *pA, int cnum, double x)
{
	//y = a*exp(-( (x-b)/c )^2/2)+d
	return pA[3]*exp(-pow((x-pA[2])/pA[1],2)/2.0)+pA[0];
}

double	GaussianRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - GaussianFuncVal(pA, cnum, x);
}

void	GaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];

	double tmp=exp(-pow((x-pA[2])/pA[1],2)/2);
	
	pDiff[0] = 1;
	pDiff[1] = pA[3]*tmp*(x-pA[2])*(x-pA[2])/(pA[1]*pA[1]*pA[1]);
	pDiff[2] = pA[3]*tmp*(x-pA[2])/(pA[1]*pA[1]);
	pDiff[3] = tmp;
}

BOOL	CALLBACK DataProcFittingGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitGaussianCofficient(plot, plot->m_nSpecial, vCoff);
		SetupDataForFittingGaussian(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_A)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_A), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_B)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_B), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[2] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_C)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_C), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_D)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_D), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingGaussian(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	vCoff)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_D, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_C, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_B, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[3];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_A, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingGaussianData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = GaussianRemain;
	//Fcn_NlFitDerivative = GaussianDerivative;
	initfunc(GaussianDerivative, GaussianRemain);

	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, GaussianFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Gaussian fitting"));
	
	delete []pX;
	delete []pY;
	return true;
}

bool	ShowFittingGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_GAUSSIAN), hWnd, (DLGPROC)DataProcFittingGaussianDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 4;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingSlopeGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff);
bool	SaveDataForFittingSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr);
bool	ProcFittingSlopeGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitSlopeGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff);

double	SlopeGaussianFuncVal(double *pA, int cnum, double x);
double	SlopeGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	SlopeGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitSlopeGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;

	vCoff.resize(5);
	
	double xRange[2], yRange[2];
	plot->GetDataRanges(xRange, yRange);

	vCoff[0] = yRange[0];
	vCoff[1] = 0.0;
	vCoff[2] = (xRange[1] - xRange[0])/5.0;
	vCoff[3] = (xRange[1] + xRange[0])/2.0;
	vCoff[4] = yRange[1] - yRange[0];
}

double	SlopeGaussianFuncVal(double *pA, int cnum, double x)
{
	//y = a*exp(-( (x-b)/c )^2/2)+d
	return pA[4]*exp(-pow((x-pA[3])/pA[2],2)/2.0)+pA[1]*x+pA[0];
}

double	SlopeGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - SlopeGaussianFuncVal(pA, cnum, x);
}

void	SlopeGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];

	double tmp=exp(-pow((x-pA[3])/pA[2],2)/2);
	
	pDiff[0] = 1;
	pDiff[1] = x;
	pDiff[2] = pA[4]*tmp*(x-pA[3])*(x-pA[3])/(pA[2]*pA[2]*pA[2]);
	pDiff[3] = pA[4]*tmp*(x-pA[3])/(pA[2]*pA[2]);
	pDiff[4] = tmp;
}

BOOL	CALLBACK DataProcFittingSlopeGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitSlopeGaussianCofficient(plot, plot->m_nSpecial, vCoff);
		SetupDataForFittingSlopeGaussian(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_A)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_A), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[4] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_B)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_B), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_C)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_C), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[2] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_D)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_D), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_E)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_E), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingSlopeGaussian(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_E, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_D, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_C, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[3];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_B, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[4];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_A, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingSlopeGaussianData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingSlopeGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = SlopeGaussianRemain;
	//Fcn_NlFitDerivative = SlopeGaussianDerivative;
	initfunc(SlopeGaussianDerivative, SlopeGaussianRemain);

	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, SlopeGaussianFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Slope Gaussian fitting"));
	
	delete []pX;
	delete []pY;
	return true;
}

bool	ShowFittingSlopeGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_SLOPEGAUSSIAN1), hWnd, (DLGPROC)DataProcFittingSlopeGaussianDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 5;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingParabolicGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff);
bool	SaveDataForFittingParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr);
bool	ProcFittingParabolicGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitParabolicGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff);

double	ParabolicGaussianFuncVal(double *pA, int cnum, double x);
double	ParabolicGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	ParabolicGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitParabolicGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;

	vCoff.resize(6);
	
	double xRange[2], yRange[2];
	plot->GetDataRanges(xRange, yRange);

	vCoff[0] = yRange[0];
	vCoff[1] = 0.0;
	vCoff[2] = 0.0;
	vCoff[3] = (xRange[1] - xRange[0])/5.0;
	vCoff[4] = (xRange[1] + xRange[0])/2.0;
	vCoff[5] = yRange[1] - yRange[0];
}

double	ParabolicGaussianFuncVal(double *pA, int cnum, double x)
{
	//y = a*exp(-( (x-b)/c )^2/2)+d*x^2+e*x+f
	return pA[5]*exp(-pow((x-pA[4])/pA[3],2.0)/2.0)+pA[2]*x*x+pA[1]*x+pA[0];
}

double	ParabolicGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - ParabolicGaussianFuncVal(pA, cnum, x);
}

void	ParabolicGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];

	double tmp=exp(-pow((x-pA[4])/pA[3],2.0)/2.0);
	
	pDiff[0] = 1;
	pDiff[1] = x;
	pDiff[2] = x*x;
	pDiff[3] = pA[5]*tmp*(x-pA[4])*(x-pA[4])/(pA[3]*pA[3]*pA[3]);
	pDiff[4] = pA[5]*tmp*(x-pA[4])/(pA[3]*pA[3]);
	pDiff[5] = tmp;
}

BOOL	CALLBACK DataProcFittingParabolicGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitParabolicGaussianCofficient(plot, plot->m_nSpecial, vCoff);
		SetupDataForFittingParabolicGaussian(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_A)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_A), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[5] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_B)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_B), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[4] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_C)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_C), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_D)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_D), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[2] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_E)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_E), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			if((int)LOWORD(wparam) == IDC_DP_FITTING_F)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_E), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingParabolicGaussian(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff)
{
	tostringstream ostr;
	tstring str;
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_F, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_E, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_D, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[3];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_C, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[4];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_B, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[5];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_A, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingParabolicGaussianData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingParabolicGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = ParabolicGaussianRemain;
	//Fcn_NlFitDerivative = ParabolicGaussianDerivative;
	initfunc(ParabolicGaussianDerivative, ParabolicGaussianRemain);
	
	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, ParabolicGaussianFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Parabolic gaussian fitting"));
	
	delete []pX;
	delete []pY;
	return true;
}

bool	ShowFittingParabolicGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_PARABOLICGAUSSIAN), hWnd, (DLGPROC)DataProcFittingParabolicGaussianDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 6;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingMultipleGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingMultipleGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff);
bool	SaveDataForFittingMultipleGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr);
bool	ProcFittingMultipleGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitMultipleGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, int nPeaks, vector<double> &vCoff);

double	MultipleGaussianFuncVal(double *pA, int cnum, double x);
double	MultipleGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	MultipleGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitMultipleGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, int nPeaks, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;
	if(nPeaks<1)return;

	vCoff.resize(3*nPeaks+1);
	
	double xRange[2], yRange[2], dx;
	plot->GetDataRanges(xRange, yRange);
	dx = (xRange[1] - xRange[0])/nPeaks;

	vCoff[0] = yRange[0];
	int i;
	for(i=0; i<nPeaks; i++)
	{
		vCoff[3*i+1] = dx/5.0;
		vCoff[3*i+2] = xRange[0] + (i+0.5)*dx;
		vCoff[3*i+3] = yRange[1] - yRange[0];
	}
}

double	MultipleGaussianFuncVal(double *pA, int cnum, double x)
{
	//y = a*exp(-( (x-b)/c )^2/2)+d*x^2+e*x+f
	double val = 0;
	int nPeaks = (cnum - 1)/3;
	int i;
	
	val += pA[0];
	for(i=0; i<nPeaks; i++)
	{
		val += pA[3*i+3]*exp(-pow((x-pA[3*i+2])/pA[3*i+1],2.0)/2.0);
	}
	return val;;
}

double	MultipleGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - MultipleGaussianFuncVal(pA, cnum, x);
}

void	MultipleGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];

	int nPeaks = (cnum-1)/3;

	double tmp;

	int i;

	pDiff[0] = 1;
	for(i=0; i<nPeaks; i++)
	{
		tmp=exp(-pow((x-pA[3*i+2])/pA[3*i+1],2.0)/2.0);
		pDiff[3*i+1] = pA[3*i+3]*tmp*(x-pA[3*i+2])*(x-pA[3*i+2])/(pA[3*i+1]*pA[3*i+1]*pA[3*i+1]);
		pDiff[3*i+2] = pA[3*i+3]*tmp*(x-pA[3*i+2])/(pA[3*i+1]*pA[3*i+1]);
		pDiff[3*i+3] = tmp;
	}
}

BOOL	CALLBACK DataProcFittingMultipleGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

#ifndef _WIN64
	int num;
#else
	LRESULT num;
#endif

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitMultipleGaussianCofficient(plot, plot->m_nSpecial, 1, vCoff);
		SetupDataForFittingMultipleGaussian(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case  CBN_SELCHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_PEAKS)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKS, CB_GETCURSEL, 0, 0);
				InitMultipleGaussianCofficient(plot, plot->m_nSpecial, (int)num+1, vCoff);
				SetupDataForFittingMultipleGaussian(hWndDlg, plot, vCoff);
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_PEAKNO)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+1];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_CK, WM_SETTEXT, 0, (LPARAM)str.c_str());

				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+2];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_BK, WM_SETTEXT, 0, (LPARAM)str.c_str());

				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+3];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_AK, WM_SETTEXT, 0, (LPARAM)str.c_str());
			}
			
			UpdateWindow(hWndDlg);
			break;
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_AK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);

				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_AK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+3] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_BK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_BK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+2] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_CK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_CK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+1] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_D)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_D), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingMultipleGaussian(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingMultipleGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff)
{
	tostringstream ostr;
	tstring str;

	int i;
	int nCurPeaks = (int)(vCoff.size()-1)/3;
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKS, CB_RESETCONTENT, 0, 0);
	for(i=0; i<101; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_PEAKS, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKS, CB_SETCURSEL, (WPARAM)nCurPeaks-1, 0);
	
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKNO, CB_RESETCONTENT, 0, 0);
	for(i=0; i<nCurPeaks; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_PEAKNO, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKNO, CB_SETCURSEL, (WPARAM)0, 0);

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_CK, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_BK, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[3];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_AK, WM_SETTEXT, 0, (LPARAM)str.c_str());

	//bkgnd
	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_D, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingMultipleGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double>	&vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingMultipleGaussianData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingMultipleGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = MultipleGaussianRemain;
	//Fcn_NlFitDerivative = MultipleGaussianDerivative;
	initfunc(MultipleGaussianDerivative, MultipleGaussianRemain);

	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, MultipleGaussianFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Multiple gaussian fitting"));
	
	delete []pX;
	delete []pY;

	return true;
}

bool	ShowFittingMultipleGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_MULTIPLEGAUSSIAN), hWnd, (DLGPROC)DataProcFittingMultipleGaussianDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 7;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingMultipleSlopeGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingMultipleSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff);
bool	SaveDataForFittingMultipleSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr);
bool	ProcFittingMultipleSlopeGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitMultipleSlopeGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, int nPeaks, vector<double> &vCoff);

double	MultipleSlopeGaussianFuncVal(double *pA, int cnum, double x);
double	MultipleSlopeGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	MultipleSlopeGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitMultipleSlopeGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, int nPeaks, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;
	if(nPeaks<1)return;

	vCoff.resize(3*nPeaks+2);
	
	double xRange[2], yRange[2], dx;
	plot->GetDataRanges(xRange, yRange);
	dx = (xRange[1] - xRange[0])/nPeaks;

	vCoff[0] = yRange[0];
	vCoff[1] = 0.0;
	int i;
	for(i=0; i<nPeaks; i++)
	{
		vCoff[3*i+2] = dx/5.0;
		vCoff[3*i+3] = xRange[0] + (i+0.5)*dx;
		vCoff[3*i+4] = yRange[1] - yRange[0];
	}
}

double	MultipleSlopeGaussianFuncVal(double *pA, int cnum, double x)
{
	//y = a*exp(-( (x-b)/c )^2/2)+d*x^2+e*x+f
	double val = 0;
	int nPeaks = (cnum - 2)/3;
	int i;

	val += pA[0] + pA[1]*x;
	for(i=0; i<nPeaks; i++)
	{
		val += pA[3*i+4]*exp(-pow((x-pA[3*i+3])/pA[3*i+2],2.0)/2.0);
	}
	return val;;
}

double	MultipleSlopeGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - MultipleSlopeGaussianFuncVal(pA, cnum, x);
}

void	MultipleSlopeGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];

	int nPeaks = (cnum-2)/3;

	double tmp;

	int i;

	pDiff[0] = 1;
	pDiff[1] = x;
	for(i=0; i<nPeaks; i++)
	{
		tmp=exp(-pow((x-pA[3*i+3])/pA[3*i+2],2.0)/2.0);
		pDiff[3*i+2] = pA[3*i+4]*tmp*(x-pA[3*i+3])*(x-pA[3*i+3])/(pA[3*i+2]*pA[3*i+2]*pA[3*i+2]);
		pDiff[3*i+3] = pA[3*i+4]*tmp*(x-pA[3*i+3])/(pA[3*i+2]*pA[3*i+2]);
		pDiff[3*i+4] = tmp;
	}
}

BOOL	CALLBACK DataProcFittingMultipleSlopeGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

#ifndef _WIN64
	int num;
#else
	LRESULT num;
#endif

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitMultipleSlopeGaussianCofficient(plot, plot->m_nSpecial, 1, vCoff);
		SetupDataForFittingMultipleSlopeGaussian(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case  CBN_SELCHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_PEAKS)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKS, CB_GETCURSEL, 0, 0);
				InitMultipleSlopeGaussianCofficient(plot, plot->m_nSpecial, (int)num+1, vCoff);
				SetupDataForFittingMultipleSlopeGaussian(hWndDlg, plot, vCoff);
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_PEAKNO)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+2];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_CK, WM_SETTEXT, 0, (LPARAM)str.c_str());

				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+3];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_BK, WM_SETTEXT, 0, (LPARAM)str.c_str());

				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+4];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_AK, WM_SETTEXT, 0, (LPARAM)str.c_str());
			}
			
			UpdateWindow(hWndDlg);
			break;
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_AK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);

				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_AK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+4] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_BK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_BK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+3] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_CK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_CK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+2] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_D)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_D), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_E)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_E), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingMultipleSlopeGaussian(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingMultipleSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff)
{
	tostringstream ostr;
	tstring str;

	int i;
	int nCurPeaks = (int)(vCoff.size()-2)/3;
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKS, CB_RESETCONTENT, 0, 0);
	for(i=0; i<101; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_PEAKS, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKS, CB_SETCURSEL, (WPARAM)nCurPeaks-1, 0);
	
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKNO, CB_RESETCONTENT, 0, 0);
	for(i=0; i<nCurPeaks; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_PEAKNO, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKNO, CB_SETCURSEL, (WPARAM)0, 0);

	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_CK, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[3];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_BK, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[4];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_AK, WM_SETTEXT, 0, (LPARAM)str.c_str());

	//bkgnd
	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_D, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_E, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingMultipleSlopeGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingMultipleSlopeGaussianData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingMultipleSlopeGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = MultipleSlopeGaussianRemain;
	//Fcn_NlFitDerivative = MultipleSlopeGaussianDerivative;
	initfunc(MultipleSlopeGaussianDerivative, MultipleSlopeGaussianRemain);

	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, MultipleSlopeGaussianFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}

	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Multiple slope gaussian fitting"));
	
	delete []pX;
	delete []pY;

	return true;
}

bool	ShowFittingMultipleSlopeGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_MULTIPLESLOPEGAUSSIAN), hWnd, (DLGPROC)DataProcFittingMultipleSlopeGaussianDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 8;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL	CALLBACK DataProcFittingMultipleParabolicGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataForFittingMultipleParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff);
bool	SaveDataForFittingMultipleParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr);
bool	ProcFittingMultipleParabolicGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double> &vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve);

void	InitMultipleParabolicGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, int nPeaks, vector<double> &vCoff);

double	MultipleParabolicGaussianFuncVal(double *pA, int cnum, double x);
double	MultipleParabolicGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no);
void	MultipleParabolicGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no);

void	InitMultipleParabolicGaussianCofficient(CXYPlotBasicImpl *plot, int nWhich, int nPeaks, vector<double> &vCoff)
{
	if(!plot)return;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return;
	if(nPeaks<1)return;

	vCoff.resize(3*nPeaks+3);
	
	double xRange[2], yRange[2], dx;
	plot->GetDataRanges(xRange, yRange);
	dx = (xRange[1] - xRange[0])/nPeaks;

	vCoff[0] = yRange[0];
	vCoff[1] = 0.0;
	vCoff[2] = 0.0;
	int i;
	for(i=0; i<nPeaks; i++)
	{
		vCoff[3*i+3] = dx/5.0;
		vCoff[3*i+4] = xRange[0] + (i+0.5)*dx;
		vCoff[3*i+5] = yRange[1] - yRange[0];
	}
}

double	MultipleParabolicGaussianFuncVal(double *pA, int cnum, double x)
{
	//y = a*exp(-( (x-b)/c )^2/2)+d*x^2+e*x+f
	double val = 0;
	int nPeaks = (cnum - 3)/3;
	int i;

	val += pA[0] + pA[1]*x + pA[2]*x*x;
	for(i=0; i<nPeaks; i++)
	{
		val += pA[3*i+5]*exp(-pow((x-pA[3*i+4])/pA[3*i+3],2.0)/2.0);
	}
	return val;;
}

double	MultipleParabolicGaussianRemain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double x, y;
	x = pX[no];
	y = pY[no];

	return y - MultipleParabolicGaussianFuncVal(pA, cnum, x);
}

void	MultipleParabolicGaussianDerivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double x;
	x = pX[no];

	int nPeaks = (cnum-3)/3;

	double tmp;

	int i;

	pDiff[0] = 1;
	pDiff[1] = x;
	pDiff[2] = x*x;
	for(i=0; i<nPeaks; i++)
	{
		tmp=exp(-pow((x-pA[3*i+4])/pA[3*i+3],2.0)/2.0);
		pDiff[3*i+3] = pA[3*i+5]*tmp*(x-pA[3*i+4])*(x-pA[3*i+4])/(pA[3*i+3]*pA[3*i+3]*pA[3*i+3]);
		pDiff[3*i+4] = pA[3*i+5]*tmp*(x-pA[3*i+4])/(pA[3*i+3]*pA[3*i+3]);
		pDiff[3*i+5] = tmp;
	}
}

BOOL	CALLBACK DataProcFittingMultipleParabolicGaussianDlgFunc(HWND hWndDlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotBasicImpl	*plot;
	bool ret;

	static vector<double>	vCoff;
	static vector<double>	vAbsErr;

	tostringstream ostr;
	tstring str;

#ifndef _WIN64
	int num;
#else
	LRESULT num;
#endif

	TCHAR strtmp[nMaxString];
	double fVal;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotBasicImpl *)lparam;
		InitMultipleParabolicGaussianCofficient(plot, plot->m_nSpecial, 1, vCoff);
		SetupDataForFittingMultipleParabolicGaussian(hWndDlg, plot, vCoff);
		
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wparam))
		{
		case  CBN_SELCHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_PEAKS)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKS, CB_GETCURSEL, 0, 0);
				InitMultipleParabolicGaussianCofficient(plot, plot->m_nSpecial, (int)num+1, vCoff);
				SetupDataForFittingMultipleParabolicGaussian(hWndDlg, plot, vCoff);
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_PEAKNO)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+3];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_CK, WM_SETTEXT, 0, (LPARAM)str.c_str());

				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+4];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_BK, WM_SETTEXT, 0, (LPARAM)str.c_str());

				ostr.str(_TEXT(""));
				ostr<<vCoff[3*num+5];
				str=ostr.str();
				SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_AK, WM_SETTEXT, 0, (LPARAM)str.c_str());
			}
			
			UpdateWindow(hWndDlg);
			break;
		case EN_CHANGE:
			if((int)LOWORD(wparam) == IDC_DP_FITTING_AK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);

				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_AK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+5] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_BK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_BK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+4] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_CK)
			{
				num = SendDlgItemMessage(hWndDlg, IDC_DP_FITTING_PEAKNO, CB_GETCURSEL, 0, 0);
				
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_CK), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[3*num+3] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_D)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_D), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[2] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_E)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_E), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[1] = fVal;
			}
			else if((int)LOWORD(wparam) == IDC_DP_FITTING_F)
			{
				GetWindowText(GetDlgItem(hWndDlg, IDC_DP_FITTING_F), strtmp, nMaxString);
				fVal = _tcstod(strtmp, 0);
				vCoff[0] = fVal;
			}
			break;
		} 
		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			ret = SaveDataForFittingMultipleParabolicGaussian(hWndDlg, plot, vCoff, vAbsErr);
			if(ret)
			{
				plot->m_pSpecial = &vCoff;
			}
			if(ret)
				EndDialog(hWndDlg, 1);
			else
				EndDialog(hWndDlg, 0);
			
			return TRUE;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataForFittingMultipleParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> vCoff)
{
	tostringstream ostr;
	tstring str;

	int i;
	int nCurPeaks = (int)(vCoff.size()-2)/3;
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKS, CB_RESETCONTENT, 0, 0);
	for(i=0; i<101; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_PEAKS, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKS, CB_SETCURSEL, (WPARAM)nCurPeaks-1, 0);
	
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKNO, CB_RESETCONTENT, 0, 0);
	for(i=0; i<nCurPeaks; i++)
	{
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		SendDlgItemMessage(hWnd,IDC_DP_FITTING_PEAKNO, CB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_PEAKNO, CB_SETCURSEL, (WPARAM)0, 0);

	ostr.str(_TEXT(""));
	ostr<<vCoff[3];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_CK, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[4];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_BK, WM_SETTEXT, 0, (LPARAM)str.c_str());
	
	ostr.str(_TEXT(""));
	ostr<<vCoff[5];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_AK, WM_SETTEXT, 0, (LPARAM)str.c_str());

	//bkgnd
	ostr.str(_TEXT(""));
	ostr<<vCoff[2];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_D, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_E, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<vCoff[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DP_FITTING_F, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1001;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_POINTSFORFITTED, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<1e-6;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXTOLERANCE, WM_SETTEXT, 0, (LPARAM)str.c_str());

	ostr.str(_TEXT(""));
	ostr<<5000;
	str=ostr.str();
	SendDlgItemMessage(hWnd,IDC_DP_FITTING_MAXITERATION, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SendDlgItemMessage(hWnd, IDC_RESERVE, BM_SETCHECK, TRUE, 0);
	return true;
}

bool	SaveDataForFittingMultipleParabolicGaussian(HWND hWnd, CXYPlotBasicImpl *plot, vector<double> &vCoff, vector<double> &vAbsErr)
{
#ifndef _WIN64
	int check;
#else
	LRESULT check;
#endif

	int nPtsFitted;
	double fMaxTol;
	int	nMaxIter;
	
	TCHAR strtmp[nMaxString];
	double	fVal;
	int nVal;
	
	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_POINTSFORFITTED), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nPtsFitted = nVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXTOLERANCE), strtmp, nMaxString);
	fVal = _tcstod(strtmp, 0);
	fMaxTol = fVal;

	GetWindowText(GetDlgItem(hWnd, IDC_DP_FITTING_MAXITERATION), strtmp, nMaxString);
	nVal = _ttoi(strtmp);
	nMaxIter = nVal;
	
	check = SendDlgItemMessage(hWnd, IDC_RESERVE, BM_GETCHECK, 0, 0);
	return ProcFittingMultipleParabolicGaussianData(plot, plot->m_nSpecial, vCoff, vAbsErr, fMaxTol, nMaxIter, nPtsFitted, check==BST_CHECKED);
}

bool	ProcFittingMultipleParabolicGaussianData(CXYPlotBasicImpl *plot, int nWhich, vector<double>	&vCoff, vector<double> &vAbsErr, double fMaxTol, int nMaxIter, int nPtsFitted, bool reserve)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>plot->GetPlotDataCount())return false;
	
	int i, dataID;
	dataID = plot->GetDataID(nWhich);
	if(dataID<0)return false;
	
	MyVData2D vData;
	vData = plot->GetAbsData(nWhich);

	int nLen = (int)vData.size();
	int cnum = (int)vCoff.size();

	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];

	for(i=0; i<nLen; i++)
	{
		pX[i] = vData[i].val[0];
		pY[i] = vData[i].val[1];
	}

	//extern FcnRemain		*Fcn_NlFitRemain;
	//extern FcnDerivative	*Fcn_NlFitDerivative;
	//Fcn_NlFitRemain = MultipleParabolicGaussianRemain;
	//Fcn_NlFitDerivative = MultipleParabolicGaussianDerivative;
	initfunc(MultipleParabolicGaussianDerivative, MultipleParabolicGaussianRemain);

	if(!PerformNlFit(plot, dataID, pX, pY, nLen, vCoff, vAbsErr, vData, cnum, fMaxTol, nMaxIter, nPtsFitted, MultipleParabolicGaussianFuncVal))
	{
		delete []pX;
		delete []pY;
		return false;
	}
	SetProcessedData(plot, nWhich, vData, reserve, _TEXT("Multiple parabolic gaussian fitting"));
	
	delete []pX;
	delete []pY;

	return true;
}

bool	ShowFittingMultipleParabolicGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich)
{
	if(!plot)return false;
	if(nWhich<0 || nWhich>=plot->GetPlotDataCount())return false;
	
	plot->m_nSpecial = nWhich;
	if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATAPROCESSING_FITTING_MULTIPLEPARABOLICGAUSSIAN), hWnd, (DLGPROC)DataProcFittingMultipleParabolicGaussianDlgFunc, (LPARAM)plot) )
	{
		plot->m_nSpecial = 9;
		HWND hDlg = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_INFORMATION), hWnd, (DLGPROC)ShowInfomationDlgFunc, (LPARAM)plot);
		ShowWindow(hDlg, SW_SHOW);
		return true;
	}
	return false;
}

Declare_Namespace_End

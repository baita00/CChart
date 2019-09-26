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

#ifndef __DATAPROCESSING_H_122333444455555__
#define __DATAPROCESSING_H_122333444455555__

#include <windows.h>
#include "../CChartNameSpace.h"


Declare_Namespace_CChart

class CXYPlotBasicImpl;

bool	ShowOffsetDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowScaleDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowAffineDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);

bool	ShowDiffentialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowIntegratialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);

bool	ShowMedianFilterDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowAverageFilterDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);

bool	ShowFittingPolynomialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingExponentialDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingLogarithmicDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingSlopeGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingParabolicGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingMultipleGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingMultipleSlopeGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);
bool	ShowFittingMultipleParabolicGaussianDataDlg(CXYPlotBasicImpl *plot, HWND hWnd, int nWhich);

Declare_Namespace_End

#endif

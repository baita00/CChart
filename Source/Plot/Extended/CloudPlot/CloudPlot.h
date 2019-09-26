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

#pragma warning(disable:4786)
#include <windows.h>
#include "CloudLegend.h"
#include "../ContourLinePlot/ContourBasicPlot.h"

Declare_Namespace_CChart

class CCloudPlot : virtual public CContourBasicPlot
{
public:
	CCloudPlot();
	virtual ~CCloudPlot();
	void	SetDefaults();

protected:
	HDC		m_hMemImageDC;
	HBITMAP	m_hOldBmp;

	int		m_nWidth,m_nHeight;

	bool	m_bExponential;

	double	m_fMaxValue, m_fMinValue;

	bool	m_bRightAxisAsLegend;
	bool	m_bTopAxisAsLegend;
	bool	m_bLeftAxisAsLegend;
	bool	m_bBottomAxisAsLegend;

public:
	inline	int		GetWidth(){return m_nWidth;}
	inline	void	SetWidth(int width){m_nWidth = width;}
	inline	int		GetHeight(){return m_nHeight;}
	inline	void	SetHeight(int height){m_nHeight = height;}

	inline	bool	IsExponential(){return m_bExponential;}
	inline	void	SetExponential(bool exp){m_bExponential = exp;}
	inline	bool	IsRightAxisAsLegend(){return m_bRightAxisAsLegend;}
	inline	void	SetRightAxisAsLegend(bool as){m_bRightAxisAsLegend = as;}
	inline	bool	IsTopAxisAsLegend(){return m_bTopAxisAsLegend;}
	inline	void	SetTopAxisAsLegend(bool as){m_bTopAxisAsLegend = as;}
	inline	bool	IsLeftAxisAsLegend(){return m_bLeftAxisAsLegend;}
	inline	void	SetLeftAxisAsLegend(bool as){m_bLeftAxisAsLegend = as;}
	inline	bool	IsBottomAxisAsLegend(){return m_bBottomAxisAsLegend;}
	inline	void	SetBottomAxisAsLegend(bool as){m_bBottomAxisAsLegend = as;}

	inline	double	GetMaxValue(){return m_fMaxValue;}
	inline	double	GetMinValue(){return m_fMinValue;}

	//inline	void	SetFieldFcn(double (*_pFieldFcn) (double, double)){CBaseContourPlot::SetFieldFcn(_pFieldFcn); m_bImageChanged = true;}

public:
	void	CreateMemDC(HWND hWnd);
	void	ReleaseMemDC();
	bool	CreateImage();
	bool	UpdateImage(RECT plotRect);

	void	DrawCloud(HDC hDC, RECT plotRect, bool bXOpposite, bool bYOpposite);
};

Declare_Namespace_End


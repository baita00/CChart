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

#include <vector>
#include <windows.h>

#include "CChartNameSpace.h"

Declare_Namespace_CChart

class CPlotDataErrorBar
{
public:
	CPlotDataErrorBar();
	virtual	~CPlotDataErrorBar();
protected:
	void	SetDefaults();
protected:
	// for error bar
	static	bool			m_bErrorBarMode;
	static	std::vector<double>	m_vfUBound;
	static	std::vector<double>	m_vfLBound;
	int				m_nErrorBarLineSize;
	int				m_nErrorBarLineStyle;
	bool			m_bErrorBarSameColor;
	COLORREF		m_crErrorBarLineColor;
	int				m_nErrorBarHeadHalfLen;
public:
	inline	bool	IsErrorBarMode(){ return m_bErrorBarMode; }
	inline	void	SetErrorBarMode( bool errorbar ){ m_bErrorBarMode = errorbar; }
	inline	std::vector<double>	&GetUBound(){return m_vfUBound;}
	inline	void	SetUBound(std::vector<double> vub){m_vfUBound.clear(); m_vfUBound = vub;}
	inline	std::vector<double>	&GetLBound(){return m_vfLBound;}
	inline	void	SetLBound(std::vector<double> vlb){m_vfLBound.clear(); m_vfLBound = vlb;}
	inline	int		GetErrorBarLineSize(){ return m_nErrorBarLineSize; }
	inline	void	SetErrorBarLineSize(int size){ m_nErrorBarLineSize = size; }
	inline	int		GetErrorBarLineStyle(){ return m_nErrorBarLineStyle; }
	inline	void	SetErrorBarLineStyle(int style){ m_nErrorBarLineStyle = style; }
	inline	bool	IsErrorBarSameColor(){return m_bErrorBarSameColor;}
	inline	void	SetErrorBarSameColor(bool same){m_bErrorBarSameColor = same;}
	inline	COLORREF	GetErrorBarLineColor(){return m_crErrorBarLineColor;}
	inline	void		SetErrorBarLineColor(COLORREF cr){m_crErrorBarLineColor = cr;}
	inline	int		GetErrorBarHeadHalfLen(){ return m_nErrorBarHeadHalfLen; }
	inline	void	SetErrorBarHeadHalfLen(int len){ m_nErrorBarHeadHalfLen = len; }

};

Declare_Namespace_End
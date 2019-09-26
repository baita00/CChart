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

#include "CChartNameSpace.h"

Declare_Namespace_CChart

// how to fill the curve
enum
{
	kDataFillClosed = 0,
	kDataFillFromBottomAxis = 1,
	kDataFillFromTopAxis = 2,
	kDataFillFromLeftAxis = 3,
	kDataFillFromRightAxis = 4,
	
	kDataFillModeCount
};

class	CPlotDataFill
{
public:
	CPlotDataFill();
	virtual ~CPlotDataFill();

protected:
	void	SetDefaults();

	void	DeleteUsedHandles();

protected:
	int			m_nDataFillMode;
	bool		m_bDataFill;
	int			m_nDataFillBrushType;
	COLORREF	m_crDataFillColor;
	int			m_nDataFillHatchStyle;
	HBITMAP		m_hDataFillPattern;
	int			m_nDataFillAlpha;

public:
	inline		int				GetDataFillMode(){ return m_nDataFillMode; }
	inline		void			SetDataFillMode(int nMode){ m_nDataFillMode = nMode; }
	inline		bool			IsDataFill(){ return m_bDataFill; }
	inline		void			SetDataFill(bool bFill){ m_bDataFill = bFill; }
	inline		int				GetDataFillBrushType(){ return m_nDataFillBrushType; }
	inline		void			SetDataFillBrushType(int nType){ m_nDataFillBrushType = nType; }
	inline		COLORREF		GetDataFillColor(){ return m_crDataFillColor; }
	inline		void			SetDataFillColor(COLORREF crFill){ m_crDataFillColor = crFill; }
	inline		int				GetDataFillHatchStyle(){ return m_nDataFillHatchStyle; }
	inline		void			SetDataFillHatchStyle(int nStyle){ m_nDataFillHatchStyle = nStyle; }
	inline		HBITMAP			GetDataFillPattern(){ return m_hDataFillPattern; }
	inline		void			SetDataFillPattern( HBITMAP pattern ){ if(m_hDataFillPattern)DeleteObject(m_hDataFillPattern); m_hDataFillPattern = pattern; }
	inline		int				GetDataFillAlpha(){ return m_nDataFillAlpha; }
	inline		void			SetDataFillAlpha(int alpha){ m_nDataFillAlpha = alpha; }
};

Declare_Namespace_End

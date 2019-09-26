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

#include "CChartNameSpace.h"

Declare_Namespace_CChart

// how the histogram draw start
enum
{
	kHistoFromBottom,
	kHistoFromTop,
	kHistoFromLeft,
	kHistoFromRight,
	
	kHistoModeCount
};

class CPlotDataHisto
{
public:
	CPlotDataHisto();
	virtual	~CPlotDataHisto();
protected:
	void		SetDefaults();
protected:
	bool		m_bHisto;
	int			m_nHistoMode;
public:
	inline		bool	IsHisto(){return m_bHisto;}
	inline		void	SetHisto(bool histo){m_bHisto = histo;}
	inline		int		GetHistoMode(){return m_nHistoMode;}
	inline		void	SetHistoMode(int mode){m_nHistoMode = mode;}
};

Declare_Namespace_End
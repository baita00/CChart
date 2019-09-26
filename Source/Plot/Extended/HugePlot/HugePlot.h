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
#include <vector>
using namespace std;

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

// Huge data Selection method
enum
{
	kHSMean = 0,
	kHSMedian,
	kHSMiddle,
	kHSBegin,
	kHSEnd,
	
	kHSCount
};


class CHugePlot
{
public:
	CHugePlot();
	virtual ~CHugePlot();

	void	SetDefaults();

protected:
	int		m_nHugeSelection;

public:
	inline	int		GetHugeSelection(){return m_nHugeSelection;}
	inline	void	SetHugeSelection(int sel){m_nHugeSelection = sel;}
};

Declare_Namespace_End
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
#include <windows.h>
Declare_Namespace_CChart

class	CReactStatus
{
public:
	CReactStatus(){};
	virtual	~CReactStatus(){};

public:
	virtual	void				UnlightAll() = 0;
	virtual	void				UnselectAll() = 0;
	virtual	void				UnlightAll( bool bTitle, bool bAxes, bool bDatasets) = 0;
	virtual	void				UnselectAll( bool bTitle, bool bAxes, bool bDatasets)  = 0;

public:
	virtual	void				SwitchSelection() = 0;
	virtual	void				SwitchSelAxes() = 0;
	virtual	void				SwitchSelPlotData() = 0;
	virtual	void				SwitchSelTitle() = 0;

public:
	virtual	bool				ShowDlgBySelection(HDC hDC) = 0;
};

Declare_Namespace_End
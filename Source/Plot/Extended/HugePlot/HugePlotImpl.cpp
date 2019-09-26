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

//#include "StdAfx.h"
#include "HugePlotImpl.h"

using namespace NsCChart;

CHugePlotImpl::CHugePlotImpl()
{
	SetFloatXTicks(true);
}

CHugePlotImpl::~CHugePlotImpl()
{
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		vector<DataPoint2D> *pData = GetAbsPlotData(i)->GetPDataSaved();
		delete pData;
		pData = 0;
	}
}

void	CHugePlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	//if there is no data, doing nothing


}


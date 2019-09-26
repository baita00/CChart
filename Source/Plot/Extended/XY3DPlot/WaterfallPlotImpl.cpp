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

#include "WaterfallPlotImpl.h"

using namespace NsCChart;

CWaterfallPlotImpl::CWaterfallPlotImpl()
{
	SetAutoDataColorGeneral(false);
}

CWaterfallPlotImpl::~CWaterfallPlotImpl()
{
}
/*
void	CWaterfallPlotImpl::SetDefaults( int index )
{
	CXY3DPlotImpl::CXY3DPlot::SetDefaults(index);
}
*/
namespace NsCChart{

bool waterZGreat(CPlotData<DataPoint2D> &d1, CPlotData<DataPoint2D> &d2)
{
	return ((XY3DProp*)d1.GetExtProp())->vfZData[0] > ((XY3DProp*)d2.GetExtProp())->vfZData[0];
}
typedef vector<CPlotData<DataPoint2D> > vpXY3DDataInfo;

}
int		CWaterfallPlotImpl::AddCurve( double *pX, double *pY, double z, int nLen )
{
	if (!pX || !pY || nLen<=0)return -1;

	double *pZ = new double[nLen];
	
	int i;
	for(i=0; i<nLen; i++)
	{
		pZ[i] = z;
	}
	int dataID = CXY3DPlotImpl::AddCurve(pX, pY, pZ, nLen);
	SetDataFill(dataID, true);
	SetDataFillColor(dataID, RGB(192, 192, 192));
	SetDataLineSize(dataID, 3);
	SetDataFillAlpha(dataID, 128);
	
	delete []pZ;

	// Sort the data by z value
	sort(*GetVPlotData().begin(), *GetVPlotData().end(), waterZGreat);

	return dataID;
}

int		CWaterfallPlotImpl::AddPoint(double x, double y, double z, int nWhich)
{
	int dataID = CXY3DPlotImpl::AddPoint(x, y, z, nWhich);

	if(dataID>=0)
	{
		if(((XY3DProp*)GetPlotData(dataID)->GetExtProp())->vfZData.size() == 1)
		{
			SetDataFill(dataID, true);
			SetDataFillColor(dataID, RGB(192, 192, 192));
			SetDataLineSize(dataID, 3);
			SetDataFillAlpha(dataID, 128);
		}
	}

	// Sort the data by z value
	sort(*GetVPlotData().begin(), *GetVPlotData().end(), waterZGreat);

	return dataID;
}
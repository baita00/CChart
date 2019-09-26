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

#include "BubblePlotImpl.h"

using namespace NsCChart;

CBubblePlotImpl::CBubblePlotImpl()
{

}

CBubblePlotImpl::~CBubblePlotImpl()
{

}

int		CBubblePlotImpl::AddBubble(double x, double y)
{
	int dataID;
	double pX[1], pY[1];
	pX[0] = x;
	pY[0] = y;
	
	dataID = AddCurve(pX, pY, 1);
	SetPlotType(dataID, kXYPlotScatter);
	SetMarkerShow(dataID, true);
	SetMarkerFrequency(dataID, 1);
	SetMarkerSize(dataID, 20);
	SetMarkerType(dataID, kXYMarkerCircle);
	SetMarkerFillAlpha(dataID, 128);
	return dataID;
}

int		CBubblePlotImpl::AddBubble(double x, double y, int which)
{
	if(which < 0 || which >= GetPlotDataCount())return -1;

	int dataID;
	dataID = AddPoint(x, y, which);
	return dataID;
}

void	CBubblePlotImpl::SetBubbleSize(int size, int which)
{
	if(which < 0 || which >= GetPlotDataCount())return;
	SetMarkerSize(GetDataID(which), size);
}

void	CBubblePlotImpl::SetBubbleColor(COLORREF color, int which)
{
	if(which < 0 || which >= GetPlotDataCount())return;
	SetDataColor(GetDataID(which), color);
}

void	CBubblePlotImpl::SetBubbleImage(HBITMAP hImage, int which)
{
	if(which < 0 || which >= GetPlotDataCount())return;
	SetMarkerImage(GetDataID(which), hImage);
}

void	CBubblePlotImpl::SetBubbleImage(tstring fileName, int which)
{
	if(which < 0 || which >= GetPlotDataCount())return;
	
	HBITMAP hBitmap = HBitmapFromFile(fileName);
	if(!hBitmap)return;

	SetBubbleImage(hBitmap, which);
}
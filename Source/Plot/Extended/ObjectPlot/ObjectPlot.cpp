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
#include "ObjectPlot.h"

using namespace NsCChart;

CObjectPlot::CObjectPlot()
{

}

CObjectPlot::~CObjectPlot()
{

}

void	CObjectPlot::LightMetaObject(int index, bool bLight)
{
	if(index >= 0 && index < (int)m_vMetaObjects.size())
	{
		m_vMetaObjects[index].bLighted = bLight;
	}
}

void	CObjectPlot::UnlightAllMetaObjects()
{
	for(int i = 0; i < (int)m_vMetaObjects.size(); i++)
	{
		m_vMetaObjects[i].bLighted = false;
	}
}

void	CObjectPlot::SelectMetaObject(int index, bool bSelect)
{
	if(index >= 0 && index < (int)m_vMetaObjects.size())
	{
		m_vMetaObjects[index].bSelected = bSelect;
	}
}

void	CObjectPlot::UnselectAllMetaObjects()
{
	for(int i = 0; i < (int)m_vMetaObjects.size(); i++)
	{
		m_vMetaObjects[i].bSelected = false;
	}
}

void	CObjectPlot::AddLine(double x1, double y1, bool bData1, double x2, double y2, bool bData2, int linestyle, int linewidth, COLORREF color)
{
	MetaObject obj;

	obj.nCode = 1;

	obj.fX1 = x1;
	obj.fY1 = y1;
	obj.bData1 = bData1;
	obj.fX2 = x2;
	obj.fY2 = y2;
	obj.bData2 = bData2;
	obj.nLineStyle = linestyle;
	obj.nLineWidth = linewidth;
	obj.crLineColor = color;
	obj.bLighted = false;
	obj.bSelected = false;

	m_vMetaObjects.push_back(obj);
}

void	CObjectPlot::AddRectangle(double x1, double y1, bool bData1, double x2, double y2, bool bData2, int linestyle, int linewidth, COLORREF linecolor, bool bFill, COLORREF fillcolor)
{
	MetaObject obj;

	obj.nCode = 2;

	obj.fX1 = x1;
	obj.fY1 = y1;
	obj.bData1 = bData1;
	obj.fX2 = x2;
	obj.fY2 = y2;
	obj.bData2 = bData2;
	obj.nLineStyle = linestyle;
	obj.nLineWidth = linewidth;
	obj.crLineColor = linecolor;
	obj.bFill = bFill;
	obj.crFillColor = fillcolor;
	obj.bLighted = false;
	obj.bSelected = false;

	m_vMetaObjects.push_back(obj);
}

void	CObjectPlot::AddEllipse(double ctx, double cty, bool bDataCT, double rx, bool bDataX, double ry, bool bDataY, int linestyle, int linewidth, COLORREF linecolor, bool bFill, COLORREF fillcolor)
{
	MetaObject obj;

	obj.nCode = 3;

	obj.fCtX = ctx;
	obj.fCtY = cty;
	obj.bDataCT = bDataCT;
	obj.fRx = rx;
	obj.bDataX = bDataX;
	obj.fRy = ry;
	obj.bDataY = bDataY;
	obj.nLineStyle = linestyle;
	obj.nLineWidth = linewidth;
	obj.crLineColor = linecolor;
	obj.bFill = bFill;
	obj.crFillColor = fillcolor;
	obj.bLighted = false;
	obj.bSelected = false;

	m_vMetaObjects.push_back(obj);
}

void	CObjectPlot::AddCircle(double ctx, double cty, bool bDataCT, double radius, bool bDataR, bool bDataRX, int linestyle, int linewidth, COLORREF linecolor, bool bFill, COLORREF fillcolor)
{
	MetaObject obj;

	obj.nCode = 4;

	obj.fCtX = ctx;
	obj.fCtY = cty;
	obj.bDataCT = bDataCT;
	obj.fRadius = radius;
	obj.bDataR = bDataR;
	obj.bDataRX = bDataRX;
	obj.nLineStyle = linestyle;
	obj.nLineWidth = linewidth;
	obj.crLineColor = linecolor;
	obj.bFill = bFill;
	obj.crFillColor = fillcolor;
	obj.bLighted = false;
	obj.bSelected = false;

	m_vMetaObjects.push_back(obj);
}
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

#include "ObjectPlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class CObjectPlotImpl : public CObjectPlot, public CXYPlotImplI
{
public:
	CObjectPlotImpl();
	virtual ~CObjectPlotImpl();

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("ObjectPlot");}
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

protected:
	void				DrawLine(HDC hDC, double x1, double y1, bool bData1, double x2, double y2, bool bData2, int linestyle, int linewidth, COLORREF color);
	void				DrawRectangle(HDC hDC, double x1, double y1, bool bData1, double x2, double y2, bool bData2, int linestyle, int linewidth, COLORREF linecolor, bool bFill, COLORREF fillcolor);
	void				DrawEllipse(HDC hDC, double ctx, double cty, bool bDataCT, double rx, bool bDataX, double ry, bool bDataY, int linestyle, int linewidth, COLORREF linecolor, bool bFill, COLORREF fillcolor);
	void				DrawCircle(HDC hDC, double ctx, double cty, bool bDataCT, double radius, bool bDataR, bool bDataRX, int linestyle, int linewidth, COLORREF linecolor, bool bFill, COLORREF fillcolor);

	void				DrawObjects(HDC hDC, RECT destRect);

protected:
	void				GetCoodinate(HDC hDC, POINT &point, double x, double y, bool bData);
	double				GetLength(HDC hDC, double val, bool bData, bool bDataX);
public:	
	bool				PtNearLine(POINT point, POINT point1, POINT point2);
	bool				PtNearObject(HDC hDC, POINT point, MetaObject object);
	int					GetNeareatObjectID(HDC hDC, POINT point);

public:
	bool		OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnMouseMove( HDC hDC, POINT point, UINT ctrlKey );

};

typedef CObjectPlotImpl CObjectPlotImplI;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

Declare_Namespace_End
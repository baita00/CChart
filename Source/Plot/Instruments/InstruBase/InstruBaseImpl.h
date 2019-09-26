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
#include "InstruBase.h"

Declare_Namespace_CChart

class CInstruBaseImpl : public CInstruBase
{
public:
	CInstruBaseImpl();
	virtual	~CInstruBaseImpl();

public:
	virtual void	OnDraw(HWND hWnd, RECT destRect) = 0;
	virtual	void	OnDraw(HDC hDC, RECT destRect) = 0;

protected:
	SIZE			GetTitleSize(HDC hDC);
	SIZE			GetMaxSingleTickLabelSize(HDC hDC);
	int				GetMaxTickLength();
	SIZE			GetPaddingSize( HDC hDC );

	double			GetMajorTickPosValue( int whichTick );
	tstring			GetMajorTickLabel( int whichTick );
	bool			GetMinorTickPosValue( int whichMajor, double *fPos);

	virtual	RECT	GetPlotRect(HDC hDC, RECT destRect);
	virtual	RECT	GetBarRect(HDC hDC, RECT plotRect);
	virtual	RECT	GetTitleRect(HDC hDC, RECT barRect, RECT plotRect);
	virtual	RECT	GetAxisRect(HDC hDC, RECT barRect);

protected:
	void			DrawBorder(HDC hDC, RECT plotRect);
	void			DrawTitle(HDC hDC, RECT barRect, RECT plotRect);
	virtual	void	DrawAxisLine(HDC hDC, RECT axisRect);
	virtual	void	DrawColorAxis(HDC hDC, RECT axisRect);
	virtual	void	DrawMajorTicks(HDC hDC, RECT axisRect);
	virtual	void	DrawSubMajorTicks(HDC hDC, RECT axisRect);
	virtual	void	DrawMinorTicks(HDC hDC, RECT axisRect);
	virtual	void	DrawTickLabels(HDC hDC, RECT axisRect, RECT barRect);
	virtual	void	DrawCurrentFilling(HDC hDC, RECT barRect);
	virtual	void	DrawWholeCircleFilling(HDC hDC, RECT barRect);
	void			DrawAxis(HDC hDC, RECT barRect);
	virtual	void	DrawBar(HDC hDC, RECT barRect);

protected:
	void			DrawStaticPointers(HDC hDC, RECT barRect);
	virtual	void	DrawPointer(HDC hDC, RECT barRect);
	
protected:
	void			DrawStick(HDC hDC, RECT barRect);
	void			DrawPointerLineShape(HDC hDC, double tht, double radius, double ctx, double cty);
	void			DrawPointerBarShape(HDC hDC, double tht, double radius, double ctx, double cty);
	void			DrawPointerSwordShape(HDC hDC, double tht, double radius, double ctx, double cty);
	void			DrawPointerArrowShape(HDC hDC, double tht, double radius, double ctx, double cty);
	void			DrawPointerTriangleShape(HDC hDC, double tht, double radius, double ctx, double cty);
	void			DrawPointerFillArcShape(HDC hDC, double tht, double radius, double ctx, double cty);
	void			DrawPointerTail(HDC hDC, double tht, double radius, double ctx, double cty);
	void			GetPointerParams(HDC hDC, RECT barRect, double &tht, double &radius, double &ctx, double &cty);
	double			GetSideTickLengh(bool outer);
	
protected:
	virtual	RECT	GetPointerRect(HDC hDC, RECT barRect);

protected:
	void			FillPolygon(HDC hDC, COLORREF color, POINT *pPt, int nPt);

protected:
	void			DrawWheelBar(HDC hDC, RECT barRect);

};

Declare_Namespace_End

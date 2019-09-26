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
#include "MyString.h"
#include "CChartNameSpace.h"
using namespace std;

Declare_Namespace_CChart

enum
{
	kGradientL2R,
	kGradientT2B,
	kGradientLT2RB,
	kGradientLB2RT,
	kGradientCt2Bd,
	kGradientHCt2Bd,
	kGradientVCt2Bd,
	kGradientRadial,
	
	kGradientTypeCount
};

const	double fMaxVal	=	1e120;
const	double fMinVal	=	1e-120;

double			RoundToRange( double value, double range );
int				GetMagnitude( double value );
double			RoundToInt( double value );
double			Round( double value, int level );

int				GetRemainLength(double value);

double			MyRange(double *range);
bool			InRange(double val, double *range);
bool			InRange(double *data, double *xRange, double *yRange);
void			MoveRange(double *range, double dx);
bool			CompareRange(double *range1, double *range2);
void			ResetTicks(double *range, int &ticks);
void			GetOptimalRange( double *inRange, double *outRange, bool closeRange, 
								bool optimalExact, bool optimalZeroRepair, bool optimalSymetryRepair, bool optimalExtendL, bool optimalExtendU);
void			GetOptimalRange( double *inRange, double *outRange,	int *nTicks, int *nMinor, bool closeRange,
								bool optimalExact, bool optimalZeroRepair, bool optimalSymetryRepair, bool optimalExtendL, bool optimalExtendU );
void			GetOptimalRangePrivate( double *inRange, double *outRange,	int *nTicks, int *nMinor, double margin, double wasted,
									   bool optimalExact, bool optimalZeroRepair, bool optimalSymetryRepair, bool optimalExtendL, bool optimalExtendU );
void			GetOptimalRangeExact( double *inRange, double *outRange,int *nTicks, int *nMinor, int nSegsPrefer=5 );
double			GetTickOptimalTickInterval( double lower, double upper );
void			GetLogValue(double val, int &ev, double &rv);
void			RegulateTicks(double dMin, double dMax, int &nTicks, int &nMinorTicks, double &tickMin, double &tickMax);
void			ComputeTicksWithGap(double dMin, double dMax, double gap, int &nTicks, int &nMinorTicks, double &tickMin, double &tickMax);

void			GradientSides( HDC hDC, POINT pt1, POINT pt2, POINT pt3, POINT pt4, COLORREF crCt, COLORREF crS1, COLORREF crS2);
void			GradientSides( HDC hDC, POINT point[6], COLORREF crCt, COLORREF crS1, COLORREF crS2);
void			GradientRect( HDC hDC, RECT rect, int type, COLORREF cr1, COLORREF cr2);
void			GradientRect( HDC hDC, RECT rect, bool bHoriz, COLORREF *pCr, long *pCord, int nCord);
void			MyGetTextExtentPoint(HDC hDC, const LOGFONT *logFont, tstring str, UINT uFormat, LPSIZE lpSize);
void			EraseRect( HDC hDC, RECT rect, COLORREF color);
void			EraseRectGap( HDC hDC, RECT outerRect, RECT innerRect, COLORREF color);
void			EraseEllipse( HDC hDC, RECT rect, COLORREF color);

HBITMAP			HBitmapFromFile(tstring fileName);
HBITMAP			HBitmapFromWnd(HWND hWnd);
HBITMAP			HBitmapFromDC(HDC hDC);
HBITMAP			CopyHBitmap(HBITMAP hSourceBitmap);
SIZE			GetBitmapSize(HBITMAP hBitmap);
bool			CopyImageToClipBrd(HWND hWnd);
bool			SaveImageToFile(HWND hWnd, tstring fileName, tstring fileext);
bool			SaveHBitmapToFile(HBITMAP hBm, tstring fileName, tstring fileext);

BOOL			MyClientToScreen(HWND hWnd, LPRECT lpRect);
BOOL			MyScreenToClient(HWND hWnd, LPRECT lpRect);

long			Width(RECT rect);
long			Height(RECT rect);
double			GetCenterX(RECT rect);
double			GetCenterY(RECT rect);
bool			SquareRect(RECT &rect);
bool			ScaleRect(RECT &rect, double xlow, double xhigh, double ylow, double yhigh);
void			NormalizeRect(RECT &rect);

RECT			GetTextRectAroundEllipse(HDC hDC, RECT ellipseRect, RECT *outerRect, double tht, double scalex, double scaley, tstring str, LOGFONT logFont, int innerGap = 4, bool outer = true);
void			DrawCircleEdge(HDC hDC, double ctx, double cty, double radius, bool toInner = true, double thick = 5, double angle1 = 135, COLORREF cr1 = RGB(150, 150, 150), COLORREF cr2 = RGB(200, 200, 200));
void			DrawArrowInCircle(HDC hDC, POINT point, int nRadius, double angle, COLORREF fillColor, COLORREF arrowColor);
void			FillEllipse(HDC hDC, POINT point, int a, int b, COLORREF fillColor);
void			DrawLine(HDC hDC, RECT plotRect, double *pt1, double *pt2, double *xRange, double *yRange, COLORREF cr, int nLineWidth, int nLineStyle);
void			DrawLine(HDC hDC, RECT plotRect, double x1, double y1, double x2, double y2, double *xRange, double *yRange, COLORREF cr, int nLineWidth, int nLineStyle);
void			DrawLine(HDC hDC, POINT pt1, POINT pt2, int style, int width, COLORREF cr);
void			DrawArrowLine(HDC hDC, POINT pt1, POINT pt2, int alpha, COLORREF cr, int nLineWidth, int nLineStyle);
void			DrawRect(HDC hDC, RECT plotRect, double *lt, double *rb, double *xRange, double *yRange, COLORREF cr, int nLineSize, int nLineStyle, bool bFill, COLORREF fillColor);
void			DrawRect(HDC hDC, RECT plotRect, double left, double top, double right, double bottom, double *xRange, double *yRange, COLORREF cr, int nLineSize, int nLineStyle, bool bFill, COLORREF fillColor);	
void			DrawTextInRect(HDC hDC, RECT rect, tstring text, const LOGFONT *logFont, int backmode, COLORREF color, UINT format = DT_CENTER | DT_BOTTOM, LOGFONT *lfReal=0);
RECT			GetTextRect(HDC hDC, POINT ptLT, tstring text, LOGFONT *logFont, bool bConfine=false, RECT *lpRect=0, bool bLT=true);


bool			MyStr2Time(tstring strTime, tstring fmt, double &fTime);
bool			MyStr2Time(tstring strTime, tstring fmt, time_t &tmTime);
bool			MyTime2Str(time_t tmTime, tstring &strTime, tstring fmt);

bool			ValInInterval(double val, double low, double high);
void			SetMyCapture(HWND hWnd);

bool			IsHandle(HANDLE handle);
//function for elastic text
void			DrawVectorText(HDC hDC, tstring strText, POINT ptPos);
FIXED			FloatToFixed(double fval);

COLORREF		MixColor(COLORREF cr1, COLORREF cr2, double rat);

Declare_Namespace_End

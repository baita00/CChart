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

#ifndef __STEMPLOTIMPL_H_122333444455555__
#define __STEMPLOTIMPL_H_122333444455555__

#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/XYHandler.h"
#include "StemPlot.h"
#include "StemHandler.h"

Declare_Namespace_CChart

class	CStemPlotImpl : public CStemPlot,
						public CXYPlotImpl
{
public:
	CStemPlotImpl();
	virtual ~CStemPlotImpl();
private:
	int		m_nMaxSize;

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("StemPlot");}

	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	int		AddData(vector<double> vData, int nStemBase = kStemBaseBottom);
	int		AddData(double *pData, int nLen, int nStemBase = kStemBaseBottom);

	void	SetXLabel(tstring label, int which);
	void	SetDrawCurve(bool bDraw, int whichData);

protected:
	void	DrawSinglePlotData( int which, HDC hDC, double *xRange, double *yRange );

	void	GetFrontFace(int which, int iPoint, RECT plotRect, double *xRange, double *yRange, RECT &faceRect);
	void	Get3DXFaceAdded(RECT faceRect, POINT pt[4]);
	void	Get3DYFaceAdded(RECT faceRect, POINT pt[4]);

	void	ChangeAxisData(RECT plotRect);

	RECT	GetValueRect(HDC hDC, tstring text, RECT faceRect);

public:
	void	GetPlotRange( double *xRange, double *yRange );
	int		GetNeareastPlotData( HDC hDC, POINT point );

public:
	bool	PtInDataArea( int which, HDC hDC, POINT point, RECT plotRect, double *xRange, double *yRange );
	void	SetMaxDataSetLen(int len);
};

class CStemPlotImplI :	public CStemPlotImpl,
						public CStemHandler<CStemPlotImplI>,
						public CXYReactStatus<CStemPlotImplI>
{
public:
	CStemPlotImplI();
	virtual ~CStemPlotImplI();
//	bool	ShowDlgBySelection(HDC hDC);
};

void	InitPageParaSpecial(CStemPlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CStemPlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CStemPlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CStemPlotImplI *plot, HMENU subMenu);
void	CreateTooltipPopup(CStemPlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CStemPlotImplI *plot, HWND hWnd, int result );
int		InterpretTooltipPopupItem( CStemPlotImplI *plot, HWND hWnd, int result );

Declare_Namespace_End

#endif





















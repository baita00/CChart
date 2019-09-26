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

#ifndef __PIEPLOTIMPL_H_122333444455555__
#define __PIEPLOTIMPL_H_122333444455555__

#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/XYHandler.h"
#include "PiePlot.h"
#include "PieHandler.h"

Declare_Namespace_CChart

typedef class CPieProp
{
public:
	CPieProp(){SetDefaults();}
	virtual ~CPieProp(){}

protected:
	void	SetDefaults(){SIZE mySize = {0, 0};	sizeTitleOffset = mySize;	fScale = 1.0;}
public:
	SIZE		sizeTitleOffset;
	double		fScale;
}PieProp, *pPieProp;

class	CPiePlotImpl : public CPiePlot, public CXYPlotImpl
{
public:
	CPiePlotImpl();
	virtual ~CPiePlotImpl();

	virtual	TCHAR*		GetTypeName(){return _TEXT("PiePlot");}
public:
	SIZE	m_sizeOffsetOrigin;

public:
	inline	SIZE		&GetPieTitleOffset(int which){ if(which >=0 && which < GetPlotDataCount() )return ((PieProp *)GetAbsPlotData(which)->GetExtProp())->sizeTitleOffset; else {static SIZE size = {0,0}; return size;} }
	inline	void		SetPieTitleOffset(int which, SIZE offset){ if(which >=0 && which < GetPlotDataCount() )((PieProp *)GetAbsPlotData(which)->GetExtProp())->sizeTitleOffset=offset; else return; }
	inline	double		GetPieScale(int which){ if(which >=0 && which < GetPlotDataCount() )return ((PieProp *)GetAbsPlotData(which)->GetExtProp())->fScale; else return 0; }
	inline	void		SetPieScale(int which, double scale){ if(which >=0 && which < GetPlotDataCount() ){ if(scale < 0.0 || scale >1.0)scale = 1.0; ((PieProp *)GetAbsPlotData(which)->GetExtProp())->fScale=scale;} else return; }

public:
	void	ResetApperance();
	int		AddPie(double value);
	int		AddPie(double value, COLORREF color, tstring title, double scale);

	void	SetPieTitle(int which, tstring title);
	void	SetPieTitleFromID(int dataID, tstring title);
	tstring	GetPieTitle(int which);
	tstring	GetPieTitleFromID(int dataID);

protected:
	void	DrawPiesBasic(HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total);
	void	DrawSinglePie(HDC hDC, RECT plotRect, int i, double ctx, double cty, double a, double b, double total, double curr, double &ratio);
	void	DrawSingleTit(HDC hDC, RECT plotRect, int i, double ctx, double cty, double a, double b, double total, double curr, double &ratio);

public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	void	GetPlotRange(double *xRange, double *yRange);
	int		GetNeareastPlotData( HDC hDC, POINT point );

	bool	IsPieDragMode(int dataID) { return IsDataDragMode(dataID);  }
	bool	IsPieDragMode(){ for(int i=0; i<GetPlotDataCount(); i++)if(GetAbsPlotData(i)->bDragMode)return true; return false; }
	int		GetPieDragMode(){ for(int i=0; i<GetPlotDataCount(); i++)if(GetAbsPlotData(i)->bDragMode)return i; return -1; }
	void	SetPieDragMode(int dataID, bool drag) { SetDataDragMode(dataID, drag); }
	void	UnsetPieDragMode(){ for(int i=0; i<GetPlotDataCount(); i++)GetAbsPlotData(i)->bDragMode = false; }

public:
	void	DrawPies( HDC hDC );

	virtual void	DrawPieAreas( HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total );
	void	DrawPieTitles( HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total );

	RECT	GetPieTitleRect( HDC hDC, RECT plotRect, int which, double ctx, double cty, double a1, double b1, double curr, double ratio, int &fontheight );
	RECT	GetPieTitleRect( HDC hDC, RECT plotRect, int which );

	tstring	GetRealTitle(int which);
	double	GetRatio(int which);

	bool	PtInPieArea( HDC hDC, POINT point, RECT plotRect );
	bool	PtInPieArea( int which, HDC hDC, POINT point, RECT plotRect, bool withTitle );
	bool	PtInPieArea( int which, HDC hDC, POINT point, RECT plotRect, bool withTitle, double ctx, double cty, double a1, double b1, double curr, double ratio);
	bool	PtInPieTitleArea(  int which, HDC hDC, POINT point, RECT plotRect );
	HRGN	GetPieRegion( int which, HDC hDC, RECT plotRect, double ctx, double cty, double a1, double b1, double curr, double ratio );

	bool	GetFreqPara( RECT plotRect, double &ctx, double &cty, double &a, double &b, double &total );
	bool	GetFreqPara( RECT plotRect, int which, double &ctx, double &cty, double &a, double &b, double &a1, double &b1, double &curr, double &ratio, double &total );

public:
	virtual	bool		WriteToFile( tstring pathName );
	virtual	bool		WriteToFile( int which, tstring pathName );
	virtual	bool		WriteToBuff( tstring &buff );
	virtual	bool		ReadFromBuff( tstring buff );

	int		AddEmpty(){return -1;};
private:
	int		AddData( MyVData2D vData );
};

class CPiePlotImplI :	public CPiePlotImpl,
						public CPieHandler<CPiePlotImplI>,
						public CXYReactStatus<CPiePlotImplI>
{
public:
	CPiePlotImplI();
	virtual ~CPiePlotImplI();

public:
//	bool	ShowDlgBySelection(HDC hDC);
};

void	InitPageParaSpecial(CPiePlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CPiePlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CPiePlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CPiePlotImplI *plot, HMENU subMenu);
void	CreateTooltipPopup(CPiePlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CPiePlotImplI *plot, HWND hWnd, int result );
int		InterpretTooltipPopupItem( CPiePlotImplI *plot, HWND hWnd, int result );

void	CreateDataPopup(CPiePlotImplI *plot, HMENU menu);
int		InterpretDataPopupItem( CPiePlotImplI *plot, HWND hWnd, int result );
void	CreateStatusPopup(CPiePlotImplI *plot, HMENU menu);
int		InterpretStatusPopupItem( CPiePlotImplI *plot, HWND hWnd, int result );

//void	CreateTitlePopup(CPiePlotImplI *plot, HMENU menu);
//int		InterpretTitlePopupItem( CPiePlotImplI *plot, HWND hWnd, int result );
void	AddGridPopup(CPiePlotImplI *plot, HMENU addMenu, tstring titleString, int startVal);

void	InitPageParaAutoSetting(CPiePlotImplI *plot, vPagePara &vPP);
void	InitPageParaRanges(CPiePlotImplI *plot, vPagePara &vPP);
void	InitPageParaMajorGrids(CPiePlotImplI *plot, vPagePara &vPP);
void	InitPageParaMinorGrids(CPiePlotImplI *plot, vPagePara &vPP);

Declare_Namespace_End

#endif





















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

#pragma warning(disable:4786)

#include "Plot.h"
#include "PlotBasicImpl.h"
#include "TitleImpl.h"
#include "SubtitleImpl.h"
#include "CanvasImpl.h"
#include "RangesImpl.h"
#include "GridsImpl.h"
#include "AxesImpl.h"
#include "PlotDataImpl.h"
#include "More/PageNoteImpl.h"
#include "More/CommentsImpl.h"

Declare_Namespace_CChart

class CPlotImpl	:	public CPlot,
					public CPlotBasicImpl,
					public CRangesImpl<CPlotImpl>,
					public CTitleImpl<CPlotImpl>,
					public CSubtitleImpl<CPlotImpl>,
					public CCanvasImpl<CPlotImpl>,
					public CAxesImpl<CPlotImpl>,
					public CGridsImpl<CPlotImpl>,
					public CPageNoteImpl<CPlotImpl>,
					public CCommentsImpl<CPlotImpl>
					
{
public:
	CPlotImpl();
	virtual ~CPlotImpl();

protected:
	virtual	RECT	GetPlotRect( HDC hDC, RECT clientRect );
	virtual	RECT	AddtionalPlotRectAdjust(HDC hDC, RECT plotRect){return plotRect;}
public:
	RECT	AdjustTitleRect(HDC hDC, RECT titleRect);

protected:
	RECT	m_rctClient, m_rctPlot;

public:
	virtual	void		OnDraw(  );
	virtual	void		OnDraw( HWND hWnd );
	virtual	void		OnDraw( HWND hWnd, RECT destRect );
	virtual	void		OnDraw( HDC hDC );
	virtual	void		OnDraw( HDC hDC, RECT destRect );
	virtual	void		FinalDraw( HDC hDC, RECT destRect );
	void	Update(HDC hDC, RECT destRect);

public:
	inline	RECT	GetLastClientRect(){return m_rctClient;}
	inline	RECT	GetLastPlotRect(){return m_rctPlot;}
	inline	void	SetLastClientRect(RECT rect){m_rctClient = rect;}
	inline	void	SetLastPlotRect(RECT rect){m_rctPlot = rect;}

	void		DataToLP( double *data, LPPOINT point );
	void		LPToData( LPPOINT point, double *data );
	void		DataToLPX( double *pX, long *pPtx, int nCnt);
	void		LPToDataX( long *pPtx, double *pX, int nCnt);
	void		DataToLPY( double *pY, long *pPty, int nCnt);
	void		LPToDataY( long *pPty, double *pY, int nCnt);
	
	virtual int		RegionIdentify(HDC hDC,POINT point);
	
	bool		PtInDataRegion(HDC hDC, POINT point);

	//virtual	tstring		GetDescriptionByMousePos(HDC hDC, POINT point) = 0;

	virtual	void		SetApperanceMode(int mode);

	void		ResetApperance();

};

Declare_Namespace_End
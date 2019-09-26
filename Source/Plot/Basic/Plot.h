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

#define WIN32_LEAN_AND_MEAN

#pragma warning(disable:4786)

#include "MyString.h"
#include <vector>
#include <windows.h>
#include "CChartNameSpace.h"

Declare_Namespace_CChart

#define DEFAULT_WHITE_BACKGROUND 1

enum
{
	kPopupError = -1,
	kPopupNoAction = -2,
	kPopupUpdate = -3
};

enum
{
	kBrushNull = 0,
	kBrushSolid,
	kBrushHatch,
	kBrushPattern,
		
	kBrushTypeCount
};

enum
{
	kPositionL,
	kPositionB,
	kPositionR,
	kPositionT,
	kPositionI,
	kPositionCount
};

enum
{
	kOrientationLR,
	kOrientationRL,
	kOrientationBT,
	kOrientationTB,
	kOrientationCount
};

enum
{
	kXYRegionLeft = 0,
	kXYRegionBottom = 1,
	kXYRegionRight = 2,
	kXYRegionTop = 3,
	kXYRegionTitle = 4,
	kXYRegionData = 5,
	kXYRegionOut = 6,
	kRegionCount
};

enum
{
	kSLShowEdge,
	kSLShowText,
	kSLShowCount
};

// Appearance mode
enum
{
	kExtraSmall,
	kSmall,
	kNormal,
	kLarge,
	kExtraLarge,
	
	kApperanceModeCount
};

// how to show the mouse tool tip
enum
{
	kDesCoords,
	kDesXY,
	kDesElements,
	kDesXAndYAll,
	kDesXYNearest,
	
	kDesCount
};

typedef void	(FcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara );
typedef void	(FcnUserDrawing)( void *plot, HDC hDC, RECT clientRect, RECT plotRect, void *pPara );
typedef void	(FcnUserUpdate)( void *plot, void *pPara );

struct stFcnUserUpdate
{
	FcnUserUpdate *fcn;
	void	*pPara;
};

class CPlot
{
public:
	CPlot();
	virtual ~CPlot();

protected:
	int			m_nMarginLeft, m_nMarginRight, m_nMarginBottom, m_nMarginTop;
	int			m_nMinMargin;
	COLORREF	m_crSelectColor;
	COLORREF	m_crLightColor;
	int			m_nSelectLineSize;
	int			m_nSelectLineStyle;
	int			m_nLightLineSize;
	int			m_nLightLineStyle;

	int			m_nApperanceMode;
protected:
protected:
	FcnDefScreen		*m_fcnDefScreen;
	void				*m_pParaDefScreen;
protected:
	FcnUserDrawing		*m_fcnUserDrawing;
	void				*m_pParaUserDrawing;
	
	FcnUserUpdate		*m_fcnUserUpdate;
	void				*m_pParaUserUpdate;
	
	vector<stFcnUserUpdate>	m_vFcnUserUpdate;
	
public:
	int			m_nPointDesType;
	int			m_nPointDesAccuracy;

protected:
	bool		m_bNewDataComming;
	bool		m_bFirstDraw;
	bool		m_bNothingElse;
	bool		m_bPlotSelected;
	bool		m_bPlotLighted;

	bool		m_bEqualXYAxis,m_bEqualXYScale;

	bool		m_bXOpposite, m_bYOpposite;

	bool		m_bSwitchReverse;

	bool		m_bDrawBasicAnyway;

	bool		m_bDoubleBuffer;

	tstring		m_strDefaultMain;
	tstring		m_strDefaultSub;

public:
	CRITICAL_SECTION	m_csPlot;

public:
	static	int			m_nColorsInTable;
	static	COLORREF	*m_pColorTable;
	static	void		SetColorTable(int nTableIndex);

protected:
	void	SetDefaults();

public:
	void				SetFcnUserDrawing(FcnUserDrawing func){m_fcnUserDrawing = func;}
	void				SetFcnUserDrawing(FcnUserDrawing func, void *pVoid){m_fcnUserDrawing = func;m_pParaUserDrawing = pVoid;}
	void				SetParaUserDrawing(void *pVoid){m_pParaUserDrawing = pVoid;}
	
	void				SetFcnUserUpdate(FcnUserUpdate func){m_fcnUserUpdate = func;}
	void				SetFcnUserUpdate(FcnUserUpdate func, void *pVoid){m_fcnUserUpdate = func;m_pParaUserUpdate = pVoid;}
	void				SetParaUserUpdate(void *pVoid){m_pParaUserUpdate = pVoid;}
	
	void				SetFcnDefScreen(FcnDefScreen func){m_fcnDefScreen = func;}
	void				SetFcnDefScreen(FcnDefScreen func, void *pVoid){m_fcnDefScreen = func; m_pParaDefScreen = pVoid;}
	void				SetParaDefScreen(void *pVoid){m_pParaDefScreen = pVoid;}

	vector<stFcnUserUpdate>	&GetVFcnUserUpdate(){return m_vFcnUserUpdate;}

public:
	inline	int				GetMarginLeft(){ return m_nMarginLeft; }
	inline	void			SetMarginLeft(int margin){ m_nMarginLeft = margin; }
	inline	int				GetMarginRight(){ return m_nMarginRight; }
	inline	void			SetMarginRight(int margin){ m_nMarginRight = margin; }
	inline	int				GetMarginTop(){ return m_nMarginTop; }
	inline	void			SetMarginTop(int margin){ m_nMarginTop = margin; }
	inline	int				GetMarginBottom(){ return m_nMarginBottom; }
	inline	void			SetMarginBottom(int margin){ m_nMarginBottom = margin; }
	inline	void			GetMargin(int &left, int &right, int &top, int &bottom){ left = m_nMarginLeft; right = m_nMarginRight; top = m_nMarginTop; bottom = m_nMarginBottom; }
	inline	void			SetMargin(int left, int right, int top, int bottom){ m_nMarginLeft = left; m_nMarginRight = right; m_nMarginTop = top; m_nMarginBottom = bottom; }
	inline	int				GetMinMargin(){ return m_nMinMargin; }
	inline	void			SetMinMargin(int margin){ m_nMinMargin = margin; }

	inline	COLORREF		GetSelectColor() { return m_crSelectColor; }
	inline	void			SetSelectColor( COLORREF color ) { m_crSelectColor = color; }
	inline	COLORREF		GetLightColor( void ) { return m_crLightColor; }
	inline	void			SetLightColor( COLORREF color ) { m_crLightColor = color; }
	inline	int				GetSelectLineSize( void ) { return m_nSelectLineSize; }
	inline	void			SetSelectLineSize( int size ) { m_nSelectLineSize = size; }
	inline	int				GetSelectLineStyle(){return m_nSelectLineStyle;}
	inline	void			SetSelectLineStyle(int style){m_nSelectLineStyle = style;}
	inline	int				GetLightLineSize( void ) { return m_nLightLineSize; }
	inline	void			SetLightLineSize( int size ) { m_nLightLineSize = size; }
	inline	int				GetLigntLineStyle(){return m_nLightLineStyle;}
	inline	void			SetLightLineStyle(int style){m_nLightLineStyle = style;}

	inline	bool			IsNewDataComming(){return m_bNewDataComming;}
	inline	void			SetNewDataComming( bool nd ){ m_bNewDataComming = nd; }
	inline	bool			IsFirstDraw(){return m_bFirstDraw;}
	inline	void			SetFirstDraw( bool fd ){ m_bFirstDraw = fd; }
	inline	bool			IsNothingElse(){return m_bNothingElse;}
	inline	void			SetNothingElse( bool ne ){ m_bNothingElse = ne; }
	inline	bool			IsPlotSelected(){return m_bPlotSelected;}
	inline	void			SetPlotSelected( bool bSel){m_bPlotSelected = bSel;}
	inline	bool			IsPlotLighted(){return m_bPlotLighted;}
	inline	void			SetPlotLighted( bool bLight){m_bPlotLighted = bLight;}

	inline	bool			IsEqualXYAxis(){ return m_bEqualXYAxis; }
	inline	void			SetEqualXYAxis( bool equal ){ m_bEqualXYAxis=equal; }
	inline	bool			IsEqualXYScale(){ return m_bEqualXYScale; }
	inline	void			SetEqualXYScale( bool equal ){ m_bEqualXYScale=equal; }

	inline	bool			IsXOpposite(){ return m_bXOpposite; }
	inline	void			SetXOpposite( bool opp ){ m_bXOpposite=opp; }
	inline	bool			IsYOpposite(){ return m_bYOpposite; }
	inline	void			SetYOpposite( bool opp ){ m_bYOpposite=opp; }

	inline	int				GetPointDesType() { return m_nPointDesType; }
	inline	void			SetPointDesType( int type ) { m_nPointDesType = type; }
	inline	int				GetPointDesAccuracy() { return m_nPointDesAccuracy; }
	inline	void			SetPointDesAccuracy( int accur ) { m_nPointDesAccuracy = accur; }

	inline	bool			IsSwitchReverse(){return m_bSwitchReverse;}
	inline	void			SetSwitchReverse(bool reverse){m_bSwitchReverse = reverse;}

	inline bool				IsDrawBasicAnyway( void ) { return m_bDrawBasicAnyway; }
	inline void				SetDrawBasicAnyway( bool draw ) { m_bDrawBasicAnyway = draw; }

	inline bool				IsDoubleBuffer() { return m_bDoubleBuffer;}
	inline void				SetDoubleBuffer( bool db ) { m_bDoubleBuffer=db; }

	inline	tstring			GetDefaultMain( void ) { return m_strDefaultMain; }
	inline	void			SetDefaultMain( tstring str) { m_strDefaultMain = str; }
	inline	tstring			GetDefaultSub( void ) { return m_strDefaultSub; }
	inline	void			SetDefaultSub( tstring str) { m_strDefaultSub = str; }
	
public:
	static	int			ChooseLineWidth(int linewidth, bool bLighted, bool bSelected);
	static	COLORREF	ChooseLineColor(COLORREF linecolor, bool bLighted, bool bSelected);	

public:
	int					GetApperanceMode();
	virtual	void		SetApperanceMode(int mode);

	virtual	bool		IsNoDataPlot() = 0;

	virtual TCHAR*		GetTypeName() = 0;
};

Declare_Namespace_End
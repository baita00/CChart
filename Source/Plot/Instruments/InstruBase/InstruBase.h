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
#include <math.h>
#include "../../Basic/MyString.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyConst.h"
#include "../../Basic/Plot.h"
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

enum
{
	kInstruShapeHorizontal,
	kInstruShapeVertical,
	kInstruShapeCircle,
	kInstruShapeWheel,
	kInstruShapeCount
};

// Axis title position
enum
{
	kInstruPositionWideSmall = 0,
	kInstruPositionWideLarge,
	kInstruPositionNarrowSmall,
	kInstruPositionNarrowLarge,
		
	kInstruPositionCount
};

// Tick position
enum
{
	kInstruPosInner = 0,
	kInstruPosOuter,
	kInstruPosBoth,
	
	kInstruPosCount
};

enum
{
	kShowLabelEdgeStart = 0,
	kShowLabelEdgeEnd,
	kShowLabelEdgeBoth,

	kShowLabelEdgeCount
};

enum
{
	kPointerLine,
	kPointerSingleBar,
	kPointerDualBar,
	kPointerSingleSword,
	kPointerDualSword,
	kPointerArrow,
	kPointerReverseArrow,
	kPointerOuterTriangle,
	kPointerFillArc,
		
	kPointerShapeCount
};

typedef struct stAxisSegColors
{
	int nTicks;
	COLORREF color;
}AxisSegColors, *pAxisSegColors;
typedef vector<AxisSegColors> vAxisSegColors;

class CInstruBase
{
public:
	CInstruBase();
	virtual	~CInstruBase();

	void	SetDefaults();

protected:
	double		m_fCurrValue;		// Current value
	COLORREF	m_crCurrColor;
	bool		m_bFillCurr;

protected:
	int			m_nInstruShape;

	int			m_nTickCount;	// number of tick marks
	int			m_nMinorTickCount;	// number of minor ticks

	int			m_nAxisPosition;
	bool		m_bOpposite;
	
	int			m_nTickLength, m_nSubTickLength, m_nMinorTickLength;	// length of tick marks (logical units)
	int			m_nTickPosition, m_nMinorTickPosition;	// position of the ticks
	bool		m_bTickBar, m_bMinorTickBar;
	int			m_nTickBarHalfWidthBase, m_nMinorTickBarHalfWidthBase;
	int			m_nTickBarHalfWidthEnd, m_nMinorTickBarHalfWidthEnd;
	int			m_nTickSize, m_nSubTickSize, m_nMinorTickSize; // thickness of the ticks
	COLORREF	m_crTickColor, m_crSubTickColor, m_crMinorTickColor; // color of the ticks
	int			m_nLineSize;		// Size of the line
	int			m_nLineStyle;		// style of the line;
	double		m_pfRange[2];		// range of the axis
	tstring		m_strTitle;		// axis title
	bool		m_bShowTitle;	// title visible
	bool		m_bTitleAsValue;
	SIZE		m_sizeTitleOffset;
	LOGFONT		m_lfTitleFont;	// font to use for title
	LOGFONT		m_lfLabelFont;	// font to use for axis labels
	COLORREF	m_crAxisColor;		// color to use for axis and font
	COLORREF	m_crTitleColor;	// color to use for title
	//bool		m_bRangeSet;		// has the range been set?
	
	int			m_nBrushType;
	int			m_nHatchStyle;
	COLORREF	m_crBkColor;
	COLORREF	m_crHatchColor;
	HBITMAP		m_hPattern;
	bool		m_bShowBorder;
	int			m_nBorderSize;
	int			m_nBorderStyle;
	COLORREF	m_crBorderColor;
	bool		m_bBorder3D;
	int			m_nBorder3DEdge;
	bool		m_bEraseBkgnd;
	
	bool		m_bShowTickLabel;
	COLORREF	m_crTickLabelColor;
	int			m_nMarginHorizontal,m_nMarginVertical;
	
	int			m_nTitlePosition;
	tstring		m_strUnit;
	bool		m_bShowUnit;

	int			m_nBarWidth;
	int			m_nBarMargin;
	int			m_nBarBorderSize;
	COLORREF	m_crBarBorderColor;
	COLORREF	m_crBarColor;
	bool		m_bFillBar;
	bool		m_bBarBorder3D;
	bool		m_bShowBar;

	bool		m_bShowAxis;
	bool		m_bShowAxisLine;
	bool		m_bShowMajorTicks;
	bool		m_bShowSubMajorTicks;
	bool		m_bShowMinorTicks;

	bool		m_bHideIdleArc;

	int			m_nEdgeGap;
	double		m_fAngleStart;
	double		m_fAngleRange;

	bool		m_bClockWise;
	int			m_nLabelEdgeType;
	int			m_nLabelPosition;

	bool		m_bFillWholeCircle;
	COLORREF	m_crWholeCircleColor;

	bool		m_bReplaceLabel;
	vector<tstring>	m_vstrAlternateLabels;

protected:
	bool		m_bShowStaticPointer;
	int			m_nStaticPointerIndex;
	int			m_nStaticPointerWithdraw;
	int			m_nStaticPointerBaseHalfWidth;
	COLORREF	m_crStaticPointerColor1;
	COLORREF	m_crStaticPointerColor2;
	int			m_nStaticPointerAdditionalWithdraw;
	
	bool		m_bShowPointer;
	int			m_nPointerShape;
	int			m_nPointerLineSize;
	int			m_nPointerWithdraw;
	COLORREF	m_crPointerColor1;
	COLORREF	m_crPointerColor2;
	bool		m_bShowStick;
	COLORREF	m_crStickColor;
	int			m_nStickRadius;
	int			m_nPointerHalfWidthBase;
	int			m_nPointerHalfWidthEnd;
	int			m_nPointerSwordLenEnd;
	bool		m_bPointerShowSwordHandle;
	int			m_nPointerSwordHandleRadiusInc;
	int			m_nPointerArrowDisplaceBase;
	int			m_nPointerArrowDistFromCenter;
	bool		m_bFillPointerCircle;
	COLORREF	m_crFillPointerCircleColor;
	bool		m_bPointerCircle3D;
	int			m_nPointerHalfWidthEndTail;
	int			m_nPointerLenTail;
	bool		m_bShowPointerTail;
	
	bool		m_bAxisColorful;
	vAxisSegColors	m_vAxisSegColors;

protected:
	bool		m_bWheelHorizontal;
	int			m_nWheelThick;
	int			m_nWheelEdges;
	double		m_fWheelEdgeRatio;
	double		m_fWheelWindowAngle;
	double		m_fCurrSizeAngle;

	bool		m_bLogarithm;
	
public:
	inline	double		GetCurrValue() { if(!m_bLogarithm)return m_fCurrValue; else return pow(10, m_fCurrValue);}
	inline	void		SetCurrValue(double val) { if(!m_bLogarithm)m_fCurrValue = val; else m_fCurrValue = log10(val);}
	inline	COLORREF	GetCurrColor() { return m_crCurrColor; }
	inline	void		SetCurrColor( COLORREF color ) { m_crCurrColor = color; }
	inline	bool		IsFillCurr(){return m_bFillCurr;}
	inline	void		SetFillCurr(bool fill){m_bFillCurr = fill;}

	inline	double		GetAxisPosition() { return m_nAxisPosition; }
	inline	void		SetAxisPosition(int pos) { m_nAxisPosition = pos; }

	inline	bool		IsOpposite() { return m_bOpposite; }
	inline	void		SetOpposite(bool opposite) { m_bOpposite = opposite; }

	// some inline functions
	inline	int			GetInstruShape( void ) { return m_nInstruShape; }
	inline	void		SetInstruShape( int shape ) { m_nInstruShape = shape; }
	inline	int			GetTickCount( void ) { return m_nTickCount; }
	inline	void		SetTickCount( int tickCount ) { m_nTickCount = tickCount; }
	inline	int			GetMinorTickCount( void ) { return m_nMinorTickCount; }
	inline	void		SetMinorTickCount( int tickCount ) { m_nMinorTickCount = tickCount; }
	inline	int			GetTickLength( void ) { return m_nTickLength; }
	inline	void		SetTickLength( int length ) { m_nTickLength=length; }
	inline	int			GetSubTickLength( void ) { return m_nSubTickLength; }
	inline	void		SetSubTickLength( int length ) { m_nSubTickLength=length; }
	inline	int			GetMinorTickLength( void ) { return m_nMinorTickLength; }
	inline	void		SetMinorTickLength( int length ) { m_nMinorTickLength=length; }
	inline	bool		IsTickBar( void ) { return m_bTickBar; }
	inline	void		SetTickBar( bool bar) { m_bTickBar = bar; }
	inline	int			GetTickBarHalfWidthBase( void ) { return m_nTickBarHalfWidthBase; }
	inline	void		SetTickBarHalfWidthBase( int width ) { m_nTickBarHalfWidthBase = width; }
	inline	int			GetTickBarHalfWidthEnd( void ) { return m_nTickBarHalfWidthEnd; }
	inline	void		SetTickBarHalfWidthEnd( int width ) { m_nTickBarHalfWidthEnd = width; }
	inline	bool		IsMinorTickBar( void ) { return m_bMinorTickBar; }
	inline	void		SetMinorTickBar( bool bar) { m_bMinorTickBar = bar; }
	inline	int			GetMinorTickBarHalfWidthBase( void ) { return m_nMinorTickBarHalfWidthBase; }
	inline	void		SetMinorTickBarHalfWidthBase( int width ) { m_nMinorTickBarHalfWidthBase = width; }
	inline	int			GetMinorTickBarHalfWidthEnd( void ) { return m_nMinorTickBarHalfWidthEnd; }
	inline	void		SetMinorTickBarHalfWidthEnd( int width ) { m_nMinorTickBarHalfWidthEnd = width; }
	inline	int			GetTickPosition( void ) { return m_nTickPosition; }
	inline	void		SetTickPosition( int position ) { m_nTickPosition=position; }
	inline	int			GetMinorTickPosition( void ) { return m_nMinorTickPosition; }
	inline	void		SetMinorTickPosition( int position ) { m_nMinorTickPosition=position; }
	inline	int			GetTickSize( void ) { return m_nTickSize; }
	inline	void		SetTickSize( int Size ) { m_nTickSize=Size; }
	inline	int			GetSubTickSize( void ) { return m_nSubTickSize; }
	inline	void		SetSubTickSize( int Size ) { m_nSubTickSize=Size; }
	inline	int			GetMinorTickSize( void ) { return m_nMinorTickSize; }
	inline	void		SetMinorTickSize( int size ) { m_nMinorTickSize=size; }
	inline	COLORREF	GetTickColor( void ) { return m_crTickColor; }
	inline	void		SetTickColor( COLORREF color ) { m_crTickColor=color; }
	inline	COLORREF	GetSubTickColor( void ) { return m_crSubTickColor; }
	inline	void		SetSubTickColor( COLORREF color ) { m_crSubTickColor=color; }
	inline	COLORREF	GetMinorTickColor( void ) { return m_crMinorTickColor; }
	inline	void		SetMinorTickColor( COLORREF color ) { m_crMinorTickColor=color; }

	inline	int			GetBrushType( void ){ return m_nBrushType; }
	inline	void		SetBrushType( int type ){ m_nBrushType = type; }
	inline	COLORREF	GetBkColor( void ){ return m_crBkColor; }
	inline	void		SetBkColor( COLORREF color ){ m_crBkColor = color; }
	inline	int			GetHatchStyle( void ){ return m_nHatchStyle; }
	inline	void		SetHatchStyle( int style ){ m_nHatchStyle = style; }
	inline	COLORREF	GetHatchColor( void ){ return m_crHatchColor; }
	inline	void		SetHatchColor( COLORREF color ){ m_crHatchColor = color; }
	inline	HBITMAP		GetPattern( void ){ return m_hPattern; }
	inline	void		SetPattern( HBITMAP pattern ){ if(m_hPattern)DeleteObject(m_hPattern); m_hPattern = pattern; }
	inline	bool		IsShowBorder( void ){ return m_bShowBorder; }
	inline	void		SetShowBorder( bool show ){ m_bShowBorder = show; }
	inline	int			GetBorderSize( void ){ return m_nBorderSize; }
	inline	void		SetBorderSize( int size ){ m_nBorderSize = size; }
	inline	int			GetBorderStyle( void ){ return m_nBorderStyle; }
	inline	void		SetBorderStyle( int size ){ m_nBorderStyle = size; }
	inline	COLORREF	GetBorderColor( void ) { return m_crBorderColor; }
	inline	void		SetBorderColor( COLORREF color ) { m_crBorderColor=color; }
	inline	bool		IsBorder3D( void ) { return m_bBorder3D; }
	inline	void		SetBorder3D( bool td ) { m_bBorder3D = td; }
	inline	int			GetBorder3DEdge( void ) { return m_nBorder3DEdge; }
	inline	void		SetBorder3dEdge( int edge ) { m_nBorder3DEdge = edge; }
	inline	bool		IsEraseBkgnd( void ) { return m_bEraseBkgnd; }
	inline	void		SetEraseBkgnd( bool erase ) { m_bEraseBkgnd = erase; }

	inline	bool		IsShowTitle( void ) { return m_bShowTitle; }
	inline	void		SetShowTitle( bool show ) { m_bShowTitle=show; }
	inline	bool		IsTitleAsValue( void ) { return m_bTitleAsValue; }
	inline	void		SetTitleAsValue( bool as ) { m_bTitleAsValue=as; }
	inline	tstring		GetTitle() { return m_strTitle; }
	inline	void		SetTitle( tstring title ) { m_strTitle = title; }
	inline	SIZE		GetTitltOffset() { return m_sizeTitleOffset; }
	inline	void		SetTitleOffset(SIZE offset) { m_sizeTitleOffset = offset; }
	inline	void		SetTitleOffset(int dx, int dy) { m_sizeTitleOffset.cx = dx; m_sizeTitleOffset.cy = dy; }
	inline	LOGFONT		&GetTitleFont() { return m_lfTitleFont; }
	inline	void		SetTitleFont( LOGFONT font)	{ m_lfTitleFont=font; }
	inline	LOGFONT		&GetLabelFont() { return m_lfLabelFont; }
	inline	void		SetLabelFont( LOGFONT font)	{ m_lfLabelFont=font; }
	inline	COLORREF	GetTitleColor( void ) { return m_crTitleColor; }
	inline	void		SetTitleColor( COLORREF color ) {m_crTitleColor = color; }
	inline	COLORREF	GetTickLabelColor( void ) { return m_crTickLabelColor; }
	inline	void		SetTickLabelColor( COLORREF color ) { m_crTickLabelColor=color; }
	inline	bool		IsShowTickLabel( void ) { return m_bShowTickLabel; }
	inline	void		SetShowTickLabel( bool show ) { m_bShowTickLabel=show; }

	inline	int			GetMarginHorizontal( void ) { return m_nMarginHorizontal; }
	inline	void		SetMarginHorizontal( int margin ) { m_nMarginHorizontal = margin; }
	inline	int			GetMarginVertical( void ) { return m_nMarginHorizontal; }
	inline	void		SetMarginVertical( int margin ) { m_nMarginHorizontal = margin; }

	inline	COLORREF	GetAxisColor( void ) { return m_crAxisColor; }
	inline	void		SetAxisColor( COLORREF color ) { m_crAxisColor = color; }
	inline	int			GetLineSize( void ) { return m_nLineSize; }
	inline	void		SetLineSize( int size ) { if( size > 0 ) m_nLineSize = size; }
	inline	int			GetLineStyle( void ) { return m_nLineStyle; }
	inline	void		SetLineStyle( int style ) { if( style > 0 ) m_nLineStyle = style; }

	//inline	bool		IsRangeSet( void ) { return m_bRangeSet; }
	//inline	void		SetRangeSet( bool set ) { m_bRangeSet=set; }

	inline	int			GetTitlePosition( void ) { return m_nTitlePosition; }
	inline	void		SetTitlePosition( int pos ) { m_nTitlePosition=pos; }
	inline	tstring		GetUnit( void ) { return m_strUnit; }
	inline	void		SetUnit( tstring unit ) { m_strUnit = unit; }
	inline	bool		InShowUnit( void ) { return m_bShowUnit; }
	inline	void		SetShowUnit( bool show ) { m_bShowUnit = show; }

	inline	void		GetRange( double *range )
	{
		if(!m_bLogarithm)
			memcpy( range, m_pfRange, 2 * sizeof( double ) );
		else
		{
			range[0] = pow(10, m_pfRange[0]);
			range[1] = pow(10, m_pfRange[1]);
		}
	}
	inline	double		GetLowerRange( void ) { return m_bLogarithm?pow(10,m_pfRange[0]):m_pfRange[0]; }
	inline	double		GetUpperRange( void ) { return m_bLogarithm?pow(10,m_pfRange[1]):m_pfRange[1]; }
	inline	void		SetRange( double low, double high )
	{
		SetRangeOnly(low, high);
		double outRange[2];
		GetOptimalRangeExact(m_pfRange, outRange, &m_nTickCount, &m_nMinorTickCount);
	}
	inline	void		SetRangeOnly( double low, double high )
	{
		if(!m_bLogarithm)
		{
			m_pfRange[0] = low;
			m_pfRange[1] = high;
		}
		else
		{
			m_pfRange[0] = log10(low);
			m_pfRange[1] = log10(high);
		}
	}
	inline	void		SetRange(double *range){SetRange(range[0], range[1]);}
	inline	void		SetRangeOnly(double *range){SetRangeOnly(range[0], range[1]);}


	inline	int			GetBarWidth(){return m_nBarWidth;}
	inline	void		SetBarWidth(int width){m_nBarWidth = width;}
	inline	int			GetBarMargin(){return m_nBarMargin;}
	inline	void		SetBarMargin(int width){m_nBarMargin = width;}
	inline	int			GetBarBorderSize(){return m_nBarBorderSize;}
	inline	void		SetBarBorderSize(int size){m_nBarBorderSize = size;}
	inline	COLORREF	GetBarBorderColor(){return m_crBarBorderColor;}
	inline	void		SetBarBorderColor(COLORREF color){m_crBarBorderColor = color;}
	inline	COLORREF	GetBarColor(){return m_crBarColor;}
	inline	void		SetBarColor(COLORREF color){m_crBarColor = color;}
	inline	bool		IsFillBar( void ) { return m_bFillBar; }
	inline	void		SetFillBar( bool fill ) { m_bFillBar = fill; }
	inline	bool		IsBarBorder3D( void ) { return m_bBarBorder3D; }
	inline	void		SetBarBorder3D( bool td ) { m_bBarBorder3D = td; }
	inline	bool		IsShowBar( void ) { return m_bShowBar; }
	inline	void		SetShowBar( bool show ) { m_bShowBar = show; }

	inline	bool		IsShowAxis(){return m_bShowAxis;}
	inline	void		SetShowAxis(bool show){m_bShowAxis = show;}
	inline	bool		IsShowAxisLine(){return m_bShowAxisLine;}
	inline	void		SetShowAxisLine(bool show){m_bShowAxisLine = show;}
	inline	bool		IsShowMajorTicks(){return m_bShowMajorTicks;}
	inline	void		SetShowMajorTicks(bool show){m_bShowMajorTicks = show;}
	inline	bool		IsShowSubMajorTicks(){return m_bShowSubMajorTicks;}
	inline	void		SetShowSubMajorTicks(bool show){m_bShowSubMajorTicks = show;}
	inline	bool		IsShowMinorTicks(){return m_bShowMinorTicks;}
	inline	void		SetShowMinorTicks(bool show){m_bShowMinorTicks = show;}

	inline	bool		IsHideIdleArc(){return m_bHideIdleArc;}
	inline	void		SetHideIdleArc(bool hide){m_bHideIdleArc = hide;}

	inline	int			GetEdgeGap(){return m_nEdgeGap;}
	inline	void		SetEdgeGap(int gap){m_nEdgeGap = gap;}
	inline	double		GetAngleStart(){return m_fAngleStart;}
	inline	void		SetAngleStart(double angle){m_fAngleStart = angle;}
	inline	double		GetAngleRange(){return m_fAngleRange;}
	inline	void		SetAngleRange(double angle){m_fAngleRange = angle;}

	inline	bool		IsClockWise(){return m_bClockWise;}
	inline	void		SetClockWise(bool clockwise){m_bClockWise = clockwise;}
	inline	int			GetLabelEdgeType(){return m_nLabelEdgeType;}
	inline	void		SetLabelEdgeType(int type){m_nLabelEdgeType = type;}
	inline	int			GetLabelPosition(){return m_nLabelPosition;}
	inline	void		SetLabelPosition(int pos){m_nLabelPosition = pos;}
	inline	bool		IsFillWholeCircle(){return m_bFillWholeCircle;}
	inline	void		SetFillWholeCircle(bool fill){m_bFillWholeCircle = fill;}
	inline	COLORREF	GetWholeCircleColor(){return m_crWholeCircleColor;}
	inline	void		SetWholeCircleColor(COLORREF color){m_crWholeCircleColor = color;}

	inline bool			IsReplaceLabel() { return m_bReplaceLabel; }
	inline void			SetReplaceLabel( bool replace ) { m_bReplaceLabel = replace; }

	inline vector<tstring>	&GetAlternateLabels() { return m_vstrAlternateLabels; }

public:
	inline	bool		IsShowStaticPointer() { return m_bShowStaticPointer; }
	inline	void		SetShowStaticPointer( bool show ) { m_bShowStaticPointer = show; }
	inline	int			GetStaticPointerIndex() { return m_nStaticPointerIndex; }
	inline	void		SetStaticPointerIndex( int index ) { m_nStaticPointerIndex = index; }
	inline	int			GetStaticPointerWithdraw() { return m_nStaticPointerWithdraw; }
	inline	void		SetStaticPointerWithdraw( int withdraw ) { m_nStaticPointerWithdraw = withdraw; }
	inline	int			GetStaticPointerBaseHalfWidth() { return m_nStaticPointerBaseHalfWidth; }
	inline	void		SetStaticPointerBaseHalfWidth( int width ) { m_nStaticPointerBaseHalfWidth = width; }
	inline	COLORREF	GetStaticPointerColor1() { return m_crStaticPointerColor1; }
	inline	void		SetStaticPointerColor1(COLORREF color) { m_crStaticPointerColor1 = color; }
	inline	COLORREF	GetStaticPointerColor2() { return m_crStaticPointerColor2; }
	inline	void		SetStaticPointerColor2(COLORREF color) { m_crStaticPointerColor2 = color; }
	inline	int			GetStaticPointerAdditionalWithdraw() { return m_nStaticPointerAdditionalWithdraw; }
	inline	void		SetStaticPointerAdditionalWithdraw( int withdraw ) { m_nStaticPointerAdditionalWithdraw = withdraw; }
	
	inline	bool		IsShowPointer() { return m_bShowPointer; }
	inline	void		SetShowPointer( bool show ) { m_bShowPointer = show; }
	inline	int			GetPointerShape() { return m_nPointerShape; }
	inline	void		SetPointerShape( int shape ) { m_nPointerShape = shape; }
	inline	int			GetPointerLineSize() { return m_nPointerLineSize; }
	inline	void		SetPointerLineSize( int size ) { m_nPointerLineSize = size; }
	inline	int			GetPointerWithdraw() { return m_nPointerWithdraw; }
	inline	void		SetPointerWithdraw( int withdraw ) { m_nPointerWithdraw = withdraw; }
	inline	COLORREF	GetPointerColor() { return m_crPointerColor1; }
	inline	void		SetPointerColor(COLORREF color) { m_crPointerColor1 = color; }
	inline	COLORREF	GetPointerColor1() { return m_crPointerColor1; }
	inline	void		SetPointerColor1(COLORREF color) { m_crPointerColor1 = color; }
	inline	COLORREF	GetPointerColor2() { return m_crPointerColor2; }
	inline	void		SetPointerColor2(COLORREF color) { m_crPointerColor2 = color; }
	inline	bool		IsShowStick() { return m_bShowStick; }
	inline	void		SetShowStick(bool show){ m_bShowStick = show; }
	inline	COLORREF	GetStickColor() { return m_crStickColor; }
	inline	void		SetStickColor(COLORREF color) { m_crStickColor = color; }
	inline	int			GetStickRadius() { return m_nStickRadius; }
	inline	void		SetStickRadius( int radius ) { m_nStickRadius = radius; }
	inline	int			GetPointerHalfWidthBase() { return m_nPointerHalfWidthBase; }
	inline	void		SetPointerHalfWidthBase( int width ) { m_nPointerHalfWidthBase = width; }
	inline	int			GetPointerHalfWidthEnd() { return m_nPointerHalfWidthEnd; }
	inline	void		SetPointerHalfWidthEnd( int width ) { m_nPointerHalfWidthEnd = width; }
	inline	int			GetPointerSwordLenEnd() { return m_nPointerSwordLenEnd; }
	inline	void		SetPointerSwordLenEnd( int len ) { m_nPointerSwordLenEnd = len; }
	inline	bool		IsPointerShowSwordHandle() { return m_bPointerShowSwordHandle; }
	inline	void		SetPointerShowSwordHandle( bool show ) { m_bPointerShowSwordHandle = show; }
	inline	int			GetPointerSwordHandleRadiusInc() { return m_nPointerSwordHandleRadiusInc; }
	inline	void		SetPointerSwordHandleRadiusInc( int radius ) { m_nPointerSwordHandleRadiusInc = radius; }
	inline	int			GetPointerArrowDisplaceBase() { return m_nPointerArrowDisplaceBase; }
	inline	void		SetPointerArrowDisplaceBase( int disp ) { m_nPointerArrowDisplaceBase = disp; }
	inline	int			GetPointerArrowDistFromCenter() { return m_nPointerArrowDistFromCenter; }
	inline	void		SetPointerArrowDistFromCenter( int dist ) { m_nPointerArrowDistFromCenter = dist; }
	inline	bool		IsFillPointerCircle() { return m_bFillPointerCircle; }
	inline	void		SetFillPointerCircle( bool fill ) { m_bFillPointerCircle = fill; }
	inline	COLORREF	GetFillPointerCircleColor() { return m_crFillPointerCircleColor; }
	inline	void		SetFillPointerCircleColor(COLORREF color) { m_crFillPointerCircleColor = color; }
	inline	bool		IsPointerCircle3D() { return m_bPointerCircle3D; }
	inline	void		SetPointerCircle3D( bool td ) { m_bPointerCircle3D = td; }
	inline	int			GetPointerHalfWidthEndTail() { return m_nPointerHalfWidthEndTail; }
	inline	void		SetPointerHalfWidthEndTail( int hw ) { m_nPointerHalfWidthEndTail = hw; }
	inline	int			GetPointerLenTail() { return m_nPointerLenTail; }
	inline	void		SetPointerLenTail( int hw ) { m_nPointerLenTail = hw; }
	inline	bool		IsShowPointerTail() { return m_bShowPointerTail; }
	inline	void		SetShowPointerTail( bool show ) { m_bShowPointerTail = show; }
	inline	bool		IsAxisColorful() { return m_bAxisColorful; }
	inline	void		SetAxisColorful( bool colorful ) { m_bAxisColorful = colorful; }
	inline	vAxisSegColors	&GetAxisSegColors(){return m_vAxisSegColors;}

	inline	bool		IsWheelHorizontal() { return m_bWheelHorizontal; }
	inline	void		SetWheelHorizontal( bool td ) { m_bWheelHorizontal = td; }
	inline	int			GetWheelThick() { return m_nWheelThick; }
	inline	void		SetWheelThick( int thick ) { m_nWheelThick = thick; }
	inline	int			GetWheelEdges() { return m_nWheelEdges; }
	inline	void		SetWheelEdges( int gap ) { m_nWheelEdges = gap; }
	inline	double		GetWheelEdgeRatio() { return m_fWheelEdgeRatio; }
	inline	void		SetWheelEdgeRatio( double ratio ) { m_fWheelEdgeRatio = ratio; }
	inline	double		GetWheelWindowAngle() { return m_fWheelWindowAngle; }
	inline	void		SetWheelWindowAngle( double angle ) { m_fWheelWindowAngle = angle; }
	inline	double		GetCurrSizeAngle() { return m_fCurrSizeAngle; }
	inline	void		SetCurrSizeAngle(double size) { m_fCurrSizeAngle = size; }

	inline	bool		IsLogarithm(){return m_bLogarithm;}
	inline	void		SetLogarithm( bool log )
	{
		m_bLogarithm = log;
		if(log)
		{
			m_pfRange[0] = log10(m_pfRange[0]);
			m_pfRange[1] = log10(m_pfRange[1]);

			m_fCurrValue = log10(m_fCurrValue);
		}
		else
		{
			m_pfRange[0] = pow(10, m_pfRange[0]);
			m_pfRange[1] = pow(10, m_pfRange[1]);

			m_fCurrValue = log10(m_fCurrValue);
		}
	}

protected:
	void				DeleteUsedHandles();
	tstring				GetRealTitle();
	void				InitAxisColors();
	COLORREF			GetAxisSegColor(int nIndex);
};

Declare_Namespace_End


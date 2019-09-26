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
#pragma warning(disable:4786)
#include <vector>
#include <map>
#include "PlotUtility.h"
#include "MyString.h"
using namespace std;

Declare_Namespace_CChart

// Axis locations
enum
{
	kLocationLeft = 0,
	kLocationBottom,
	kLocationRight,
	kLocationTop,
	kLocationCenterVL,
	kLocationCenterVR,
	kLocationCenterHB,
	kLocationCenterHT,
	
	kLocation2DCount,
	
	kLocationTDX = kLocation2DCount,
	kLocationTDY,
	kLocationTDZ,
	
	kLocationCount
};

// Tick position
enum
{
	kTickPositionNone = 0,
	kTickPositionInner,
	kTickPositionOuter,
	kTickPositionBoth,
	
	kTickPositionCount
};

typedef struct stDataRange
{
	double val[2];
}DataRange, *pDataRange;


class CAxis
{
public:
	CAxis();
	virtual ~CAxis();

protected:
	void	SetDefaults();

protected:
	int			m_nApperanceMode;
		
	int			m_nTickLength, m_nMinorTickLength;	// length of tick marks (logical units)
	int			m_nTickPosition, m_nMinorTickPosition;	// position of the ticks
	int			m_nTickSize, m_nMinorTickSize; // thickness of the ticks
	COLORREF	m_crTickColor, m_crMinorTickColor; // color of the ticks
	int			m_nLineSize;		// Size of the line
	int			m_nLineStyle;		// style of the line;

	bool		m_bVisible;		// is axis visible?
	bool		m_bShowAxisLine;// the line
	tstring		m_strTitle;		// axis title
	bool		m_bShowTitle;	// title visible
	int			m_nLocation;		// location of axis (left, bottom, top, etc.)
	int			m_nTitleGap;	// Gap of title and labels
	LOGFONT		m_lfTitleFont;	// font to use for title
	LOGFONT		m_lfLabelFont;	// font to use for axis labels
	COLORREF	m_crAxisColor;		// color to use for axis and font
	COLORREF	m_crTitleColor;	// color to use for title
	bool		m_bShowTitleRect;
	int			m_nTitleRectLineSize;
	int			m_nTitleRectLineStyle;
//	bool		m_bRangeSet;		// has the range been set?
//	bool		m_bAutoRange;
	int			m_nAxisGap;
	int			m_nLabelGap;
	bool		m_bDespiteLow;
	bool		m_bDespiteHigh;
	
	int			m_nBrushType;
	int			m_nHatchStyle;
	COLORREF	m_crBgColor;
	COLORREF	m_crHatchColor;
	HBITMAP		m_hPattern;
	bool		m_bBorderShow;
	int			m_nBorderSize;
	
	bool		m_bShowMajorTicks;
	bool		m_bShowMinorTicks;
	
	bool		m_bArrowStart;
	bool		m_bArrowEnd;
	int			m_nArrowSize;
	int			m_nArrowRatio;
	bool		m_bArrowFlatBottom;
	bool		m_bArrowReverse;
	
	bool		m_bTickLabelSEOnly;
	bool		m_bTickLabelShow;
	COLORREF	m_crTickLabelColor;
	int			m_nMarginHorizontal,m_nMarginVertical;
	
	bool		m_bSameColor;
	
	bool		m_bLighted;
	bool		m_bSelected;
	
	int			m_nTitlePosition;
	SIZE		m_sizeTitleOffset;
	SIZE		m_sizeLabelOffset;
	
	bool		m_bReplaceLabel;
	vector<tstring>		m_vstrAlternateLabels;
	bool		m_bMapAlternateMode;
	map<int, tstring>	m_mapAlternateLabels;
	double		m_fMapAlternateStartVal;
	tstring		m_strLabelSuffix;
	tstring		m_strLabelPrefix;
	tstring		m_strLabelFormat;
		
	bool		m_bColorBar;
	int			m_nColorBarWidth;
	COLORREF	m_crBarColor1, m_crBarColor2;
	int			m_nColorBarMarginOuter, m_nColorBarMarginInner;
	
	bool		m_bTime;
	tstring		m_strTimeFormat;
	
	//bool		m_bIndependent;
	
	bool		m_bMinusTicks;
	bool		m_bTwoStoreyX;
	bool		m_bSciNotation;
	double		m_fSnFontRatio;
	
	bool		m_bYTitleVerticle;
	
	vector<DataRange>	m_vRanges4LabelHiden;

public:
	inline int		GetTickLength( void ) { return m_nTickLength; }
	inline void		SetTickLength( int length ) { m_nTickLength=length; }
	inline int		GetMinorTickLength( void ) { return m_nMinorTickLength; }
	inline void		SetMinorTickLength( int length ) { m_nMinorTickLength=length; }
	inline int		GetTickPosition( void ) { return m_nTickPosition; }
	inline void		SetTickPosition( int position ) { m_nTickPosition=position; }
	inline int		GetMinorTickPosition( void ) { return m_nMinorTickPosition; }
	inline void		SetMinorTickPosition( int position ) { m_nMinorTickPosition=position; }
	inline int		GetTickSize( void ) { return m_nTickSize; }
	inline void		SetTickSize( int Size ) { m_nTickSize=Size; }
	inline int		GetMinorTickSize( void ) { return m_nMinorTickSize; }
	inline void		SetMinorTickSize( int size ) { m_nMinorTickSize=size; }
	inline COLORREF	GetTickColor( void ) { return m_crTickColor; }
	inline void		SetTickColor( COLORREF color ) { m_crTickColor=color; }
	inline COLORREF	GetMinorTickColor( void ) { return m_crMinorTickColor; }
	inline void		SetMinorTickColor( COLORREF color ) { m_crMinorTickColor=color; }
	inline bool		IsShowMajorTicks() { return m_bShowMajorTicks; }
	inline void		SetShowMajorTicks( bool show ) { m_bShowMajorTicks=show; }
	inline bool		IsShowMinorTicks() { return m_bShowMinorTicks; }
	inline void		SetShowMinorTicks( bool show ) { m_bShowMinorTicks=show; }
	inline bool		IsTickLabelSEOnly() { return m_bTickLabelSEOnly; }
	inline void		SetTickLabelSEOnly( bool se ) { m_bTickLabelSEOnly = se; }
	inline int		GetAxisGap() { return m_nAxisGap; }
	inline void		SetAxisGap(int gap) { m_nAxisGap = gap; }
	inline int		GetLabelGap() { return m_nLabelGap; }
	inline void		SetLabelGap(int gap) { m_nLabelGap = gap; }
	inline bool		IsDespiteLow() { return m_bDespiteLow; }
	inline void		SetDespiteLow( bool des ) { m_bDespiteLow = des; }
	inline bool		IsDespiteHigh() { return m_bDespiteHigh; }
	inline void		SetDespiteHigh( bool des ) { m_bDespiteHigh = des; }

	inline bool		IsMapAlternateMode() { return m_bMapAlternateMode; }
	inline void		SetMapAlternateMode( bool mode ) { m_bMapAlternateMode = mode; }
	inline double	GetMapAlternateStartVal() { return m_fMapAlternateStartVal; }
	inline void		SetMapAlternateStartVal(double val) { m_fMapAlternateStartVal = val; }
	void		SetAlternateMap(int nVal, tstring strVal);
	tstring		GetAlternateMap(int nVal);

	inline int			GetBrushType( void ){ return m_nBrushType; }
	inline void			SetBrushType( int type ){ m_nBrushType = type; }
	inline COLORREF		GetBgColor( void ){ return m_crBgColor; }
	inline void			SetBgColor( COLORREF color ){ m_crBgColor = color; }
	inline int			GetHatchStyle( void ){ return m_nHatchStyle; }
	inline void			SetHatchStyle( int style ){ m_nHatchStyle = style; }
	inline COLORREF		GetHatchColor( void ){ return m_crHatchColor; }
	inline void			SetHatchColor( COLORREF color ){ m_crHatchColor = color; }
	inline HBITMAP		GetPattern( void ){ return m_hPattern; }
	inline void			SetPattern( HBITMAP pattern ){ if(m_hPattern)DeleteObject(m_hPattern); m_hPattern = CopyHBitmap(pattern); }
	inline bool			IsBorderShow( void ){ return m_bBorderShow; }
	inline void			SetBorderShow( bool show ){ m_bBorderShow = show; }
	inline int			GetBorderSize( void ){ return m_nBorderSize; }
	inline void			SetBorderSize( int size ){ m_nBorderSize = size; }

	inline bool		IsArrowStart( void ) { return m_bArrowStart; }
	inline void		SetArrowStart( bool arrow ) { m_bArrowStart = arrow; }
	inline bool		IsArrowEnd( void ) { return m_bArrowEnd; }
	inline void		SetArrowEnd( bool arrow ) { m_bArrowEnd = arrow; }
	inline int		GetArrowSize( void ) { return m_nArrowSize; }
	inline void		SetArrowSize( int size ) { m_nArrowSize = size; }
	inline int		GetArrowRatio( void ) { return m_nArrowRatio; }
	inline void		SetArrowRatio( int ratio ) { m_nArrowRatio = ratio; }
	inline bool		IsArrowFlatBottom( void ) { return m_bArrowFlatBottom; }
	inline void		SetArrowFlatBottom( bool flat ) { m_bArrowFlatBottom = flat; }
	inline bool		IsArrowReverse( void ) { return m_bArrowReverse; }
	inline void		SetArrowReverse( bool reverse ) { m_bArrowReverse = reverse; }

	inline bool		IsVisible( void ) { return m_bVisible; }
	inline void		SetVisible( bool visible ) { m_bVisible=visible; }
	inline bool		IsShowAxisLine( void ) { return m_bShowAxisLine; }
	inline void		SetShowAxisLine( bool show ) { m_bShowAxisLine = show; }

	inline bool		IsShowTitle( void ) { return m_bShowTitle; }
	inline void		SetShowTitle( bool show ) { m_bShowTitle=show; }
	inline tstring	GetTitle() { return m_strTitle; }
	inline void		SetTitle( tstring title ) { m_strTitle = title; }
	inline LOGFONT	&GetTitleFont() { return m_lfTitleFont; }
	inline void		SetTitleFont( LOGFONT font)	{ m_lfTitleFont=font; }
	inline LOGFONT	&GetLabelFont() { return m_lfLabelFont; }
	inline void		SetLabelFont( LOGFONT font)	{ m_lfLabelFont=font; }
	inline COLORREF GetTitleColor( void ) { return m_crTitleColor; }
	inline void		SetTitleColor( COLORREF color ) {m_crTitleColor = color; }
	inline bool		IsShowTitleRect( void ) { return m_bShowTitleRect; }
	inline void		SetShowTitleRect( bool show ) { m_bShowTitleRect = show; }
	inline int		GetTitleRectLineSize( void ) { return m_nTitleRectLineSize; }
	inline void		SetTitleRectLineSize( int linesize ) { m_nTitleRectLineSize = linesize; }
	inline int		GetTitleRectLineStyle( void ) { return m_nTitleRectLineStyle; }
	inline void		SetTitleRectLineStyle( int style ) { m_nTitleRectLineStyle = style; }
	inline COLORREF	GetTickLabelColor( void ) { return m_crTickLabelColor; }
	inline void		SetTickLabelColor( COLORREF color ) { m_crTickLabelColor=color; }
	inline bool		IsTickLabelShow( void ) { return m_bTickLabelShow; }
	inline void		SetTickLabelShow( bool show ) { m_bTickLabelShow=show; }
	inline int		GetTitleGap( void ) { return m_nTitleGap; }
	inline void		SetTitleGap( int gap ) { m_nTitleGap = gap; }

	inline int		GetMarginHorizontal( void ) { return m_nMarginHorizontal; }
	inline void		SetMarginHorizontal( int margin ) { m_nMarginHorizontal = margin; }
	inline int		GetMarginVertical( void ) { return m_nMarginHorizontal; }
	inline void		SetMarginVertical( int margin ) { m_nMarginHorizontal = margin; }

	inline COLORREF	GetAxisColor( void ) { return m_crAxisColor; }
	inline void		SetAxisColor( COLORREF color ) { m_crAxisColor = color; }
	inline int		GetLineSize( void ) { return m_nLineSize; }
	inline void		SetLineSize( int size ) { if( size > 0 ) m_nLineSize = size; }
	inline int		GetLineStyle( void ) { return m_nLineStyle; }
	inline void		SetLineStyle( int style ) { if( style > 0 ) m_nLineStyle = style; }


	inline short	GetLocation( void ) { return m_nLocation; }
	inline void		SetLocation( short location ) { m_nLocation = location; }

	inline int		GetTitlePosition( void ) { return m_nTitlePosition; }
	inline void		SetTitlePosition( int pos ) { m_nTitlePosition=pos;  }
	inline SIZE		GetTitleOffset() { return m_sizeTitleOffset; }
	inline void		SetTitleOffset( SIZE offset ) { m_sizeTitleOffset=offset; }
	inline SIZE		GetLabelOffset() { return m_sizeLabelOffset; }
	inline void		SetLabelOffset( SIZE offset ) { m_sizeLabelOffset=offset; }
	inline bool		IsReplaceLabel() { return m_bReplaceLabel; }
	inline void		SetReplaceLabel( bool replace ) { m_bReplaceLabel = replace; }
	inline tstring	GetLabelSuffix() { return m_strLabelSuffix; }
	inline void		SetLabelSuffix(tstring suffix) { m_strLabelSuffix = suffix; }
	inline tstring	GetLabelPrefix() { return m_strLabelPrefix; }
	inline void		SetLabelPrefix(tstring prefix) { m_strLabelPrefix = prefix; }
	inline tstring	GetLabelFormat() { return m_strLabelFormat; }
	inline void		SetLabelFormat(tstring fmt) { m_strLabelFormat = fmt; }
	
	inline bool		IsLighted(){return m_bLighted;}
	inline void		SetLighted(bool cur){m_bLighted=cur;}
	inline bool		IsSelected(){return m_bSelected;}
	inline void		SetSelected(bool cur){m_bSelected=cur;}

	inline bool		IsSameColor(){return m_bSameColor;}
	inline void		SetSameColor(bool same){m_bSameColor=same;}

	inline bool		IsTime(){return m_bTime;}
	inline void		SetTime(bool time){m_bTime = time;}
	inline tstring	GetTimeFormat(){return m_strTimeFormat;}
	inline void		SetTimeFormat(tstring format){m_strTimeFormat = format;}

	inline bool		IsShowColorBar(){return m_bColorBar;}
	inline void		SetShowColorBar( bool crBar ){m_bColorBar = crBar;if(crBar){SetTickPosition(kTickPositionInner); SetMinorTickPosition(kTickPositionInner);} /*else{SetTickPosition(kTickPositionOuter); SetMinorTickPosition(kTickPositionOuter);}*/}
	inline int		GetColorBarWidth(){return m_nColorBarWidth;}
	inline void		SetColorBarWidth(int width){m_nColorBarWidth = width;}
	inline COLORREF	GetBarColor1( void ){ return m_crBarColor1; }
	inline void		SetBarColor1( COLORREF color ){ m_crBarColor1 = color; }
	inline COLORREF	GetBarColor2( void ){ return m_crBarColor2; }
	inline void		SetBarColor2( COLORREF color ){ m_crBarColor2 = color; }
	inline int		GetColorBarMarginOuter(){return m_nColorBarMarginOuter;}
	inline void		SetColorBarMarginOuter(int width){m_nColorBarMarginOuter = width;}
	inline int		GetColorBarMarginInner(){return m_nColorBarMarginInner;}
	inline void		SetColorBarMarginInner(int width){m_nColorBarMarginInner = width;}

	inline bool		IsMinusTicks(){return m_bMinusTicks;}
	inline void		SetMinusTicks( bool minus ){m_bMinusTicks = minus;}
	inline bool		IsTwoStoreyX(){return m_bTwoStoreyX;}
	inline void		SetTwoStroeyX(bool two){m_bTwoStoreyX = two;}
	inline bool		IsSciNotation(){return m_bSciNotation;}
	inline void		SetSciNotation( bool sn ){m_bSciNotation = sn;}
	inline double	GetSnFontRatio(){return m_fSnFontRatio;}
	inline void		SetSnFontRatio(double ratio){m_fSnFontRatio = ratio;}

	inline bool		IsYTitleVerticle(){return m_bYTitleVerticle;}
	inline void		SetYTitleVerticle(bool vert){m_bYTitleVerticle = vert;}

	inline vector<tstring>		&GetAlternateLabels() { return m_vstrAlternateLabels; }
	inline map<int, tstring>	&GetMapAlternateLabels() { return m_mapAlternateLabels; }

	inline vector<DataRange>	&GetRanges4LabelHiden(){ return m_vRanges4LabelHiden; }
	inline void					SetRanges4LabelHiden(vector<DataRange> vRanges){ m_vRanges4LabelHiden = vRanges; }

	bool			IsXAxis();

	int				GetApperanceMode();
	void			SetApperanceMode(int mode);

public:
	tstring			GetDescription();
	static bool		IsXAxis(int location);

protected:
	void			DeleteUsedHandles();
};

Declare_Namespace_End
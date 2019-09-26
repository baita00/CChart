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
#include "PlotUtility.h"
#include "Plot.h"

Declare_Namespace_CChart

enum
{
	kLegendArbitrary,
		
	kLegendInnerLeft,
	kLegendInnerLeftTop,
	kLegendInnerTopLeft = kLegendInnerLeftTop,
	kLegendInnerLeftBottom,
	kLegendInnerBottomLeft = kLegendInnerLeftBottom,
	kLegendInnerRight,
	kLegendInnerRightTop,
	kLegendInnerTopRight = kLegendInnerRightTop,
	kLegendInnerRightBottom,
	kLegendInnerBottomRight = kLegendInnerRightBottom,
	kLegendInnerTop,
	kLegendInnerBottom,
	
	kLegendLeft,
	kLegendLeftTop,
	kLegendLeftBottom,
	kLegendRight,
	kLegendRightTop,
	kLegendRightBottom,
	kLegendTop,
	kLegendTopLeft,
	kLegendTopRight,
	kLegendBottom,
	kLegendBottomLeft,
	kLegendBottomRight,
	
	kLegendPositionCount
};

class CLegend
{
public:
	CLegend();
	virtual	~CLegend();
protected:
	void	SetDefaults();

protected:
	bool				m_bLegendShow;
	int					m_nPosition;
	SIZE				m_sizeOffset;
	int					m_nHorizontalNumOfLegend;
	int					m_nMaxVerticalNumOfLegend;
	COLORREF			m_crBorderColor;
	int					m_nBorderLineSize;
	int					m_nBorderLineStyle;
	bool				m_bBorderShow;
	bool				m_bShadow;
	COLORREF			m_crShadow;
	SIZE				m_sizeShadowOffset;

	bool				m_bLineShow;
	bool				m_bMarkerShow;
	int					m_nLineLength;
	bool				m_bTransparent;
	int					m_nMarginSizeXOuter;
	int					m_nMarginSizeYOuter;
	int					m_nMarginSizeXInner;
	int					m_nMarginSizeYInner;
	bool				m_bShowDragMargin;
	int					m_nDragMarginSize;
	
	bool				m_bConfined;
	
	bool				m_bLighted;
	
	LOGFONT				m_lfFont;
	
	COLORREF			m_crBkgndColor;
	int					m_nBkgndBrushType;
	int					m_nBkgndHatchStyle;
	COLORREF			m_crBkgndHatchColor;
	HBITMAP				m_hBkgndPattern;
	
	bool				m_bLegendDragMode;
protected:
	bool				m_bColorRect;
	
	int					m_nGap;
	
	bool				m_bRound;
	int					m_nRoundRadius;
	int					m_nRightMarginAdded;
	
	int					m_nLegends;

	bool				m_bReactLegend;
public:
	inline bool		IsLegendShow(){return m_bLegendShow;}
	inline void		SetLegendShow(bool show){m_bLegendShow = show;}
	inline int		GetLegendPosition(){return m_nPosition;}
	inline void		SetLegendPosition(int pos){m_nPosition = pos;}
	inline SIZE		&GetLegendOffset(){return m_sizeOffset;}
	inline void		SetLegendOffset(SIZE offset){m_sizeOffset = offset;}
	inline int		GetHorizontalNumOfLegend(){return m_nHorizontalNumOfLegend;}
	inline void		SetHorizontalNumOfLegend(int num){if(num >= 0)m_nHorizontalNumOfLegend = num;}
	inline int		GetMaxVerticalNumOfLegend(){return m_nMaxVerticalNumOfLegend;}
	inline void		SetMaxVerticalNumOfLegend(int num){if(num >= 0)m_nMaxVerticalNumOfLegend = num;}
	inline bool		IsLegendLineShow(){return m_bLineShow;}
	inline void		SetLegendLineShow(bool show){m_bLineShow = show;}
	inline bool		IsLegendMarkerShow(){return m_bMarkerShow;}
	inline void		SetLegendMarkerShow(bool show){m_bMarkerShow = show;}
	inline int		GetLegendLineLength(){return m_nLineLength;}
	inline void		SetLegendLineLength(int len){m_nLineLength = len;}
	inline COLORREF	GetLegendBorderColor(){return m_crBorderColor;}
	inline void		SetLegendBorderColor(COLORREF color){m_crBorderColor = color;}
	inline int		GetLegendBorderLineSize(){return m_nBorderLineSize;}
	inline void		SetLegendBorderLizeSize(int size){if(size>0)m_nBorderLineSize = size;}
	inline int		GetLegendBorderLineStyle(){return m_nBorderLineStyle;}
	inline void		SetLegendBorderLizeStyle(int style){m_nBorderLineStyle = style;}
	inline	bool	IsLegendBorderShow( void ) { return m_bBorderShow; }
	inline	void	SetLegendBorderShow( bool show ) { m_bBorderShow = show; }
	inline	bool	IsLegendShadow( void ) { return m_bShadow; }
	inline	void	SetLegendShadow( bool bShadow) { m_bShadow = bShadow; }
	inline COLORREF	GetLegendShadowColor( void ) { return m_crShadow; }
	inline void		SetLegendShadowColor( COLORREF cr ) { m_crShadow = cr; }
	inline SIZE		GetLegendShadowOffset( void ) { return m_sizeShadowOffset; }
	inline void		SetLegendShadowOffset( SIZE offset ) { m_sizeShadowOffset = offset; }
	inline bool		IsLegendTransparent(){return m_bTransparent;}
	inline void		SetLegendTransparent(bool transparent){m_bTransparent = transparent;}
	inline bool		IsLegendDragMode(){return m_bLegendDragMode;}
	inline void		SetLegendDragMode(bool drag){m_bLegendDragMode = drag;}
//	inline POINT	GetLegendCurrentPoint(){return m_ptLegendCurr;}
//	inline void		SetLegendCurrentPoint(POINT point){m_ptLegendCurr = point;}
	inline int		GetLegendMarginSizeXOuter(){return m_nMarginSizeXOuter;}
	inline void		SetLegendMarginSizeXOuter(int size){m_nMarginSizeXOuter = size; }
	inline int		GetLegendMarginSizeYOuter(){return m_nMarginSizeYOuter;}
	inline void		SetLegendMarginSizeYOuter(int size){m_nMarginSizeYOuter = size; }
	inline int		GetLegendMarginSizeXInner(){return m_nMarginSizeXInner;}
	inline void		SetLegendMarginSizeXInner(int size){m_nMarginSizeXInner = size; }
	inline int		GetLegendMarginSizeYInner(){return m_nMarginSizeYInner;}
	inline void		SetLegendMarginSizeYInner(int size){m_nMarginSizeYInner = size; }
	inline bool		IsLegendShowDragMargin(){return m_bShowDragMargin;}
	inline void		SetLegendShowDragMargin(bool show){m_bShowDragMargin = show;}
	inline int		GetLegendDragMarginSize(){return m_nDragMarginSize;}
	inline void		SetLegendDragMarginSize(int size){m_nDragMarginSize = size; }
	inline bool		IsLegendConfined(){return m_bConfined && m_nPosition == kLegendArbitrary;}
	inline void		SetLegendConfined(bool confine){m_bConfined = confine;}
	inline bool		IsLegendColorRect(){return m_bColorRect;}
	inline void		SetLegendColorRect(bool cr){m_bColorRect = cr;}
	inline int		GetLegendGap(){return m_nGap;}
	inline void		SetLegendGap(int gap){m_nGap = gap;}
	inline bool		IsLegendRound(){return m_bRound;}
	inline void		SetLegendRound(bool round){m_bRound = round;}
	inline int		GetLegendRoundRadius(){return m_nRoundRadius;}
	inline void		SetLegendRoundRadius(int radius){m_nRoundRadius = radius;}
	
	inline int		GetLegends(){return m_nLegends;}
	inline void		SetLegends(int legends){m_nLegends = legends;}
	
	inline COLORREF		GetLegendBkgndColor(){return m_crBkgndColor;}
	inline void			SetLegendBkgndColor(COLORREF color){m_crBkgndColor = color;}
	inline int			GetLegendBkgndBrushType( void ){ return m_nBkgndBrushType; }
	inline void			SetLegendBkgndBrushType( int type ){ m_nBkgndBrushType = type; }
	inline int			GetLegendBkgndHatchStyle( void ){ return m_nBkgndHatchStyle; }
	inline void			SetLegendBkgndHatchStyle( int style ){ m_nBkgndHatchStyle = style; }
	inline COLORREF		GetLegendBkgndHatchColor( void ){ return m_crBkgndHatchColor; }
	inline void			SetLegendBkgndHatchColor( COLORREF color ){ m_crBkgndHatchColor = color; }
	inline HBITMAP		GetLegendBkgndPattern( void ){ return m_hBkgndPattern; }
	inline void			SetLegendBkgndPattern( HBITMAP pattern ){ if(m_hBkgndPattern)DeleteObject(m_hBkgndPattern); m_hBkgndPattern = CopyHBitmap(pattern); }
	
	inline LOGFONT	&GetLegendFont() { return m_lfFont; }
	inline void		SetLegendFont(LOGFONT font) { m_lfFont = font;}
	inline void		SetLegendFontHeight(int height){if(height>0)m_lfFont.lfHeight = height;}
	
	inline bool		IsLegendLighted(){return m_bLighted;}
	inline void		SetLegendLighted(bool cur){m_bLighted = cur;}

	inline bool		IsReactLegend(){return m_bReactLegend;}
	inline void		SetReactLegend(bool react){m_bReactLegend = react;}
	
protected:
	void				DeleteUsedHandles();
};

Declare_Namespace_End
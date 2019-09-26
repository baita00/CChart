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

Declare_Namespace_CChart

class CCanvas
{
public:
	CCanvas();
	virtual ~CCanvas();

protected:
	void		SetDefaults();

	void		DeleteUsedHandles();

protected:
	COLORREF	m_crBorderColor;
	int			m_nBorderSize, m_nBorderStyle;
	bool		m_bBorderShow;

	bool		m_bRoundEdgeConor;
	int			m_nEdgeConorRadius;
	COLORREF	m_crEdgeColor;
	int			m_nEdgeSize, m_nEdgeStyle;
	bool		m_bEdgeShow;
	bool		m_bEdgeButton;
	int			m_nEdgeType,m_nEdgeFlag;
	bool		m_bEdgeBelowTitle;
	
	bool		m_bEraseBkgnd;
	int			m_nBkgndBrushType;
	COLORREF	m_crBkgndColor;
	int			m_nBkgndHatchStyle;
	COLORREF	m_crBkgndHatchColor;
	HBITMAP		m_hBkgndPattern;
	HBITMAP		m_hDefScreenImage;
	bool		m_bGradientBkgnd;
	int			m_nGradientType;
	COLORREF	m_crGradientColor1,m_crGradientColor2;
	bool		m_bBkgndImage;
	HBITMAP		m_hBkgndImage;

	bool		m_bUseDataRegionBrush;
	int			m_nDataRegionBrushType;
	COLORREF	m_crDataRegionColor;
	int			m_nDataRegionHatchStyle;
	COLORREF	m_crDataRegionHatchColor;
	HBITMAP		m_hDataRegionPattern;
	bool		m_bGradientDataRegion;
	int			m_nDataRegionGradientType;
	COLORREF	m_crDataRegionGradientColor1,m_crDataRegionGradientColor2;

	bool		m_bLightDataRegion;

	bool		m_bEraseReservePlotRect;

public:
	inline	COLORREF		GetBorderColor( void ) { return m_crBorderColor; }
	inline	void			SetBorderColor( COLORREF color ) { m_crBorderColor = color; }
	inline	int				GetBorderSize( void ) { return m_nBorderSize; }
	inline	void			SetBorderStyle( int style ) { m_nBorderStyle = style; }
	inline	int				GetBorderStyle( void ) { return m_nBorderStyle; }
	inline	void			SetBorderSize( int size ) { m_nBorderSize = size; }
	inline	bool			IsBorderShow( void ) { return m_bBorderShow; }
	inline	void			SetBorderShow( bool show ) { m_bBorderShow = show; }

	inline	bool			IsRoundEdgeConor( void ) { return m_bRoundEdgeConor; }
	inline	void			SetRoundEdgeConor( bool round ) { m_bRoundEdgeConor = round; }
	inline	int				GetEdgeConorRadius( void ) { return m_nEdgeConorRadius; }
	inline	void			SetEdgeConorRadius( int radius ) { m_nEdgeConorRadius = radius; }	
	inline	COLORREF		GetEdgeColor( void ) { return m_crEdgeColor; }
	inline	void			SetEdgeColor( COLORREF color ) { m_crEdgeColor = color; }
	inline	int				GetEdgeSize( void ) { return m_nEdgeSize; }
	inline	void			SetEdgeStyle( int style ) { m_nEdgeStyle = style; }
	inline	int				GetEdgeStyle( void ) { return m_nEdgeStyle; }
	inline	void			SetEdgeSize( int size ) { m_nEdgeSize = size; }
	inline	bool			IsEdgeShow( void ) { return m_bEdgeShow; }
	inline	void			SetEdgeShow( bool show ) { m_bEdgeShow = show; }
	inline	bool			IsEdgeButton( void ) { return m_bEdgeButton; }
	inline	void			SetEdgeButton( bool butt ) { m_bEdgeButton = butt; }
	inline	int				GetEdgeType( void ) { return m_nEdgeType; }
	inline	void			SetEdgeType( int type ) { m_nEdgeType = type; }
	inline	int				GetEdgeFlag( void ) { return m_nEdgeFlag; }
	inline	void			SetEdgeFlag( int flag ) { m_nEdgeFlag = flag; }
	inline	bool			IsEdgeBelowTitle( void ) { return m_bEdgeBelowTitle; }
	inline	void			SetEdgeBelowTitle( bool below ) { m_bEdgeBelowTitle = below; }

	inline	bool			IsEraseBkgnd( void ) { return m_bEraseBkgnd; }
	inline	void			SetEraseBkgnd( bool how ) { m_bEraseBkgnd = how; }
	inline	int				GetBkgndBrushType( void ){ return m_nBkgndBrushType; }
	inline	void			SetBkgndBrushType( int type ){ m_nBkgndBrushType = type; }
	inline	COLORREF		GetBkgndColor( void ) { return m_crBkgndColor; }
	inline	void			SetBkgndColor( COLORREF color ) { m_crBkgndColor = color; }
	inline	int				GetBkgndHatchStyle( void ){ return m_nBkgndHatchStyle; }
	inline	void			SetBkgndHatchStyle( int style ){ m_nBkgndHatchStyle = style; }
	inline	COLORREF		GetBkgndHatchColor( void ){ return m_crBkgndHatchColor; }
	inline	void			SetBkgndHatchColor( COLORREF color ){ m_crBkgndHatchColor = color; }
	inline	HBITMAP			GetDefScreenImage( void ){ return m_hDefScreenImage; }
	inline	void			SetDefScreenImage( HBITMAP pattern ){ if(m_hDefScreenImage)DeleteObject(m_hDefScreenImage); m_hDefScreenImage = CopyHBitmap(pattern); }
	inline	HBITMAP			GetBkgndPattern( void ){ return m_hBkgndPattern; }
	inline	void			SetBkgndPattern( HBITMAP pattern ){ if(m_hBkgndPattern)DeleteObject(m_hBkgndPattern); m_hBkgndPattern = CopyHBitmap(pattern); }
	inline	bool			IsGradientBkgnd( void ) { return m_bGradientBkgnd; }
	inline	void			SetGradientBkgnd( bool set) { m_bGradientBkgnd=set; }
	inline	int				GetGradientType( void ) { return m_nGradientType; }
	inline	void			SetGradientType( int type) { m_nGradientType=type; }
	inline	COLORREF		GetGradientColor1( void ) { return m_crGradientColor1; }
	inline	void			SetGradientColor1( COLORREF color ) { m_crGradientColor1=color; }
	inline	COLORREF		GetGradientColor2( void ) { return m_crGradientColor2; }
	inline	void			SetGradientColor2( COLORREF color ) { m_crGradientColor2=color; }
	inline	bool			IsBkgndImage( void ) { return m_bBkgndImage; }
	inline	void			SetBkgndImage( bool image ) { m_bBkgndImage = image; }
	inline	HBITMAP			GetBkgndImage( void ){ return m_hBkgndImage; }
	inline	void			SetBkgndImage( HBITMAP image ){ if(m_hBkgndImage)DeleteObject(m_hBkgndImage); m_hBkgndImage = CopyHBitmap(image); }

	inline	int				GetDataRegionBrushType( void ){ return m_nDataRegionBrushType; }
	inline	void			SetDataRegionBrushType( int type ){ m_nDataRegionBrushType = type; }
	inline	int				GetDataRegionHatchStyle( void ){ return m_nDataRegionHatchStyle; }
	inline	void			SetDataRegionHatchStyle( int style ){ m_nDataRegionHatchStyle = style; }
	inline	COLORREF		GetDataRegionHatchColor( void ){ return m_crDataRegionHatchColor; }
	inline	void			SetDataRegionHatchColor( COLORREF color ){ m_crDataRegionHatchColor = color; }
	inline	HBITMAP			GetDataRegionPattern( void ){ return m_hDataRegionPattern; }
	inline	void			SetDataRegionPattern( HBITMAP pattern ){ if(m_hDataRegionPattern)DeleteObject(m_hDataRegionPattern); m_hDataRegionPattern = CopyHBitmap(pattern); }
	inline	COLORREF		GetDataRegionColor( void ) { return m_crDataRegionColor; }
	inline	void			SetDataRegionColor( COLORREF color ) { m_crDataRegionColor = color; }
	inline	bool			IsUseDataRegionBrush( void ) { return m_bUseDataRegionBrush; }
	inline	void			SetUseDataRegionBrush( bool use ) { m_bUseDataRegionBrush = use; }
	inline	bool			IsGradientDataRegion( void ) { return m_bGradientDataRegion; }
	inline	void			SetGradientDataRegion( bool set) { m_bGradientDataRegion=set; }
	inline	int				GetDataRegionGradientType( void ) { return m_nDataRegionGradientType; }
	inline	void			SetDataRegionGradientType( int type) { m_nDataRegionGradientType=type; }
	inline	COLORREF		GetDataRegionGradientColor1( void ) { return m_crDataRegionGradientColor1; }
	inline	void			SetDataRegionGradientColor1( COLORREF color ) { m_crDataRegionGradientColor1=color; }
	inline	COLORREF		GetDataRegionGradientColor2( void ) { return m_crDataRegionGradientColor2; }
	inline	void			SetDataRegionGradientColor2( COLORREF color ) { m_crDataRegionGradientColor2=color; }

	inline	bool			IsLightDataRegion() { return m_bLightDataRegion; }
	inline	void			SetLightDataRegion( bool light ) { m_bLightDataRegion=light; }

	inline	bool			IsEraseReservePlotRect() { return m_bEraseReservePlotRect; }
	inline	void			SetEraseReservePlotRect(bool reserve) { m_bEraseReservePlotRect = reserve; }
};

Declare_Namespace_End
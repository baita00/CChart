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

#ifndef __PIEPLOT_H_122333444455555__
#define __PIEPLOT_H_122333444455555__

#include <windows.h>

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class	CPiePlot
{
public:
	CPiePlot();
	virtual ~CPiePlot();

	void		SetDefaults();

public:
	int			m_nMaxPies;
	double		m_fScale;
	int			m_nLineSize;
	int			m_nLineStyle;
	COLORREF	m_crLineColor;
	bool		m_bCircleMode;
	SIZE		m_sizeOffset;
	bool		m_bDragMode;
	
	LOGFONT		m_lfPieTitleFont;
	bool		m_bShowPieTitle;
	bool		m_bShowPieTitleRatio;
	int			m_nPieTitleRatioPrecision;
	double		m_fStartAngle;
	bool		m_bClockWise;

	int			m_n3DHalfLength;
	double		m_f3DAngle;
	bool		m_bShadowing;
	COLORREF	m_crShadowColor;
	bool		m_b3DStyle;
	bool		m_bSplit;

public:
	inline	int		GetMaxPies() { return m_nMaxPies; }
	inline	void	SetMaxPies( int num ) { m_nMaxPies = num; }
	inline	double	GetScale() { return m_fScale; }
	inline	void	SetScale(double scale) { m_fScale = scale; }

	inline	int		GetLineSize() { return m_nLineSize; }
	inline	void	SetLineSize(int size){ m_nLineSize = size; }
	inline	int		GetLineStyle() { return m_nLineStyle; }
	inline	void	SetLineStyle(int style) { m_nLineStyle = style; }
	inline	bool	IsCircleMode() { return m_bCircleMode; }
	inline	void	SetCircleMode(bool circle) { m_bCircleMode = circle; }
	inline	COLORREF	GetLineColor() { return m_crLineColor; }
	inline	void		SetLineColor( COLORREF color ) { m_crLineColor = color; }
	inline	LOGFONT	&GetPieTitleFont() { return m_lfPieTitleFont; }
	inline	void	SetPieTitleFont(LOGFONT font) { m_lfPieTitleFont = font; }
	inline	bool	IsShowPieTitle() { return m_bShowPieTitle; }
	inline	void	SetShowPieTitle( bool show ) { m_bShowPieTitle = show; }
	inline	bool	IsShowPieTitleRatio() { return m_bShowPieTitleRatio; }
	inline	void	SetShowPieTitleRatio( bool show ) { m_bShowPieTitleRatio = show; }
	inline	int		GetPieTitleRatioPrecision() { return m_nPieTitleRatioPrecision; }
	inline	void	SetPieTitleRatioPrecision(int precision) { m_nPieTitleRatioPrecision = precision; }
	inline	double	GetStartAngle() { return m_fStartAngle; }
	inline	void	SetStartAngle(double angle) { m_fStartAngle = angle; }
	inline	bool	IsClockWise() { return m_bClockWise; }
	inline	void	SetClockWise(bool clockwise) { m_bClockWise = clockwise; }

	inline	bool	IsShadowing() { return m_bShadowing; }
	inline	void	SetShadowing(bool shadow) { m_bShadowing = shadow; }
	inline	int		Get3DHalfLength() { return m_n3DHalfLength; }
	inline	void	Set3DHalfLength(int len){ m_n3DHalfLength = len; }
	inline	double	Get3DAngle() { return m_f3DAngle; }
	inline	void	Set3DAngle(double angle) { m_f3DAngle = angle; }
	inline	COLORREF	GetShadowColor() { return m_crShadowColor; }
	inline	void		SetShadowAngle(COLORREF color) { m_crShadowColor = color; }
	inline	bool	Is3DStyle() { return m_b3DStyle; }
	inline	void	Set3DStyle(bool style) { m_b3DStyle = style; }
	inline	bool	IsSplit() { return m_bSplit; }
	inline	void	SetSplit(bool split) { m_bSplit = split; }



	inline	SIZE	GetOffset() { return m_sizeOffset; }
	inline	void	SetOffset(SIZE offset) { m_sizeOffset = offset; }
	inline	bool	IsDragMode() { return m_bDragMode; }
	inline	void	SetDragMode(bool drag) { m_bDragMode = drag; }
};

Declare_Namespace_End

#endif
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
#include "../CChartNameSpace.h"

Declare_Namespace_CChart

class COscilloScope
{
public:
	COscilloScope();
	virtual ~COscilloScope();
protected:
	void	SetDefaults();

public:
	int					m_nDraggingID;
	int					m_nSteppingID;
	int					m_nSteppingZoneEdgeIdx;
	int					m_nSteppingPrecisionLow;
	int					m_nSteppingPrecisionHigh;
	bool				m_bSteppingHighPrecision;

	bool				m_bCursorByMouse;
	bool				m_bShowVCursor,m_bShowHCursor;
	double				m_fCursorX,m_fCursorY;
	bool				m_bHoldCursorToCurve;
	int					m_nIndexToHoldCursor;
	int					m_nCursorSize, m_nCursorStyle;
	COLORREF			m_crCursorColor;
	
	bool				m_bEnableXZone,m_bEnableYZone;
	double				m_fXZoneLower,m_fXZoneUpper;
	double				m_fYZoneLower,m_fYZoneUpper;
	COLORREF			m_crZoneColor;
	int					m_nZoneAlpha;
	int					m_nZoneEdgeWidth;
	
	//bool				m_bDragZoneEdge[4];
	int					m_nCurZoneEdge;
	
	bool				m_bEnableZoneDrag;
	bool				m_bEnableDataDrag;
	bool				m_bStartOscilloScope;
	bool				m_bOscilloScopeMode;
	
	double				m_fTrigLevel;
	bool				m_bShowZeroArrow;
	bool				m_bShowTrigArrow;
	COLORREF			m_crTrigArrowColor;
	
	bool				m_bLayerOscilloScopeMode;

public:
	inline bool			IsCursorByMouse() { return m_bCursorByMouse; }
	inline void			SetCursorByMouse( bool show ) { m_bCursorByMouse=show; }
	inline bool			IsShowHCursor() { return m_bShowHCursor; }
	inline void			SetShowHCursor( bool show ) { m_bShowHCursor=show; }
	inline bool			IsShowVCursor() { return m_bShowVCursor; }
	inline void			SetShowVCursor( bool show ) { m_bShowVCursor=show; }
	inline void			SetShowCursor( bool show ) { m_bShowHCursor=show; m_bShowVCursor=show; }
	inline bool			IsHoldCursorToCurve() { return m_bHoldCursorToCurve; }
	inline void			SetHoldCursorToCurve( bool hold ){ m_bHoldCursorToCurve = hold; }
	inline double		GetCursorX() { return m_fCursorX; }
	inline void			SetCursorX( double cursor ) { m_fCursorX = cursor; }
	inline double		GetCursorY() { return m_fCursorY; }
	inline void			SetCursorY( double cursor ) { m_fCursorY = cursor; }
	inline void			GetCursor( double *val ) { val[0]=m_fCursorX; val[1]=m_fCursorY; }
	inline void			SetCursor( double *val ) { m_fCursorX=val[0]; m_fCursorY=val[1]; }
	inline int			GetIndexToHoldCursor() { return m_nIndexToHoldCursor; }
	inline void			SetIndexToHoldCursor( int nIndex) { m_nIndexToHoldCursor=nIndex; }
	inline int			GetCursorSize(){return m_nCursorSize;}
	inline void			SetCursorSize(int size){m_nCursorSize = size;}
	inline int			GetCursorStyle(){return m_nCursorStyle;}
	inline void			SetCursorStyle(int style){m_nCursorStyle =style; }
	inline COLORREF		GetCursorColor() { return m_crCursorColor; }
	inline void			SetCursorColor( COLORREF color ) { m_crCursorColor = color; }
	
	inline bool			IsEnableXZone() { return m_bEnableXZone; }
	inline void			SetEnableXZone( bool enable ) { m_bEnableXZone=enable; }
	inline bool			IsEnableYZone() { return m_bEnableYZone; }
	inline void			SetEnableYZone( bool enable ) { m_bEnableYZone=enable; }
	inline double		GetXZoneLower() { return m_fXZoneLower; }
	inline void			SetXZoneLower( double lower) { m_fXZoneLower=lower; }
	inline double		GetXZoneUpper() { return m_fXZoneUpper; }
	inline void			SetXZoneUpper( double upper) { m_fXZoneUpper=upper; }
	inline double		GetYZoneLower() { return m_fYZoneLower; }
	inline void			SetYZoneLower( double lower) { m_fYZoneLower=lower; }
	inline double		GetYZoneUpper() { return m_fYZoneUpper; }
	inline void			SetYZoneUpper( double upper) { m_fYZoneUpper=upper; }
	inline COLORREF		GetZoneColor() { return m_crZoneColor; }
	inline void			SetZoneColor( COLORREF color) { m_crZoneColor=color; }
	inline int			GetZoneAlpha() { return m_nZoneAlpha; }
	inline void			SetZoneAlpha( int alpha) { m_nZoneAlpha=alpha; }
	inline int			GetZoneEdgeWidth() { return m_nZoneEdgeWidth; }
	inline void			SetZoneEdgeWidth( int width) { m_nZoneEdgeWidth=width; }
	inline int			GetCurZoneEdge() { return m_nCurZoneEdge; }
	inline void			SetCurZoneEdge(int cur) { m_nCurZoneEdge = cur; }
	inline void			MoveXZoneLower(double dx) { m_fXZoneLower += dx; }
	inline void			MoveXZoneUpper(double dx) { m_fXZoneLower += dx; }
	inline void			MoveYZoneLower(double dy) { m_fYZoneLower += dy; }
	inline void			MoveYZoneUpper(double dy) { m_fYZoneLower += dy; }
	inline double		GetXZoneGap() { return m_fXZoneUpper - m_fXZoneLower; }
	inline double		GetYZoneGap() { return m_fYZoneUpper - m_fYZoneLower; }
	
	inline bool			IsEnableZoneDrag() { return m_bEnableZoneDrag; }
	inline void			SetEnableZoneDrag( bool enable ) { m_bEnableZoneDrag=enable; }
	inline bool			IsEnableDataDrag() { return m_bEnableDataDrag; }
	inline void			SetEnableDataDrag( bool enable ) { m_bEnableDataDrag=enable; }
	inline bool			IsStartOscilloScope() { return m_bStartOscilloScope; }
	inline void			SetStartOscilloScope( bool enable ) { m_bStartOscilloScope=enable; }
	inline bool			IsOscilloScopeMode() { return m_bOscilloScopeMode; }
	inline void			SetOscilloScopeMode( bool enable ) { m_bOscilloScopeMode=enable; }
	
	inline double		GetTrigLevel() { return m_fTrigLevel; }
	inline void			SetTrigLevel( double level) { m_fTrigLevel=level; }
	inline bool			IsShowZeroArrow() { return m_bShowZeroArrow; }
	inline void			SetShowZeroArrow( bool show ) { m_bShowZeroArrow=show; }
	inline bool			IsShowTrigArrow() { return m_bShowTrigArrow; }
	inline void			SetShowTrigArrow( bool show ) { m_bShowTrigArrow=show; }
	inline COLORREF		GetTrigArrowColor() { return m_crTrigArrowColor; }
	inline void			SetTrigArrowColor( COLORREF color ) { m_crTrigArrowColor = color;  }
	
	inline		bool	IsLayerOscilloScopeMode(){ return m_bLayerOscilloScopeMode; }
	inline		void	SetLayerOscilloScopeMode( bool bOscillo){ m_bLayerOscilloScopeMode = bOscillo; }
};

Declare_Namespace_End
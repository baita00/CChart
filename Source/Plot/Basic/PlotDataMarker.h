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

#include "CChartNameSpace.h"

Declare_Namespace_CChart

// Marker type
enum
{
	kXYMarkerNone = 0,
	kXYMarkerCircle = 1,
	kXYMarkerSquareUpright = 2,
	kXYMarkerSquareOblique = 3,
	kXYMarkerTriangleLeft = 4,
	kXYMarkerTriangleRight = 5,
	kXYMarkerTriangleUp = 6,
	kXYMarkerTriangleDown = 7,
	kXYMarkerX = 8,
	kXYMarkerCross = 9,
	kXYMarkerBar = 10,
	kXYMarkerDot = 11,
	
	kMarkerTypeCount
};

class CPlotDataMarker
{
public:
	CPlotDataMarker();
	virtual ~CPlotDataMarker();
protected:
	void		SetDefaults();

	void		DeleteUsedHandles();

protected:
	bool		m_bMarkerShow;
	int			m_nMarkerType;	// how to mark data values (see enum)
	int			m_nMarkerSize;
	int			m_nMarkerFrequency;
	int			m_nMarkerStartIndex;
	HBITMAP		m_hMarkerImage;
	bool		m_bMarkerImageRound;
	bool		m_bMarkerFill;
	int			m_nMarkerFillAlpha;
	bool		m_bMarkerBarVertical;
	int			m_nMarkerBarHHeight;
	int			m_nMarkerBarHWidth;
	int			m_nMarkerBarExtLen;
	bool		m_bMarkerFillOtherColor;
	COLORREF	m_crMarkerFillColor;

	bool				m_bMarkFirstPoint;
	bool				m_bMarkLastPoint;

public:
	inline		bool		IsMarkerShow(){ return m_bMarkerShow; }
	inline		void		SetMarkerShow( bool show ){ m_bMarkerShow = show; }
	inline		int			GetMarkerType(){ return m_nMarkerType; }
	inline		void		SetMarkerType( int type ){ m_nMarkerType = type; }
	inline		int			GetMarkerSize(){ return m_nMarkerSize; }
	inline		void		SetMarkerSize( int size ){ m_nMarkerSize = size; }
	inline		int			GetMarkerFrequency(){ return m_nMarkerFrequency; }
	inline		void		SetMarkerFrequency( int freq ){ m_nMarkerFrequency = freq; }
	inline		int			GetMarkerStartIndex(){ return m_nMarkerStartIndex; }
	inline		void		SetMarkerStartIndex( int index ){ m_nMarkerStartIndex = index; }
	inline		HBITMAP		GetMarkerImage(){ return m_hMarkerImage; }
	inline		void		SetMarkerImage(HBITMAP hbi){ if(m_hMarkerImage)DeleteObject(m_hMarkerImage); m_hMarkerImage=hbi; }
	inline		bool		IsMarkerImageRound(){ return m_bMarkerImageRound; }
	inline		void		SetMarkerImageRound( bool round ){ m_bMarkerImageRound = round; }
	inline		bool		IsMarkerFill(){ return m_bMarkerFill; }
	inline		void		SetMarkerFill( bool fill ){ m_bMarkerFill = fill; }
	inline		int			GetMarkerFillAlpha(){ return m_nMarkerFillAlpha; }
	inline		void		SetMarkerFillAlpha( int alpha ){ m_nMarkerFillAlpha = alpha; }
	inline		bool		IsMarkerBarVertical(){ return m_bMarkerBarVertical; }
	inline		void		SetMarkerBarVertical( bool vert ){ m_bMarkerBarVertical = vert; }
	inline		int			GetMarkerBarHHeight(){ return m_nMarkerBarHHeight; }
	inline		void		SetMarkerBarHHeight( int height ){ m_nMarkerBarHHeight = height; }
	inline		int			GetMarkerBarHWidth(){ return m_nMarkerBarHWidth; }
	inline		void		SetMarkerBarHWidth( int width ){ m_nMarkerBarHWidth = width; }
	inline		int			GetMarkerBarExtLen(){ return m_nMarkerBarExtLen; }
	inline		void		SetMarkerBarExtLen( int len ){ m_nMarkerBarExtLen = len; }
	inline		bool		IsMarkerFillOtherColor(){ return m_bMarkerFillOtherColor; }
	inline		void		SetMarkerFillOtherColor( bool fill ){ m_bMarkerFillOtherColor = fill; }
	inline		COLORREF	GetMarkerFillColor(){ return m_crMarkerFillColor; }
	inline		void		SetMarkerFillColor(COLORREF cr){ m_crMarkerFillColor = cr; }

	inline bool			IsMarkFirstPoint() { return m_bMarkFirstPoint; }
	inline void			SetMarkFirstPoint( bool mark ){ m_bMarkFirstPoint=mark;}
	inline bool			IsMarkLastPoint() { return m_bMarkLastPoint; }
	inline void			SetMarkLastPoint( bool mark ){ m_bMarkLastPoint=mark;}

};

Declare_Namespace_End

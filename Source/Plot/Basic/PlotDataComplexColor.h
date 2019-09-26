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

#include <vector>
#include <windows.h>

#include <algorithm>

#include "CChartNameSpace.h"

Declare_Namespace_CChart

// mode for input segment color
enum
{
	kInputColorByPointRatio,
	kInputColorByXVal,
	kInputColorByYVal,
	
	kInputColorCount
};

typedef struct stSegmentColor
{
	double val;
	COLORREF color;
}SegmentColor, *pSegmentColor;

bool segcolor_less(const SegmentColor &s1, const SegmentColor &s2);
bool segcolor_great(const SegmentColor &s1, const SegmentColor &s2);

class CPlotDataComplexColor
{
public:
	CPlotDataComplexColor();
	virtual	~CPlotDataComplexColor();
protected:
	void	SetDefaults();

protected:
	COLORREF	m_crDataColor2; //
	bool		m_bBiColorMode; // two color mode
	bool		m_bMultiColorMode; // multi color mode
	int			m_nMultiColorInputType; 
	std::vector<SegmentColor>	m_vSegColors;
	bool		m_bColorPtByPt;
	std::vector<COLORREF>	m_vPointColors;

public:
	inline	COLORREF	GetDataColor2(){return m_crDataColor2;}
	inline	void		SetDataColor2(COLORREF cr){m_crDataColor2 = cr;}
	inline	bool		IsBiColorMode(){return m_bBiColorMode;}
	inline	void		SetBiColorMode(bool bi){m_bBiColorMode = bi;}
	inline	bool		IsMultiColorMode(){return m_bMultiColorMode;}
	inline	void		SetMultiColorMode(bool multi){m_bMultiColorMode = multi;}
	inline	int			GetMultiColorInputType(){return m_nMultiColorInputType;}
	inline	void		SetMultiColorInputType(int type){m_nMultiColorInputType = type;}
	inline	std::vector<SegmentColor>	&GetSegColors(){return m_vSegColors;}
	inline	void						SetSegColors(std::vector<SegmentColor> vsc){m_vSegColors.clear(); m_vSegColors=vsc;}
	inline	bool		IsColorPtByPt(){return m_bColorPtByPt;}
	inline	void		SetColorPtByPt(bool pbp){m_bColorPtByPt = pbp;}
	inline	std::vector<COLORREF>		&GetPointColors(){return m_vPointColors;}
	inline	void						SetPointColors(std::vector<COLORREF> vpc){m_vPointColors.clear(); m_vPointColors=vpc;}

	inline void			AddSegColor(double ratio, COLORREF color)
	{
		SegmentColor sc;
		sc.val = ratio;
		sc.color = color;
		m_vSegColors.push_back(sc);
	}
	inline void			SortSegColors(int dataID)
	{
		std::sort(m_vSegColors.begin(), m_vSegColors.end(), segcolor_less);
	}
	inline void			AddPointColor(COLORREF color)
	{
		m_vPointColors.push_back(color);
	}
	inline void			SetPointColor(int nPointIndex, COLORREF color)
	{
		if(nPointIndex < 0 || nPointIndex > (int)m_vPointColors.size()-1)return;
		m_vPointColors[nPointIndex] = color;
	}
};

Declare_Namespace_End

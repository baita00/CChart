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

#include "../../Basic/CChartNameSpace.h"
#include <windows.h>

Declare_Namespace_CChart

const	bool	Seven_Segment_Code[10][7] =
{
	{true, true, true, true, true, false, true},		//0
	{false, false, false, true, true, false, false},	//1
	{false, true, true, true, false, true, true},		//2
	{false, false, true, true, true, true, true},		//3
	{true, false, false, true, true, true, false},		//4
	{true, false, true, false, true, true, true},		//5
	{true, true, true, false, true, true, true},		//6
	{false, false, true, true, true, false, false},		//7
	{true, true, true, true, true, true, true},			//8
	{true, false, true, true, true, true, true},		//9
};

class CSevenSegmentCode
{
public:
	CSevenSegmentCode();
	virtual	~CSevenSegmentCode();

	void	SetDefaults();

protected:
	int		m_nHBarLen;
	int		m_nVBarLen;
	int		m_nHBarThick;
	int		m_nVBarThick;

	bool	m_bSlope;
	double	m_fAngleSlope;

	bool	m_bSunkenVCenter;

	int		m_nInnerGap;
	int		m_nPointCodeSize;

	COLORREF	m_crBkColor;
	COLORREF	m_crCodeColor;
	COLORREF	m_crSegEdgeColor;
	COLORREF	m_crOffColor;

public:
	inline	int		GetHBarLen(){return m_nHBarLen;}
	inline	void	SetHBarLen(int len){m_nHBarLen = len;}
	inline	int		GetVBarLen(){return m_nVBarLen;}
	inline	void	SetVBarLen(int len){m_nVBarLen = len;}
	inline	int		GetHBarThick(){return m_nHBarThick;}
	inline	void	SetHBarThick(int thick){m_nHBarThick = thick;}
	inline	bool	IsSlope(){return m_bSlope;}
	inline	void	SetSlope(bool slope){m_bSlope = slope;}
	inline	double	GetAngleSlope(){return m_fAngleSlope;}
	inline	void	SetAngleSlope(double angle){m_fAngleSlope = angle;}
	inline	bool	IsSunkenVCenter(){return m_bSunkenVCenter;}
	inline	void	SetSunkenVCenter(bool sunken){m_bSunkenVCenter = sunken;}
	inline	int		GetInnerGap(){return m_nInnerGap;}
	inline	void	SetInnerGap(int gap){m_nInnerGap = gap;}
	inline	int		GetPointCodeSize(){return m_nPointCodeSize;}
	inline	void	SetPointCodeSize(int size){m_nPointCodeSize = size;}
	inline	COLORREF	GetBkColor(){return m_crBkColor;}
	inline	void		SetBkColor(COLORREF color){m_crBkColor = color;}
	inline	COLORREF	GetCodeColor(){return m_crCodeColor;}
	inline	void		SetCodeColor(COLORREF color){m_crCodeColor = color;}
	inline	COLORREF	GetSegEdgeColor(){return m_crSegEdgeColor;}
	inline	void		SetSegEdgeColor(COLORREF color){m_crSegEdgeColor = color;}
	inline	COLORREF	GetOffColor(){return m_crOffColor;}
	inline	void		SetOffColor(COLORREF color){m_crOffColor = color;}
};

Declare_Namespace_End

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

#include "CChartNameSpace.h"

Declare_Namespace_CChart

// Method of set colors
enum
{
	kSetColorFromTable = 0,
	kSetColorFromTwoColor = 1,
	
	kSetColorModeCount
};

class CXYPlot
{
public:
	CXYPlot();
	virtual	~CXYPlot();

protected:
	void	SetDefaults();

protected:
	bool				m_bAutoDataColorGeneral;
	int					m_nModeOfSetColors;
	COLORREF			m_crLineColor1,m_crLineColor2;
	bool				m_bAutoLineStyleGeneral;
	int					m_nAutoColorStartIndex;

	bool				m_bKeepFormer;

	double				m_fHitPrecision;

	bool				m_bClipEllipse;

public:
	int					m_nSpecial;// for special use, wawa
	void				*m_pSpecial; // for special use, wawa

public:
	inline bool			IsAutoDataColorGeneral(){return m_bAutoDataColorGeneral;}
	inline void			SetAutoDataColorGeneral(bool bAuto){m_bAutoDataColorGeneral=bAuto;}
	inline int			GetModeOfSetColors(){return m_nModeOfSetColors;}
	inline void			SetModeOfSetColors(int mode){m_nModeOfSetColors=mode;};
	inline COLORREF		GetLineColor1(){return m_crLineColor1;}
	inline void			SetLineColor1(COLORREF color){m_crLineColor1=color;}
	inline COLORREF		GetLineColor2(){return m_crLineColor2;}
	inline void			SetLineColor2(COLORREF color){m_crLineColor2=color;}
	inline bool			IsAutoLineStyleGeneral(){return m_bAutoLineStyleGeneral;}
	inline void			SetAutoLineStyleGeneral(bool bAuto){m_bAutoLineStyleGeneral=bAuto;}
	inline int			GetAutoColorStartIndex(){return m_nAutoColorStartIndex;}
	inline void			SetAutoColorStartIndex(int idx){m_nAutoColorStartIndex=idx;};

	inline bool			IsKeepFormer() { return m_bKeepFormer;}
	inline void			SetKeepFormer( bool keep ) { m_bKeepFormer=keep; }

	inline double		GetHitPrecision(){return m_fHitPrecision;}
	inline void			SetHitPrecision( double val ){m_fHitPrecision=val;}

	inline bool			IsClipEllipse(){return m_bClipEllipse;}
	inline void			SetClipEllipse(bool clip){m_bClipEllipse = clip;}
};

Declare_Namespace_End

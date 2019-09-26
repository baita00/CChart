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

#ifndef __STEMPLOT_H_122333444455555__
#define __STEMPLOT_H_122333444455555__

#include <windows.h>
#include "../../Basic/MyString.h"
#include "../../Basic/CChartNameSpace.h"
#include <vector>
using namespace std;

Declare_Namespace_CChart

enum
{
	kStemBaseBottom		=	0,
	kStemBaseTop		=	1,
	kStemBaseLeft		=	2,
	kStemBaseRight		=	3,

	kStemBaseCount		=	4
};

class	CStemPlot
{
public:
	CStemPlot();
	virtual ~CStemPlot();

	void	SetDefaults();
protected:
	int			m_nStemBase;
	int			m_nMaxDataSets;
	int			m_nMaxDataSetLen;
	double		m_fRatioOccupy;
	bool		m_bAddUp;
	bool		m_b3DStem;
	double		m_f3DStemThick;
	double		m_f3DStemAngle;
	bool		m_bForceYZeroStart;
	COLORREF	m_crStemEdgeColor;
	vector<tstring> m_vstrLabels;
	bool		m_bShowValOnTop;
	LOGFONT		m_lfValOnTop;
	bool		m_bStemGradientEffect;
	int			m_nStemGradientType;
	
public:
	inline	int		GetStemBase() { return m_nStemBase; }
	inline	void	SetStemBase( int base ) { m_nStemBase = base; }
	inline	int		GetMaxDataSets() { return m_nMaxDataSets; }
	inline	void	SetMaxDataSets(int num) { if( num>0 )m_nMaxDataSets = num; }
	inline	int		GetMaxDataSetLen() { return m_nMaxDataSetLen; }
	inline	virtual	void	SetMaxDataSetLen(int len) { if( len>0 )m_nMaxDataSetLen = len; }
	inline	double	GetRatioOccupy() { return m_fRatioOccupy; }
	inline	void	SetRatioOccupy( double ratio ) { if(ratio<=0 || ratio>1)return; m_fRatioOccupy = ratio;}
	inline	bool	IsAddUp() { return m_bAddUp; }
	inline	void	SetAddUp(bool addup) { m_bAddUp = addup; }
	inline	bool	Is3DStem() { return m_b3DStem; }
	inline	void	Set3DStem(bool td) { m_b3DStem = td;}
	inline	double	Get3DStemThick() { return m_f3DStemThick; }
	inline	void	Set3DStemThick(double thick) {m_f3DStemThick = thick;}
	inline	double	Get3DStemAngle() { return m_f3DStemAngle; }
	inline	void	Set3DStemAngle(double angle) {m_f3DStemAngle = angle;}
	inline	bool	IsForceYZeroStart() { return m_bForceYZeroStart; }
	inline	void	SetForceYZeroStart(bool force) { m_bForceYZeroStart = force; }
	inline	COLORREF	GetStemEdgeColor() { return m_crStemEdgeColor; }
	inline	void		SetStemEdgeColor(COLORREF color) { m_crStemEdgeColor = color; }
	inline	bool	IsShowValOnTop() { return m_bShowValOnTop; }
	inline	void	SetShowValOnTop(bool ontop) { m_bShowValOnTop = ontop; }
	inline	LOGFONT &GetLogFontValOnTop() { return m_lfValOnTop; }
	inline	void	SetLogFontValOnTop(LOGFONT lf) { m_lfValOnTop = lf; }
	inline	int		GetValHeight() { return m_lfValOnTop.lfHeight; }
	inline	void	SetValHeight(int height) { m_lfValOnTop.lfHeight = height; }
	inline	bool	IsStemGradientEffect() { return m_bStemGradientEffect; }
	inline	void	SetStemGradientEffect(bool grad) { m_bStemGradientEffect = grad; }
	inline	int		GetStemGradientType() { return m_nStemGradientType; }
	inline	void	SetStemGradientType(int type) { m_nStemGradientType = type; }
};

Declare_Namespace_End

#endif
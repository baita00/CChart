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
#include <time.h>

#include "CChartNameSpace.h"

Declare_Namespace_CChart

enum
{
	kRangeZoomCenterL,
	kRangeZoomCenterH,
	kRangeZoomCenterM,
	kRangeZoomCenterCount
};

class CRange
{
public:
	CRange();
	virtual ~CRange();

protected:
	void	SetDefaults();
protected:
	bool		m_bStaticRange;
	bool		m_bAutoRange;
	bool		m_bExactRange;
	bool		m_bOptimalExactRange;
	bool		m_bOptimalZeroRepair;
	bool		m_bOptimalSymetryRepair;
	bool		m_bOptimalLExtend, m_bOptimalUExtend;
	
	double		m_pfPlotRange[2];
	double		m_pfLastPlotRange[2];
	bool		m_bRangeSet;
	
	bool		m_bAutoMajorTick;
	bool		m_bAutoMinorTick;
	
	bool		m_bFloatTicks;	
	int			m_nMajorTicks, m_nMinorTicks;
	double		m_fTickMin, m_fTickMax;
	double		m_fAtom;
	double		m_fAtomZero;
	int			m_nSegsPrefer;
	bool		m_bGapDetermined;
	double		m_fTickGap;
	bool		m_bDefExpandWhileGapDet;
	
	bool		m_bLogarithm;

	int			m_nZoomCenter;

	time_t		m_tmtStartTime;

public:
	bool		m_bFloatTicksOld;
	
public:
	inline	bool			IsStaticRange(){return m_bStaticRange;}
	inline	void			SetStaticRange(bool bStatic){m_bStaticRange = bStatic;}
	inline	bool			IsAutoRange(){return m_bAutoRange;}
	inline	void			SetAutoRange(bool bAuto){m_bAutoRange = bAuto; if(bAuto)m_bRangeSet = false;}

	inline	bool			IsExactRange() { return m_bExactRange; }
	inline	void			SetExactRange(bool exact) { m_bExactRange = exact; }
	
	inline	bool			IsOptimalExactRange() { return m_bOptimalExactRange; }
	inline	void			SetOptimalExactRange(bool exact) { m_bOptimalExactRange = exact; }
	
	inline	bool			IsOptimalZeroRepair() { return m_bOptimalZeroRepair; }
	inline	void			SetOptimalZeroRepair(bool repair) { m_bOptimalZeroRepair = repair; }
	
	inline	bool			IsOptimalSymetryRepair() { return m_bOptimalSymetryRepair; }
	inline	void			SetOptimalSymetryRepair(bool repair) { m_bOptimalSymetryRepair = repair; }
	
	inline	bool			IsOptimalLExtend() { return m_bOptimalLExtend; }
	inline	void			SetOptimalLExtend(bool extend) { m_bOptimalLExtend = extend; }
	inline	bool			IsOptimalUExtend() { return m_bOptimalUExtend; }
	inline	void			SetOptimalUExtend(bool extend) { m_bOptimalUExtend = extend; }
	
	double*				GetRange(){return m_pfPlotRange;}
	void				GetRange( double *range ){memcpy(range, m_pfPlotRange, 2*sizeof(double));}
	double				GetLowerRange(){return m_pfPlotRange[0];}
	double				GetUpperRange(){return m_pfPlotRange[1];}
	
	double*				GetLastPlotRange(){return m_pfLastPlotRange;};
	void				GetLastPlotRange(double *range){memcpy(range, m_pfLastPlotRange, 2*sizeof(double));}
	void				SetLastPlotRange(double *range){memcpy(m_pfLastPlotRange, range, 2*sizeof(double));}
	
	inline bool				IsAutoMajorTick( void ) { return m_bAutoMajorTick; }
	inline void				SetAutoMajorTick( bool at ) { m_bAutoMajorTick=at; }
	inline bool				IsAutoMinorTick( void ) { return m_bAutoMinorTick; }
	inline void				SetAutoMinorTick( bool amt ) { m_bAutoMinorTick=amt; }
	
	inline	bool			IsFloatTicks( void ) { return m_bFloatTicks; }
	inline	void			SetFloatTicks( bool flt ) { m_bFloatTicks = flt; m_bRangeSet = false; }
	inline	bool			IsFloatTicksOld( void ) { return m_bFloatTicksOld; }
	inline	void			SetFloatTicksOld( bool flt ) { m_bFloatTicksOld = flt; }
	inline	int				GetTickCount(){ return m_nMajorTicks; }
	inline	void			SetTickCount(int count){ if(count<0)return; m_nMajorTicks = count; }
	inline	int				GetMinorTickCount(){ return m_nMinorTicks; }
	inline	void			SetMinorTickCount(int count){ if(count<=0)return; m_nMinorTicks = count; }
	inline	double			GetTickMin(){ return m_fTickMin; }
	inline	void			SetTickMin(double val){ m_fTickMin = val; }
	inline	double			GetTickMax(){ return m_fTickMax; }
	inline	void			SetTickMax(double val){ m_fTickMax = val; }
	inline	double			GetAtom() { return m_fAtom; }
	inline	void			SetAtom( double val ) { m_fAtom = val; }
	inline	double			GetAtomZero() { return m_fAtomZero; }
	inline	void			SetAtomZero( double val ) { m_fAtomZero = val; }
	inline	int				GetSegsPrefer() { return m_nSegsPrefer; }
	inline	void			SetSegsPrefer( int val ) { m_nSegsPrefer = val; }
	
	inline	bool			IsGapDetermined( void ) { return m_bGapDetermined; }
	inline	void			SetGapDetermined( bool det ){m_bGapDetermined = det;}
	inline	double			GetTickGap(){ return m_fTickGap; }
	inline	void			SetTickGap(double val){ m_fTickGap = val; }
	inline	bool			IsDefExpandWhileGapDet( void ) { return m_bDefExpandWhileGapDet; }
	inline	void			SetDefExpandWhileGapDet( bool de ) { m_bDefExpandWhileGapDet = de; }

	inline	bool			IsLogarithm( void ) { return m_bLogarithm; }
	inline	void			SetLogarithm( bool log ){m_bLogarithm = log;}

	inline	bool			IsRangeSet() { return m_bRangeSet; }
	inline	void			SetRangeSet(bool set) { m_bRangeSet = set; }

	inline	int				GetZoomCenter() { return m_nZoomCenter; }
	inline	void			SetZoomCenter( int ct ) { m_nZoomCenter = ct; }

	inline	time_t			GetStartTime() { return m_tmtStartTime; }
	inline	void			SetStartTime(time_t tmt) { m_tmtStartTime = tmt; }
};

Declare_Namespace_End
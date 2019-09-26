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
#include <vector>
#include "CChartNameSpace.h"
#include "RangeImpl.h"

#define m_X0(A) GetClsRange(0)->A()
#define m_X1(A, B) GetClsRange(0)->A(B)
#define m_X2(A, B, C) GetClsRange(0)->A(B, C)
#define m_Y0(A) GetClsRange(1)->A()
#define m_Y1(A, B) GetClsRange(1)->A(B)
#define m_Y2(A, B, C) GetClsRange(1)->A(B, C)

Declare_Namespace_CChart

template<int nDims>
class CRanges
{
public:
	CRanges();
	virtual ~CRanges();

protected:
	void	SetDefaults();
protected:
	std::vector<CRangeImpl *>	m_vpRanges;
	
	bool		m_bEqualXYRange;
	//bool		m_bStaticRange;

public:
	CRangeImpl*				GetClsRange(int nIndex);
	CRangeImpl*				GetXClsRange();
	CRangeImpl*				GetYClsRange();
	CRangeImpl*				GetZClsRange();
	
public:
	inline	bool			IsXAutoRange(){return m_X0(IsAutoRange);}
	inline	void			SetXAutoRange(bool bAuto){m_X1(SetAutoRange, bAuto); if(bAuto)m_X1(SetRangeSet, false);}
	inline	bool			IsYAutoRange(){return m_Y0(IsAutoRange);}
	inline	void			SetYAutoRange(bool bAuto){m_Y1(SetAutoRange, bAuto); if(bAuto)m_Y1(SetRangeSet, false);}

	inline	bool			IsExactXRange() { return m_X0(IsExactRange); }
	inline	void			SetExactXRange(bool exact) { m_X1(SetExactRange, exact); }
	inline	bool			IsExactYRange() { return m_Y0(IsExactRange); }
	inline	void			SetExactYRange(bool exact) { m_Y1(SetExactRange, exact); }
	inline	void			SetExactRange(bool exact) { SetExactYRange(exact); SetExactXRange(exact); }
	
	inline	bool			IsOptimalExactXRange() { return m_X0(IsOptimalExactRange); }
	inline	void			SetOptimalExactXRange(bool exact) { m_X1(SetOptimalExactRange, exact); }
	inline	bool			IsOptimalExactYRange() { return m_Y0(IsOptimalExactRange); }
	inline	void			SetOptimalExactYRange(bool exact) { m_Y1(SetOptimalExactRange, exact); }
	inline	void			SetOptimalExactRange(bool exact) { SetOptimalExactYRange(exact); SetOptimalExactXRange(exact); }
	
	inline	bool			IsOptimalXZeroRepair() { return m_X0(IsOptimalZeroRepair); }
	inline	void			SetOptimalXZeroRepair(bool repair) { m_X1(SetOptimalZeroRepair, repair); }
	inline	bool			IsOptimalYZeroRepair() { return m_Y0(IsOptimalZeroRepair); }
	inline	void			SetOptimalYZeroRepair(bool repair) { m_Y1(SetOptimalZeroRepair, repair); }
	inline	void			SetOptimalZeroRepair(bool repair) { SetOptimalXZeroRepair(repair); SetOptimalYZeroRepair(repair); }
	
	inline	bool			IsOptimalXSymetryRepair() { return m_X0(IsOptimalSymetryRepair); }
	inline	void			SetOptimalXSymetryRepair(bool repair) { m_X1(SetOptimalSymetryRepair, repair); }
	inline	bool			IsOptimalYSymetryRepair() { return m_Y0(IsOptimalSymetryRepair); }
	inline	void			SetOptimalYSymetryRepair(bool repair) { m_Y1(SetOptimalSymetryRepair, repair); }
	inline	void			SetOptimalSymetryRepair(bool repair) { SetOptimalXSymetryRepair(repair); SetOptimalYSymetryRepair(repair); }
	
	inline	bool			IsOptimalXlExtend() { return m_X0(IsOptimalLExtend); }
	inline	void			SetOptimalXlExtend(bool extend) { m_X1(SetOptimalLExtend, extend); }
	inline	bool			IsOptimalXuExtend() { return m_X0(IsOptimalUExtend); }
	inline	void			SetOptimalXuExtend(bool extend) { m_X1(SetOptimalUExtend, extend); }
	inline	void			SetOptimalXExtend(bool extend) { SetOptimalXlExtend(extend); SetOptimalXuExtend(extend); }
	inline	bool			IsOptimalYlExtend() { return m_Y0(IsOptimalLExtend); }
	inline	void			SetOptimalYlExtend(bool extend) { m_Y1(SetOptimalLExtend, extend); }
	inline	bool			IsOptimalYuExtend() { return m_Y0(IsOptimalUExtend); }
	inline	void			SetOptimalYuExtend(bool extend) { m_Y1(SetOptimalUExtend, extend); }
	inline	void			SetOptimalYExtend(bool extend) { SetOptimalYlExtend(extend); SetOptimalYuExtend(extend); }
	inline	void			SetOptimalExtend(bool extend) { SetOptimalXExtend(extend); SetOptimalYExtend(extend); }
	
	void				GetRange( double *xRange, double *yRange ){GetXRange(xRange); GetYRange(yRange);}
	void				GetXRange( double *range ){m_X1(GetRange, range);}
	void				GetYRange( double *range ){m_Y1(GetRange, range);}
	double*				GetXRange(){return m_X0(GetRange);}
	double*				GetYRange(){return m_Y0(GetRange);}
	double				GetXLowerRange(){return GetXRange()[0];}
	double				GetXUpperRange(){return GetXRange()[1];}
	double				GetYLowerRange(){return GetYRange()[0];}
	double				GetYUpperRange(){return GetYRange()[1];}
	
	virtual	void		GetDataRange1D( int whichDim, double *range )=0;
	void				GetDataRanges(  ){double xRange[2]; double yRange[2]; GetDataRanges(xRange, yRange); }
	void				GetDataRanges( double *xRange, double *yRange){GetDataRange1D(0, xRange); GetDataRange1D(1, yRange);}
	void				GetDataRanges( double *xRange, double *yRange, double *zRange){GetDataRange1D(0, xRange); GetDataRange1D(1, yRange); GetDataRange1D(2, zRange);}
	void				GetXDataRange( double *range ){GetDataRange1D(0, range);}
	void				GetYDataRange( double *range ){GetDataRange1D(1, range);}

	void				GetLastPlotRange(double *xRange, double *yRange){GetLastXPlotRange(xRange); GetLastYPlotRange(yRange);}
	void				GetLastXPlotRange(double *range){m_X1(GetLastPlotRange, range);}
	void				GetLastYPlotRange(double *range){m_Y1(GetLastPlotRange, range);}
	void				SetLastPlotRange(double *xRange, double *yRange){SetLastXPlotRange(xRange); SetLastYPlotRange(yRange);}
	void				SetLastXPlotRange(double *range){m_X1(SetLastPlotRange, range);}
	void				SetLastXPlotRange(double low, double high){double range[2]; range[0]=low; range[1]=high; SetLastXPlotRange(range);}
	void				SetLastYPlotRange(double *range){m_Y1(SetLastPlotRange, range);}
	void				SetLastYPlotRange(double low, double high){double range[2]; range[0]=low; range[1]=high; SetLastYPlotRange(range);}

	void				SetRange( double *xRange, double *yRange ){m_X2(SetRange, xRange[0], xRange[1]); m_Y2(SetRange, yRange[0], yRange[1]);}
	void				SetRange( double xl, double xu, double yl, double yu ){m_X2(SetRange, xl, xu); m_Y2(SetRange, yl, yu);}

	void				SetXRange( double *range ){m_X1(SetRange, range);}
	void				SetXRange( double low, double high, bool bRangeSet){m_X2(SetRange,low, high); m_X1(SetRangeSet, bRangeSet);}
	virtual	void		SetXRange( double low, double high ){SetXRange(low, high, true);}
	void				SetXRange( double low, double high, int nTicks, int nMinorTicks, double fTickMin, double fTickMax){GetClsRange(0)->SetRange(low, high, nTicks, nMinorTicks, fTickMin, fTickMax, true);}
	void				SetYRange( double *range ){m_Y1(SetRange, range);}
	void				SetYRange( double low, double high, bool bRangeSet){m_Y2(SetRange, low, high); m_Y1(SetRangeSet, bRangeSet);}
	virtual	void		SetYRange( double low, double high ){SetYRange(low, high, true);}
	void				SetYRange( double low, double high, int nTicks, int nMinorTicks, double fTickMin, double fTickMax){GetClsRange(1)->SetRange(low, high, nTicks, nMinorTicks, fTickMin, fTickMax, true);}

	void				SetOptimalXRange( double low, double high ){m_X2(SetOptimalRange, low, high);}
	void				SetOptimalYRange( double low, double high ){m_Y2(SetOptimalRange, low, high);}
	
	inline	bool			IsEqualXYRange(){ return m_bEqualXYRange; }
	inline	void			SetEqualXYRange( bool equal ){ m_bEqualXYRange=equal; }

	inline	void			SetStaticRange( bool staticrange ){ SetStaticXRange(staticrange); SetStaticYRange(staticrange); }
	inline	bool			IsStaticXRange(){ return m_X0(IsStaticRange); }
	inline	void			SetStaticXRange( bool staticrange ){ m_X1(SetStaticRange, staticrange); }
	inline	bool			IsStaticYRange(){ return m_Y0(IsStaticRange); }
	inline	void			SetStaticYRange( bool staticrange ){ m_Y1(SetStaticRange, staticrange); }

	inline bool				IsAutoMajorTickX( void ) { return m_X0(IsAutoMajorTick); }
	inline void				SetAutoMajorTickX( bool at ) { m_X1(SetAutoMajorTick, at); }
	inline bool				IsAutoMajorTickY( void ) { return m_Y0(IsAutoMajorTick); }
	inline void				SetAutoMajorTickY( bool at ) { m_Y1(SetAutoMajorTick, at); }
	inline bool				IsAutoMinorTickX( void ) { return m_X0(IsAutoMinorTick); }
	inline void				SetAutoMinorTickX( bool amt ) { m_X1(SetAutoMinorTick, amt); }
	inline bool				IsAutoMinorTickY( void ) { return m_Y0(IsAutoMinorTick); }
	inline void				SetAutoMinorTickY( bool amt ) { m_Y1(SetAutoMinorTick, amt); }
	
	inline	bool			IsFloatXTicks( void ) { return m_X0(IsFloatTicks); }
	inline	void			SetFloatXTicks( bool flt ) { m_X1(SetFloatTicks, flt); }
	inline	bool			IsFloatYTicks( void ) { return m_Y0(IsFloatTicks); }
	inline	void			SetFloatYTicks( bool flt ) { m_Y1(SetFloatTicks, flt); }
	inline	bool			IsFloatXTicksOld( void ) { return m_X0(IsFloatTicksOld); }
	inline	void			SetFloatXTicksOld( bool flt ) { m_X1(SetFloatTicksOld, flt); }
	inline	bool			IsFloatYTicksOld( void ) { return m_Y0(IsFloatTicksOld); }
	inline	void			SetFloatYTicksOld( bool flt ) { m_Y1(SetFloatTicksOld, flt); }
	inline	int				GetXTickCount(){ return m_X0(GetTickCount); }
	inline	void			SetXTickCount(int count){ if(count<0)return; m_X1(SetTickCount, count); }
	inline	int				GetYTickCount(){ return m_Y0(GetTickCount); }
	inline	void			SetYTickCount(int count){ if(count<0)return; m_Y1(SetTickCount, count); }
	inline	int				GetXMinorTickCount(){ return m_X0(GetMinorTickCount); }
	inline	void			SetXMinorTickCount(int count){ if(count<=0)return; m_X1(SetMinorTickCount, count); }
	inline	int				GetYMinorTickCount(){ return m_Y0(GetMinorTickCount); }
	inline	void			SetYMinorTickCount(int count){ if(count<=0)return; m_Y1(SetMinorTickCount, count); }
	inline	double			GetXTickMin(){ return m_X0(GetTickMin); }
	inline	void			SetXTickMin(double val){ m_X1(SetTickMin, val); }
	inline	double			GetXTickMax(){ return m_X0(GetTickMax); }
	inline	void			SetXTickMax(double val){ m_X1(SetTickMax, val); }
	inline	double			GetYTickMin(){ return m_Y0(GetTickMin); }
	inline	void			SetYTickMin(double val){ m_Y1(SetTickMin, val); }
	inline	double			GetYTickMax(){ return m_Y0(GetTickMax); }
	inline	void			SetYTickMax(double val){ m_Y1(SetTickMax, val); }
	inline	double			GetAtomX() { return m_X0(GetAtom); }
	inline	void			SetAtomX( double val ) { m_X1(SetAtom, val); }
	inline	double			GetAtomY() { return m_Y0(GetAtom); }
	inline	void			SetAtomY( double val ) { m_Y1(SetAtom, val); }
	inline	double			GetAtomZeroX() { return m_X0(GetAtomZero); }
	inline	void			SetAtomZeroX( double val ) { m_X1(SetAtomZero, val); }
	inline	double			GetAtomZeroY() { return m_Y0(GetAtomZero); }
	inline	void			SetAtomZeroY( double val ) { m_Y1(SetAtomZero, val); }
	inline	int				GetSegsPreferX() { return m_X0(GetSegsPrefer); }
	inline	void			SetSegsPreferX( int val ) { m_X1(SetSegsPrefer, val); }
	inline	int				GetSegsPreferY() { return m_Y0(GetSegsPrefer); }
	inline	void			SetSegsPreferY( int val ) { m_Y1(SetSegsPrefer, val); }
	inline	bool			IsXGapDetermined() { return m_X0(IsGapDetermined); }
	inline	void			SetXGapDetermined(bool det) { m_X1(SetGapDetermined, det);}
	inline	bool			IsYGapDetermined() { return m_Y0(IsGapDetermined); }
	inline	void			SetYGapDetermined(bool det) { m_Y1(SetGapDetermined, det);}
	inline	double			GetXTickGap() { return m_X0(GetTickGap); }
	inline	void			SetXTickGap( double val ) { m_X1(SetTickGap, val); }
	inline	double			GetYTickGap() { return m_Y0(GetTickGap); }
	inline	void			SetYTickGap( double val ) { m_Y1(SetTickGap, val); }
	inline	bool			IsXDefExpandWhileGapDet() { return m_X0(IsDefExpandWhileGapDet); }
	inline	void			SetXDefExpandWhileGapDet( bool de ) { m_X1(SetDefExpandWhileGapDet, de); }
	inline	bool			IsYDefExpandWhileGapDet() { return m_Y0(IsDefExpandWhileGapDet); }
	inline	void			SetYDefExpandWhileGapDet( bool de ) { m_Y1(SetDefExpandWhileGapDet, de); }
	
	inline	bool			IsXLogarithm( void ) { return m_X0(IsLogarithm); }
	inline	void			SetXLogarithm( bool log ){m_X1(SetLogarithm, log);}
	inline	bool			IsYLogarithm( void ) { return m_Y0(IsLogarithm); }
	inline	void			SetYLogarithm( bool log ){m_Y1(SetLogarithm, log);}

	inline	bool			IsXRangeSet() { return m_X0(IsRangeSet); }
	inline	void			SetXRangeSet(bool set) { m_X1(SetRangeSet, set); }
	inline	bool			IsYRangeSet() { return m_Y0(IsRangeSet); }
	inline	void			SetYRangeSet(bool set) { m_Y1(SetRangeSet, set); }
	inline	void			IsRangeSet( bool &xRangeSet, bool &yRangeSet ) { xRangeSet = m_X0(IsRangeSet); yRangeSet = m_Y0(IsRangeSet); }
	inline	void			SetRangeSet(bool xRangeSet, bool yRangeSet){ m_X1(SetRangeSet, xRangeSet); m_Y1(SetRangeSet, yRangeSet); }
	inline  void			SetRangeSet(bool bRangeSet){for(int i=0; i<(int)m_vpRanges.size(); i++)GetClsRange(i)->SetRangeSet(bRangeSet);}

	inline	void			ExpandXL(){int tick=GetXTickCount(); double range[2]; GetXRange(range); double len=MyRange(range); range[0]-=len/(tick+1); tick++; SetXRange(range); SetXTickCount(tick);}
	inline	void			ExpandXH(){int tick=GetXTickCount(); double range[2]; GetXRange(range); double len=MyRange(range); range[1]+=len/(tick+1); tick++; SetXRange(range); SetXTickCount(tick);}
	inline	void			ExpandXLH(){ExpandXL(); ExpandXH();}
	inline	void			ExpandYL(){int tick=GetYTickCount(); double range[2]; GetYRange(range); double len=MyRange(range); range[0]-=len/(tick+1); tick++; SetYRange(range); SetYTickCount(tick);}
	inline	void			ExpandYH(){int tick=GetYTickCount(); double range[2]; GetYRange(range); double len=MyRange(range); range[1]+=len/(tick+1); tick++; SetYRange(range); SetYTickCount(tick);}
	inline	void			ExpandYLH(){ExpandYL(); ExpandYH();}
	inline	void			ShrinkXL(){int tick=GetXTickCount(); if(tick==0)return; double range[2]; GetXRange(range); double len=MyRange(range); range[0]+=len/(tick+1); tick--; SetXRange(range); SetXTickCount(tick);}
	inline	void			ShrinkXH(){int tick=GetXTickCount(); if(tick==0)return; double range[2]; GetXRange(range); double len=MyRange(range); range[1]-=len/(tick+1); tick--; SetXRange(range); SetXTickCount(tick);}
	inline	void			ShrinkXLH(){ExpandXL(); ExpandXH();}
	inline	void			ShrinkYL(){int tick=GetYTickCount(); if(tick==0)return; double range[2]; GetYRange(range); double len=MyRange(range); range[0]+=len/(tick+1); tick--; SetYRange(range); SetYTickCount(tick);}
	inline	void			ShrinkYH(){int tick=GetYTickCount(); if(tick==0)return; double range[2]; GetYRange(range); double len=MyRange(range); range[1]-=len/(tick+1); tick--; SetYRange(range); SetYTickCount(tick);}
	inline	void			ShrinkYLH(){ExpandYL(); ExpandYH();}

	inline	void			MoveXL(){ExpandXL(); ShrinkXH();}
	inline	void			MoveXH(){ExpandXH(); ShrinkXL();}
	inline	void			MoveYL(){ExpandYL(); ShrinkYH();}
	inline	void			MoveYH(){ExpandYH(); ShrinkYL();}

	inline	time_t			GetXStartTime(){return m_X0(GetStartTime);}
	inline	void			SetXStartTime(time_t tmt){m_X1(SetStartTime, tmt);}
	inline	time_t			GetYStartTime(){return m_Y0(GetStartTime);}
	inline	void			SetYStartTime(time_t tmt){m_Y1(SetStartTime, tmt);}

	//virtual	double				GetMajorTickPosValue( bool bX, int whichTick ){return 0.0;};
	//virtual bool				GetMinorTickPosValue( bool bX, int whichMajor, double *fPos ){return 0.0;};
};

template<int nDims>
CRanges<nDims>::CRanges()
{
	m_vpRanges.resize(nDims);
	CRangeImpl *pRI;
	for(int i=0; i<nDims; i++)
	{
		pRI = new CRangeImpl;
		m_vpRanges[i] = pRI;
	}
	SetDefaults();
}

template<int nDims>
CRanges<nDims>::~CRanges()
{
	while(m_vpRanges.size()>0)
	{
		delete m_vpRanges[0];
		m_vpRanges.erase(m_vpRanges.begin());
	}
}

template<int nDims>
void	CRanges<nDims>::SetDefaults()
{
	//	m_pfXPlotRange[0] = 0.0;
	//	m_pfXPlotRange[1] = 1.0;
	//	m_pfYPlotRange[0] = 0.0;
	//	m_pfYPlotRange[1] = 1.0;
	//	m_pfLastXPlotRange[0] = 0.0;
	//	m_pfLastXPlotRange[1] = 1.0;
	//	m_pfLastYPlotRange[0] = 0.0;
	//	m_pfLastYPlotRange[1] = 1.0;
	//m_bXRangeSet = false;
	//m_bYRangeSet = false;
	
	m_bEqualXYRange = false;
	//m_bStaticRange = false;
	
	//	m_bXAutoRange = true;
	//	m_bYAutoRange = true;
	
	//	m_bExactXRange = false;
	//	m_bExactYRange = false;
	//	m_bOptimalExactXRange = true;
	//	m_bOptimalExactYRange = true;
	//	m_bOptimalXZeroRepair = true;
	//	m_bOptimalYZeroRepair = true;
	//	m_bOptimalXSymetryRepair = true;
	//	m_bOptimalYSymetryRepair = true;
	//	m_bOptimalXlExtend	= false;
	//	m_bOptimalXuExtend	= false;
	//	m_bOptimalYlExtend	= false;
	//	m_bOptimalYuExtend	= false;
	
	//	m_bAutoMajorTickX = true;
	//	m_bAutoMajorTickY = true;
	//	m_bAutoMinorTickX = true;
	//	m_bAutoMinorTickY = true;
	
	//	m_fAtomX = 1.0;
	//	m_fAtomY = 1.0;
	//	m_fAtomZeroX = 0.0;
	//	m_fAtomZeroY = 0.0;
	
	//	m_bXLogarithm = false;
	//	m_bYLogarithm = false;
	
	//	m_bFloatXTicks = false;
	//	m_bFloatYTicks = false;
	
	//	m_bFloatXTicksOld = false;
	//	m_bFloatYTicksOld = false;
}

template<int nDims>
CRangeImpl*		CRanges<nDims>::GetClsRange(int nIndex)
{
	if(nIndex<0 || nIndex>(int)m_vpRanges.size())
		return 0;
	else
		return m_vpRanges[nIndex];
}

template<int nDims>
CRangeImpl*		CRanges<nDims>::GetXClsRange()
{
	return GetClsRange(0);
}

template<int nDims>
CRangeImpl*		CRanges<nDims>::GetYClsRange()
{
	return GetClsRange(1);
}

template<int nDims>
CRangeImpl*		CRanges<nDims>::GetZClsRange()
{
	return GetClsRange(2);
}

Declare_Namespace_End
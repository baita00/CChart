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

#include "AxisImpl.h"

Declare_Namespace_CChart

const int kMaxAxes = 8;

class CAxes
{
public:
	CAxes();
	virtual ~CAxes();
protected:
	void	SetDefaults();

protected:
	std::vector<CAxisImpl *>	m_vpAxis;
	bool						m_bReactAxes;

public:
	inline	bool		IsReactAxes() {return m_bReactAxes;}
	inline	void		SetReactAxes(bool react) { m_bReactAxes = react; }
	inline	int			GetAxesCount( )const{ return (int)m_vpAxis.size(); }
	inline	CAxisImpl	*GetAxis( int which ) { if( which >= 0 && which < (int)m_vpAxis.size() ) return m_vpAxis[which]; else return NULL; }

	virtual	CAxisImpl	*AddAxis( int location );
	virtual	CAxisImpl	*NewAxis()=0;
	CAxisImpl			*GetAxisByLocation( int location );
	CAxisImpl			*GetAxisByTitle( tstring title );
	CAxisImpl			*GetAxisByRegion( int region );
	int					GetAxisIndex( int location );
	bool				IsAxesVisible();
	int					GetVisibleAxisCount();
	//	void				RejectAxisByLocation(int location);
	void				DeleteAxis( int which );
	//	void				RejectAllAxes();
	void				DeleteAllAxes();
	void				DeletaAxesExcept(vector<int> locations);
	void				DeleteAxisByLocation(int location);
	//void				DeleteAddtionalAxes();
	int					GetXAxesCount();
	int					GetVisibleXAxesCount();
	int					GetYAxesCount();
	int					GetVisibleYAxesCount();
	int					GetIndexOfFirstVisibleAxis();
	int					GetIndexOfLastVisibleAxis();
	int					GetIndexOfNextVisibleAxis(int idxCurr);
	int					GetIndexOfPrevVisibleAxis(int idxCurr);
	bool				IsXTime();
	void				SetXToTime(bool set);
	bool				IsYTime();
	void				SetYToTime(bool set);
	vector<CAxisImpl *>		GetXAxes();
	vector<CAxisImpl *>		GetYAxes();
	vector<CAxisImpl *>		GetAxes();
	virtual	void		AddLRAxis();
	virtual	void		AddTBAxis();
	virtual void		AddNormalAxis();
	virtual	void		AddBLAxis();
	virtual	void		AddBRAxis();
	virtual	void		AddTLAxis();
	virtual	void		AddTRAxis();
	void				SetAxesCompact();
	virtual	void		SetLRAxis();
	virtual	void		SetTBAxis();
	virtual void		SetNormalAxis();
	virtual	void		SetBLAxis();
	virtual	void		SetBRAxis();
	virtual	void		SetTLAxis();
	virtual	void		SetTRAxis();
	
	virtual	bool		IsAxisLighted();
	virtual	void		UnlightAllAxis();
	virtual	bool		IsAxisSelected();
	virtual	void		UnselectAllAxis();
	int					GetIndexOfLightedAxis();
	int					GetIndexOfSelectedAxis();
	
	void				SetAxesGap(bool bGap, int nGap=5);
};

Declare_Namespace_End
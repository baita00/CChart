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

#include <vector>

Declare_Namespace_CChart

//class CShareXLayerImplI;
template<class PlotLayerImplIT>
class CLayeredPlot
{
public:
	CLayeredPlot();
	virtual ~CLayeredPlot();

	void		SetDefaults();
//	void		CopySettings(const CLayeredPlot *plot);
	
	//CShareXLayerImplI*		GetLayer(int nIndex);
	//CShareXLayerImplI*		GetSubPlot(int nIndex){return GetLayer(nIndex);}
	PlotLayerImplIT*		GetLayer(int nIndex);
	PlotLayerImplIT*		GetSubPlot(int nIndex){return GetLayer(nIndex);}

	virtual	int	AddLayer() = 0;
	int			ResizePlots(int nLayers);
	
	void		ClearLayers(int nIndex);
	void		ClearLayers();

	int			GetLayerCount(){return (int)m_vpLayers.size();}
	int			GetPlotCount(){return GetLayerCount();}
	int			GetSubPlotCount(){return GetLayerCount();}
	virtual	int	SetLayerCount( int num ) = 0;

	void		DeleteAllData( void ){ClearLayers();}

protected:
	//vector<CShareXLayerImplI*>	m_vpLayers;
	std::vector<PlotLayerImplIT*>	m_vpLayers;

protected:
	bool		m_bShowParentTitle;
	bool		m_bUniteLayerColor;
	int			m_nGridBindLayer;

public:
	inline	bool	IsShowParentTitle() {return m_bShowParentTitle;}
	inline	void	SetShowParentTitle(bool show) {m_bShowParentTitle = show;}
	inline	bool	IsUniteLayerColor(){return m_bUniteLayerColor;}
	inline	void	SetUniteLayerColor(bool unite){m_bUniteLayerColor = unite;}
	inline	int		GetGridBindLayer(){return m_nGridBindLayer;}
	inline	void	SetGridBindLayer(int layer){m_nGridBindLayer = layer;}
};

template<class PlotLayerImplIT>
CLayeredPlot<PlotLayerImplIT>::CLayeredPlot()
{
	SetDefaults();
}

template<class PlotLayerImplIT>
CLayeredPlot<PlotLayerImplIT>::~CLayeredPlot()
{
	
}

template<class PlotLayerImplIT>
void	CLayeredPlot<PlotLayerImplIT>::SetDefaults()
{
	m_bShowParentTitle = true;
	m_bUniteLayerColor = true;
	m_nGridBindLayer = 0;
}

template<class PlotLayerImplIT>
PlotLayerImplIT*		CLayeredPlot<PlotLayerImplIT>::GetLayer(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpLayers.size())return NULL;
	return m_vpLayers[nIndex];
}

template<class PlotLayerImplIT>
void	CLayeredPlot<PlotLayerImplIT>::ClearLayers(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetLayerCount() )return;
	if(m_vpLayers[nIndex])delete m_vpLayers[nIndex];
	m_vpLayers.erase(m_vpLayers.begin()+nIndex);
}

template<class PlotLayerImplIT>
void	CLayeredPlot<PlotLayerImplIT>::ClearLayers()
{
	for(int i=(int)m_vpLayers.size()-1; i>=0; i--)
	{
		if(m_vpLayers[i])delete m_vpLayers[i];
		m_vpLayers.pop_back();
	}
}

template<class PlotLayerImplIT>
int		CLayeredPlot<PlotLayerImplIT>::ResizePlots(int nLayers)
{
	if(nLayers<=0)
		return -1;
	int i, nL = (int)m_vpLayers.size();
	if(nLayers > nL)
	{
		for(i=nL; i<nLayers; i++)
		{
			AddLayer();
		}
	}
	else if(nLayers < nL)
	{
		for(i=nLayers-1; i>=nL; i--)
		{
			delete m_vpLayers[i];
			m_vpLayers.pop_back();
		}
	}
	return nLayers;
}

Declare_Namespace_End

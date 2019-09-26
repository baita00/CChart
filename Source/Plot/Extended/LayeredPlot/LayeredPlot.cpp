/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2011 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

//#include "StdAfx.h"
//#include "ShareXLayerImpl.h"
#include "LayeredPlot.h"

using namespace NsCChart;
/*
CShareXPlot::CShareXPlot()
{
	SetDefaults();
}

CShareXPlot::~CShareXPlot()
{

}

void	CShareXPlot::SetDefaults()
{
	m_bShowParentTitle = true;
	m_bUniteLayerColor = true;
	m_nGridBindLayer = 0;
}

void	CShareXPlot::CopySettings(const CShareXPlot *plot)
{
	m_bShowParentTitle	= plot->m_bShowParentTitle;
	m_bUniteLayerColor = plot->m_bUniteLayerColor;
	m_nGridBindLayer = plot->m_nGridBindLayer;
}

CShareXLayerImplI*		CShareXPlot::GetLayer(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpLayers.size())return NULL;
	return m_vpLayers[nIndex];
}

void	CShareXPlot::ClearLayers(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetLayerCount() )return;
	if(m_vpLayers[nIndex])delete m_vpLayers[nIndex];
	m_vpLayers.erase(m_vpLayers.begin()+nIndex);
}

void	CShareXPlot::ClearLayers()
{
	for(int i=(int)m_vpLayers.size()-1; i>=0; i--)
	{
		if(m_vpLayers[i])delete m_vpLayers[i];
		m_vpLayers.pop_back();
	}
}

int		CShareXPlot::ResizePlots(int nLayers)
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
*/
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
#include "../../Basic/ReactStatus.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CSplitReactStatus : public CReactStatus
{
public:
	CSplitReactStatus();
	virtual	~CSplitReactStatus();

public:
	virtual void				UnlightAll();
	virtual void				UnselectAll();
	virtual void				UnlightAll( bool bTitle, bool bAxes, bool bDatasets);
	virtual void				UnselectAll( bool bTitle, bool bAxes, bool bDatasets);

public:
	void				SwitchSelection();
	void				SwitchSelAxes(){}
	void				SwitchSelPlotData(){}
	void				SwitchSelTitle();

public:
	bool				ShowDlgBySelection(HDC hDC);
};

template<class PlotImplT>
CSplitReactStatus<PlotImplT>::CSplitReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CSplitReactStatus<PlotImplT>::~CSplitReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
void	CSplitReactStatus<PlotImplT>::UnlightAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT)return;

	pT->SetLightTitle(false);
	int i;
	for(i=0; i<pT->GetPlotCount(); i++)
	{
		if(pT->GetReactStatus(i))pT->GetReactStatus(i)->UnlightAll();
	}
}

template<class PlotImplT>
void	CSplitReactStatus<PlotImplT>::UnselectAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT)return;

	pT->SetSelectTitle(false);
	int i;
	for(i=0; i<pT->GetPlotCount(); i++)
	{
		pT->GetSubPlot(i)->SetPlotSelected(false);
		if(pT->GetReactStatus(i))pT->GetReactStatus(i)->UnselectAll();
	}
}

template<class PlotImplT>
void	CSplitReactStatus<PlotImplT>::UnlightAll( bool bTitle, bool bAxes, bool bDatasets)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT)return;

	if(bTitle)
	{
		pT->SetLightTitle(false);
	}
}

template<class PlotImplT>
void	CSplitReactStatus<PlotImplT>::UnselectAll( bool bTitle, bool bAxes, bool bDatasets)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT)return;

	if(bTitle)
	{
		pT->SetSelectTitle(false);
	}
}

template<class PlotImplT>
void	CSplitReactStatus<PlotImplT>::SwitchSelection()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT)return;

	if(!pT->IsTitleRealyShow() && pT->GetPlotCount()<=0) return;
	if(pT->IsSelectTitle())
	{
		if(pT->GetPlotCount()<=0)return;
		pT->SetSelectTitle(false);
		if(!pT->IsSwitchReverse())
			if(pT->GetSubPlot(0))pT->GetSubPlot(0)->SetPlotSelected(true);
		else
			if(pT->GetSubPlot(pT->GetPlotCount()-1))pT->GetSubPlot(pT->GetPlotCount()-1)->SetPlotSelected(true);
	}
	else if(pT->IsSubPlotSelected())
	{
		int i, index = pT->GetIndexOfSelectedSubPlot();
		if(!pT->IsSwitchReverse())
			i = index+1;
		else
			i = index-1;
		
		if(pT->GetSubPlot(index))
		{
			//pT->GetSubPlot(index)->SetPlotSelected(false);
			pT->UnselectAll();
		}
		if(i>=0 && i<pT->GetPlotCount())
		{
			if(pT->GetSubPlot(i))pT->GetSubPlot(i)->SetPlotSelected(true);
		}
		else
		{
			if(pT->IsTitleRealyShow())
				pT->SetSelectTitle(true);
			else
			{
				if(i<0)
					pT->GetSubPlot(pT->GetPlotCount()-1)->SetPlotSelected(true);
				else if(i>=pT->GetPlotCount())
					pT->GetSubPlot(0)->SetPlotSelected(true);
			}
		}
	}
	else
	{
		if(pT->IsTitleRealyShow())
			pT->SetSelectTitle(true);
		else
			pT->GetSubPlot(0)->SetPlotSelected(true);
	}
}

template<class PlotImplT>
void	CSplitReactStatus<PlotImplT>::SwitchSelTitle()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT)return;

	if(!pT->IsTitleRealyShow())
	{
		if(!pT->IsSwitchReverse())
			SwitchSelAxes();
		else
			SwitchSelPlotData();
	}
	else if(pT->IsSelectTitle())
	{
		pT->SetSelectTitle(false);
		if(!pT->IsSwitchReverse())
			SwitchSelAxes();
		else
			SwitchSelPlotData();
	}
	else
	{
		pT->SetSelectTitle(true);
	}
}

template<class PlotImplT>
bool	CSplitReactStatus<PlotImplT>::ShowDlgBySelection(HDC hDC)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	bool needUpdate = false;
	
	return needUpdate;
}

Declare_Namespace_End
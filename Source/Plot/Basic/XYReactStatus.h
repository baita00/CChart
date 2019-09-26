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

#include "CChartNameSpace.h"
#include "ReactStatus.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CXYReactStatus : public CReactStatus
{
public:
	CXYReactStatus();
	virtual	~CXYReactStatus();

public:
	void				UnlightAll();
	void				UnselectAll();
	void				UnlightAll( bool bTitle, bool bAxes, bool bDatasets);
	void				UnselectAll( bool bTitle, bool bAxes, bool bDatasets);

public:
	void				SwitchSelection();
	void				SwitchSelAxes();
	void				SwitchSelPlotData();
	void				SwitchSelTitle();

public:
	bool				ShowDlgBySelection(HDC hDC);
};

template<class PlotImplT>
CXYReactStatus<PlotImplT>::CXYReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CXYReactStatus<PlotImplT>::~CXYReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::UnlightAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	pT->SetLightTitle(false);
	pT->UnlightAllPlotData();
	pT->UnlightAllAxis();
	//if(pT->FindComponent(_TEXT("legend")))pT->SetLegendLighted(false);
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::UnselectAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	pT->SetSelectTitle(false);
	pT->SelectAllPlotData(false);
	pT->UnselectAllAxis();
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::UnlightAll( bool bTitle, bool bAxes, bool bDatasets)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(bTitle)
	{
		pT->SetLightTitle(false);
	}
	if(bAxes)
	{
		pT->UnlightAllAxis();
	}
	if(bDatasets)
	{
		pT->UnlightAllPlotData();
	}
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::UnselectAll( bool bTitle, bool bAxes, bool bDatasets)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(bTitle)
	{
		pT->SetSelectTitle(false);
	}
	if(bAxes)
	{
		pT->UnselectAllAxis();
	}
	if(bDatasets)
	{
		pT->SelectAllPlotData(false);
	}
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::SwitchSelection()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(!pT->IsTitleRealyShow() && pT->GetVisibleAxisCount()<=0 && pT->GetVisiblePlotDataCount()<=0 )
		return;

	if(pT->IsSelectTitle())
		SwitchSelTitle();
	else if(pT->IsAxisSelected())
		SwitchSelAxes();
	else if(pT->IsPlotDataSelected())
		SwitchSelPlotData();
	else
		SwitchSelTitle();
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::SwitchSelAxes()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->GetVisibleAxisCount()<=0)
		SwitchSelPlotData();
	else
	{
		int index, i;
		int idx1, idx2;
		idx1 = pT->GetIndexOfFirstVisibleAxis();
		idx2 = pT->GetIndexOfLastVisibleAxis();
		index = pT->GetIndexOfSelectedAxis();
		
		if(!pT->IsSwitchReverse())
		{
			if(index == idx2)
			{
				pT->GetAxis(index)->SetSelected(false);
				SwitchSelPlotData();
				return;
			}

			CAxisImpl *axis;

			i=(index<0)?idx1:(index+1);
			axis = pT->GetAxis(i);
			while(!axis->IsVisible())
			{
				i++;
				axis = pT->GetAxis(i);
				if(i>=pT->GetAxesCount())break;
			}
			
			if(index>=0)pT->GetAxis(index)->SetSelected(false);
			if(i>=idx2+1)
			{
				SwitchSelPlotData();
			}
			else
			{
				pT->GetAxis(i)->SetSelected(true);
			}
		}
		else
		{
			if(index == idx1)
			{
				pT->GetAxis(index)->SetSelected(false);
				SwitchSelTitle();
				return;
			}
			
			CAxisImpl *axis;
			
			i=(index<0)?idx2:(index-1);
			axis = pT->GetAxis(i);
			while(!axis->IsVisible())
			{
				i--;
				axis = pT->GetAxis(i);
				if(i<0)break;
			}
			
			if(index>=0)pT->GetAxis(index)->SetSelected(false);
			if(i<idx1)
			{
				SwitchSelTitle();
			}
			else
			{
				pT->GetAxis(i)->SetSelected(true);
			}
		}
	}
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::SwitchSelPlotData()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->GetVisiblePlotDataCount()<=0)
		SwitchSelTitle();
	else
	{
		int index, i;
		int idx1,idx2;
		idx1 = pT->GetIndexOfFirstVisiblePlotData();
		idx2 = pT->GetIndexOfLastVisiblePlotData();
		index = pT->GetIndexOfSelected();
		if(!pT->IsSwitchReverse())
		{
			if(index == idx2)
			{
				pT->GetAbsPlotData(index)->SetSelected(false);
				SwitchSelTitle();
				return;
			}
			
			i=(index<0)?idx1:(index+1);
			while(!pT->GetAbsPlotData(i)->IsVisible())
			{
				i++;
				if(i>=pT->GetPlotDataCount())break;
			}
			
			if(index>=0)pT->GetAbsPlotData(index)->SetSelected(false);
			if(i>=idx2+1)
			{
				SwitchSelTitle();
			}
			else
			{
				pT->GetAbsPlotData(i)->SetSelected(true);
			}
		}
		else
		{
			if(index == idx1)
			{
				pT->GetAbsPlotData(index)->SetSelected(false);
				SwitchSelAxes();
				return;
			}
			
			i=(index<0)?idx2:(index-1);
			while(!pT->GetAbsPlotData(i)->IsVisible())
			{
				i--;
				if(i<0)break;
			}
			
			if(index>=0)pT->GetAbsPlotData(index)->SetSelected(false);
			if(i<idx1)
			{
				SwitchSelAxes();
			}
			else
			{
				pT->GetAbsPlotData(i)->SetSelected(true);
			}
		}
		
	}
}

template<class PlotImplT>
void	CXYReactStatus<PlotImplT>::SwitchSelTitle()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

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
bool	CXYReactStatus<PlotImplT>::ShowDlgBySelection(HDC hDC)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	bool needUpdate = false;
	if(pT->IsSelectTitle())
	{
		if(DefPlotSettings(pT, hDC))
		{
			needUpdate = true;
		}
	}
	else if(pT->IsAxisSelected())
	{
		int index, location;
		index = pT->GetIndexOfSelectedAxis();
		location = pT->GetAxis(index)->GetLocation();
		if(DefAxisSettings(pT,hDC,location))
		{
			needUpdate = true;
		}
	}
	else if(pT->IsPlotDataSelected())
	{
		int which;
		which = pT->GetIndexOfSelected();
		if(DefDataSettings(pT,hDC,which))
		{
			needUpdate = true;
		}
	}
	else
	{
		if(DefPlotSettings(pT,hDC))
		{
			needUpdate = true;
		}
	}
	
	return needUpdate;
}

Declare_Namespace_End
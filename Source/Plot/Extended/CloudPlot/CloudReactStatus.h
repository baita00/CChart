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
class	CCloudReactStatus : public CReactStatus
{
public:
	CCloudReactStatus();
	virtual	~CCloudReactStatus();

public:
	void				UnlightAll();
	void				UnselectAll();
	void				UnlightAll( bool bTitle, bool bAxes, bool bDatasets);
	void				UnselectAll( bool bTitle, bool bAxes, bool bDatasets);

public:
	void				SwitchSelection();
	void				SwitchSelAxes();
	void				SwitchSelPlotData(){}
	void				SwitchSelTitle();

public:
	bool	ShowDlgBySelection(HDC hDC);
};

template<class PlotImplT>
CCloudReactStatus<PlotImplT>::CCloudReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CCloudReactStatus<PlotImplT>::~CCloudReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
void	CCloudReactStatus<PlotImplT>::UnlightAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	pT->SetLightTitle(false);
	pT->UnlightAllAxis();
	pT->SetLegendLighted(false);
}

template<class PlotImplT>
void	CCloudReactStatus<PlotImplT>::UnselectAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	pT->SetSelectTitle(false);
	pT->UnselectAllAxis();
}

template<class PlotImplT>
void	CCloudReactStatus<PlotImplT>::UnlightAll( bool bTitle, bool bAxes, bool bDatasets)
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
}

template<class PlotImplT>
void	CCloudReactStatus<PlotImplT>::UnselectAll( bool bTitle, bool bAxes, bool bDatasets)
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
}

template<class PlotImplT>
void	CCloudReactStatus<PlotImplT>::SwitchSelection()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(!pT->IsTitleRealyShow() && pT->GetVisibleAxisCount()<=0 )
		return;

	if(pT->IsSelectTitle())
		SwitchSelTitle();
	else if(pT->IsAxisSelected())
		SwitchSelAxes();
	else
		SwitchSelTitle();
}

template<class PlotImplT>
void	CCloudReactStatus<PlotImplT>::SwitchSelAxes()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->GetVisibleAxisCount()<=0)
		SwitchSelTitle();
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
				SwitchSelTitle();
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
				SwitchSelTitle();
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
void	CCloudReactStatus<PlotImplT>::SwitchSelTitle()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(!pT->IsTitleRealyShow())
	{
		SwitchSelAxes();
	}
	else if(pT->IsSelectTitle())
	{
		pT->SetSelectTitle(false);
		SwitchSelAxes();
	}
	else
	{
		pT->SetSelectTitle(true);
	}
}

template<class PlotImplT>
bool	CCloudReactStatus<PlotImplT>::ShowDlgBySelection(HDC hDC)
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
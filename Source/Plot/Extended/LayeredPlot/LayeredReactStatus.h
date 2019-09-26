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
class	CLayeredReactStatus : public CReactStatus
{
public:
	CLayeredReactStatus();
	virtual	~CLayeredReactStatus();

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
CLayeredReactStatus<PlotImplT>::CLayeredReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CLayeredReactStatus<PlotImplT>::~CLayeredReactStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
void	CLayeredReactStatus<PlotImplT>::UnlightAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	pT->SetLightTitle(false);
	pT->UnlightAllPlotData();
	pT->UnlightAllAxis();
	//if(pT->FindComponent(_TEXT("legend")))pT->SetLegendLighted(false);
}

template<class PlotImplT>
void	CLayeredReactStatus<PlotImplT>::UnselectAll()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	pT->SetSelectTitle(false);
	pT->SelectAllPlotData(false);
	pT->UnselectAllAxis();
}

template<class PlotImplT>
void	CLayeredReactStatus<PlotImplT>::UnlightAll( bool bTitle, bool bAxes, bool bDatasets)
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
void	CLayeredReactStatus<PlotImplT>::UnselectAll( bool bTitle, bool bAxes, bool bDatasets)
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
void	CLayeredReactStatus<PlotImplT>::SwitchSelection()
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
void	CLayeredReactStatus<PlotImplT>::SwitchSelAxes()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->GetVisibleAxisCount()<=0)
	{
		if(!pT->IsSwitchReverse())
			SwitchSelPlotData();
		else
			SwitchSelTitle();
	}
	else
	{
		int idxInLayer, idxOfLayer;
		pT->GetIndexOfSelectedAxis(idxOfLayer, idxInLayer);
		int idxIn = idxInLayer, idxOf = idxOfLayer;
		if(idxOfLayer<0 && idxInLayer<0)idxOf = pT->GetLayerCount();
		
		if(!pT->IsSwitchReverse())
		{
			pT->GetIndexOfNextVisibleAxis(idxOf, idxIn);
			if(idxOf<0)
				SwitchSelPlotData();
			pT->SelectAxis(idxOfLayer, idxInLayer, false);
			pT->SelectAxis(idxOf, idxIn, true);
		}
		else
		{
			pT->GetIndexOfPrevVisibleAxis(idxOf, idxIn);
			if(idxOfLayer<0)
				SwitchSelTitle();
			pT->SelectAxis(idxOfLayer, idxInLayer, false);
			pT->SelectAxis(idxOf, idxIn, true);
		}
	}
}

template<class PlotImplT>
void	CLayeredReactStatus<PlotImplT>::SwitchSelPlotData()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->GetVisibleAxisCount()<=0)
	{
		if(!pT->IsSwitchReverse())
			SwitchSelTitle();
		else
			SwitchSelAxes();
	}
	else
	{
		int idxInLayer, idxOfLayer;
		pT->GetIndexOfSelectedPlotData(idxOfLayer, idxInLayer);
		int idxIn = idxInLayer, idxOf = idxOfLayer;
		
		if(!pT->IsSwitchReverse())
		{
			pT->GetIndexOfNextVisiblePlotData(idxOf, idxIn);
			if(idxOf<0)
				SwitchSelTitle();
			pT->SelectPlotData(idxOfLayer, idxInLayer, false);
			pT->SelectPlotData(idxOf, idxIn, true);
		}
		else
		{
			pT->GetIndexOfPrevVisiblePlotData(idxOf, idxIn);
			if(idxOfLayer<0)
				SwitchSelAxes();
			pT->SelectPlotData(idxOfLayer, idxInLayer, false);
			pT->SelectPlotData(idxOf, idxIn, true);
		}
	}
}

template<class PlotImplT>
void	CLayeredReactStatus<PlotImplT>::SwitchSelTitle()
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
bool	CLayeredReactStatus<PlotImplT>::ShowDlgBySelection(HDC hDC)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	bool needUpdate = false;

	int top = pT->GetTop();

	if(pT->IsSelectTitle())
	{
		if(top>=0)
		{
			if(DefPlotSettings(pT->GetLayer(top), hDC))
			{
				needUpdate = true;
			}
		}
		else
		{
			if(DefPlotSettings(pT, hDC))
			{
				needUpdate = true;
			}
		}
		
	}
	else if(pT->IsAxisSelected())
	{
		int index, location;
		if(top>=0)
		{
			index = pT->GetLayer(top)->GetIndexOfSelectedAxis();
			location = pT->GetLayer(top)->GetAxis(index)->GetLocation();
			if(DefAxisSettings(pT->GetLayer(top),hDC,location))
			{
				needUpdate = true;
			}
		}
		else
		{
			int idxOfLayer, idxInLayer;
			index = pT->GetIndexOfSelectedAxis(idxOfLayer, idxInLayer);
			location = pT->CPlotImpl::GetAxis(index)->GetLocation();
			if(DefAxisSettings(pT,hDC,location))
			{
				needUpdate = true;
			}
		}
		
	}
	else if(pT->IsPlotDataSelected())
	{
		int which;
		if(top>=0)
		{
			which = pT->GetLayer(top)->GetIndexOfSelected();
			if(DefDataSettings(pT->GetLayer(top),hDC,which))
			{
				needUpdate = true;
			}
		}
		
	}
	else
	{
		if(top>=0)
		{
			if(DefPlotSettings(pT->GetLayer(top), hDC))
			{
				needUpdate = true;
			}
		}
		else
		{
			if(DefPlotSettings(pT, hDC))
			{
				needUpdate = true;
			}
		}
	}
	
	return needUpdate;
}

Declare_Namespace_End
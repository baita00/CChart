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

#include "Axes.h"

#include "Plot.h"

using namespace NsCChart;

CAxes::CAxes()
{
	SetDefaults();
}

CAxes::~CAxes()
{
 
}

void	CAxes::SetDefaults()
{
	m_bReactAxes = true;
}

// Adds an axis to the plot. Returns a pointer to
// this axis which need not be stored by the user
// but is useful for initial setup of the axis
CAxisImpl	*CAxes::AddAxis( int location )
{
	CAxisImpl		*axis = 0;
	
	if( m_vpAxis.size() >= kMaxAxes ) return NULL;
	
	if( (axis = GetAxisByLocation( location )) == NULL )
	{
		axis = NewAxis();//new CAxisImpl(this);
		axis->SetLocation( location );
		
		// Add to our axis data sets
		m_vpAxis.push_back(axis);
	}
	
	return axis;
}

// Returns the axis at a specified location (left, top, etc)
// or NULL if no such axis exists
CAxisImpl	*CAxes::GetAxisByLocation( int location )
{
	int			i;
	CAxisImpl	*axis;

	for( i = 0; i < GetAxesCount(); i++ )
	{
		axis = GetAxis(i);
		if( axis != NULL && axis->GetLocation() == location )
			return axis;
	}

	return NULL;
}

CAxisImpl	*CAxes::GetAxisByTitle( tstring title )
{
	int			i;
	CAxisImpl		*axis;
	
	for( i = 0; i < GetAxesCount(); i++ )
	{
		axis = GetAxis(i);
		if( axis != NULL && axis->GetTitle() == title )
			return axis;
	}
	
	return NULL;
}

CAxisImpl	*CAxes::GetAxisByRegion( int region )
{
	if(region == kXYRegionLeft)return GetAxisByLocation(kLocationLeft);
	else if(region == kXYRegionRight)return GetAxisByLocation(kLocationRight);
	else if(region == kXYRegionTop)return GetAxisByLocation(kLocationTop);
	else if(region == kXYRegionBottom)return GetAxisByLocation(kLocationBottom);
	else return 0;
}

int		CAxes::GetAxisIndex( int location )
{
	int			i;
	CAxisImpl		*axis;

	for( i = 0; i < GetAxesCount(); i++ )
	{
		axis = GetAxis(i);
		if( axis != NULL && axis->GetLocation() == location )
			return i;
	}

	return -1;
}

bool	CAxes::IsAxesVisible()
{
	int i;
	for(i=0; i<(int)m_vpAxis.size(); i++)
	{
		if(m_vpAxis[i]->IsVisible())return true;
	}
	return false;
}

int		CAxes::GetVisibleAxisCount()
{
	int i, count = 0;
	for(i=0; i<(int)m_vpAxis.size(); i++)
	{
		if(m_vpAxis[i]->IsVisible())count++;
	}
	return count;
}

void	CAxes::DeleteAxisByLocation(int location)
{
	int		i,which=-1;
	CAxis	*axis;
	for( i = 0; i < GetAxesCount(); i++)
	{
		axis = GetAxis(i);
		if( axis != NULL && axis->GetLocation() == location )
		{
			which=i;
			break;
		}
	}
	if(which==-1)return;

	DeleteAxis(which);
}

void	CAxes::DeleteAxis(int which)
{
	if( which<0 || which> GetAxesCount() )return;

	delete m_vpAxis[which];
	m_vpAxis.erase(m_vpAxis.begin()+which);
}

void	CAxes::DeleteAllAxes()
{
	while( GetAxesCount() >0 )
	{
		DeleteAxis(0);
	}
}

void	CAxes::DeletaAxesExcept(vector<int> locations)
{
	bool hit;
	int nCount = GetAxesCount();
	for(int i=nCount-1; i>=0; i++)
	{
		hit = false;
		for(int j=0; j<(int)locations.size(); j++)
		{
			if(m_vpAxis[i]->GetLocation()==locations[j])
			{
				hit = true;
				break;
			}
		}
		if(hit)continue;

		DeleteAxis(i);
	}
}

int		CAxes::GetXAxesCount()
{
	int count = 0, Num = GetAxesCount();
	for( int i=0; i<Num; i++)
	{
		if(m_vpAxis[i]->IsXAxis())count++;
	}
	return count;
}
int		CAxes::GetVisibleXAxesCount()
{
	int count = 0, Num = GetAxesCount();
	for( int i=0; i<Num; i++)
	{
		if(!m_vpAxis[i]->IsVisible())continue;
		if(m_vpAxis[i]->IsXAxis())count++;
	}
	return count;
}
int		CAxes::GetYAxesCount()
{
	int count = 0, Num = GetAxesCount();
	for( int i=0; i<Num; i++)
	{
		if(!m_vpAxis[i]->IsXAxis())count++;
	}
	return count;
}
int		CAxes::GetVisibleYAxesCount()
{
	int count = 0, Num = GetAxesCount();
	for( int i=0; i<Num; i++)
	{
		if(!m_vpAxis[i]->IsVisible())continue;
		if(!m_vpAxis[i]->IsXAxis())count++;
	}
	return count;
}

int		CAxes::GetIndexOfFirstVisibleAxis()
{
	int index = -1;
	for( int i=0; i<GetAxesCount(); i++ )
	{
		if(m_vpAxis[i]->IsVisible())
		{
			index = i;
			break;
		}
	}
	return index;
}
int		CAxes::GetIndexOfLastVisibleAxis()
{
	int index = -1;
	for( int i=GetAxesCount()-1; i>=0; i-- )
	{
		if(m_vpAxis[i]->IsVisible())
		{
			index = i;
			break;
		}
	}
	return index;
}

int		CAxes::GetIndexOfNextVisibleAxis(int idxCurr)
{
	int index = -1;
	if(idxCurr >= GetAxesCount())
	{
		return -1;
	}
	index = (idxCurr<0)?0:idxCurr+1;
	while(!GetAxis(index)->IsVisible())
	{
		index++;
		if(index >= GetAxesCount())
		{
			index = -1;
			break;
		}
	}
	return index;
}
int		CAxes::GetIndexOfPrevVisibleAxis(int idxCurr)
{
	int index = -1;
	if(idxCurr >= GetAxesCount())
	{
		return -1;
	}
	index = (idxCurr<0)?GetAxesCount()-1:idxCurr-1;
	while(!GetAxis(index)->IsVisible())
	{
		index--;
		if(index < 0)
		{
			index = -1;
			break;
		}
	}
	return index;
}

bool	CAxes::IsAxisLighted()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(m_vpAxis[i]->IsLighted())
		{
			return true;
		}
	}
	return false;	
}

void	CAxes::UnlightAllAxis()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		m_vpAxis[i]->SetLighted(false);
	}
}

bool	CAxes::IsAxisSelected()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(m_vpAxis[i]->IsSelected())
		{
			return true;
		}
	}
		return false;
}

void	CAxes::UnselectAllAxis()
{
	for(int i=0;i<GetAxesCount();i++)
	{
		m_vpAxis[i]->SetSelected(false);
	}
}

int		CAxes::GetIndexOfLightedAxis()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(m_vpAxis[i]->IsLighted())
			return i;
	}
	return -1;
}

int		CAxes::GetIndexOfSelectedAxis()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(m_vpAxis[i]->IsSelected())
			return i;
	}
	return -1;
}

bool	CAxes::IsXTime()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(m_vpAxis[i]->IsXAxis() && m_vpAxis[i]->IsTime())return true;
	}
	return false;
}

void	CAxes::SetXToTime(bool set)
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(m_vpAxis[i]->IsXAxis())m_vpAxis[i]->SetTime(set);
	}
}

bool	CAxes::IsYTime()
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(!m_vpAxis[i]->IsXAxis() && m_vpAxis[i]->IsTime())return true;
	}
	return false;
}

void	CAxes::SetYToTime(bool set)
{
	for(int i=0; i<GetAxesCount(); i++)
	{
		if(!m_vpAxis[i]->IsXAxis())m_vpAxis[i]->SetTime(set);
	}
}

vector<CAxisImpl *>	CAxes::GetXAxes()
{
	vector<CAxisImpl *> vAxis;
	CAxisImpl *axis;
	for( int i = 0; i < GetAxesCount(); i++ )
	{
		axis = GetAxis( i );
		if( axis == NULL ) continue;
		
		if(axis->IsXAxis())vAxis.push_back(axis);
	}
	return vAxis;
}

vector<CAxisImpl *>	CAxes::GetYAxes()
{
	vector<CAxisImpl *> vAxis;
	CAxisImpl *axis;
	for( int i = 0; i < GetAxesCount(); i++ )
	{
		axis = GetAxis( i );
		if( axis == NULL ) continue;
		
		if(!axis->IsXAxis())vAxis.push_back(axis);
	}
	return vAxis;
}

vector<CAxisImpl *>	CAxes::GetAxes()
{
	return m_vpAxis;
}

void		CAxes::AddLRAxis()
{
// 	vector<int> locations;
// 	locations.push_back(kLocationLeft);
// 	locations.push_back(kLocationRight);
// 	DeletaAxesExcept(locations);

	AddAxis(kLocationLeft);
	AddAxis(kLocationRight);
}

void		CAxes::AddTBAxis()
{
// 	vector<int> locations;
// 	locations.push_back(kLocationTop);
// 	locations.push_back(kLocationBottom);
// 	DeletaAxesExcept(locations);

	AddAxis(kLocationTop);
	AddAxis(kLocationBottom);
}

void		CAxes::AddNormalAxis()
{
	AddBLAxis();
}

void		CAxes::AddBLAxis()
{
// 	vector<int> locations;
// 	locations.push_back(kLocationBottom);
// 	locations.push_back(kLocationLeft);
// 	DeletaAxesExcept(locations);

	AddAxis(kLocationBottom);
	AddAxis(kLocationLeft);
}

void		CAxes::AddBRAxis()
{
// 	vector<int> locations;
// 	locations.push_back(kLocationBottom);
// 	locations.push_back(kLocationRight);
// 	DeletaAxesExcept(locations);

	AddAxis(kLocationBottom);
	AddAxis(kLocationRight);
}

void		CAxes::AddTLAxis()
{
// 	vector<int> locations;
// 	locations.push_back(kLocationTop);
// 	locations.push_back(kLocationLeft);
// 	DeletaAxesExcept(locations);

	AddAxis(kLocationTop);
	AddAxis(kLocationLeft);
}

void		CAxes::AddTRAxis()
{
// 	vector<int> locations;
// 	locations.push_back(kLocationTop);
// 	locations.push_back(kLocationRight);
// 	DeletaAxesExcept(locations);

	AddAxis(kLocationTop);
	AddAxis(kLocationRight);
}

void		CAxes::SetAxesCompact()
{
	vector<CAxisImpl *> vAxis = GetAxes();
	for(int i=0; i<(int)vAxis.size(); i++)
	{
		vAxis[i]->SetAxisGap(0);
		vAxis[i]->SetTickPosition(kTickPositionOuter);
		vAxis[i]->SetMinorTickPosition(kTickPositionOuter);
	}
}

void		CAxes::SetLRAxis()
{
	vector<int> locations;
	locations.push_back(kLocationLeft);
	locations.push_back(kLocationRight);
	DeletaAxesExcept(locations);
	
	AddAxis(kLocationLeft);
	AddAxis(kLocationRight);
}

void		CAxes::SetTBAxis()
{
	vector<int> locations;
	locations.push_back(kLocationTop);
	locations.push_back(kLocationBottom);
	DeletaAxesExcept(locations);
	
	AddAxis(kLocationTop);
	AddAxis(kLocationBottom);
}

void		CAxes::SetNormalAxis()
{
	AddBLAxis();
}

void		CAxes::SetBLAxis()
{
	vector<int> locations;
	locations.push_back(kLocationBottom);
	locations.push_back(kLocationLeft);
	DeletaAxesExcept(locations);
	
	AddAxis(kLocationBottom);
	AddAxis(kLocationLeft);
}

void		CAxes::SetBRAxis()
{
	vector<int> locations;
	locations.push_back(kLocationBottom);
	locations.push_back(kLocationRight);
	DeletaAxesExcept(locations);
	
	AddAxis(kLocationBottom);
	AddAxis(kLocationRight);
}

void		CAxes::SetTLAxis()
{
	vector<int> locations;
	locations.push_back(kLocationTop);
	locations.push_back(kLocationLeft);
	DeletaAxesExcept(locations);
	
	AddAxis(kLocationTop);
	AddAxis(kLocationLeft);
}

void		CAxes::SetTRAxis()
{
	vector<int> locations;
	locations.push_back(kLocationTop);
	locations.push_back(kLocationRight);
	DeletaAxesExcept(locations);
	
	AddAxis(kLocationTop);
	AddAxis(kLocationRight);
}

void		CAxes::SetAxesGap(bool bGap, int nGap/* =5 */)
{
	int i;
	for(i=0; i<GetAxesCount(); i++)
	{
		if(bGap)
		{
			m_vpAxis[i]->SetAxisGap(nGap);
			//			m_pAxes[i]->SetTickPosition(kTickPositionInner);
			//			m_pAxes[i]->SetMinorTickPosition(kTickPositionInner);
		}
		else
		{
			m_vpAxis[i]->SetAxisGap(0);
			//			m_pAxes[i]->SetTickPosition(kTickPositionOuter);
			//			m_pAxes[i]->SetMinorTickPosition(kTickPositionOuter);
		}
	}
}
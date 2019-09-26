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

//#include "StdAfx.h"
#include "BendingPlot.h"
#include "../../Basic/MyConst.h"
#include <math.h>

using namespace NsCChart;

CBendingPlot::CBendingPlot()
{
	m_fDeltaAngle=0.5*myPi/180.0;
}

CBendingPlot::~CBendingPlot()
{

}

bool CBendingPlot::AddBendingSect(double z1, double psi1, double z2, double psi2)
{
	if( z1>=z2 || fabs(psi2-psi1)>myPi/2.0 )return false;
	if( m_vBendingSect.size()>0 && z1!=m_vBendingSect[m_vBendingSect.size()-1].z2 )return false;
	
	BendingSect bs;
	bs.z1=z1;
	bs.z2=z2;
	bs.psi1=psi1;
	bs.psi2=psi2;
	m_vBendingSect.push_back(bs);

	return true;
}

bool CBendingPlot::GetCoodOfSect(int idx, double &x1, double &y1, double &x2, double &y2)
{
	if((int)m_vBendingSect.size()<=0 || idx<0 || idx>=(int)m_vBendingSect.size())return false;

	int i;
	double xs,ys,xe,ye;
	double tt,len,t1,t2,radi,xc,yc;

	xs=m_vBendingSect[0].z1;
	ys=0.0;
	xe=xs;
	ye=ys;

	for(i=0; i<=idx; i++)
	{
		xs=xe;
		ys=ye;
		
		
		len=m_vBendingSect[i].z2-m_vBendingSect[i].z1;
		
		if(m_vBendingSect[i].psi1==m_vBendingSect[i].psi2)
		{
			tt=m_vBendingSect[i].psi1-myPi/2.0;
			xe=xs+len*cos(tt);
			ye=xs+len*sin(tt);
		}
		else
		{
			tt=m_vBendingSect[i].psi2-m_vBendingSect[i].psi1;
			radi=len/tt;//may be negative

			t1=m_vBendingSect[i].psi1;
			xc=xs+radi*cos(t1);
			yc=xs+radi*sin(t1);
			t2=m_vBendingSect[i].psi2;
			xe=xc-radi*cos(t2);
			ye=yc-radi*sin(t2);
		}
	}

	x1=xs;
	y1=ys;
	x2=xe;
	y2=ye;
	return true;
}

void CBendingPlot::GetCoodOfZAxis(double z, double &x, double &y)
{
	ConvertData(z, 0, x, y);
}

void CBendingPlot::ConvertData(double x, double y, double &xn, double &yn)
{	
	if(m_vBendingSect.size()<=0)//do not convert any more
	{
		xn=x;
		yn=y;
		return;
	}

	int i;
	double tt,len,t1,t2,radi,xc,yc;

	double x1,y1,x2,y2;
	
	if(x<m_vBendingSect[0].z1)
	{
		t1=m_vBendingSect[0].psi1;
		tt=m_vBendingSect[0].psi1-myPi/2.0;
		len=m_vBendingSect[0].z1-x;

		xn=m_vBendingSect[0].z1-len*cos(tt)+y*cos(t1);
		yn=-len*sin(tt)+y*sin(t1);
	}
	else if(x>m_vBendingSect[m_vBendingSect.size()-1].z2)
	{
		t2=m_vBendingSect[m_vBendingSect.size()-1].psi2;
		tt=m_vBendingSect[m_vBendingSect.size()-1].psi2-myPi/2.0;
		len=x-m_vBendingSect[m_vBendingSect.size()-1].z2;
		GetCoodOfSect((int)m_vBendingSect.size()-1, x1, y1, x2, y2);
		xn=x2+len*cos(tt)+y*cos(t2);
		yn=y2+len*sin(tt)+y*sin(t2);
	}
	else
	{
		for(i=0; i<(int)m_vBendingSect.size(); i++)
		{
			if(x>=m_vBendingSect[i].z1 && x<=m_vBendingSect[i].z2)
				break;
		}
		GetCoodOfSect(i,x1,y1,x2,y2);
		if(m_vBendingSect[i].psi1==m_vBendingSect[i].psi2)
		{
			t1=m_vBendingSect[i].psi1;
			tt=m_vBendingSect[i].psi1-myPi/2.0;
			len=x-m_vBendingSect[i].z1;

			xn=x1+len*cos(tt)+y*cos(t1);
			yn=y1+len*sin(tt)+y*sin(t1);
		}
		else
		{
			tt=m_vBendingSect[i].psi2-m_vBendingSect[i].psi1;
			len=m_vBendingSect[i].z2-m_vBendingSect[i].z1;
			radi=len/tt;//may be negative
			
			t1=m_vBendingSect[i].psi1;
			xc=x1+radi*cos(t1);
			yc=y1+radi*sin(t1);
			t2=m_vBendingSect[i].psi1+tt*(x-m_vBendingSect[i].z1)/len;
			xn=xc-radi*cos(t2)+y*cos(t2);
			yn=yc-radi*sin(t2)+y*sin(t2);
		}
	}
}
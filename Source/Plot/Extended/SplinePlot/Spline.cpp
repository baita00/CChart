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

//#include "stdafx.h"
#include "Spline.h"
#include "../../Accessary/Numerical/NlFit/NlFit.h"

using namespace NsCChart;

void ClearAdjoinCoincide(MyVData2D& vData)
{
	if (vData.size() <= 1)
		return;
	for (int i = (int)vData.size() - 1; i >= 1; i--)
	{
		if (vData[i].val[0] == vData[i - 1].val[0] && vData[i].val[1] == vData[i - 1].val[1])
			vData.erase(vData.begin() + i);
	}
}

CSpline::CSpline()
{
	m_nSegments=10;
	m_nEdgeMode=kSplEdgeModeFree;
	m_dpTanStart.val[0]=0.0;
	m_dpTanStart.val[1]=0.0;
	m_dpTanEnd.val[0]=0.0;
	m_dpTanEnd.val[1]=0.0;
}

CSpline::~CSpline()
{
}

DataPoint2D CSpline::GetEdgePoint(int realIdx, const MyVData2D &vData)
{
	int nPt=(int)vData.size();
	if(nPt<3)return vData[0];

	DataPoint2D datapoint;

	switch(m_nEdgeMode)
	{
	case kSplEdgeModeClose:
		if(realIdx<=-2)
			return vData[nPt-2];
		else if(realIdx==-1)
			return vData[nPt-1];
		else if(realIdx==nPt)
			return vData[0];
		else if(realIdx>=nPt+1)
			return vData[1];
		else
			return vData[realIdx];
	case kSplEdgeModeTangent:
		if(realIdx>=0 && realIdx<=nPt-1)
			return vData[realIdx];
		else if(realIdx<=-1)
		{
			datapoint.val[0]=vData[1].val[0]-m_dpTanStart.val[0];
			datapoint.val[1]=vData[1].val[1]-m_dpTanStart.val[1];
			return datapoint;
		}
		else if(realIdx>=nPt)
		{
			datapoint.val[0]=vData[nPt-2].val[0]+m_dpTanEnd.val[0];
			datapoint.val[1]=vData[nPt-2].val[1]+m_dpTanEnd.val[1];
			return datapoint;
		}
	default:
		if(realIdx<=-1)
			return vData[0];
		else if(realIdx>=nPt)
			return vData[nPt-1];
		else
			return vData[realIdx];
	}
}

DataPoint2D CSpline::GetBSplCtrlPoint(const MyVData2D &vData,int index,int order,bool bLeft)
{
	DataPoint2D datapoint;
	datapoint.val[0]=0.0;
	datapoint.val[1]=0.0;

	if(order<2)return datapoint;
	if((int)vData.size()<=order)return datapoint;

	if(index<0 || index+order>=(int)vData.size())return datapoint;//n阶共有n+1个点参与计算

	int nSeg=GetSegments();
	SetSegments(1);
	if(bLeft)
	{
		GetBSplineValue(datapoint,vData,index,order,0);
	}
	else
	{
		GetBSplineValue(datapoint,vData,index,order,GetSegments());
	}
	SetSegments(nSeg);

	return datapoint;
}

double CSpline::GetXLength(const MyVData2D &vData)
{
	if(vData.size()<=1)return 0.0;
	
	double length=0.0;

	for(int i=0;i<=(int)vData.size()-2;i++)
	{
		length+=fabs(vData[i+1].val[0]-vData[i].val[0]);
	}
	return length;
}

bool CSpline::Tridag(double *a,double *b,double *c,double *r,double *u,int n)
{
	bool retval=false;
	if(n<3)return retval;

	double *gam=new double[n];

	if(b[0]==0)
	{
		delete []gam;
		return retval;
	}

	double bet=b[0];
	u[0]=r[0]/bet;

	int j;

	for(j=1;j<n;j++)
	{
		gam[j-1]=c[j-1]/bet;
		bet=b[j]-a[j]*gam[j-1];
		if(bet==0.0)
		{
			delete []gam;
			return retval;
		}
		u[j]=(r[j]-a[j]*u[j-1])/bet;
	}
	for(j=n-2;j>=0;j--)
	{
		u[j]=u[j]-gam[j]*u[j+1];
	}

	delete []gam;
	retval=true;
	return retval;
}

bool CSpline::GetParSplineValue(DataPoint2D &val,const MyVData2D &vData,int idxLeft,int idxSeg)
{
	bool retval=false;

	int nPt=(int)vData.size();
	if(nPt<3)return retval;

	double t,t1,t2,t3;
	double a,b,c,d;
	t=0.5/m_nSegments;
	t1=t*idxSeg;
	t2=t1*t1;
	t3=t2*t1;

	a=4.0*t2-t1-4.0*t3;
	b=1.0-10.0*t2+12.0*t3;
	c=t1+8.0*t2-12.0*t3;
	d=4.0*t3-2.0*t2;

	double x0,x1,x2,x3,y0,y1,y2,y3;
	DataPoint2D datapoint;
	if(idxLeft-1>=0)
	{
		x0=vData[idxLeft-1].val[0];y0=vData[idxLeft-1].val[1];
	}
	else
	{
		datapoint=GetEdgePoint(idxLeft-1,vData);
		x0=datapoint.val[0];y0=datapoint.val[1];
	}

	x1=vData[idxLeft].val[0];y1=vData[idxLeft].val[1];
	x2=vData[idxLeft+1].val[0];y2=vData[idxLeft+1].val[1];
	if(idxLeft+2<=nPt-1)
	{
		x3=vData[idxLeft+2].val[0];y3=vData[idxLeft+2].val[1];
	}
	else
	{
		datapoint=GetEdgePoint(idxLeft+2,vData);
		x3=datapoint.val[0];y3=datapoint.val[1];
	}
	val.val[0]=a*x0+b*x1+c*x2+d*x3;
	val.val[1]=a*y0+b*y1+c*y2+d*y3;

	retval=true;
	return retval;
}

bool CSpline::GetCubicSplineValue(DataPoint2D &val,const MyVData2D &vData,int idxLeft,int idxSeg)
{
	bool retval=false;

	int nPt=(int)vData.size();
	if(nPt<3)return retval;
	if(idxLeft<0 || idxLeft>=nPt-1)return retval;

	int i;
	double tt,B1,B2,B3,B4,p1,p2,p1p,p2p;

	double *t,*a,*b,*c,*rx,*ry,*ux,*uy;
	t=new double[nPt];
	a=new double[nPt];
	b=new double[nPt];
	c=new double[nPt];
	rx=new double[nPt];
	ry=new double[nPt];
	ux=new double[nPt];
	uy=new double[nPt];

	for(i=1;i<nPt;i++)
	{
		t[i]=sqrt((vData[i].val[0]-vData[i-1].val[0])*(vData[i].val[0]-vData[i-1].val[0])
			+(vData[i].val[1]-vData[i-1].val[1])*(vData[i].val[1]-vData[i-1].val[1]));
		if(fabs(t[i])<=1.0e-15)goto CubicRet;
	}
	tt=idxSeg*t[idxLeft+1]/m_nSegments;

	
	switch(m_nEdgeMode)
	{
	case kSplEdgeModeClamp:
		a[0]=0.0;
		b[0]=1.0;
		c[0]=0.0;
		rx[0]=m_dpTanStart.val[0];
		ry[0]=m_dpTanStart.val[1];

		a[nPt-1]=0.0;
		b[nPt-1]=1.0;
		c[nPt-1]=0.0;
		rx[nPt-1]=m_dpTanEnd.val[0];
		ry[nPt-1]=m_dpTanEnd.val[1];
		break;
	default:
		a[0]=0.0;
		b[0]=1.0;
		c[0]=0.5;
		rx[0]=3.0*(vData[1].val[0]-vData[0].val[0])/(2.0*t[1]);
		ry[0]=3.0*(vData[1].val[1]-vData[0].val[1])/(2.0*t[1]);

		a[nPt-1]=1.0;
		b[nPt-1]=2.0;
		c[nPt-1]=0.0;
		rx[nPt-1]=3.0*(vData[nPt-1].val[0]-vData[nPt-2].val[0])/t[nPt-1];
		ry[nPt-1]=3.0*(vData[nPt-1].val[1]-vData[nPt-2].val[1])/t[nPt-1];
		break;
	}
	for(i=1;i<nPt-1;i++)
	{
		a[i]=t[i+1];
		b[i]=2.0*(t[i+1]+t[i]);
		c[i]=t[i];

		rx[i]=3.0*(t[i]/t[i+1]*(vData[i+1].val[0]-vData[i].val[0])+t[i+1]/t[i]*(vData[i].val[0]-vData[i-1].val[0]));
		ry[i]=3.0*(t[i]/t[i+1]*(vData[i+1].val[1]-vData[i].val[1])+t[i+1]/t[i]*(vData[i].val[1]-vData[i-1].val[1]));
	}

	if(!Tridag(a,b,c,rx,ux,nPt))goto CubicRet;
	if(!Tridag(a,b,c,ry,uy,nPt))goto CubicRet;

	p1=vData[idxLeft].val[0];
	p2=vData[idxLeft+1].val[0];
	switch(m_nEdgeMode)
	{
	case kSplEdgeModeClamp:
		if(idxLeft==0)
			p1p=m_dpTanStart.val[0];
		else
			p1p=ux[idxLeft];
		if(idxLeft==nPt-2)
			p2p=m_dpTanEnd.val[0];
		else
			p2p=ux[idxLeft+1];
		break;
	default:
		p1p=ux[idxLeft];
		p2p=ux[idxLeft+1];
		break;
	}
	B1=p1;
	B2=p1p;
	B3=3.0*(p2-p1)/(t[idxLeft+1]*t[idxLeft+1])-2.0*p1p/t[idxLeft+1]-p2p/t[idxLeft+1];
	B4=2.0*(p1-p2)/(t[idxLeft+1]*t[idxLeft+1]*t[idxLeft+1])+p1p/(t[idxLeft+1]*t[idxLeft+1])+p2p/(t[idxLeft+1]*t[idxLeft+1]);
	val.val[0]=B1+B2*tt+B3*tt*tt+B4*tt*tt*tt;

	p1=vData[idxLeft].val[1];
	p2=vData[idxLeft+1].val[1];
	switch(m_nEdgeMode)
	{
	case kSplEdgeModeClamp:
		if(idxLeft==0)
			p1p=m_dpTanStart.val[1];
		else
			p1p=uy[idxLeft];
		if(idxLeft==nPt-2)
			p2p=m_dpTanEnd.val[1];
		else
			p2p=uy[idxLeft+1];
		break;
	default:
		p1p=uy[idxLeft];
		p2p=uy[idxLeft+1];
		break;
	}
	B1=p1;
	B2=p1p;
	B3=3.0*(p2-p1)/(t[idxLeft+1]*t[idxLeft+1])-2.0*p1p/t[idxLeft+1]-p2p/t[idxLeft+1];
	B4=2.0*(p1-p2)/(t[idxLeft+1]*t[idxLeft+1]*t[idxLeft+1])+p1p/(t[idxLeft+1]*t[idxLeft+1])+p2p/(t[idxLeft+1]*t[idxLeft+1]);
	val.val[1]=B1+B2*tt+B3*tt*tt+B4*tt*tt*tt;

	retval=true;

CubicRet:
	delete []t;
	delete []a;
	delete []b;
	delete []c;
	delete []rx;
	delete []ry;
	delete []ux;
	delete []uy;

	return retval;
}

bool CSpline::GetBezierSplineValue(DataPoint2D &val,const MyVData2D &vData,int idxSeg)
{
	bool retval=false;
	int nPt=(int)vData.size();
	if(nPt<3)return retval;

	double t=idxSeg/(double)m_nSegments;

	val.val[0]=0.0;
	val.val[1]=0.0;

	double bern=1.0;

	for(int i=0;i<nPt;i++)
	{
		//val.val[0]+=vData[i].val[0]*Bernstain(nPt-1,i,t);
		//val.val[1]+=vData[i].val[1]*Bernstain(nPt-1,i,t);
		//调用Bernstain函数效率较低，改算法
		if(i==0)
		{
			bern=pow(1.0-t,nPt-1);
		}
		else
		{
			if(fabs(1.0-t)>=1.0e-15)
			{
				bern*=((double)(nPt-1-i+1)/i)*t/(1.0-t);
			}
			else
			{
				bern= (i<nPt-1)?0.0:1.0;
			}

		}
		val.val[0]+=vData[i].val[0]*bern;
		val.val[1]+=vData[i].val[1]*bern;
	}
	retval=true;
	return retval;
}

double CSpline::Bernstnew(int n,int k,double t)
{
	double val=0,dv;
	double comb=1.0;
	for(int j=0;j<=n-k;j++)
	{
		//dv=Combination(n+1,j)*pow(t+n-k-j,n);
		//if(j%2==1)dv*=-1.0;
		//优化算法
		if(j==0)
		{
			comb=1.0;
			dv=pow(t+n-k,n);
		}
		else
		{
			comb*=(double(n+1-j+1)/j);
			dv=comb*pow(t+n-k-j,n);
		}
		if(j%2==1)dv*=-1.0;

		val+=dv;
	}
	return val/Factorial(n);
}

bool CSpline::GetBSplineValue(DataPoint2D &val,const MyVData2D &vData,int nIndex,int nOrder,int idxSeg)
{
	bool retval=false;

	int nPt=(int)vData.size();
	if(nPt<nIndex+nOrder+1)return retval;

	double t;
	t=1.0/m_nSegments*idxSeg;
	val.val[0]=0.0;
	val.val[1]=0.0;
	double bern;
	for(int i=nIndex; i<=nIndex+nOrder; i++)
	{
		bern=Bernstnew(nOrder,i-nIndex,t);
		val.val[0]+=vData[i].val[0]*bern;
		val.val[1]+=vData[i].val[1]*bern;
	}
	retval=true;

	return retval;
}

bool CSpline::GetLeastSqualValue(DataPoint2D &val,const MyVData2D &vData,int nOrder)
{
	bool retval=false;

	int nPt=(int)vData.size();
	if(nPt<3)return retval;
	if(nOrder<2)return retval;

	double *a,*b;
	a=new double[(nOrder+1)*(nOrder+1)];
	b=new double[nOrder+1];

	int i,j,k,l;

	double sum;
	for(k=0;k<=2*nOrder;k++)
	{
		if(k==0)
		{
			sum=(double)vData.size();
			a[0*(nOrder+1)+nOrder]=sum;
			continue;
		}

		sum=0.0;
		for(l=0;l<nPt;l++)
		{
			sum+=pow(vData[l].val[0],k);
		}
		for(i=0;i<=nOrder;i++)
		{
			for(j=0;j<=nOrder;j++)
			{
				if(j-i==nOrder-k)
				{
					a[i*(nOrder+1)+j]=sum;
				}
			}
		}
	}
	for(i=0;i<=nOrder;i++)
	{
		sum=0.0;
		for(j=0;j<nPt;j++)
		{
			if(i==0)
				sum+=vData[j].val[1];
			else
				sum+=vData[j].val[1]*pow(vData[j].val[0],i);
		}
		b[i]=sum;
	}

	int flag;
	gaussj(a,nOrder+1,b,flag);
	if(flag)
	{
		goto LeastSqualRet;
	}

	val.val[1]=0;
	for(i=0;i<=nOrder;i++)
	{
		val.val[1]+=b[i]*pow(val.val[0],nOrder-i);
	}
	retval=true;

LeastSqualRet:
	delete []a;
	delete []b;

	return retval;
}
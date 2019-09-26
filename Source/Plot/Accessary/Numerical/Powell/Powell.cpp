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
#include "Powell.h"
#include <math.h>

namespace NsCChart{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

double TestFunction(double x, double y)
{
	//return 0.5 * (cos(x + 3.1415926536 / 4) + sin(y + 3.1415926536 / 4));
	return 1.0/((x-1.5)*(x-1.5)+(y-0.8)*(y-0.8)+1.0)+1.0/((x+1.5)*(x+1.5)+(y-0.8)*(y-0.8)+1.0)+1.0/(x*x+(y+0.8)*(y+0.8)+1.0);
};


extern "C" int bobyqa_(void (*fcn)(int *,double *, double*),int* n, int* npt, double* x, double* xlb, double* xub, double* rhtbeg, double* rhoend, int* iprint, int* maxfun, double* workspace, int *iflag);

bool BoundedPowell(void (*fcn)(int *,double *, double*), int n, double *x, double *xlb, double *xub, double rhobeg, double rhoend,int maxfun,int &iflag)
{
	if(n<2)return false;

	int npt=2*n+1;
	double *w=new double[(npt+5)*(npt+n)+3*n*(n+5)/2];
	int iprint=0;

	//确定rhobeg
	if(rhobeg<=0.0)rhobeg=1.0;
	for(int i=0;i<n;i++)
	{
		if(rhobeg>fabs(xub[i]-xlb[i])/10.0)rhobeg=fabs(xub[i]-xlb[i])/10.0;
	}
	//确定rhoend;
	if(rhoend<=0.0)rhoend=1.0e-6;
	if(rhoend>rhobeg/10.0)rhoend=rhobeg/10.0;

	bobyqa_(fcn,&n,&npt,x,xlb,xub,&rhobeg,&rhoend,&iprint,&maxfun,w,&iflag);

	delete []w;

	if(iflag!=0)return false;

	return true;
}

void Shft2(double &a, double &b, const double c){ a=b; b=c; } 
void Shft3(double &a, double &b, double &c, const double d){ a=b; b=c; c=d; } 
void Swap(double &a, double &b){ double dum=a; a=b; b=dum; } 
double Sign(const double &a, const double &b){return b >= 0?(a>=0?a:-a):(a>=0?-a:a);} 
double Max(const double &a, const double &b){return b > a ? (b) : (a);}
double Confine(double &x,double mx,double mn){if(mx<mn)Swap(mx,mn);if(x>mx)x=mx;if(x<mn)x=mn;return x;}

void mnbrak(double &ax, double &bx, double &cx, double &fa, double &fb, double &fc,
			double mx, double mn, void (*fcn)(int *,double *, double*)) 
{ 
	const double GOLD=1.618034,GLIMIT=100.0,TINY=1.0e-20; 
	double ulim,u,r,q,fu; 
	double val;
	double thedlt;

	int n0=1;

	if(mx<mn)Swap(mx,mn);
	if(fabs(mx-mn)<=1.0e-9)
	{
		ax=bx=cx=(mx+mn)/2;
		fcn(&n0,&cx,&fc);
		fa=fb=fc;
		return;
	}

	Confine(ax,mx,mn);
	Confine(bx,mx,mn);

	fcn(&n0,&ax,&fa); 
	fcn(&n0,&bx,&fb); 
	if (fb > fa)
	{ 
		Swap(ax,bx); 
		Swap(fb,fa); 
	} 
	cx=bx+GOLD*(bx-ax); 
	Confine(cx,mx,mn);
	fcn(&n0,&cx,&fc); 
	while (fb > fc)
	{ 
		r=(bx-ax)*(fb-fc); 
		q=(bx-cx)*(fb-fa); 
		//u=bx-((bx-cx)*q-(bx-ax)*r)/(2.0*SIGN(Max(fabs(q-r),TINY),q-r));
		//为了消除VC6的Internal Compiler Error,把上一句分解
		//理解，Internal Compiler Error是由于函数的多层嵌套调用造成的
		thedlt=fabs(q-r);
		u=bx-((bx-cx)*q-(bx-ax)*r)/(2.0*Sign(Max(thedlt,TINY),q-r)); 
		Confine(u,mx,mn);
		ulim=bx+GLIMIT*(cx-bx); 
		Confine(ulim,mx,mn);
		if ((bx-u)*(u-cx) > 0.0)
		{ 
			fcn(&n0,&u,&fu); 
			if (fu < fc)
			{ 
				ax=bx; 
				bx=u; 
				fa=fb; 
				fb=fu; 
				return; 
			}
			else if (fu > fb)
			{ 
				cx=u; 
				fc=fu; 
				return; 
			} 
			u=cx+GOLD*(cx-bx); 
			Confine(u,mx,mn);
			fcn(&n0,&u,&fu); 
		}
		else if ((cx-u)*(u-ulim) > 0.0)
		{ 
			fcn(&n0,&u,&fu);
			if (fu < fc)
			{ 
				//shft3(bx,cx,u,cx+GOLD*(cx-bx)); 
				val=cx+GOLD*(cx-bx);
				Confine(val,mx,mn);
				Shft3(bx,cx,u,val); 
				fcn(&n0,&u,&val);
				Shft3(fb,fc,fu,val); 
			} 
		}
		else if ((u-ulim)*(ulim-cx) >= 0.0)
		{ 
			u=ulim; 
			fcn(&n0,&u,&fu);
		}
		else
		{ 
			u=cx+GOLD*(cx-bx); 
			Confine(u,mx,mn);
			fcn(&n0,&u,&fu);
		} 
		Shft3(ax,bx,cx,u); 
		Shft3(fa,fb,fc,fu); 
	} 
} 

double brent(double ax, double bx, double cx,void (*fcn)(int *,double *, double*),
			 double tol, double &xmin, int maxiter, int &iflag) 
{ 
	iflag = 0; 
	const double CGOLD=0.3819660; 
	const double ZEPS=1.0e-20; 
	int iter; 
	double a,b,d=0.0,etemp,fu,fv,fw,fx; 
	double p,q,r,tol1,tol2,u,v,w,x,xm; 
	double e=0.0; 

	int n0=1;

	a=(ax < cx ? ax : cx); 
	b=(ax > cx ? ax : cx); 
	x=w=v=bx; 
	fcn(&n0,&x,&fx);
	fw=fv=fx;
	for (iter=0;iter<maxiter;iter++)
	{ 
		xm=0.5*(a+b); 
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS); 
		if (fabs(x-xm) <= (tol2-0.5*(b-a)))
		{ 
			xmin=x;
			return fx; 
		} 
		if (fabs(e) > tol1)
		{ 
			r=(x-w)*(fx-fv); 
			q=(x-v)*(fx-fw); 
			p=(x-v)*q-(x-w)*r; 
			q=2.0*(q-r); 
			if (q > 0.0) p = -p; 
				q=fabs(q); 
			etemp=e; 
			e=d; 
			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x)) 
				d=CGOLD*(e=(x >= xm ? a-x : b-x)); 
			else
			{ 
				d=p/q; 
				u=x+d; 
				if (u-a < tol2 || b-u < tol2) 
				d=Sign(tol1,xm-x); 
			} 
		}
		else
		{ 
			d=CGOLD*(e=(x >= xm ? a-x : b-x)); 
		} 
		u=(fabs(d) >= tol1 ? x+d : x+Sign(tol1,d)); 
		fcn(&n0,&u,&fu); 
		if (fu <= fx)
		{ 
			if (u >= x) a=x; else b=x; 
			Shft3(v,w,x,u); 
			Shft3(fv,fw,fx,fu); 
		}
		else
		{ 
			if (u < x) a=u; else b=u; 
			if (fu <= fw || w == x)
			{ 
				v=w; 
				w=u; 
				fv=fw; 
				fw=fu; 
			}
			else if (fu <= fv || v == x || v == w)
			{ 
				v=u; 
				fv=fu; 
			} 
		} 
	} 
	iflag = 16; 
	xmin=x; 
	return fx; 
} 

bool ExecuteBrent(void (*fcn)(int *,double *, double*), double mx,double mn, double vFrom, double vTo,double &vmin, double &fret, double ftol, int maxiter, int &iflag) 
{ 
	double cx, fa, fb, fc; 
	bool good=true;
	mnbrak(vFrom,vTo,cx,fa,fb,fc,mx,mn,fcn); 
	fret = brent(vFrom,vTo,cx,fcn, ftol,vmin, maxiter, iflag); 
	return iflag ? false : true; 
} 

}//Namespace endding

bool BoundedPowell(void (*fcn)(int *,double *, double*), int n, double *x, double *xlb, double *xub, double rhobeg, double rhoend,int maxfun,int &iflag)
{
	return NsCChart::BoundedPowell(fcn, n, x, xlb, xub, rhobeg, rhoend, maxfun, iflag);
}

bool ExecuteBrent(void (*fcn)(int *,double *, double*), double mx,double mn, double vFrom, double vTo,double &vmin, double &fret, double ftol, int maxiter, int &iflag)
{
	return NsCChart::ExecuteBrent(fcn, mx, mn, vFrom, vTo, vmin, fret, ftol, maxiter, iflag);
}

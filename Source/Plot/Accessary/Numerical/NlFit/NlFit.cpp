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
#include "nlfit.h"
#include <math.h>
#include <memory>

//残差计算程序
double remain(double *pA,int cnum,double *pX,double *pY,int no);
//偏导数计算程序
void derivative(double *pA,double *pDiff,int cnum,double *pX,int no);

static FcnRemain		*Fcn_NlFitRemain = remain;
static FcnDerivative	*Fcn_NlFitDerivative = derivative;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double sign(double val)
{
	if(val > 0.0)
		return 1.0;
	else if(val < 0.0)
		return -1.0;
	else
		return 0.0;
}

//用Gauss-Jordan消去法解线性方程组
//flag:0正常;1错误
void gaussj(double *a,int n,double *b,int &flag)
{
	int i,j,k,l,ll,irow,icol;
	double big,pivinv,dum;
    //int ipiv[50], indxr[50], indxc[50];
	int *ipiv=new int[n];
	int *indxr=new int[n];
	int *indxc=new int[n];
    for (j=0;j<=n-1;j++)
	{
        ipiv[j]=0;
    }
    for (i=0;i<=n-1;i++)
	{
        big=0.0;
        for (j=0;j<=n-1;j++)
		{
            if(ipiv[j]!=1)
			{
                for(k=0;k<=n-1;k++)
				{
					if(ipiv[k]==0)
					{
						if(fabs(a[j*n+k])>=big)
						{
							big=fabs(a[j*n+k]);
							irow=j;
							icol=k;
						}
						else if(ipiv[k]>1)
						{
							//cout<<"singular matrix";
							flag=1;
							return;
						}
					}
                }
            }
        }
        ipiv[icol]=ipiv[icol]+1;
        if(irow!=icol)
		{
            for(l=0;l<=n-1;l++)
			{
                dum=(a[irow*n+l]);
                a[irow*n+l]=a[icol*n+l];
                a[icol*n+l]=dum;
            }
            dum=b[irow];
            b[irow]=b[icol];
            b[icol]=dum;
		}
        indxr[i]=irow;
        indxc[i]=icol;
        if(a[icol*n+icol]==0.0)
		{
			//cout<< "singular matrix.";
			flag=1;
			return;
		}
        pivinv=1.0/(a[icol*n+icol]);
        a[icol*n+icol]=1.0;
        for(l=0;l<=n-1;l++)
		{
            a[icol*n+l]=a[icol*n+l]*pivinv;
        }
        b[icol]=b[icol]*pivinv;
        for(ll=0;ll<=n-1;ll++)
        {
            if(ll!=icol)
			{
                dum=a[ll*n+icol];
                a[ll*n+icol]=0.0;
                for(l=0;l<=n-1;l++)
				{
                    a[ll*n+l]=a[ll*n+l]-a[icol*n+l]*dum;
                }
                b[ll]=b[ll]-b[icol]*dum;
            }
        }
	}	
    for(l=n-1;l<=0;l--)
	{
        if(indxr[l]!=indxc[l])
		{
            for(k=0;k<=n-1;k++)
			{
                dum=a[k*n+indxr[l]];
                a[k*n+indxr[l]]=a[k*n+indxc[l]];
                a[k*n+indxr[l]]=dum;
            }
        }
    }

	delete []indxc;
	delete []indxr;
	delete []ipiv;

	flag=0;
	return;
}

//残差平方和计算程序
//pA:待求参数
//cnum:待求参数个数
//pX:自变量
//pY:因变量
//len:数据个数
//pErr:残差
double sum(double *pA,int cnum,double *pX,double *pY,int len,double *pErr)
{
	int i;
	double q,err;
	err=0.0;
	for(i=0;i<len;i++)
	{
		q=(*Fcn_NlFitRemain)(pA,cnum,pX,pY,i);
		pErr[i]=q;
		err+=q*q;
	}
	return err;
}

//非线性拟和
//pA:待求参数,程序开始时输入猜测值,结束时输出拟和值
//cnum:待求参数个数
//pX:自变量
//pY:因变量
//len:数据个数
//err:允许误差
//iter:允许迭代次数
//method:拟和方法
//0:牛顿法;1:麦夸脱法
//fail:迭代成功标志
//0:计算成功;－1:超过最大迭代次数;-2:无法继续迭代;-3:Gauss消去失败
void nlfit(double *pA, double *pAbsErr, int cnum, double *pX, double *pY, int len,
		   double &err, int iter, int method, int &fail)
{
	double *pA0=new double[cnum];//代求系数临时存储
	double *matrix=new double[cnum*cnum];//方程组系数矩阵
	double *matrix0=new double[cnum*cnum];//方程组系数矩阵初始值
	double *b=new double[cnum];//方程组右端系数
	double *b0=new double[cnum];//方程组右端系数初始值
	double *delta=new double[cnum];//方程组的解
	double *pErr=new double[len];//储存各个点的误差
	double *pDiff=new double[cnum];//储存偏导数

	int i,j,k;//循环变量

	double d,d0;//麦夸脱法因子,d=0即牛顿法
	double alpha,c=10.0;//d变化因子

	double q,q0,dq;//残差平方和
	int flag;//高斯消去法成功标志

	fail=0;//拟和失败标志

	//确定初始的系数向量
	for(i=0;i<cnum;i++)
	{
		pA0[i]=pA[i];
	}

	//确定初始麦夸脱因子
	if(method==0)d0=0;
	else d0=0.01;

	int it=0;//外循环次数
	int it0;//内循环次数

	//计算残差平方和，pErr中返回各个数据点的残差
	q=sum(pA,cnum,pX,pY,len,pErr);
	q0=q;

	if(method==0)//采用牛顿法
	do
	{
		it++;
		if(it>iter)//超过最大循环次数，退出
		{
			fail=-1;
			break;
		}
		//计算方程组的系数矩阵
		for(j=0;j<cnum;j++)
		{
			for(i=0;i<cnum;i++)
			{
				matrix0[j*cnum+i]=0;
			}
			b0[j]=0;
		}
		for(k=0;k<len;k++)//对各个数据点循环
		{
			//首先产生偏导数向量
			(*Fcn_NlFitDerivative)(pA,pDiff,cnum,pX,k);
			for(j=0;j<cnum;j++)//对行循环
			{
				for(i=0;i<cnum;i++)//每行中循环
				{
					matrix0[j*cnum+i]+=pDiff[j]*pDiff[i];
				}
				b0[j]+=pErr[k]*pDiff[j];
			}
		}
		for(j=0;j<cnum;j++)
		{
			pA[j]=pA0[j];
			for(i=0;i<cnum;i++)
			{
				matrix[j*cnum+i]=matrix0[j*cnum+i];
			}
			b[j]=b0[j];
		}
		//解cnum元1次方程组
		gaussj(matrix,cnum,b,flag);
		if(flag==1)//高斯消元解方程组失败
		{
			fail=-3;
			break;
		}
		for(i=0;i<cnum;i++)
		{
			delta[i]=b[i];
			pA[i]+=delta[i];
		}
		q=sum(pA,cnum,pX,pY,len,pErr);//求出残差平方和，并在pErr中返回残差
		dq=q0-q;
		q0=q;
	}while( fabs(dq)>err && (fail==0) );

	else //采用麦夸脱法,外循环开始
	do
	{
		it++;
		if(it>iter)//超过最大循环次数，退出
		{
			fail=-1;
			break;
		}

		//计算方程组的系数矩阵
		for(j=0;j<cnum;j++)
		{
			for(i=0;i<cnum;i++)
			{
				matrix0[j*cnum+i]=0;
			}
			b0[j]=0;
		}
		for(k=0;k<len;k++)//对各个数据点循环
		{
			//首先产生偏导数向量
			(*Fcn_NlFitDerivative)(pA,pDiff,cnum,pX,k);
			for(j=0;j<cnum;j++)//对行循环
			{
				for(i=0;i<cnum;i++)//每行中循环
				{
					matrix0[j*cnum+i]+=pDiff[j]*pDiff[i];
				}
				b0[j]+=pErr[k]*pDiff[j];
			}
		}

		//内循环开始
		d=d0;
		alpha=-1;
		it0=0;
		do
		{
			it0++;
			if(it0>iter)//超过最大循环次数，退出
			{
				fail=-2;
				break;
			}
			for(j=0;j<cnum;j++)
			{
				pA[j]=pA0[j];
				for(i=0;i<cnum;i++)
				{
					matrix[j*cnum+i]=matrix0[j*cnum+i];
				}
				b[j]=b0[j];
			}
			//为方程组系数矩阵添加麦夸脱因子
			d=d0*pow(c,alpha);
			for(i=0;i<cnum;i++)
			{
				matrix[i*cnum+i]*=(1+d);
			}
			//解cnum元1次方程组
			gaussj(matrix,cnum,b,flag);
			if(flag==1)
			{
				fail=-3;
				break;
			}
			for(i=0;i<cnum;i++)
			{
				delta[i]=b[i];
				pA[i]+=delta[i];
			}
			q=sum(pA,cnum,pX,pY,len,pErr);//求出残差平方和，并在pErr中返回残差
			alpha++;//如果q>q0,只好增大d
		}while(q>q0);
		dq=q0-q;
		q0=q;
		d0=d;
		for(j=0;j<cnum;j++)
		{
			pA0[j]=pA[j];
		}
	}while( (dq>err) && (fail==0) );

	err = dq;
	memcpy(pAbsErr, delta, cnum*sizeof(double));
	
	delete []pDiff;
	delete []pErr;
	delete []b0;
	delete []b;
	delete []delta;
	delete []matrix0;
	delete []matrix;
	delete []pA0;
}

//残差计算程序
//函数：a1+a2*exp(-( (x-a3)/a4 )^2/2)
//pA:待求参数
//cnum:待求参数个数
//pX:自变量
//pY:因变量
//no:序号
double remain(double *pA,int cnum,double *pX,double *pY,int no)
{
	double a1,a2,a3,a4;
	double val,x,y;

	a1=pA[0];
	a2=pA[1];
	a3=pA[2];
	a4=pA[3];

	x=pX[no];
	y=pY[no];

	val=a1+a2*exp(-pow((x-a3)/a4,2)/2);

	return y-val;//这里必须这样减，val-y的话就不行
/*	double val,x,y;
	int i;

	x=pX[no];
	y=pY[no];

	val=pA[0];
	for(i=1;i<cnum;i+=3)
	{
		val+=pA[i]*exp(-pow((x-pA[i+1])/pA[i+2],2)/2);
	}
	return y-val;*/
}

//对系数的偏导数计算程序
//函数：a1+a2*exp(-( (x-a3)/a4 )^2/2)
//pA:待求参数
//pDiff:偏导数
//cnum:待求参数个数
//pX:自变量
//no:数据序号
void derivative(double *pA,double *pDiff,int cnum,double *pX,int no)
{
	double a1,a2,a3,a4;
	double x,tmp;

	a1=pA[0];
	a2=pA[1];
	a3=pA[2];
	a4=pA[3];

	x=pX[no];

	tmp=exp(-pow((x-a3)/a4,2)/2);

	pDiff[0]=1;
	pDiff[1]=tmp;
	pDiff[2]=a2*tmp*(x-a3)/(a4*a4);
	pDiff[3]=a2*tmp*(x-a3)*(x-a3)/(a4*a4*a4);

	return;
/*	double x,tmp;
	int i;

	x=pX[no];

	pDiff[0]=1;
	for(i=1;i<cnum;i++)
	{
		pDiff[i]=0;
	}
	for(i=1;i<cnum;i+=3)
	{
		tmp=exp(-pow((x-pA[i+1])/pA[i+2],2)/2);

		pDiff[i]+=tmp;
		pDiff[i+1]+=pA[i]*tmp*(x-pA[i+1])/(pA[i+2]*pA[i+2]);
		pDiff[i+2]+=pA[i]*tmp*(x-pA[i+1])*(x-pA[i+1])/(pA[i+2]*pA[i+2]*pA[i+2]);
	}

	return;*/
}

void	initfunc(FcnDerivative *der, FcnRemain *rem)
{
	Fcn_NlFitDerivative = der;
	Fcn_NlFitRemain = rem;
}
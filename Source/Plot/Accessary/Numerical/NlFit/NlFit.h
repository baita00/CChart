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

#ifndef __NLFIT_32167_H__
#define __NLFIT_32167_H__

//简单求符号
double sign(double val);
//用Gauss-Jordan消去法解线性方程组
void gaussj(double *a,int n,double *b,int &flag);
//残差平方和计算
double sum(double *pA,int cnum,double *pX,double *pY,int len,double *pErr);
//非线性拟和主程序
void nlfit(double *pA, double *pAbsErr, int cnum, double *pX, double *pY, int len, double &err, int iter, int method, int &fail);

typedef double	(FcnRemain)( double *pA,int cnum,double *pX,double *pY,int no );
typedef void	(FcnDerivative)( double *pA,double *pDiff,int cnum,double *pX,int no );
typedef	double	(FcnFitFunction)( double *, int, double );

void initfunc(FcnDerivative *der, FcnRemain *rem);

#endif
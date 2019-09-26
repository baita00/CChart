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

#ifndef __POWELL_32167_H__
#define __POWELL_32167_H__

namespace NsCChart{
//
//BOBYQA iflag返回值的含义：
//IFLAG=1, Return from BOBYQA because NPT is not in the required interval
//IFLAG=2, Return from BOBYQA because one of the differences XU(I)-XL(I)/6X is less than 2*RHOBEG.
//IFLAG=3, Return from BOBYQA because FCN has been called MAXFUN times.
//IFLAG=4, Return from BOBYQA because of much cancellation in a denominator.
//IFLAG=5, Return from BOBYQA because a trust region step has failed to reduce Q.
//

bool BoundedPowell(void (*fcn)(int *,double *, double*), int n, double *x, double *xlb, double *xub, double rhobeg, double rhoend,int maxfun,int &iflag);

bool ExecuteBrent(void (*fcn)(int *,double *, double*), double mx,double mn, double vFrom, double vTo,double &vmin, double &fret, double ftol, int maxiter, int &iflag);

}

bool BoundedPowell(void (*fcn)(int *,double *, double*), int n, double *x, double *xlb, double *xub, double rhobeg, double rhoend,int maxfun,int &iflag);

bool ExecuteBrent(void (*fcn)(int *,double *, double*), double mx,double mn, double vFrom, double vTo,double &vmin, double &fret, double ftol, int maxiter, int &iflag);


#endif
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

#ifndef __MYCONST_32167_H__
#define __MYCONST_32167_H__

const double myPi = 3.14159265358979323846264338328;
const double myDeg = myPi/180.0;
const double myC = 2.99792458e8;
const double myC2 = myC*myC;
const double myC3 = myC*myC*myC;
const double myQE = 1.6021892e-19;
const double myMu0 = 4.0*myPi*1e-7;
const double myEpsl0 = 1.0/(myMu0*myC2);

const double myMP0 = 1.6726485e-27;
const double myMN0 = 1.6749543e-27;
const double myMU0 = 1.6605402e-27;
const double myME0 = 9.109534e-31;
const double myMeV = 1.0e6*myQE;
const double myMP0InMeV = myMP0/myMeV;
const double myEP0 = myMP0*myC2;
const double myEU0 = myMU0*myC2;

const double myGravitation = 9.80665;

const double u_kg = 1.0;
const double u_g = u_kg*1.0e-3;
const double u_mg = u_g*1.0e-3;
const double u_ug = u_g*1.0e-6;
const double u_m = 1.0;
const double u_m2 = u_m*u_m;
const double u_m3 = u_m2*u_m;
const double u_km = 1000.0*u_m;
const double u_dm = 0.1*u_m;
const double u_cm = 0.01*u_m;
const double u_cm2 = u_cm*u_cm;
const double u_cm3 = u_cm2*u_cm;
const double u_mm = 0.001*u_m;
const double u_mm2 = u_mm*u_mm;
const double u_mm3 = u_mm2*u_mm;
const double u_um = 1.0e-6;
const double u_mrad = 0.001;
const double u_GeV = myMeV*1.0e3;
const double u_MeV = myMeV;
const double u_keV = myMeV*1.0e-3;
const double m_eV = myMeV*1.0e-6;
const double u_deg = myDeg;
const double u_mA = 0.001;
const double u_MHz = 1.0e6;
const double u_kHz = 1.0e3;
const double u_s = 1.0;
const double u_ms = 1.0e-3*u_s;
const double u_us = 1.0e-6*u_s;
const double u_ns = 1.0e-9*u_s;
const double u_ps = 1.0e-12*u_s;
const double u_fs = 1.0e-15*u_s;
const double u_tesla = 1.0;
const double u_gauss = 1.0e-4;
const double u_V = 1.0;
const double u_kV = 1.0e3;
const double u_MV = 1.0e6;

// error limit
const double myEpsl = 1.0e-18;

#endif
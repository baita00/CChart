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

#ifndef _COLORCONTROL_H
#define _COLORCONTROL_H

#include <windows.h>
#include "../../CChartNameSpace.h"

#define WM_GETCOLOR WM_USER+500
#define WM_SETCOLOR WM_USER+501

Declare_Namespace_CChart

ATOM RegisterColorControl();

Declare_Namespace_End

#endif
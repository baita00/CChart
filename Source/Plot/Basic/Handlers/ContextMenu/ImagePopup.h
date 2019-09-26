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

#include <windows.h>
#include "../../Global.h"

#include "MyMenuID.h"

Declare_Namespace_CChart

template<class PlotImplT>
void	CreateImagePopup(PlotImplT *plot, HMENU menu)
{
	HMENU subMenu;
	
	AppendMenu(menu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	subMenu = CreatePopupMenu();
	switch(CGlobal::m_nLangurage)
	{
	case kLangEnglish:
		AppendMenu(subMenu, MF_STRING, kMenuImage, _TEXT("&Copy to clipborad"));
		AppendMenu(subMenu, MF_STRING, kMenuImage+1, _TEXT("&Save to file..."));
		AppendMenu(subMenu, MF_STRING, kMenuImage+2, _TEXT("&Print screen..."));
		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT)subMenu, _TEXT("&Image"));
		break;
	case kLangChinese:
		AppendMenu(subMenu, MF_STRING, kMenuImage, _TEXT("拷贝到剪贴板(&C)"));
		AppendMenu(subMenu, MF_STRING, kMenuImage+1, _TEXT("保存到文件(&S)..."));
		AppendMenu(subMenu, MF_STRING, kMenuImage+2, _TEXT("打印屏幕图像(&P)..."));
		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT)subMenu, _TEXT("屏幕图像(&I)"));
		break;
	}	
	DestroyMenu(subMenu);
}

template<class PlotImplT>
int		InterpretImagePopupItem( PlotImplT *plot, HWND hWnd, int result )
{
	tstring filename, fileext, filepath;
	
	if( result == kMenuImage )
	{
		plot->MyLock();
		CopyImageToClipBrd(hWnd);
		plot->MyUnlock();
		return kPopupNoAction;
	}
	else if( result == kMenuImage + 1 )
	{
		plot->MyLock();	
		HBITMAP hBm = HBitmapFromWnd(hWnd);
		if(!hBm)
		{
			plot->MyUnlock();
			return kPopupNoAction;
		}
		if( !ShowFileDialog(hWnd, false, filename, fileext, filepath, _TEXT("Bitmap Files(*.bmp)\0*.BMP\0Png Files(*.png)\0*.PNG\0Jpeg Files(*.jpg)\0*.JPG\0Tiff Files(*.tif)\0*.TIF\0Gif Files(*.gif)\0*.GIF\0All Files(*.*)\0*.*\0\0")) )
		{
			DeleteObject(hBm);
			plot->MyUnlock();
			return kPopupNoAction;
		}
		SaveHBitmapToFile(hBm, filename, fileext);
		DeleteObject(hBm);
		plot->MyUnlock();
		return kPopupNoAction;
	}
	else if( result == kMenuImage + 2 )
	{
		if( ShowPrintDialog(hWnd, false) )
			return kPopupUpdate;
		else
			return kPopupNoAction;
	}
	return kPopupNoAction;
}

template<class PlotImplT>
bool	IsImageItem(PlotImplT *plot, int result)
{
	return ( result >= kMenuImage && result < kMenuImage + nMenuImage);
}

Declare_Namespace_End

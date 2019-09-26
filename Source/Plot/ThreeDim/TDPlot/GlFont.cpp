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
#include "GlFont.h"
#include "../../Basic/MyString.h"

#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>

using namespace NsCChart;

CGlFont::CGlFont()
{
	m_hFont=NULL;
	InitFont(_TEXT("宋体"));
	m_fExtrusion = 0.1f;
}

CGlFont::~CGlFont()
{
	DeleteMyFont();
}

LOGFONT CGlFont::GetLogFont()
{
	LOGFONT lf;
	GetObject(m_hFont,sizeof(LOGFONT),&lf);
	return lf;
}

void CGlFont::DeleteMyFont()
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont=NULL;
	}
}

bool CGlFont::InitFont(LOGFONT &lf)
{
	DeleteMyFont();
	m_hFont=CreateFontIndirect(&lf);
	if( m_hFont )return true;
	else return false;;
}

bool CGlFont::InitFont(const TCHAR *fontName)
{
	DeleteMyFont();
	m_hFont = CreateFont(
		-16,									//< lfHeight 字体高度
		0,										//< lfWidth 字体宽度 
		0,										//< lfEscapement 字体的旋转角度 Angle Of Escapement 
		0,										//< lfOrientation 字体底线的旋转角度Orientation Angle 
		FW_BOLD,								//< lfWeight 字体的重量 
		FALSE,									//< lfItalic 是否使用斜体 
		FALSE,									//< lfUnderline 是否使用下划线 
		FALSE,									//< lfStrikeOut 是否使用删除线 
		GB2312_CHARSET,							//< lfCharSet 设置字符集 
		OUT_TT_PRECIS,							//< lfOutPrecision 输出精度 
		CLIP_DEFAULT_PRECIS,					//< lfClipPrecision 裁剪精度 
		ANTIALIASED_QUALITY,					//< lfQuality 输出质量 
		FF_DONTCARE|DEFAULT_PITCH,				//< lfPitchAndFamily Family And Pitch 
		fontName);								//< lfFaceName 字体名称 
	if(m_hFont)return true;
	else return false;
}

// 在指定位置输出字符串 
void CGlFont::Draw2DTextByGdi(const TCHAR *str, float x, float y)
{
	if(_tcslen(str)<=0)return;

	HBITMAP hBitmap,hOldBmp;									//< 定义两个位图句柄 
	BITMAP bm;													//< 位图结构变量 
	SIZE size;													//< 位图尺寸 

	GLboolean lp,tp,dp;
	HDC hDC = ::CreateCompatibleDC(NULL);						//< 暂存设备场景 
	glGetBooleanv(GL_LIGHTING,&lp);								//< 查看场景中是否有光照 
	glGetBooleanv(GL_TEXTURE_2D,&tp);							//< 查看场景中是否启用纹理 
	glGetBooleanv(GL_DEPTH_TEST,&dp);							//< 查看场景中是否启用纹理 
	// 保存和设置一些属性 
	//glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(0,0,-10.0f);
	glDisable(GL_LIGHTING);										//< 关闭光照 
	glDisable(GL_TEXTURE_2D);									//< 关闭纹理 
	glDisable(GL_DEPTH_TEST);									//< 关闭深度测试 

	HFONT hOldFont=NULL;
	hOldFont=(HFONT)SelectObject(hDC, m_hFont);					//< 选择字体 
	if(!hOldFont)
	{
		return;
	}
	::GetTextExtentPoint32(hDC, str, (int)_tcslen(str), &size);	//< 获取字符位图大小 
	hBitmap = CreateBitmap(size.cx, size.cy,1, 1, NULL);		//< 创建与hDC相关单色位图 
	hOldBmp = (HBITMAP)SelectObject(hDC,hBitmap);				//< 选择位图 

	SetBkColor(hDC, RGB(0, 0, 0));								//< 背景色为黑色 
	SetTextColor(hDC, RGB(255, 255, 255));						//< 字体颜色白色 
	SetBkMode(hDC, OPAQUE);										//< 用当前的背景颜色填充背景 
	TextOut(hDC, 0, 0, str, (int)_tcslen(str));					//< 输出文字到暂存hDC 
	
	SelectObject(hDC, hOldFont);

	// 获得相关位图数据结构 
	GetObject(hBitmap, sizeof(bm), &bm);
	size.cx = (bm.bmWidth + 31) & (~31);						//< 边缘对齐 
	size.cy = bm.bmHeight;
	int bufsize = size.cx * size.cy/8;							//< 图形数据长度 
	
	//　定义单色位图结构  
	struct
	{
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
	}bic;

	// 获取单色位图结构信息 
	BITMAPINFO *binf = (BITMAPINFO *)&bic;
	binf->bmiHeader.biSize = sizeof(binf->bmiHeader);			//< 修改结构信息 
	binf->bmiHeader.biWidth = bm.bmWidth;
	binf->bmiHeader.biHeight = bm.bmHeight;
	binf->bmiHeader.biPlanes = 1;
	binf->bmiHeader.biBitCount = 1;								//< 单色 
	binf->bmiHeader.biCompression = BI_RGB;						//< 颜色方式 
	binf->bmiHeader.biSizeImage = bufsize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed = 0;
	binf->bmiHeader.biClrImportant = 0;

	// 定义图形数据块  
	UCHAR* pBmpBits = new UCHAR[bufsize];    
	memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
	// 将设备无关数据保存在pBmpBits指向的数据块中 
	::GetDIBits(hDC, hBitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);

	// 显示字符串 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);						//< 指定像素存储模式 
	glRasterPos2f(x, y);										//< 定位 
	glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0, 0.0, pBmpBits);	//< 位图显示 

	delete []pBmpBits;											//< 删除指针 
	SelectObject(hDC, hOldBmp);									//< 恢复原来位图信息 
	DeleteObject(hBitmap);
	::DeleteDC(hDC);

	// 恢复一些属性 
	if(lp) glEnable(GL_LIGHTING);								//< 启用光照 
	if(tp) glEnable(GL_TEXTURE_2D);								//< 启用纹理 
	if(dp) glEnable(GL_DEPTH_TEST);								//< 启用深度测试 

	//glPopMatrix();
}

void CGlFont::Draw2DText(const TCHAR *str, float x, float y)
{
	if(_tcslen(str)<=0)return;

	//HDC hDC=wglGetCurrentDC();
	HDC hDC = ::CreateCompatibleDC(NULL);

	HFONT hOldFont = (HFONT)SelectObject(hDC,m_hFont);
	if(!hOldFont)
	{
		return;
	}

	//convert TCHAR to char
	string mbStr;
#if defined(_UNICODE) || defined(UNICODE)
	mywcs2mbs(str, mbStr);
#else
	mbStr = str;
#endif

	glRasterPos2f(x, y);

	DWORD dwChar;
	int ListNum;
	for(size_t i=0;i<strlen((char *)mbStr.c_str());i++)
	{
		//if(IsDBCSLeadByte(str[i]))
		if(IsDBCSLeadByte(mbStr.c_str()[i]))
		{
			dwChar=((unsigned char)mbStr.c_str()[i])*0x100+(unsigned char)mbStr.c_str()[i+1];
			i++;
		}
		else
		{
			dwChar=mbStr.c_str()[i];
		}
		ListNum=glGenLists(1);
		wglUseFontBitmaps(hDC,dwChar,1,ListNum);
		glCallList(ListNum);
		glDeleteLists(ListNum,1);
	}
	
	SelectObject(hDC,hOldFont);
	::DeleteDC(hDC);
}

float CGlFont::Draw3DText(const TCHAR *str)
{
	if(_tcslen(str)<=0)return 0.0;

	//
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	GLYPHMETRICSFLOAT pgmf[1];
	//HDC hDC=wglGetCurrentDC();
	HDC hDC = ::CreateCompatibleDC(NULL);

	//设置当前字体
	HFONT hOldFont = (HFONT)SelectObject(hDC,m_hFont);
	if(!hOldFont)
	{
		return 0.0;
	}

	//convert TCHAR to char
	string mbStr;
#if defined(_UNICODE) || defined(UNICODE)
	mywcs2mbs(str, mbStr);
#else
	mbStr = str;
#endif

	DWORD dwChar;
	int ListNum;

	float length = 0.0;
	for(size_t i=0;i<strlen((char *)mbStr.c_str());i++)
	{
		if(IsDBCSLeadByte(mbStr.c_str()[i]))
		{
			dwChar=((unsigned char)mbStr.c_str()[i])*0x100+(unsigned char)mbStr.c_str()[i+1];
			i++;
		}
		else
		{
			dwChar=mbStr.c_str()[i];
		}
		ListNum=glGenLists(1);
		
		wglUseFontOutlinesA(hDC,dwChar,1,ListNum,0.0,m_fExtrusion,WGL_FONT_POLYGONS,pgmf);
		glCallList(ListNum);
		
		//Get the length of this character
		length += pgmf[0].gmfCellIncX;
		glDeleteLists(ListNum,1);
	}
	
	SelectObject(hDC,hOldFont);
	::DeleteDC(hDC);

	//
	glPopAttrib();
	glPopMatrix();

	
	return length;
}

float CGlFont::Get3DLength(const TCHAR *str)
{
	if(_tcslen(str)<=0)return 0.0;
	
	GLYPHMETRICSFLOAT pgmf[1];
	//HDC hDC=wglGetCurrentDC();

	HDC hDC = ::CreateCompatibleDC(NULL);
	
	//设置当前字体
	HFONT hOldFont = (HFONT)SelectObject(hDC,m_hFont);
	if(!hOldFont)
	{
		return 0.0;
	}

	//convert TCHAR to char
	string mbStr;
#if defined(_UNICODE) || defined(UNICODE)
	mywcs2mbs(str, mbStr);
#else
	mbStr = str;
#endif

	DWORD dwChar;
	int ListNum;
	
	float length = 0.0;
	for(size_t i=0;i<strlen((char *)mbStr.c_str());i++)
	{
		if(IsDBCSLeadByte(mbStr.c_str()[i]))
		{
			dwChar=((unsigned char)mbStr.c_str()[i])*0x100+(unsigned char)mbStr.c_str()[i+1];
			i++;
		}
		else
		{
			dwChar=mbStr.c_str()[i];
		}
		ListNum=glGenLists(1);
		wglUseFontOutlinesA(hDC,dwChar,1,ListNum,0.0,m_fExtrusion,WGL_FONT_POLYGONS,pgmf);
		//Get the length of this character
		length += pgmf[0].gmfCellIncX;
		glDeleteLists(ListNum,1);
	}
	
	SelectObject(hDC,hOldFont);
	::DeleteDC(hDC);
	
	return length;
}

float	CGlFont::Get3DHeight()
{	
	GLYPHMETRICSFLOAT pgmf[1];
	//HDC hDC=wglGetCurrentDC();
	HDC hDC = ::CreateCompatibleDC(NULL);
	
	//设置当前字体
	HFONT hOldFont = (HFONT)SelectObject(hDC,m_hFont);
	if(!hOldFont)
	{
		return 0.0;
	}
	DWORD dwChar;
	int ListNum;
	
	float height;

	dwChar = 'a';

	ListNum=glGenLists(1);
	wglUseFontOutlinesA(hDC,dwChar,1,ListNum,0.0,m_fExtrusion,WGL_FONT_POLYGONS,pgmf);
	//Get the length of this character
	height = pgmf[0].gmfBlackBoxY;
	glDeleteLists(ListNum,1);
	
	SelectObject(hDC,hOldFont);
	::DeleteDC(hDC);
	
	return height;
}
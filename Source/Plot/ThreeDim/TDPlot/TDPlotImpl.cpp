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
#include <windows.h>
#include "TDPlotImpl.h"
#include "Axis3Dimpl.h"
#include "GlFont.h"

using namespace NsCChart;

CTDPlotImpl::CTDPlotImpl(HWND hWnd)
{
	AddAxes();
	InitializeOpenGL(hWnd);
}

CTDPlotImpl::~CTDPlotImpl()
{
	FinallizeOpenGL();
	DeleteAxes();
}

void	CTDPlotImpl::OnDraw()
{
	if(m_hWnd == 0)return;
	m_bRectConfined?OnDraw(m_hWnd, m_rctConfined):OnDraw(m_hWnd);
}

void	CTDPlotImpl::OnDraw( HWND hWnd )
{
	HDC hDC = GetDC(hWnd);
	RECT destRect;
	GetClientRect(hWnd,&destRect);
	
	OnDraw( hDC, destRect );
	
	ReleaseDC(hWnd, hDC);
}

void CTDPlotImpl::OnDraw( HWND hWnd, RECT destRect )
{
	HDC hDC = GetDC(hWnd);
	
	OnDraw( hDC, destRect );
	
	ReleaseDC(hWnd, hDC);
}

void CTDPlotImpl::OnDraw( HDC hDC )
{
	HWND hWnd = WindowFromDC(hDC);
	
	RECT destRect;
	GetClientRect(hWnd,&destRect);
	
	OnDraw(hDC, destRect);
}

void	CTDPlotImpl::OnDraw(HDC hDC, RECT destRect)
{
	FinalDraw(hDC, destRect);
}

void	CTDPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_DEPTH_BUFFER_BIT);

	double xRange[2], yRange[2], zRange[2];
	GetPlotRange(xRange, yRange, zRange);

	SetProjection( abs(Width(destRect)), abs(Height(destRect)) );
	EraseBkgnd(hDC, destRect);
	InitialTransform();
	DrawFrame();
	DrawAxisLabel();
	DrawAxisTick(xRange, yRange, zRange);
	DrawGrids(xRange, yRange, zRange);
	
	if(m_bTitleShow)DrawTitle();
	
	m_rctLastClientRect = destRect;
}

void	CTDPlotImpl::Update(HDC hDC, RECT destRect)
{
	OnDraw(hDC, destRect);
}

void	CTDPlotImpl::EraseBkgnd( HDC hDC, RECT destRect )
{
	glClearColor(m_fvBkgndColor[0], m_fvBkgndColor[1], m_fvBkgndColor[2], 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void	CTDPlotImpl::SetProjection(int cx, int cy)
{
	int	w = cx;
	int h = cy;
	
	GLfloat fRange = m_fProjectionRange;
	
	if(h == 0) h = 1;
	if(w == 0) w = 1;
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
		if( w <= h )
			glOrtho(-m_pfLen[0]/m_fZoomFactor, m_pfLen[0]/m_fZoomFactor, -m_pfLen[1]/m_fZoomFactor*h/w, m_pfLen[1]/m_fZoomFactor*h/w, 1.0f, 1.0f+m_fProjectionRange);
		else
			glOrtho(-m_pfLen[0]/m_fZoomFactor*w/h, m_pfLen[0]/m_fZoomFactor*w/h, -m_pfLen[1]/m_fZoomFactor, m_pfLen[1]/m_fZoomFactor, 1.0f, 1.0f+m_fProjectionRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void	CTDPlotImpl::InitialTransform()
{
	glTranslatef(m_pfOffset[0], m_pfOffset[1], m_pfOffset[2]);
	glTranslatef(-m_pfLen[0]/2.0f, -m_pfLen[1]/2.0f, -1.0f-m_fProjectionRange/2.0f);
	glRotatef(m_pfAngle[0], 1.0f, 0.0f, 0.0f);
	glRotatef(m_pfAngle[2], 0.0f, 0.0f, 1.0f);	
	glRotatef(m_pfAngle[1], 0.0f, 1.0f, 0.0f);
}

void	CTDPlotImpl::DrawFrame()
{
	glPushMatrix();
	//Draw axes start and end points	
	glPointSize(10.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex3f(m_pfLen[0], 0.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex3f(0.0f, m_pfLen[1], 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
		glVertex3f(0.0f, 0.0f, m_pfLen[2]);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
//	
	// Draw the axis lines
	glBegin(GL_LINES);
	glColor4fv(m_fvFrameColor);
	// the three axis line
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(m_pfLen[0], 0.0f, 0.0f);
	
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, m_pfLen[1], 0.0f);
	
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, m_pfLen[2]);
	
	// others
	glVertex3f(m_pfLen[0], 0.0f, 0.0f);
	glVertex3f(m_pfLen[0], m_pfLen[1], 0.0f);
	
	glVertex3f(m_pfLen[0], 0.0f, 0.0f);
	glVertex3f(m_pfLen[0], 0.0f, m_pfLen[2]);
	
	glVertex3f(0.0f, m_pfLen[1], 0.0f);
	glVertex3f(m_pfLen[0], m_pfLen[1], 0.0f);
	
	glVertex3f(0.0f, m_pfLen[1], 0.0f);
	glVertex3f(0.0f, m_pfLen[1], m_pfLen[2]);
	
	glVertex3f(0.0f, 0.0f, m_pfLen[2]);
	glVertex3f(m_pfLen[0], 0.0f, m_pfLen[2]);
	
	glVertex3f(0.0f, 0.0f, m_pfLen[2]);
	glVertex3f(0.0f, m_pfLen[1], m_pfLen[2]);
	glEnd();
	
	// Draw the outer frame
	GLfloat color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4fv(m_fvAxisPlaneColor);
	GLfloat	lineWidth;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	glLineWidth(2.0f);
	if(m_bShowXYFrame)
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(m_pfLen[0], 0.0f, 0.0f);
			glVertex3f(m_pfLen[0], m_pfLen[1], 0.0f);
			glVertex3f(0.0f, m_pfLen[1], 0.0f);
		glEnd();
	}
	if(m_bShowYZFrame)
	{
		glNormal3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, m_pfLen[1], 0.0f);
			glVertex3f(0.0f, m_pfLen[1], m_pfLen[2]);
			glVertex3f(0.0f, 0.0f, m_pfLen[2]);
		glEnd();
	}
	if(m_bShowZXFrame)
	{
		glNormal3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, m_pfLen[2]);
			glVertex3f(m_pfLen[0], 0.0f, m_pfLen[2]);
			glVertex3f(m_pfLen[0], 0.0f, 0.0f);
		glEnd();
	}

	glLineWidth(lineWidth);
	glColor4fv(color);
	glPopMatrix();

//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
}

void	CTDPlotImpl::DrawAxisLabel()
{
	CGlFont glFont;
	GLfloat length, height;
	
	GLfloat color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4fv(m_fvAxisColor);
	// Draw "X"
	glPushMatrix();
	length = glFont.Get3DLength(GetAxis(1)->GetTitle().c_str());
	height = glFont.Get3DHeight();
	glTranslatef(m_pfLen[0], -length*1.5f, -height*2.0f);
	glScalef(m_fSF4AxisLabel, m_fSF4AxisLabel, m_fSF4AxisLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(GetAxis(0)->GetTitle().c_str());	
	glPopMatrix();
	
	// Draw "Y"
	glPushMatrix();
	glTranslatef(0.0f, m_pfLen[1]+length*0.2f, 0.0f);
	glScalef(m_fSF4AxisLabel, m_fSF4AxisLabel, m_fSF4AxisLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	
	glFont.Draw3DText(GetAxis(1)->GetTitle().c_str());
	glPopMatrix();
	
	// Draw "Z"
	glPushMatrix();
	length = glFont.Get3DLength(GetAxis(1)->GetTitle().c_str());
	height = glFont.Get3DHeight();
	glTranslatef(0.0f, 0.0f, m_pfLen[2]+height*0.2f);
	glScalef(m_fSF4AxisLabel, m_fSF4AxisLabel, m_fSF4AxisLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	
	glFont.Draw3DText(GetAxis(2)->GetTitle().c_str());
	glPopMatrix();

	glColor4fv(color);
}

void	CTDPlotImpl::DrawAxisTick( double *xRange, double *yRange, double *zRange )
{

	CGlFont glFont;

	GLfloat length, height;

	tostringstream ostr;
	tstring str;
	
	GLfloat color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4fv(m_fvTickLabelColor);
	
	// Draw "X1"
	glPushMatrix();
	ostr.str(_TEXT(""));
	ostr<<xRange[0];//<<_TEXT("(x)");
	str = ostr.str();
	height = glFont.Get3DHeight();
	glTranslatef(0.0f, m_pfLen[1], height*m_fSF4AxisTickLabel+height);
	glScalef(m_fSF4AxisTickLabel, m_fSF4AxisTickLabel, m_fSF4AxisTickLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(str.c_str());
	glPopMatrix();
	
	// Draw "X2"
	glPushMatrix();
	ostr.str(_TEXT(""));
	ostr<<xRange[1];//<<_TEXT("(x)");
	str = ostr.str();
	height = glFont.Get3DHeight();
	glTranslatef(m_pfLen[0], m_pfLen[1], height*m_fSF4AxisTickLabel+height);
	glScalef(m_fSF4AxisTickLabel, m_fSF4AxisTickLabel, m_fSF4AxisTickLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(str.c_str());
	glPopMatrix();

	// Draw "Y1"
	glPushMatrix();
	ostr.str(_TEXT(""));
	ostr<<yRange[0];//<<_TEXT("(y)");
	str = ostr.str();
	height = glFont.Get3DHeight();
	glTranslatef(m_pfLen[0], 0.0f, -height*m_fSF4AxisTickLabel-height);
	glScalef(m_fSF4AxisTickLabel, m_fSF4AxisTickLabel, m_fSF4AxisTickLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(str.c_str());
	glPopMatrix();

	// Draw "Y2"
	glPushMatrix();
	ostr.str(_TEXT(""));
	ostr<<yRange[1];//<<_TEXT("(y)");
	str = ostr.str();
	height = glFont.Get3DHeight();
	glTranslatef(m_pfLen[0], m_pfLen[1], -height*m_fSF4AxisTickLabel-height);
	glScalef(m_fSF4AxisTickLabel, m_fSF4AxisTickLabel, m_fSF4AxisTickLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(str.c_str());
	glPopMatrix();

	// Draw "Z1"
	glPushMatrix();
	ostr.str(_TEXT(""));
	ostr<<zRange[0];//<<_TEXT("(z)");
	str = ostr.str();
	length = glFont.Get3DLength(str.c_str());
	glTranslatef(m_pfLen[0], -length/2.0f, 0.0f);
	glScalef(m_fSF4AxisTickLabel, m_fSF4AxisTickLabel, m_fSF4AxisTickLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(str.c_str());
	glPopMatrix();

	// Draw "Z2"
	glPushMatrix();
	ostr.str(_TEXT(""));
	ostr<<zRange[1];//<<_TEXT("(z)");
	str = ostr.str();
	length = glFont.Get3DLength(str.c_str());
	glTranslatef(m_pfLen[0], -length/2.0f, m_pfLen[2]);
	glScalef(m_fSF4AxisTickLabel, m_fSF4AxisTickLabel, m_fSF4AxisTickLabel);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glFont.Draw3DText(str.c_str());
	glPopMatrix();

	glColor4fv(color);
}

void	CTDPlotImpl::DrawGrids( double *xRange, double *yRange, double *zRange )
{
	glPushMatrix();
	float lineWidth;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	glLineWidth(0.5f);
	DrawMajorGrids(xRange, yRange, zRange);
	DrawMinorGrids(xRange, yRange, zRange);
	glLineWidth(lineWidth);
	glPopMatrix();
}

void	CTDPlotImpl::DrawMajorGrids( double *xRange, double *yRange, double *zRange )
{
	if(!m_bShowXYMajorGrid && !m_bShowYZMajorGrid && !m_bShowZXMajorGrid)return;

	GLfloat x, y, z;
	int i;

	int nMajorTicks;

	glPushMatrix();
	GLfloat color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4fv(m_fvMajorGridColor);

	nMajorTicks = GetClsRange(0)->GetTickCount();
	for(i=1; i<=nMajorTicks; i++)
	{
		x = i * m_pfLen[0]/(nMajorTicks + 1);
		glBegin(GL_LINES);
		if(m_bShowXYMajorGrid)
		{
			glVertex3f(x, 0.0f, 0.0f);
			glVertex3f(x, m_pfLen[1], 0.0f);
		}
		if(m_bShowZXMajorGrid)
		{
			glVertex3f(x, 0.0f, 0.0f);
			glVertex3f(x, 0.0f, m_pfLen[2]);
		}
		
		glEnd();
	}
	nMajorTicks = GetClsRange(1)->GetTickCount();
	for(i=1; i<=nMajorTicks; i++)
	{
		y = i * m_pfLen[1]/(nMajorTicks + 1);
		glBegin(GL_LINES);
		if(m_bShowXYMajorGrid)
		{
			glVertex3f(0.0f, y, 0.0f);
			glVertex3f(m_pfLen[0], y, 0.0f);
		}
		if(m_bShowYZMajorGrid)
		{
			glVertex3f(0.0f, y, 0.0f);
			glVertex3f(0.0f, y, m_pfLen[2]);
		}
		glEnd();
	}
	if(m_bShowXYMajorGrid)
	{
		nMajorTicks = GetClsRange(2)->GetTickCount();
		for(i=1; i<=nMajorTicks; i++)
		{
			z = i * m_pfLen[2]/(nMajorTicks + 1);
			glBegin(GL_LINES);
			if(m_bShowZXMajorGrid)
			{
				glVertex3f(0.0f, 0.0f, z);
				glVertex3f(m_pfLen[0], 0.0f, z);
			}
			if(m_bShowYZMajorGrid)
			{
				glVertex3f(0.0f, 0.0f, z);
				glVertex3f(0.0f, m_pfLen[1], z);
			}
			glEnd();
		}
	}
	

	glColor4fv(color);
	glPopMatrix();
}

void	CTDPlotImpl::DrawMinorGrids( double *xRange, double *yRange, double *zRange )
{
	if(!m_bShowXYMinorGrid && !m_bShowYZMinorGrid && !m_bShowZXMinorGrid)return;

	GLfloat x, y, z;
	int i, j;
	
	glPushMatrix();
	GLfloat color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4fv(m_fvMinorGridColor);

	int nMajorTicks, nMinorTicks;

	nMajorTicks = GetClsRange(0)->GetTickCount();
	nMinorTicks = GetClsRange(0)->GetMinorTickCount();
	for(i=0; i<=nMajorTicks; i++)
	{
		for(j=1; j<=nMinorTicks; j++)
		{
			x = i * m_pfLen[0]/(nMajorTicks + 1) + j * m_pfLen[0]/(nMajorTicks + 1)/(nMinorTicks + 1);
			glBegin(GL_LINES);
			if(m_bShowXYMinorGrid)
			{
				glVertex3f(x, 0.0f, 0.0f);
				glVertex3f(x, m_pfLen[1], 0.0f);
			}
			if(m_bShowZXMinorGrid)
			{
				glVertex3f(x, 0.0f, 0.0f);
				glVertex3f(x, 0.0f, m_pfLen[2]);
			}
			glEnd();
		}
		
	}
	
	nMajorTicks = GetClsRange(1)->GetTickCount();
	nMinorTicks = GetClsRange(1)->GetMinorTickCount();
	for(i=0; i<=nMajorTicks; i++)
	{
		for(j=1; j<=nMinorTicks; j++)
		{
			y = i * m_pfLen[1]/(nMajorTicks + 1) + j * m_pfLen[1]/(nMajorTicks + 1)/(nMinorTicks + 1);
			glBegin(GL_LINES);
			if(m_bShowXYMinorGrid)
			{
				glVertex3f(0.0f, y, 0.0f);
				glVertex3f(m_pfLen[0], y, 0.0f);
			}
			if(m_bShowYZMinorGrid)
			{
				glVertex3f(0.0f, y, 0.0f);
				glVertex3f(0.0f, y, m_pfLen[2]);
			}
			glEnd();
		}
		
	}

	nMajorTicks = GetClsRange(2)->GetTickCount();
	nMinorTicks = GetClsRange(2)->GetMinorTickCount();
	for(i=0; i<=nMajorTicks; i++)
	{
		for(j=1; j<=nMinorTicks; j++)
		{
			z = i * m_pfLen[2]/(nMajorTicks + 1) + j * m_pfLen[1]/(nMajorTicks + 1)/(nMinorTicks + 1);
			glBegin(GL_LINES);
			if(m_bShowZXMinorGrid)
			{
				glVertex3f(0.0f, 0.0f, z);
				glVertex3f(m_pfLen[0], 0.0f, z);
			}
			if(m_bShowYZMinorGrid)
			{
				glVertex3f(0.0f, 0.0f, z);
				glVertex3f(0.0f, m_pfLen[1], z);
			}
			glEnd();
		}
		
	}
	
	glColor4fv(color);
	glPopMatrix();
}

void CTDPlotImpl::DrawTitle()
{
	CGlFont glFont;
	glFont.SetExtrusion(0.2f);
	
	double length = glFont.Get3DLength(m_strTitle.c_str());
	double height = glFont.Get3DHeight();
	
	GLfloat dy = (GLfloat)(m_pfLen[0]-length*m_fSF4Title)/2.0f;

	glPushMatrix();

	GLfloat color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4fv(m_fvTitleColor);

	//glTranslatef(dx, 0.0f, m_pfLen[2]*1.1f);
	glTranslatef(0.0f, dy, m_pfLen[2]+(GLfloat)height);
	glScalef(m_fSF4Title, m_fSF4Title, m_fSF4Title);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	glFont.Draw3DText(m_strTitle.c_str());

	glColor4fv(color);
	glPopMatrix();
}

void	CTDPlotImpl::AddAxes()
{
	DeleteAxes();
	
	m_pAxes[0] = new CAxis3DImpl(this);
	m_pAxes[0]->SetLocation(kLocationTDX);
	m_pAxes[0]->SetTitle(_TEXT("x"));
	m_pAxes[1] = new CAxis3DImpl(this);
	m_pAxes[1]->SetLocation(kLocationTDY);
	m_pAxes[1]->SetTitle(_TEXT("y"));
	m_pAxes[2] = new CAxis3DImpl(this);
	m_pAxes[2]->SetLocation(kLocationTDZ);
	m_pAxes[2]->SetTitle(_TEXT("z"));
}

void	CTDPlotImpl::DeleteAxes()
{
	int count = 3;//GetAxisCount();
	for(int i=0; i<count; i++)
	{
		if(m_pAxes[i])
		{
			delete m_pAxes[i];
			m_pAxes[0] = 0;
		}
	}
}

CAxis	*CTDPlotImpl::GetAxis(int which)
{
	int count = GetAxisCount();
	if(which<0 || which>=count)
		return 0;
	else
		return m_pAxes[which];
}

int CTDPlotImpl::GetAxisCount()
{
	return 3;
}
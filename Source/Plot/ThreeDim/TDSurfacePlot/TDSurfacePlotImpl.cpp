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
#include <math.h>
#include "TDSurfacePlotImpl.h"
#include "../../Accessary/Numerical/Powell/Powell.h"
#include "../../Accessary/Numerical/NlFit/NlFit.h"

using namespace NsCChart;

CTDSurfacePlotImpl::CTDSurfacePlotImpl(HWND hWnd) : CTDPlotImpl(hWnd)
{
	m_fTextureParams[0] = 0.0f;
	m_fTextureParams[1] = 0.0f;
	m_fTextureParams[2] = 2.0f;
	m_fTextureParams[3] = 0.0f;

	GetClsRange(0)->SetRange(-5, 5);
	GetClsRange(1)->SetRange(-5, 5);
	GetZRange();
}

CTDSurfacePlotImpl::~CTDSurfacePlotImpl()
{
}

void	CTDSurfacePlotImpl::GetDataRanges(double xRange[2], double yRange[2], double zRange[2])
{
	//memcpy(xRange, m_fXRange, 2*sizeof(double));
	//memcpy(yRange, m_fYRange, 2*sizeof(double));
	GetClsRange(0)->GetRange(xRange);
	GetClsRange(1)->GetRange(yRange);
	GetZRange();
	//memcpy(zRange, m_fZRange, 2*sizeof(double));
	GetClsRange(2)->GetRange(zRange);
}

void	CTDSurfacePlotImpl::SetContourRange(double xRange[2], double yRange[2])
{
	//m_fXRange[0] = xRange[0];
	//m_fXRange[1] = xRange[1];
	//m_fYRange[0] = yRange[0];
	//m_fYRange[1] = yRange[1];
	GetClsRange(0)->SetRange(xRange);
	GetClsRange(0)->SetRangeSet(true);
	GetClsRange(1)->SetRange(xRange);
	GetClsRange(1)->SetRangeSet(true);
	GetZRange();
}

void	CTDSurfacePlotImpl::SetContourRange(double xl, double xu, double yl, double yu)
{
	double xRange[2], yRange[2];
	xRange[0] = xl; xRange[1] = xu;
	yRange[0] = yl; yRange[1] = yu;
	SetContourRange(xRange, yRange);
}

void	CTDSurfacePlotImpl::GetZRange()
{
	int iflag;
	double x[2],xlb[2],xub[2];
	xlb[0] = GetClsRange(0)->GetRange()[0];
	xub[0] = GetClsRange(0)->GetRange()[1];
	xlb[1] = GetClsRange(1)->GetRange()[0];
	xub[1] = GetClsRange(1)->GetRange()[1];
	//xlb[1] = m_fYRange[0];
	//xub[0] = m_fXRange[1];
	//xub[1] = m_fYRange[1];
	double zl, zu;
	BoundedPowell(f4min,2,x,xlb,xub,1.0,1e-6,10000,iflag);
	zl = m_pFieldFcn(x[0],x[1]);
	BoundedPowell(f4max,2,x,xlb,xub,1.0,1e-6,10000,iflag);
	zu = m_pFieldFcn(x[0],x[1]);
	GetClsRange(2)->SetOptimalRange(zl, zu);
	GetClsRange(2)->SetRangeSet(true);
}

void	CTDSurfacePlotImpl::GetPlotRange( double *xRange, double *yRange, double *zRange )
{
	GetDataRanges(xRange, yRange, zRange);
	
	double *pRange;
	double optRange[2];
	int	optNTicks, optMinorTicks;
	for(int i=0; i<3; i++)
	{
		if(i==0)pRange = xRange;
		else if(i==1)pRange = yRange;
		else pRange = zRange;

		if(pRange[0]==pRange[1])
		{
			if(pRange[0]==0.0)
			{
				pRange[0]-=0.5;
				pRange[1]+=0.5;
			}
			else
			{
				pRange[0]-=pow(10,floor(log10(fabs(pRange[0]))));;
				pRange[1]+=pow(10,floor(log10(fabs(pRange[0]))));;
			}
		}
		GetOptimalRangeExact( pRange, optRange, &optNTicks, &optMinorTicks );

		memcpy(pRange, optRange, 2*sizeof(double));
		GetClsRange(i)->SetRange(pRange[0],pRange[1]);
		GetClsRange(i)->SetTickCount(optNTicks);
		GetClsRange(i)->SetMinorTickCount(optMinorTicks);
	}
}

void	CTDSurfacePlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	double xRange[2], yRange[2], zRange[2];
	GetPlotRange(xRange, yRange, zRange);

	wglMakeCurrent( hDC, m_hGLRC );
	glPushMatrix();

	// Draw the basics
	CTDPlotImpl::FinalDraw( hDC, destRect );
	
	// Draw the Datasets
	InitEnv();
	DrawSurface(xRange, yRange, zRange);
	glFlush();
	SwapBuffers( hDC );
	ResetEnv();

	glPopMatrix();
	wglMakeCurrent(	hDC, NULL);
	
	m_rctLastClientRect = destRect;
}

void	CTDSurfacePlotImpl::DrawSurface( double *xRange, double *yRange, double *zRange )
{
	double xScale, yScale, zScale;
	double x, y, z;
	xScale = m_pfLen[0]/(xRange[1] - xRange[0]);
	yScale = m_pfLen[1]/(yRange[1] - yRange[0]);
	zScale = m_pfLen[1]/(zRange[1] - zRange[0]);

	GLfloat *CtrlPoints = new GLfloat[m_nXSteps*m_nYSteps*3];
	int i,j;
	for(i=0; i<m_nYSteps; i++)
	{
		for(j=0; j<m_nXSteps; j++)
		{
			x = xRange[0] + j*(xRange[1] - xRange[0])/m_nXSteps;
			y = yRange[0] + i*(yRange[1] - yRange[0])/m_nYSteps;
			z = m_pFieldFcn(x,y);

			CtrlPoints[3*(m_nXSteps*i+j)+0] = j*m_pfLen[0]/m_nXSteps;
			CtrlPoints[3*(m_nXSteps*i+j)+1] = i*m_pfLen[1]/m_nYSteps;
			CtrlPoints[3*(m_nXSteps*i+j)+2] = (GLfloat)(zScale*(z - zRange[0]));
		}
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3*m_nYSteps, m_nXSteps, 0.0f, 1.0f, 3, m_nYSteps, CtrlPoints);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(m_nXMapSteps,0.0f,1.0f,m_nYMapSteps,0.0f,1.0f);
	glEvalMesh2(GL_LINE,0,m_nXMapSteps,0,m_nYMapSteps);
	glEvalMesh2(GL_FILL,0,m_nXMapSteps,0,m_nYMapSteps);
	glDisable(GL_MAP2_VERTEX_3);

	delete []CtrlPoints;
}

double		CTDSurfacePlotImpl::GetStepValue(int index, double v1, double v2, int nSegs, bool bExponential)
{
	if(v1 == v2)return v1;

	if(!m_bExponential)
	{
		return v1+index*(v2-v1)/nSegs;
	}
	else
	{
		if(v1!=0.0)
			return v1*pow(pow(fabs(v2/v1), 1.0/nSegs)*sign(v2/v1),index);
		else
			return sign(v2)*exp(log(fabs(v2))/nSegs*index);
	}
}

void	CTDSurfacePlotImpl::InitEnv()
{
	GLfloat light_diffuse[]= { 0.0f, 0.0f, 1.0f, 1.0f};   /* 蓝色 */	
	//GLfloat light_ambient[] = {0.9f, 0.6f, 0.8f, 1.0f};
	GLfloat light_position[4];
	
	GLfloat mat1_ambient[]= { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat1_diffuse[]= { 0.8f, 0.0f, 0.8f, 1.0f };    /* 紫色 */
    GLfloat mat1_specular[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    GLfloat mat1_shininess[] = { 50.0f };

	GLfloat mat2_ambient[]= { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat2_diffuse[]= { 0.1f, 0.5f, 0.4f, 1.0f };    /* 紫色 */
    GLfloat mat2_specular[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    GLfloat mat2_shininess[] = { 50.0f };
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	light_position[0] = m_pfLen[0];
	light_position[1] = m_pfLen[1];
	light_position[2] = m_pfLen[2];
	light_position[3] = 1.0f;
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	light_position[0] = 0.0f;
	light_position[1] = 0.0f;
	light_position[2] = 0.0f;
	light_position[3] = 1.0f;
	//glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat1_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat2_ambient);
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat2_diffuse);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat2_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat2_shininess);
	
	glPointSize(5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex3fv(light_position);
	glEnd();

	//
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	//
	double xRange[2], yRange[2], zRange[2];
	GetDataRanges(xRange, yRange, zRange);
	m_fTextureParams[2] = float(1.0f/zRange[1]);
	
	BuildImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, 3, nTextureWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, m_mImage);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, m_fTextureParams);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_1D);
}

void	CTDSurfacePlotImpl::ResetEnv()
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	//
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_NORMALIZE);

	//
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_1D);
}

void	CTDSurfacePlotImpl::BuildImage()
{
	for(int j=0; j<nTextureWidth; j++)
	{
		m_mImage[3*j+0] = 3*j;
		m_mImage[3*j+1] = 255-3*j;
		m_mImage[3*j+2] = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////
//

CTDSurfacePlotImplI::CTDSurfacePlotImplI( HWND hWnd) : CTDSurfacePlotImpl(hWnd)
{
	
}

CTDSurfacePlotImplI::~CTDSurfacePlotImplI()
{
	
}
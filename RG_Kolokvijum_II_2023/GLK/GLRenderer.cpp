#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include"DImage.h"
#include<corecrt_math_defines.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0.7f, 0.85f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//glShadeModel(GL_SMOOTH);
	//glDisable(GL_LIGHTING);

	m_texTop = LoadTexture("res\\top.jpg");
	m_texBot = LoadTexture("res\\bot.jpg");
	m_texSide = LoadTexture("res\\side.jpg");
	m_texLamp = LoadTexture("res\\lamp.jpg");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 20, 50, 0, 30, 0, 0, 1, 0);

	double cubeSide = 40;


	DrawAxes();

	glColor3f(1, 1, 1);

	glPushMatrix();
	{
		glTranslatef(0, cubeSide, 0);
		DrawEnvCube(80);
	}
	glPopMatrix();

	DrawLamp();

	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double aspect = (double)w / (double)h;

	gluPerspective(50, aspect, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}

	glDeleteTextures(1, &m_texTop);
	glDeleteTextures(1, &m_texBot);
	glDeleteTextures(1, &m_texSide);
	glDeleteTextures(1, &m_texLamp);
}

void CGLRenderer::DrawAxes() {
	float axisLength = 50;

	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(axisLength, 0, 0);

		glColor3f(0, 1.0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, axisLength, 0);

		glColor3f(0, 0, 1.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, axisLength);
	}
	glEnd();
}

UINT CGLRenderer::LoadTexture(char* fileName) {
	UINT textureID = 0;
	DImage img;

	if (img.Load(CString(fileName))) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
					      GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return textureID;
}

void CGLRenderer::DrawEnvCube(double a) {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glPushMatrix(); //front
	{
		glTranslatef(0, 0, -a);
		DrawTexQuad(a, m_texSide);
	}
	glPopMatrix();

	glPushMatrix(); //back
	{
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, 0, -a);

		DrawTexQuad(a, m_texSide);
	}
	glPopMatrix();

	glPushMatrix(); //right
	{
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, 0, -a);

		DrawTexQuad(a, m_texSide);
	}
	glPopMatrix();

	glPushMatrix(); //left
	{
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, 0, -a);

		DrawTexQuad(a, m_texSide);
	}
	glPopMatrix();

	glPushMatrix(); //top
	{
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, -a);

		DrawTexQuad(a, m_texTop);
	}
	glPopMatrix();

	glPushMatrix(); //bot
	{
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0, 0, -a);

		DrawTexQuad(a, m_texBot);
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawTexQuad(double a, UINT textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(1, 0);
			glVertex3f(a, a, 0);

			glTexCoord2f(0, 0);
			glVertex3f(-a, a, 0);

			glTexCoord2f(0, 1);
			glVertex3f(-a, -a, 0);

			glTexCoord2f(1, 1);
			glVertex3f(a, -a, 0);
		}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0); //unbind
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen) {
	double angleStep = 2 * M_PI / nSeg;
	double t_bot, t_top;

	if (texMode == 0) {
		t_bot = 0;
		t_top = 0.5;
	}
	else {
		t_bot = 0.5;
		t_top = 1.0;
	}

	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSeg; ++i) {
			double angle = i * angleStep;
			double x = cos(angle);
			double z = sin(angle);

			double s = (double)i / nSeg;

			glTexCoord2f(s, t_bot);
			glVertex3f(r1 * x, 0, r1 * z);

			glTexCoord2f(s, t_top);
			glVertex3f(r2 * x, h, r2 * z);
		}
	}
	glEnd();

	if (bIsOpen)
		return;

	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(0.5, 0.25);
		glVertex3f(0, 0, 0);

		for (int i = 0; i <= nSeg; ++i) {
			double angle = i * angleStep;

			double x = cos(angle);
			double z = sin(angle);

			double s = 0.5 + 0.25 * x;
			double t = 0.25 + 0.25 * z;

			glTexCoord2f(s, t);
			glVertex3f(x*r1, 0, z*r1);
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(0.5, 0.25);
		glVertex3f(0, h, 0);

		for (int i = 0; i <= nSeg; ++i) {
			double angle = -i * angleStep;

			double x = cos(angle);
			double z = sin(angle);

			double s = 0.5 + 0.25 * x;
			double t = 0.25 + 0.25 * z;

			glTexCoord2f(s, t);
			glVertex3f(x * r2, h, z * r2);
		}
	}
	glEnd();
}

void CGLRenderer::DrawLampBase() {
	double r1 = 8, r2 = 7, h = 2;
	int nSeg = 16;

	DrawCylinder(r1, r2, h, nSeg, 0, false);
}

void CGLRenderer::DrawLampArm() {
	double r1 = 3, h1 = 2;
	double r2 = 1, h2 = 15;
	int nSeg = 16;

	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, -h1 / 2, 0);
		DrawCylinder(r1, r1, h1, nSeg, 0, false);

		glRotatef(90, -1, 0, 0);		
	}
	glPopMatrix();

	glPushMatrix();
	{

		glTranslatef(0, r1 - h1/2, 0);
		DrawCylinder(r2, r2, h2, nSeg, 1, true);
	}
	glPopMatrix();

}

void CGLRenderer::DrawLampHead() {
	double r1 = 2, r2 = 3, r3 = 6;
	double h1 = 1, h2 = 2, h3 = 5;
	int nSeg = 16;

	glPushMatrix();
	{
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0, -h2/2, 0);
		DrawCylinder(r1, r1, h2, nSeg, 0, false);

		glRotatef(90, 1, 0, 0);
		glTranslatef(-2*r1, -2*h1, 0);
		DrawCylinder(r2, r2, h3, nSeg, 1, true);

		glTranslatef(0, h3, 0);
		DrawCylinder(r2, r3, h3, nSeg, 0, true);

		glTranslatef(0, -h3-h1, 0);
		//glRotatef(90, 1, 0, 0);
		DrawCylinder(r1, r2, h1, nSeg, 1, false);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLamp() {
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, m_texLamp);
		{
			glTranslatef(0, 2, 0);
			DrawLampBase();

			glTranslatef(0, 3, 0);
			glRotatef(m_arm1Angle, 0, 0, 1);
			DrawLampArm();

			//glRotatef(m_arm1Angle, 0, 0, 1);
			glTranslatef(0, 19, 0);
			glRotatef(-m_arm2Angle - m_arm1Angle, 0, 0, 1);
			DrawLampArm();

			glTranslatef(0, 19, 0);
			glRotatef(-m_arm2Angle - m_headAngle, 0, 0, 1);
			DrawLampHead();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}
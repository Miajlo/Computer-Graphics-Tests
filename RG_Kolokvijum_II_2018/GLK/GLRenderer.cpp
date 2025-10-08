#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include "corecrt_math_defines.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_cameraDistance = 10.0;
	m_cameraAngleY = 0.0;
	m_cameraAngleX = 45.0;
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	m_texBack = LoadTexture("res\\back.jpg");
	m_texFront = LoadTexture("res\\front.jpg");
	m_texTop = LoadTexture("res\\top.jpg");
	m_texBot = LoadTexture("res\\bot.jpg");
	m_texLeft = LoadTexture("res\\left.jpg");
	m_texRight = LoadTexture("res\\right.jpg");
	m_texShipT1 = LoadTexture("res\\ShipT1.png");	

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double centerX = 0.0, centerY = 0.0, centerZ = 0.0;

	double radY = m_cameraAngleY * M_PI / 180.0;
	double radX = m_cameraAngleX * M_PI / 180.0;

	double eyeX = centerX + m_cameraDistance * sin(radX) * cos(radY);
	double eyeY = centerY + m_cameraDistance * cos(radX);
	double eyeZ = centerZ + m_cameraDistance * sin(radX) * sin(radY);

	gluLookAt(eyeX, eyeY, eyeZ,     // Eye position
		centerX, centerY, centerZ,  // Look at center [0,10,0]
		0, 1, 0);

	

	DrawAxis();

	glColor3f(1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, m_texShipT1);

	DrawShip();

	glBindTexture(GL_TEXTURE_2D, 0);

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

	gluPerspective(50, aspect, 0.1, 100);

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
}

void CGLRenderer::DrawAxis() {
	float axisLength = 10;

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
	UINT textID = 0;
	DImage img;

	if (img.Load(CString(fileName))) {

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &textID);
		glBindTexture(GL_TEXTURE_2D, textID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
			              GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	}

	return textID;
}


void CGLRenderer::DrawTriangle(float d1, float d2, float rep) {
	
	glBegin(GL_TRIANGLES);
	{
		glNormal3f(0, 0, 1);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(0, 0, 0);

		glTexCoord2f(rep, 0.0f);
		glVertex3f(d1, 0, 0);

		glTexCoord2f(0, rep);
		glVertex3f(d1, d2, 0);
	}
	glEnd();

}


void CGLRenderer::DrawShip() {
	const float d1 = 5.8f;
	const float d2 = 2.15f;
	const float angleY = 13.0f;    // degrees — tilt for top pair (around Z after orienting to "top")
	const float angleHyp = 4.75f;  // degrees — small tilt for bottom hypotenuse
	// local hypotenuse midpoint in triangle local space
	const float midX = d1 * 0.5f;
	const float midY = d2 * 0.5f;
	float rep = 1;
	// We'll place all hypotenuse midpoints at the same world origin (0,0,0).
	// If you want a seam offset, change seamOffset.
	const float seamOffset = 0.0f;

	glPushMatrix();
	{
		glRotatef(4.75, 0, 0, 1);
		glRotatef(-103, 1, 0, 0);

		DrawTriangle(d1, d2, rep);
	}
	glPopMatrix();

	glPushMatrix();
	{
		
	/*	glRotatef(-103, 1, 0, 0);*/
		glScalef(1.0f, -1.0f, 1.0f);
		glRotatef(4.75, 0, 0, 1);
		glRotatef(103, 1, 0, 0);

		DrawTriangle(d1, d2, 3);
	}
	glPopMatrix();

	glFrontFace(GL_CW);

	glPushMatrix();
	{
		glScalef(1.0f, 1.0f, -1.0f);  // Mirror in Z
		glRotatef(4.75, 0, 0, 1);
		glRotatef(-103, 1, 0, 0);
		DrawTriangle(d1, d2, 3);
	}
	glPopMatrix();

	// TRIANGLE 4 (mirrored in BOTH Y and Z)
	glPushMatrix();
	{
		glScalef(1.0f, -1.0f, -1.0f);  // Mirror in Y AND Z
		glRotatef(4.75, 0, 0, 1);
		glRotatef(103, 1, 0, 0);
		DrawTriangle(d1, d2, 3);
	}
	glPopMatrix();

	glFrontFace(GL_CCW);
}

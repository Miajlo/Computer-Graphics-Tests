#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include"DImage.h"
#include<corecrt_math_defines.h>
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
	glEnable(GL_CULL_FACE);          // turn on face culling
	glCullFace(GL_BACK);             // cull (don’t draw) back faces
	glFrontFace(GL_CCW);

	m_texFront = LoadTexture("res\\front.jpg");
	m_texBack = LoadTexture("res\\back.jpg");
	m_texLeft = LoadTexture("res\\left.jpg");
	m_texRight = LoadTexture("res\\right.jpg");
	m_texTop = LoadTexture("res\\top.jpg");
	m_texBot = LoadTexture("res\\bot.jpg");
	m_texSpider = LoadTexture("res\\spider.png");	
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

	gluLookAt(20, -5, 0, 0, 0, 0, 0, 1, 0);

	float frontSize = 100;
	float sphereR = 1.0f, eyeU = 0.75f, eyeV = 0.75f, eyeR = 0.25f;
	float coneR = 1.0f, leg1U = 0.5f, leg1V = 0.75f, leg1R = 0.25f, h = 5.0f;
	int nSeg = 32;

	glPushMatrix();
	glTranslatef(0, frontSize / 2 - 0.01f, 0);
	DrawEnvCube(frontSize);

	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, 10, 0);
		glBindTexture(GL_TEXTURE_2D, m_texSpider);

			//DrawSphere(sphereR, nSeg, eyeU, eyeV, eyeR);

			DrawCone(coneR, h, nSeg,leg1U, leg1V, leg1R);

		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	DrawAxis(10);

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

	gluPerspective(50, aspect, 0.1, 2000.0);

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

UINT CGLRenderer::LoadTexture(char* fileName) {
	UINT textID = 0;
	DImage img;

	if (img.Load(CString(fileName))) {

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &textID);
		glBindTexture(GL_TEXTURE_2D, textID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
						  GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	}
	return textID;
}

void CGLRenderer::DrawAxis(float length) {

	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, length, 0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0, 0, 0);
		glVertex3f(length, 0, 0);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, length);
	glEnd();
}

void CGLRenderer::DrawTextureQuad(float width, UINT textureID) {
	float halfW = width / 2, halfH = width / 2;
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(halfW, halfH, 0); // top-right
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-halfW, halfH, 0); // top-left
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-halfW, -halfH, 0); // bottom-left
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(halfW, -halfH, 0); // bottom-right
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR) {
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < nSeg; ++i) {
		double phi1 = M_PI * i / nSeg;
		double phi2 = M_PI * (i + 1) / nSeg;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= 2 * nSeg; ++j) {
			double theta = 2 * M_PI * j / (2 * nSeg);

			// Use the full texture range (0 to 1) for proper mapping
			double u = (double)j / (2.0 * nSeg);
			double v1 = (double)(i + 1) / (double)nSeg;
			double v2 = (double)i / (double)nSeg;

			double x1 = r * sin(phi2) * cos(theta);
			double y1 = r * sin(phi2) * sin(theta);
			double z1 = r * cos(phi2);

			glNormal3d(x1 / r, y1 / r, z1 / r);
			glTexCoord2d(u, v1);
			glVertex3d(x1, y1, z1);

			double x2 = r * sin(phi1) * cos(theta);
			double y2 = r * sin(phi1) * sin(theta);
			double z2 = r * cos(phi1);

			glNormal3d(x2 / r, y2 / r, z2 / r);
			glTexCoord2d(u, v2);
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR) {

	double angleStep = 2.0f * M_PI / nSeg;

	glBegin(GL_TRIANGLE_FAN);

		glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2d(texU + 0.5 * texR, texV + 0.5 * texR);
		glVertex3d(0, 0, 0);

		for (int i = 0; i <= nSeg; ++i) {
			double angle = i * angleStep;
			double x = r * cos(angle);
			double z = r * sin(angle);

			glNormal3f(0, -1.0, 0);
			glTexCoord2d(texU + (0.5 + 0.5 * cos(angle)) * texR,
				texV + (0.5 + 0.5 * sin(angle)) * texR);
			glVertex3d(x, 0.0, z);
		}

	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	
	glNormal3d(0.0, 1.0, 0.0);
	glTexCoord2d(texU + 0.5 * texR, texV + texR);
	glVertex3d(0.0, h, 0.0);
	
	for (int i = 0; i <= nSeg; i++) {
		double angle = i * angleStep;
		double x = r * cos(angle);
		double z = r * sin(angle);

		double slant = sqrt(r * r + h * h); 
		double nx = (h / slant) * cos(angle);
		double ny = r / slant; 
		double nz = (h / slant) * sin(angle);

		glNormal3d(nx, ny, nz);
		glTexCoord2d(texU + (double)i / nSeg * texR, texV);
		glVertex3d(x, 0.0, z);
	}
	glEnd();
}

void CGLRenderer::DrawEnvCube(double frontSize) {
	double hfSize = frontSize / 2.0f;
	glPushMatrix();
	glTranslatef(0, 0, -hfSize);
	
	DrawTextureQuad(frontSize, m_texFront);
	
	glPopMatrix();

	glPushMatrix(); //left

	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -hfSize);

	DrawTextureQuad(frontSize, m_texLeft);

	glPopMatrix();

	glPushMatrix(); //left

	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, 0, -hfSize);

	DrawTextureQuad(frontSize, m_texLeft);

	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -hfSize);

	DrawTextureQuad(frontSize, m_texBack);

	glPopMatrix();

	glPushMatrix();

	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -hfSize);	

	DrawTextureQuad(frontSize, m_texBot);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -hfSize);	

	DrawTextureQuad(frontSize, m_texTop);

	glPopMatrix();
}

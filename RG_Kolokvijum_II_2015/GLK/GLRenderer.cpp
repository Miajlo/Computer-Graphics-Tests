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

CGLRenderer::CGLRenderer(void) {
	m_texNames[(int)TextureIndeces::TSC0] = "res\\TSC0.jpg";
	m_texNames[(int)TextureIndeces::TSC1] = "res\\TSC1.jpg";
	m_texNames[(int)TextureIndeces::TSC2] = "res\\TSC2.jpg";
	m_texNames[(int)TextureIndeces::TSC3] = "res\\TSC3.jpg";
	m_texNames[(int)TextureIndeces::TSC4] = "res\\TSC4.jpg";
	m_texNames[(int)TextureIndeces::TSC5] = "res\\TSC5.jpg";

	m_texNames[(int)TextureIndeces::S0] = "res\\S0.jpg";
	m_texNames[(int)TextureIndeces::S1] = "res\\S1.jpg";
	m_texNames[(int)TextureIndeces::S2] = "res\\S2.jpg";
	m_texNames[(int)TextureIndeces::S3] = "res\\S3.jpg";
	m_texNames[(int)TextureIndeces::S4] = "res\\S4.jpg";
	m_texNames[(int)TextureIndeces::S5] = "res\\S5.jpg";

	m_texNames[(int)TextureIndeces::M0] = "res\\M0.jpg";
	m_texNames[(int)TextureIndeces::M1] = "res\\M1.jpg";
	m_texNames[(int)TextureIndeces::M2] = "res\\M2.jpg";
	m_texNames[(int)TextureIndeces::M3] = "res\\M3.jpg";
	m_texNames[(int)TextureIndeces::M4] = "res\\M4.jpg";
	m_texNames[(int)TextureIndeces::M5] = "res\\M5.jpg";
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

	for (int i = 0; i < (int)TextureIndeces::COUNT; ++i)
		m_texIDArray[i] = LoadTexture(m_texNames[i]);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float lightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };  // w=0 = direkciono svetlo
	float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // bela boja
	float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

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

	//gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	//UpdateCamera();
	glDisable(GL_LIGHTING);

	glBegin(GL_TRIANGLES);
	{
		glColor3f(0.01, 0, 0);
		glVertex3f(0, 0.01, 0);
		glVertex3f(-0.01, -0.005, 0);
		glVertex3f(0.01, -0.005, 0);
	}
	glEnd();
	glColor3f(1, 1, 1);
	double EarthR = 0.6731, n = 64;
	double moonR = 0.1737f, EarthMoonDistance = 38.4399f;
	double spaceR = 100;

	DrawSpace(spaceR, n);

	if (m_lightEnabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
	glPushMatrix();
	{
		UpdateCamera();

		DrawEarth(EarthR, n);

		glPushMatrix();
		{
			glTranslatef(0, 0, -EarthMoonDistance); 
			glRotatef(m_moonRotAngle, 0, 1, 0);
			DrawMoon(moonR, n);
		}
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();

	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	double aspect = (double)w / (double)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, aspect, 0.1, 200);

	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures((int)TextureIndeces::COUNT, m_texIDArray);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName) {
	UINT textureID = 0;
	DImage img;

	if (img.Load(CString(fileName))) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
							  GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return textureID;
}

void CGLRenderer::DrawPatch(double R, int n) {
	double step = 2.0 / n;

	for (int i = 0; i < n; ++i) {
		glBegin(GL_TRIANGLE_STRIP);
		{
			double x1 = -1 + i * step;
			double x2 = -1 + (i + 1) * step;
			for (int j = 0; j <= n; ++j) {
				double y = -1 + j * step;

				// PRVO TEME
				double phi1 = atan(x1);
				double theta1 = atan(y * cos(phi1));

				double X1 = R * cos(theta1) * sin(phi1);
				double Y1 = R * sin(theta1);
				double Z1 = R * cos(theta1) * cos(phi1);

				double s1 = (x1 + 1.0) / 2.0;
				double t1 = 1.0 - (y + 1.0) / 2.0;  // FLIP po Y!

				glNormal3d(X1 / R, Y1 / R, Z1 / R);
				glTexCoord2d(s1, t1);
				glVertex3d(X1, Y1, Z1);

				// DRUGO TEME
				double phi2 = atan(x2);
				double theta2 = atan(y * cos(phi2));

				double X2 = R * cos(theta2) * sin(phi2);
				double Y2 = R * sin(theta2);
				double Z2 = R * cos(theta2) * cos(phi2);

				double s2 = (x2 + 1.0) / 2.0;
				double t2 = 1.0 - (y + 1.0) / 2.0;  // FLIP po Y!

				glNormal3d(X2 / R, Y2 / R, Z2 / R);
				glTexCoord2d(s2, t2);
				glVertex3d(X2, Y2, Z2);
			}
		}
		glEnd();
	}
}

void CGLRenderer::DrawTexurePatch(double R, int n, UINT textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	{
		DrawPatch(R, n);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}


void CGLRenderer::UpdateCamera() {
	glTranslatef(0, 0, -m_cameraDistance);
	glRotatef(m_angleX, 1, 0, 0);
	glRotatef(m_angleY, 0, 1, 0);
}

void CGLRenderer::DrawEarth(double R, int tes) {
	DrawTextureSphere(R, tes, TextureIndeces::TSC0);
}

void CGLRenderer::DrawTextureSphere(double r, int tes, TextureIndeces start) {
	int startIndex = (int)start;

	// 4 patch-a oko ekvatora
	for (int i = 0; i < 4; ++i) {
		glPushMatrix();
		glRotatef(i * 90, 0, 1, 0);
		DrawTexurePatch(r, tes, m_texIDArray[startIndex + i]);
		glPopMatrix();
	}

	// Gornji pol
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	DrawTexurePatch(r, tes, m_texIDArray[startIndex + 4]);
	glPopMatrix();

	// Donji pol
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	DrawTexurePatch(r, tes, m_texIDArray[startIndex + 5]);
	glPopMatrix();
}

void CGLRenderer::DrawMoon(double R, int tes) {
	DrawTextureSphere(R, tes, TextureIndeces::M0);
}

void CGLRenderer::DrawSpace(double R, int tes) {
	DrawTextureSphere(R, tes, TextureIndeces::S0);
}



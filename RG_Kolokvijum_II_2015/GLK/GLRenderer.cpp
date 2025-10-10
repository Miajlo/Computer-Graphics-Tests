#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include"DImage.h"
#include<corecrt_math_defines.h>
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
	UpdateCamera();


	glBegin(GL_TRIANGLES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(-1, -0.5, 0);
		glVertex3f(1, -0.5, 0);
	}
	glEnd();
	glColor3f(1, 1, 1);
	double R = 1.3, n = 64;
	DrawEarth(R, n);

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

	gluPerspective(40, aspect, 0.1, 200);

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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

				double phi1 = (x1 + 1) * M_PI / 3;
				double theta1 = (y + 1) * M_PI / 3;

				double X1 = R * sin(theta1) * cos(phi1);
				double Y1 = R * sin(theta1) * sin(phi1);
				double Z1 = R * cos(theta1);


				glNormal3d(X1 / R, Y1 / R, Z1 / R);
				glTexCoord2d((x1 + 1.0) / 2.0, (y + 1.0) / 2.0);  // jednostavno mapiranje
				glVertex3d(X1, Y1, Z1);

				double phi2 = (x2 + 1.0) * M_PI / 3;
				double theta2 = (y + 1.0) * M_PI / 3.0;

				double X2 = R * sin(theta2) * cos(phi2);
				double Y2 = R * sin(theta2) * sin(phi2);
				double Z2 = R * cos(theta2);

				glNormal3d(X2 / R, Y2 / R, Z2 / R);
				glTexCoord2d((x2 + 1.0) / 2.0, (y + 1.0) / 2.0);
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

	for (int i = 0; i < 4; ++i) {
		glPushMatrix();
		glRotatef(i * 90, 0, 1, 0);  
		DrawTexurePatch(r, tes, m_texIDArray[startIndex + i]);
		glPopMatrix();
	}


	/*glPushMatrix();
	glRotatef(90, 1, 0, 0);   
	glRotatef(180, 0, 0, 1);  
	DrawTexurePatch(r, tes, m_texIDArray[startIndex + 4]);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); 
	DrawTexurePatch(r, tes, m_texIDArray[startIndex + 5]);
	glPopMatrix();*/
}



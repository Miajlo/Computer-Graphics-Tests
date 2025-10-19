#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include"DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void) {
	m_fileNames = {
		"res\\top.jpg",
		"res\\bot.jpg",
		"res\\front.jpg",
		"res\\back.jpg",
		"res\\left.jpg",
		"res\\right.jpg",
		"res\\excavator.png"
	};
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
	glClearColor(0.7, 0.85, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	for (auto i = 0; i < (int)TextureIndeces::LENGTH; ++i)
		m_textureArray[i] = LoadTexture(const_cast<char*>(m_fileNames[i].c_str()));
	

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

	float envCubeSize = 100;

	gluLookAt(0, -10, -10, 0, 0, 0, 0, 1, 0);

	DrawAxes(10);

	glColor3f(1, 1, 1); //clear color
	DrawEnvCube(envCubeSize);

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
	gluPerspective(55, aspect, 0.1, 200);

	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures((GLsizei)TextureIndeces::LENGTH, m_textureArray);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes(float length) {
	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(length, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, length, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, length);
	}
	glEnd();
}

UINT CGLRenderer::LoadTexture(char* fileName) {
	UINT textireID = 0;
	DImage img;

	if (img.Load(CString(fileName))) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &textireID);

		glBindTexture(GL_TEXTURE_2D, textireID);
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), 
							  GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return textireID;
}

void CGLRenderer::DrawEnvCube(double a) {
	float half = a / 2;		
	glEnable(GL_CULL_FACE);
	{
		// turn on face culling
		glCullFace(GL_BACK);             // cull (don’t draw) back faces
		glFrontFace(GL_CCW);
		glPushMatrix(); //front
		{
			glTranslatef(0, 0, -half);
			DrawTextureQuad(half, m_textureArray[(int)TextureIndeces::FRONT]);
		}
		glPopMatrix();

		glPushMatrix(); //back
		{
			glRotatef(180, 0, 1, 0);
			glTranslatef(0, 0, -half);
			DrawTextureQuad(half, m_textureArray[(int)TextureIndeces::BACK]);
		}
		glPopMatrix();

		glPushMatrix(); //top
		{
			glRotatef(90, 1, 0, 0);
			glTranslatef(0, 0, -half);
			DrawTextureQuad(half, m_textureArray[(int)TextureIndeces::TOP]);
		}
		glPopMatrix();

		glPushMatrix(); //top
		{
			glRotatef(-90, 1, 0, 0);
			glTranslatef(0, 0, -half);
			DrawTextureQuad(half, m_textureArray[(int)TextureIndeces::BOTTOM]);
		}
		glPopMatrix();

		glPushMatrix(); //left
		{
			glRotatef(90, 0, 1, 0);
			glTranslatef(0, 0, -half);
			DrawTextureQuad(half, m_textureArray[(int)TextureIndeces::LEFT]);
		}
		glPopMatrix();

		glPushMatrix(); //right
		{
			glRotatef(-90, 0, 1, 0);
			glTranslatef(0, 0, -half);
			DrawTextureQuad(half, m_textureArray[(int)TextureIndeces::RIGHT]);
		}
		glPopMatrix();
	}
	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawTextureQuad(double a, UINT textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex3f(-a, a, 0);

			glTexCoord2f(0, 1);
			glVertex3f(-a, -a, 0);

			glTexCoord2f(1, 1);
			glVertex3f(a, -a, 0);

			glTexCoord2f(1, 0);
			glVertex3f(a, a, 0);
		}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}


void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n) {
	if (!points || n < 3)
		return;

	bool hasTexture = texCoords != nullptr;

	glNormal3f(0.0f, 0.0f, 1.0f);
	

	glBegin(GL_TRIANGLE_FAN);
	{
		if (hasTexture)
			glTexCoord2f(texCoords[0].x, texCoords[0].y);

		glVertex2f(points[0].x, points[0].y);

		for (int i = 0; i < n; ++i) {
			if(hasTexture)
				glTexCoord2f(texCoords[i].x, texCoords[i].y);

			glVertex2f(points[i].x, points[i].y);
		}

		if (hasTexture)
			glTexCoord2f(texCoords[1].x, texCoords[1].y);
		glVertex2f(points[1].x, points[1].y);
	}
	glEnd();
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r,
	float g, float b)
{

	DrawPolygon(points, texCoords, n);

	glPushMatrix();
	{
		glTranslatef(0, 0, zh);
		DrawPolygon(points, texCoords, n);
	}
	glPopMatrix();

	glColor3f(r, g, b);

	glBegin(GL_QUADS);
	{
		for (auto i = 0; i < n; ++i) {
			int ni = (i + 1) % n;

			float x0 = points[i].x, y0 = points[i].y;
			float x1 = points[ni].x, y1 = points[ni].y;
			glVertex3f(x0, y0, 0.0f);

			glVertex3f(x1, y1, 0.0f);

			glVertex3f(x1, y1, zh);

			glVertex3f(x0, y0, zh);
		}
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void CGLRenderer::DrawBase() {
	//0,13

	POINTF points[8] = {

	};
}
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
	glDisable(GL_LIGHTING);
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


	glColor3f(1, 1, 1); //clear color
	glPushMatrix();
	{
		glRotatef(-m_cameraYAngle, 1, 0, 0);      // Vertical rotation
		glRotatef(-m_cameraXZAngle, 0, 1, 0);     // Horizontal rotation
		DrawEnvCube(100);
	}
	glPopMatrix();


	glTranslatef(0, -5, -m_cameraDistance);
	glRotatef(-m_cameraYAngle, 1, 0, 0);          // Pitch
	glRotatef(-m_cameraXZAngle, 0, 1, 0);         // Yaw

	DrawAxes(10);
	
	glColor3f(1, 1, 1); //clear color
	DrawExcavator();

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
	gluPerspective(55, aspect, 0.1, 2000);

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
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
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
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
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

	
	glDisable(GL_TEXTURE_2D);
	{
		glBegin(GL_QUADS);
		{
		
			glColor3f(r, g, b);
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
	}
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void CGLRenderer::DrawBase() {
	//0,13

	POINTF points[8] = {
		{-4, 0.5f},
		{-3.5, 1},
		{3.5f, 1},
		{4, 0.5f},
		{4, -0.5f},
		{3.5f, -1},
		{-3.5f, -1.0f},
		{-4.0f, -0.5f},
	};

	double part = 1.0 / 16.0;

	POINTF textureCoords[8] = {
		{0, 13*part},
		{1*part, 12 * part},
		{15*part, 12 * part},
		{16*part, 13 * part},
		{16*part, 15 * part},
		{15 * part, 16 * part},
		{1 * part, 16 * part},
		{0, 15 * part},
	};

	DrawExtrudedPolygon(points, textureCoords, 8, 5, 0, 0, 0);
}

void CGLRenderer::DrawBody() {
	//0.96, 0.5, 0.12

	POINTF points[5] = {
		{-2, 2},
		{0, 2},
		{2, 0},
		{2, -2},
		{-2, -2},
	};
	double part = 1.0 / 16.0;
	POINTF textureCoords[5] = {
		{8 * part, 0},
		{12 * part, 0},
		{16 * part, 4*part},
		{16 * part, 8 * part},
		{8 * part, 8 * part}
	};

	DrawExtrudedPolygon(points, textureCoords, 5, 4, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawArm(double zh) {


	POINTF points[6] = {
		{-4, 0.5f},
		{-3.5, 1},		
		{4, 0.5f},
		{4, -0.5f},
		{-3.5f, -1},
		{-4, -0.5}
	};

	double part = 1.0 / 16.0;

	POINTF textureCoords[6] = {
		{0, 9 * part},
		{part, 8 * part},
		{16 * part, 9 * part},
		{16 * part, 11 * part},
		{1 * part, 12 * part},
		{0, 11 * part}
	};

	DrawExtrudedPolygon(points, textureCoords, 6, zh, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawFork() {
	POINTF points[6] = {
		{-2, 1.75f},
		{-1.75, 2},
		{1.75, 2},
		{2, 1.75},
		{2, -1.5},
		{-2, -2}
	};

	double part = 1.0 / 16.0;

	POINTF textureCoords[6] = {
		{0, part},
		{part, 0},
		{8 * part, 0},
		{8 * part, part},
		{8 * part, 6*part},
		{0, 6 * part}
	};

	DrawExtrudedPolygon(points, textureCoords, 6, 1.0, 0.7, 0.7, 0.7);
}

void CGLRenderer::DrawExcavator() {
	glBindTexture(GL_TEXTURE_2D, m_textureArray[(int)TextureIndeces::EXCEVATOR]);
	{
		glPushMatrix();
		{
			glRotatef(m_cabineAngle, 0, 1, 0);
			glPushMatrix();
			{
				glTranslatef(0, 1, -2.5);
				DrawBase();
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0, 4, -2);
				DrawBody();
				glTranslatef(0, 0, 2);
			}
			glPopMatrix();



			glPushMatrix();
			{
				glTranslatef(2, 3, -0.5);
				glRotatef(m_arm1Angle, 0, 0, 1);
				glTranslatef(3.5, 0, 0);
				DrawArm(1);

				glTranslatef(4, 0, -0.25);
				glRotatef(m_arm2Angle, 0, 0, 1);
				glTranslatef(3.5, 0, 0);
				DrawArm(1.5);

				glTranslatef(3.5, -1, 0);
				glRotatef(m_forkAngle, 0, 0, 1);
				glTranslatef(0, 0, 0.25);

				DrawFork();
			}
			glPopMatrix();
		}
		glPopMatrix();
		//DrawFork();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
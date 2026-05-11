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
	m_texNames[(int)TextureIndeces::JUPITER] = "res\\jupiter.jpg";
	m_texNames[(int)TextureIndeces::SUN] = "res\\sun.jpg";
	m_texNames[(int)TextureIndeces::MARS] = "res\\mars.jpg";
	m_texNames[(int)TextureIndeces::SATRUN] = "res\\saturn.jpg";
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

	//glClearColor(0.7f, 0.85f, 1.0f, 1.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < (int)TextureIndeces::COUNT; ++i)
		m_texIDArray[i] = LoadTexture(m_texNames[i]);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//float lightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };  // w=0 = direkciono svetlo
	//float lightAmbient[] = { 0.05f, 0.05f, 0.05f, 1.0f };  // nearly black ambient = space-like
	//float lightDiffuse[] = { 2.0f,  1.9f,  1.4f,  1.0f };  // over-bright warm white (OpenGL allows >1)
	//float lightSpecular[] = { 2.0f,  2.0f,  1.5f,  1.0f };
	
	// PrepareScene
	float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiffuse[] = { 3.0f, 2.8f, 2.4f, 1.0f };  // high to compensate modulate
	float lightSpecular[] = { 0.3f,  0.3f,  0.4f, 1.0f };  // nearly off

	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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
	double EarthR = 0.6731, n = 64, sunR = 69.57f;
	double moonR = 0.1737f, EarthMoonDistance = 38.4399f, earthSunDistance = m_earthSunDistance;
	double spaceR = earthSunDistance * 2;
	
	//DrawSpace(spaceR, n);

	if (m_lightEnabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
	float lightPos[] = { 0.0f, 0.0f, -(float)earthSunDistance, 1.0f };
	glPushMatrix();
	{
		glDisable(GL_COLOR_MATERIAL);
		UpdateCamera();


		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);

		glPushMatrix();
		{
			glRotatef(m_inclanation, 1, 0, 0);
			glRotatef(m_earthRotAngle, 0, 1, 0);

			DrawEarth(EarthR, n);
		}
		glPopMatrix();

		float noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		float noAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, noAmbient);

		glPushMatrix();
		{
			glRotatef(m_moonEarthRotAngle, 0, 1, 0); //rotate around earth
			glRotatef(m_moonEclipticInclination, 1, 0, 0); //to make it tilted
			glTranslatef(0, 0, -EarthMoonDistance);
			glRotatef(m_moonRotAngle, 0, 1, 0);

			DrawMoon(moonR, n);

			// Restore Earth light settings
			float earthDiffuseLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
			glLightfv(GL_LIGHT0, GL_DIFFUSE, earthDiffuseLight);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0, 0, -25);
			DrawJupiter(m_jupiterR, 2 * n);
		}
		glPopMatrix();


		glPushMatrix();
		{
			glTranslatef(0, 0, 4);
			glRotatef(m_marsTilt, 1, 0, 0);
			DrawMars(m_marsR, n);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0, 0, -10);
			glRotatef(m_satrunTilt, 1, 0, 0);

			DrawSatrun(m_satrunR, 4 * n);
		}
		glPopMatrix();

		glTranslatef(0, 0, -earthSunDistance);

		//glColor3f(1.0f, 1.0f, 0); // sun color
		//DrawSphere(sunR, n);

		DrawSun(sunR, n);

		glEnable(GL_COLOR_MATERIAL);
	}
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

	gluPerspective(45, aspect, 0.1, 50000);

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

void CGLRenderer::DrawSphere(double R, int n) {
	double phiStep = M_PI / n;
	double thetaStep = 2.0 * M_PI / n;

	

	glDisable(GL_LIGHTING);

	for (int i = 0; i < n; ++i) {
		double phi1 = i * phiStep;
		double phi2 = (i + 1) * phiStep;

		glBegin(GL_TRIANGLE_STRIP);
		{
			for (int j = 0; j <= n; ++j) {
				double theta = j * thetaStep;

				double x1 = sin(phi1) * cos(theta);
				double y1 = cos(phi1);
				double z1 = sin(phi1) * sin(theta);
				glNormal3d(x1, y1, z1);
				glVertex3d(R * x1, R * y1, R * z1);

				double x2 = sin(phi2) * cos(theta);
				double y2 = cos(phi2);
				double z2 = sin(phi2) * sin(theta);
				glNormal3d(x2, y2, z2);
				glVertex3d(R * x2, R * y2, R * z2);
			}
		}
		glEnd();
	}

	if (m_lightEnabled)
		glEnable(GL_LIGHTING);
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
	float matAmbient[] = { 0.1f, 0.12f, 0.18f, 1.0f };
	float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float matSpecular[] = { 0.5f, 0.6f, 0.8f, 1.0f };  // blue-white ocean glint	
	float matShininess = 64.0f;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

	float earthLight[] = { 20.0f, 15.0f, 15.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, earthLight);

	float earthAmbient[] = { 0.2f, 0.2f, 0.25f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, earthAmbient);

	float matEmission[] = { 0.06f, 0.08f, 0.13f, 1.0f };  // faint blue atmosphere
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	DrawTextureSphere(R, tes, TextureIndeces::TSC0);
}

void CGLRenderer::DrawJupiter(double r, int n) {
	float matAmbient[] = { 0.3f,  0.25f, 0.2f,  1.0f };  // warm self-glow
	float matDiffuse[] = { 1.0f,  1.0f,  1.0f,  1.0f };
	float matSpecular[] = { 0.05f, 0.04f, 0.03f, 1.0f };  // gas giant = almost no specular
	float matEmission[] = { 0.08f, 0.06f, 0.04f, 1.0f };  // faint warm glow in shadow
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

	// much brighter diffuse than earth — Jupiter reflects ~52% of light (albedo)
	float jupiterLight[] = { 60.0f, 55.0f, 45.0f, 1.0f };
	float jupiterAmbient[] = { 0.4f,  0.35f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, jupiterLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, jupiterAmbient);


	DrawLatLonSphere(r, n, m_texIDArray[(int)TextureIndeces::JUPITER]);
}

void CGLRenderer::DrawSun(double r, int n)
{

	glDisable(GL_LIGHTING);  // sun is emissive, not lit

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  // ignore lighting, show pure texture

	DrawLatLonSphere(r, n, m_texIDArray[(int)TextureIndeces::SUN]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  // restore for other objects
	if (m_lightEnabled)
		glEnable(GL_LIGHTING);

	//DrawLatLonSphere(r, n, m_texIDArray[(int)TextureIndeces::SUN]);
}

void CGLRenderer::DrawMars(double r, int n)
{

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	float matAmbient[] = { 0.2f,  0.1f,  0.07f, 1.0f };  // dark reddish-brown
	float matDiffuse[] = { 1.0f,  1.0f,  1.0f,  1.0f };
	float matSpecular[] = { 0.02f, 0.01f, 0.01f, 1.0f };  // dusty surface, almost no glint
	float matEmission[] = { 0.02f, 0.01f, 0.005f,1.0f };  // nearly none
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

	// Mars is further from sun, dimmer, and has low albedo (0.25)
	// slightly warm/reddish light to enhance the rust color
	float marsLight[] = { 8.0f,  6.0f,  4.5f, 1.0f };
	float marsAmbient[] = { 0.05f, 0.03f, 0.02f, 1.0f };  // very dark shadow side
	glLightfv(GL_LIGHT0, GL_DIFFUSE, marsLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, marsAmbient);

	DrawLatLonSphere(r, n, m_texIDArray[(int)TextureIndeces::MARS]);
}

void CGLRenderer::DrawSatrun(double r, int n)
{

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	float matAmbient[] = { 0.25f, 0.22f, 0.15f, 1.0f };  // warm golden self-glow
	float matDiffuse[] = { 1.0f,  1.0f,  1.0f,  1.0f };
	float matSpecular[] = { 0.03f, 0.03f, 0.02f, 1.0f };  // gas giant, no hard surface
	float matEmission[] = { 0.06f, 0.05f, 0.03f, 1.0f };  // faint golden glow in shadow
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

	// Saturn is ~9.5 AU out, so noticeably dimmer than Jupiter
	// warm golden-white light to bring out the caramel banding
	float saturnLight[] = { 35.0f, 32.0f, 24.0f, 1.0f };
	float saturnAmbient[] = { 0.3f,  0.27f, 0.2f,  1.0f };  // gas giant retains warmth in shadow
	glLightfv(GL_LIGHT0, GL_DIFFUSE, saturnLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, saturnAmbient);

	DrawLatLonSphere(r, n, m_texIDArray[(int)TextureIndeces::SATRUN]);
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
	// Moon material - back to modulate, simple diffuse
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	float moonAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float moonDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float moonSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float moonEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, moonAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moonDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, moonSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moonEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

	// Moon needs stronger light since we're using MODULATE not ADD
	float moonDiffuseLight[] = { 2.0f, 2.0f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, moonDiffuseLight);


	DrawTextureSphere(R, tes, TextureIndeces::M0);
}

void CGLRenderer::DrawSpace(double R, int tes) {
	DrawTextureSphere(R, tes, TextureIndeces::S0);
}


void CGLRenderer::DrawLatLonSphere(double R, int n, UINT textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	double phiStep = M_PI / n;

	for (int i = 0; i < n; ++i) {
		double phi1 = -M_PI / 2.0 + i * phiStep;
		double phi2 = -M_PI / 2.0 + (i + 1) * phiStep;

		double v1 = 1.0 - (double)i / n;
		double v2 = 1.0 - (double)(i + 1) / n;

		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= n; ++j) {
			double theta = 2.0 * M_PI * j / n;
			double u = (double)j / n;

			double x1 = cos(phi1) * cos(theta);
			double y1 = sin(phi1);
			double z1 = cos(phi1) * sin(theta);
			glNormal3d(x1, y1, z1);
			glTexCoord2d(u, v1);
			glVertex3d(R * x1, R * y1, R * z1);

			double x2 = cos(phi2) * cos(theta);
			double y2 = sin(phi2);
			double z2 = cos(phi2) * sin(theta);
			glNormal3d(x2, y2, z2);
			glTexCoord2d(u, v2);
			glVertex3d(R * x2, R * y2, R * z2);
		}
		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}


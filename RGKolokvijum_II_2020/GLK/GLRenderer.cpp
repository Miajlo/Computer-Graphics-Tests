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
	m_cameraDistance = 50.0;
	m_cameraAngleY = 0.0;
	m_cameraAngleX = 45.0;
	m_hrc = NULL;
	m_texBack = m_texBot = m_texTop =  m_texFront = m_texLeft = m_texRight = m_texSpider = 0;
}

CGLRenderer::~CGLRenderer(void) {
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

	double centerX = 0.0, centerY = 10.0, centerZ = 0.0;

	double radY = m_cameraAngleY * M_PI / 180.0;
	double radX = m_cameraAngleX * M_PI / 180.0;

	double eyeX = centerX + m_cameraDistance * sin(radX) * cos(radY);
	double eyeY = centerY + m_cameraDistance * cos(radX);
	double eyeZ = centerZ + m_cameraDistance * sin(radX) * sin(radY);

	gluLookAt(eyeX, eyeY, eyeZ,     // Eye position
		centerX, centerY, centerZ,  // Look at center [0,10,0]
		0, 1, 0);

	float frontSize = 100;
	float sphereR = 1.0f, eyeU = 0.75f, eyeV = 0.75f, eyeR = 0.25f;
	float coneR = 1.0f, leg1U = 0.5f, leg1V = 0.75f, leg1R = 0.25f, h = 5.0f;
	int nSeg = 32;
	double axisLength = 50;

	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

	glTranslatef(0, frontSize / 2 - 0.01f, 0);
	DrawEnvCube(frontSize);

	glPopMatrix();

	DrawSpider();

	glPopMatrix();

	DrawAxis(axisLength);

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

	gluPerspective(50, aspect, 0.1, 200.0);

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
		glNormal3f(0, 0, 1);

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
	
	for (int i = 0; i < nSeg; ++i) {
		double phi1 = i * M_PI / nSeg;
		double phi2 = (i + 1) * M_PI / nSeg;

		glBegin(GL_QUAD_STRIP);

			for (int j = 0; j <= 2 * nSeg; ++j) {
				double theta = M_PI * j / nSeg;
			
				double x2 = r * sin(phi2) * cos(theta);
				double y2 = r * cos(phi2);
				double z2 = r * sin(phi2) * sin(theta);

				double u2 = texU + x2 / r * texR;
				double v2 = texV + z2 / r * texR;

				glNormal3d(x2 / r, y2 / r, z2 / r);
				glTexCoord2d(u2, v2);
				glVertex3d(x2, y2, z2);

				double x1 = r * sin(phi1) * cos(theta);
				double y1 = r * cos(phi1);
				double z1 = r * sin(phi1) * sin(theta);

				double u1 = texU + x1 / r * texR;
				double v1 = texV + z1 / r * texR;

				glNormal3d(x1 / r, y1 / r, z1 / r);
				glTexCoord2d(u1, v1);
				glVertex3d(x1, y1, z1);
			}

		glEnd();
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR) {
	double angleStep = 2.0f * M_PI / nSeg;
	double centerU = texU + texR * 0.5;
	double centerV = texV + texR * 0.5;
	double radius = texR * 0.5;

	// Draw the base (using circular mapping)
	glBegin(GL_TRIANGLE_FAN);
		glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2d(centerU, centerV);  // Center of circle
		glVertex3d(0, 0, 0);

		for (int i = 0; i <= nSeg; ++i) {  // FORWARD instead of reversed
			double angle = i * angleStep;
			double x = r * cos(angle);
			double z = r * sin(angle);

			glNormal3f(0, -1.0, 0);
			// Map to circle edge with margin
			glTexCoord2d(centerU + radius * cos(angle),
				centerV + radius * sin(angle));
			glVertex3d(x, 0.0, z);
		}
	glEnd();

	// Draw the side surface
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < nSeg; ++i) {
			double angle1 = i * angleStep;
			double angle2 = (i + 1) * angleStep;

			double x1 = r * cos(angle1);
			double z1 = r * sin(angle1);
			double x2 = r * cos(angle2);
			double z2 = r * sin(angle2);

			// Calculate normals for the side faces
			double slant = sqrt(r * r + h * h);
			double nx1 = (h / slant) * cos(angle1);
			double nz1 = (h / slant) * sin(angle1);
			double nx2 = (h / slant) * cos(angle2);
			double nz2 = (h / slant) * sin(angle2);
			double ny = r / slant;

			// For cone side, map height linearly from center to edge
			// Apex at center, base at edge of circle
			double u1 = cos(angle1);
			double v1 = sin(angle1);
			double u2 = cos(angle2);
			double v2 = sin(angle2);

			// Apex vertex (at center of texture circle)
			glNormal3d((nx1 + nx2) * 0.5, ny, (nz1 + nz2) * 0.5);
			glTexCoord2d(centerU, centerV);
			glVertex3d(0.0, h, 0.0);

			// Base vertex 2 FIRST (for CCW order)
			glNormal3d(nx2, ny, nz2);
			glTexCoord2d(centerU + radius * u2, centerV + radius * v2);
			glVertex3d(x2, 0.0, z2);

			// Base vertex 1 SECOND
			glNormal3d(nx1, ny, nz1);
			glTexCoord2d(centerU + radius * u1, centerV + radius * v1);
			glVertex3d(x1, 0.0, z1);
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

		glPushMatrix(); //right

			glRotatef(-90, 0, 1, 0);
			glTranslatef(0, 0, -hfSize);

			DrawTextureQuad(frontSize, m_texLeft);

		glPopMatrix();

		glPushMatrix(); //back

			glRotatef(180, 0, 1, 0);
			glTranslatef(0, 0, -hfSize);

			DrawTextureQuad(frontSize, m_texBack);

		glPopMatrix();

		glPushMatrix(); //bot

			glRotatef(-90, 1, 0, 0);
			glTranslatef(0, 0, -hfSize);	

			DrawTextureQuad(frontSize, m_texBot);

		glPopMatrix();

		glPushMatrix(); //top

			glRotatef(90, 1, 0, 0);
			glTranslatef(0, 0, -hfSize);	

		DrawTextureQuad(frontSize, m_texTop);

	glPopMatrix();
}

void CGLRenderer::DrawLegSegment(double r, double h, double nSeg) {
	float legU = 0.0f, legV = 0.0f, legR = 0.5f;        // Changed
	float jointU = 0.75f, jointV = 0.75f, jointR = 0.25f;

	glPushMatrix();

		DrawSphere(r, nSeg, jointU, jointV, jointR);
		
		DrawCone(r, h, 2 * nSeg, legU, legV, legR);

	glPopMatrix();
}

void CGLRenderer::DrawLeg() {
	float seg1Height = 10, seg2Height = 15;
	float rotAngle = 85;
	float segR = 1.0f;
	int nSeg = 5;

	glPushMatrix();
		glRotatef(45, 1, 0, 0);
		DrawLegSegment(segR, seg1Height, nSeg);

		glTranslatef(0, seg1Height, segR);
		glRotatef(rotAngle, 1, 0, 0);	

		DrawLegSegment(segR, seg2Height, nSeg);

	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody() {
	double headR = 2.0f, chestR = 3.0f, stomachR = STOMACK_R;
	int sphereSeg = 10;
	float headU = 0.75f, headV = 0.25f, headTexR = 0.25f;
	float chestBodyU = 0.25f, chestBodyV = 0.25f, chestBodyR = 0.25f;

	glPushMatrix();	
		//chest
		glPushMatrix();

			glScalef(1.0f, 0.5f, 1.0f);

			DrawSphere(chestR, sphereSeg, chestBodyU, chestBodyV, chestBodyR);

			//head
			glTranslatef(-chestR, 0, 0);

			DrawSphere(headR, sphereSeg, headU, headV, headTexR);

		glPopMatrix();

	glPushMatrix();

		glScalef(1.0f, 0.8f,1.0f);

		glTranslatef(chestR*2, 0, 0);

		DrawSphere(stomachR, sphereSeg, chestBodyU, chestBodyV, chestBodyR);

	glPopMatrix();
}

void CGLRenderer::DrawSpider() {
	glBindTexture(GL_TEXTURE_2D, m_texSpider);
		glPushMatrix();
			glTranslatef(0, STOMACK_R, 0);
	
			DrawSpiderBody();

			glRotatef(-45, 0, 1, 0);

			DrawLeg();

			glRotatef(30, 0, 1, 0);

			DrawLeg();


			glRotatef(30, 0, 1, 0);

			DrawLeg();

			glRotatef(30, 0, 1, 0); // end of "left" side

			DrawLeg();

			glRotatef(90, 0, 1, 0);

			DrawLeg(); //r4

			glRotatef(30, 0, 1, 0);

			DrawLeg(); //r3

			glRotatef(30, 0, 1, 0);

			DrawLeg(); //r2

			glRotatef(30, 0, 1, 0);

			DrawLeg();

		glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

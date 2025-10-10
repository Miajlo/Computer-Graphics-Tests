#pragma once
#include<unordered_map>
#include<string>
#include<corecrt_math_defines.h>
#define TO_DEG 180 / M_PI
#define TO_RAD M_PI/180

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxes();

	UINT LoadTexture(char* fileName);

	void DrawEnvCube(double a);
	void DrawTexQuad(double a, UINT textureID);

	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen);

	void DrawLampBase();

	void DrawLampArm();

	void DrawLampHead();

	void DrawLamp();

	void UpdateCamera();

	float m_arm1Angle;
	float m_arm2Angle;
	float m_headAngle;
	float m_angleY;
	float m_angleX;
	float m_cameraDistance;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT m_texTop = 0;
	UINT m_texBot = 0;
	UINT m_texSide = 0;
	UINT m_texLamp = 0;
};

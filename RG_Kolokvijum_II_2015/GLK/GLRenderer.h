#pragma once
#include<string>

enum  class TextureIndeces {
	TSC0 = 0,
	TSC1,
	TSC2,
	TSC3,
	TSC4,
	TSC5,
	S0,
	S1,
	S2,
	S3,
	S4,
	S5,
	M0,
	M1,
	M2,
	M3,
	M4,
	M5,
	COUNT
};


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

	UINT LoadTexture(char* fileName);

	void DrawPatch(double R, int n);
	void DrawTexurePatch(double R, int n, UINT textureID);
	void UpdateCamera();
	void DrawEarth(double R, int tes);
	void DrawTextureSphere(double r, int tes, TextureIndeces start);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);

	float m_cameraDistance = 80;
	float m_angleY = 0;
	float m_angleX = 0;
	float m_moonRotAngle = 0;
	bool m_lightEnabled = true;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT m_texIDArray[(int)TextureIndeces::COUNT];
	char* m_texNames[(int)TextureIndeces::COUNT];
};

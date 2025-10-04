#pragma once

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

	void DrawAxis(float length);
	void DrawTextureQuad(float width, UINT textureID);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawEnvCube(double a);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT m_texSpider;
	UINT m_texFront;
	UINT m_texLeft;
	UINT m_texRight;
	UINT m_texBack;
	UINT m_texTop;
	UINT m_texBot;
};

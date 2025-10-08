#pragma once


class CGLRenderer {
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxis();
	UINT LoadTexture(char* fileName);
	void DrawTriangle(float d1, float d2, float rep);
	void DrawShip();

	void DrawSpaceCube(double a);

	void DrawQuad(double a, UINT textureID);

	float m_cameraAngleY;
	float m_cameraAngleX;
	float m_cameraDistance;
	bool m_LightOn = true;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT m_texBack = 0;
	UINT m_texFront = 0;
	UINT m_texTop = 0;
	UINT m_texBot = 0;
	UINT m_texLeft = 0;
	UINT m_texRight = 0;
	UINT m_texShipT1 = 0;
};

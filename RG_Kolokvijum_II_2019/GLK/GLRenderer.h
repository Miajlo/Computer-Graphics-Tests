#pragma once
#include<string>
#include<vector>
#include<corecrt_math_defines.h>
#define TO_RAD M_PI/180
enum class TextureIndeces {
	TOP = 0,
	BOTTOM,
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	EXCEVATOR,
	LENGTH
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

	void DrawAxes(float length);
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawTextureQuad(double a, UINT textureID);
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawBase();

	void DrawBody();

	void DrawArm(double zh);

	void DrawFork();

	void DrawExcavator();


	float m_arm1Angle = 45;
	float m_arm2Angle = -90;
	float m_cabineAngle = 45;
	float m_forkAngle = -90;
	float m_cameraDistance = 20;
	float m_cameraYAngle = 0;
	float m_cameraXZAngle = 0;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	std::vector<std::string> m_fileNames;
	UINT m_textureArray[(int)TextureIndeces::LENGTH];
};

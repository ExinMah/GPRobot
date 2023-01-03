#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cmath>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define PI 3.1429												// Value of pi
# define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))		// To calculate array size

#define WINDOW_TITLE "Graphics Programming Robot Assignment"

/*
 * --------------------------------------------------------------------
 *								Variables
 * --------------------------------------------------------------------
*/

/********************** ROBOT **********************/


/********************** PROJECTION **********************/
// Variables for projection transformation
float pTx = 0.0, pTy = 0.0, pTSpeed = 0.5;
float pRy = 0.0, pRSpeed = 1.0;

// Variables for rotation transformation
float rx = 0, ry = 0, rz = 0, rSpeed = 0.0;

// Varaibles for views (Orthographic and Perspective)
bool isOrtho = true;
double oNear = -10.0, oFar = 10.0;
double pNear = 1.0, pFar = 20.0;

/********************** TEXTURE **********************/
// Variables for texture bitmap
BITMAP bmp;
HBITMAP hBmp = NULL;

/*
 * --------------------------------------------------------------------
 *					Windows Procedure (Input Messages)
 * --------------------------------------------------------------------
*/
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 0x58:
			rx = -1.0;
			ry = 0.0;
			rz = 0.0;
			rSpeed = 0.05;
			break;
		case 0x59:
			rx = 0.0;
			ry = -1.0;
			rz = 0.0;
			rSpeed = 0.05;
			break;
		case 0x5A:
			rx = 0.0;
			ry = 0.0;
			rz = 1.0;
			rSpeed = 0.05;
			break;
		case VK_SPACE:
			rSpeed = 0.0;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
 * --------------------------------------------------------------------
 *						Windows Initialization
 * --------------------------------------------------------------------
*/

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * --------------------------------------------------------------------
 *					Texture and Projection Group
 * --------------------------------------------------------------------
*/

GLuint LoadTexture(LPCSTR textureFile)
{
	GLuint texture = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	HBITMAP hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), textureFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(bmp), &bmp);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

	DeleteObject(hBmp);
	return texture;
}
void DestroyTexture(GLuint textureArray[])
{
	for (int i = 0; i < my_sizeof(textureArray) / my_sizeof(textureArray[0]); i++)
	{
		glDeleteTextures(1, &textureArray[i]);
	}

	glDisable(GL_TEXTURE_2D);
}

void Projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glTranslatef(pTx, pTy, 0.0);
	glRotatef(pRy, 0.0, 1.0, 0.0);

	if (isOrtho) {
		glOrtho(-10.0, 10.0, -10.0, 10.0, oNear, oFar);
	}
	else {
		gluPerspective(20.0, 1.0 /* width / height of screen */, -1.0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, pNear, pFar);
	}
}

/*
 * --------------------------------------------------------------------
 *						Objects Drawing Group
 * --------------------------------------------------------------------
*/

/************************** DRAW 2D OBJECTS **************************/
// Draw Circle - TD: Add Texture Coords
void DrawLineCircle(float xOrigin, float yOrigin, float radius)
{
	glBegin(GL_LINE_LOOP);
	for (float angle = 0; angle <= 2 * PI; angle += (2 * PI) / 30)
	{
		float xPos = xOrigin + radius * cos(angle);
		float yPos = yOrigin + radius * sin(angle);
		glVertex2f(xPos, yPos);
	}
	glEnd();
}
void DrawFullCircle(float xOrigin, float yOrigin, float radius)
{
	glBegin(GL_TRIANGLE_FAN);
	for (float angle = 0; angle <= 2 * PI; angle += (2 * PI) / 30)
	{
		float xPos = xOrigin + radius * cos(angle);
		float yPos = yOrigin + radius * sin(angle);
		glVertex2f(xPos, yPos);
	}
	glEnd();
}

// Draw Square
void DrawLineSquare(float leftXAxis, float topYAxis, float rightXAxis, float botYAxis)
{
	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(leftXAxis, botYAxis);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(leftXAxis, topYAxis);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(rightXAxis, topYAxis);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(rightXAxis, botYAxis);
	glEnd();
}
void DrawFullSquare(float leftXAxis, float topYAxis, float rightXAxis, float botYAxis)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(leftXAxis, botYAxis);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(leftXAxis, topYAxis);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(rightXAxis, topYAxis);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(rightXAxis, botYAxis);
	glEnd();
}

// Draw Triangle
/*
 * !! IMPORTANT !!
 * The texture coordinate is set up so that the coord will map it like this
 *
 *            (x2, y2)
 *              ^
 *            /  \
 *          /     \
 *        /________\
 * (x1, y1)       (x3, x3)
 *
 * Write your coordinates with something like this shape in mind
 */
void DrawLineTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(x1, y1);
	glTexCoord2f(0.5, 1.0);
	glVertex2f(x2, y2);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(x3, y3);
	glEnd();
}
void DrawFullTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(x1, y1);
	glTexCoord2f(0.5, 1.0);
	glVertex2f(x2, y2);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(x3, y3);
	glEnd();
}

// Draw Polygon? - I technically have until decagon lol but I don't want to think it's texture coordinate QAQ

/************************** DRAW 3D OBJECTS **************************/

// Draw Sphere
void DrawLineSphere(GLdouble radius)
{
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluQuadricTexture(sphere, TRUE);
	gluSphere(sphere, radius, 30, 30);
	gluDeleteQuadric(sphere);
}
void DrawFillSphere(GLdouble radius)
{
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluSphere(sphere, radius, 30, 30);
	gluDeleteQuadric(sphere);
}

// Draw Cube
void DrawFullCube(float size)
{
	glBegin(GL_LINE_LOOP);
	// Face 1 : Bottom face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Face 2 : Left Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 : Front Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(size, 0.0f, size);

	// Face 4 : Right Face
	glTexCoord2f(0, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, size);

	// Face 5 : Top Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, 0.0f);

	// Face 6 : Back Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();
}
void DrawFillCube(float size){
	glBegin(GL_QUADS);
	// Face 1 : Bottom face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Face 2 : Left Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 : Front Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(size, 0.0f, size);

	// Face 4 : Right Face
	glTexCoord2f(0, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, size);

	// Face 5 : Top Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, 0.0f);

	// Face 6 : Back Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();
}
void DrawAdjustableFillCube(float width, float height, float size)
{
	glBegin(GL_QUADS);
	// Face 1 : Bottom face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Face 2 : Left Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 : Front Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(size, 0.0f, size);

	// Face 4 : Right Face
	glTexCoord2f(0, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, size);

	// Face 5 : Top Face
	glTexCoord2f(0, 1);
	glVertex3f(width, height, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, height, size);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(width, height, 0.0f);

	// Face 6 : Back Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();
}

// Draw Cone
void DrawLineCone(double radius, double height)
{
	GLUquadricObj* cone = NULL;
	cone = gluNewQuadric();

	gluQuadricDrawStyle(cone, GLU_LINE);
	gluQuadricTexture(cone, TRUE);
	gluCylinder(cone, radius, 0, height, 30, 30);
	gluDeleteQuadric(cone);
}
void DrawFillCone(double radius, double height)
{
	GLUquadricObj* cone = NULL;
	cone = gluNewQuadric();

	gluQuadricDrawStyle(cone, GLU_FILL);
	gluQuadricTexture(cone, TRUE);
	gluCylinder(cone, radius, 0, height, 30, 30);
	gluDeleteQuadric(cone);
}

// Draw Cylinder
void DrawLineCylinder(double baseRadius, double topRadius, double height)
{
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluQuadricTexture(cylinder, TRUE);
	gluCylinder(cylinder, baseRadius, topRadius, height, 30, 30);
	gluDeleteQuadric(cylinder);
}
void DrawFillCylinder(double baseRadius, double topRadius, double height)
{
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, TRUE);
	gluCylinder(cylinder, baseRadius, topRadius, height, 30, 30);
	gluDeleteQuadric(cylinder);
}

// Draw Pyramid - Consolidate the texCoord again?
void DrawLinePyramid(float size, float height){
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size / 2, height, size / 2);

	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size / 2, height, size / 2);

	glVertex3f(size, 0.0f, size);
	glVertex3f(size / 2, height, size / 2);

	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size / 2, height, size / 2);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}
void DrawFillPyramid(float size, float height){
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(size / 2, height, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(0.5, 0.5);
	glVertex3f(size / 2, height, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);

	glTexCoord2f(0.5, 0.5);
	glVertex3f(size / 2, height, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(0.5, 0.5);
	glVertex3f(size / 2, height, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
}

//	Draw Prism
void DrawLinePrism(float width, float height, float size){
	glBegin(GL_LINE_LOOP);
	glVertex3f(-width / 4, 0, size / 2);
	glVertex3f(-width / 4, 0, -size / 2);
	glVertex3f(-3 * width / 4, 0, -size / 2);
	glVertex3f(-3 * width / 4, 0, size / 2);

	glVertex3f(-width / 4, 0, -size / 2);
	glVertex3f(-width / 4, height, -size / 2);
	glVertex3f(-3 * width / 4, height, -size / 2);
	glVertex3f(-3 * width / 4, 0, -size / 2);

	glVertex3f(-width / 4, height, -size / 2);
	glVertex3f(-3 * width / 4, height, -size / 2);
	glVertex3f(-3 * width / 4, 0, size / 2);
	glVertex3f(-width / 4, 0, size / 2);

	glVertex3f(-width / 4, 0, size / 2);
	glVertex3f(-width / 4, height, -size / 2);
	glVertex3f(-width / 4, 0, -size / 2);

	glVertex3f(-3 * width / 4, 0, size / 2);
	glVertex3f(-3 * width / 4, height, -size / 2);
	glVertex3f(-3 * width / 4, 0, -size / 2);
	glEnd();
}
void DrawFillPrism(float width, float height, float size){
	glBegin(GL_QUADS);
	glVertex3f(width / 2, 0, size / 2);
	glVertex3f(width / 2, 0, -size / 2);
	glVertex3f(-width / 2, 0, -size / 2);
	glVertex3f(-width / 2, 0, size / 2);

	glVertex3f(width / 2, 0, -size / 2);
	glVertex3f(width / 2, height, -size / 2);
	glVertex3f(-width / 2, height, -size / 2);
	glVertex3f(-width / 2, 0, -size / 2);

	glVertex3f(width / 2, height, -size / 2);
	glVertex3f(-width / 2, height, -size / 2);
	glVertex3f(-width / 2, 0, size / 2);
	glVertex3f(width / 2, 0, size / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(width / 2, 0, size / 2);
	glVertex3f(width / 2, height, -size / 2);
	glVertex3f(width / 2, 0, -size / 2);

	glVertex3f(-width / 2, 0, size / 2);
	glVertex3f(-width / 2, height, -size / 2);
	glVertex3f(-width / 2, 0, -size / 2);
	glEnd();
}
void DrawFillLeftHalfPrism(float width, float height, float size)
{
	glBegin(GL_QUADS);
	glVertex3f(-width / 2, 0, size / 2);
	glVertex3f(-width / 2, 0, -size / 2);
	glVertex3f(0, 0, -size / 2);
	glVertex3f(0, 0, size / 2);

	glVertex3f(-width / 2, 0, -size / 2);
	glVertex3f(-width / 2, height, -size / 2);
	glVertex3f(0, height, -size / 2);
	glVertex3f(0, 0, -size / 2);

	glVertex3f(-width / 2, height, -size / 2);
	glVertex3f(0, height, -size / 2);
	glVertex3f(0, 0, size / 2);
	glVertex3f(-width / 2, 0, size / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(-width / 2, 0, size / 2);
	glVertex3f(-width / 2, height, -size / 2);
	glVertex3f(-width / 2, 0, -size / 2);

	glVertex3f(0, 0, size / 2);
	glVertex3f(0, height, -size / 2);
	glVertex3f(0, 0, -size / 2);
	glEnd();
}
void DrawFillRightHalfPrism(float width, float height, float size){
	glBegin(GL_QUADS);
	glVertex3f(width / 2, 0, size / 2);
	glVertex3f(width / 2, 0, -size / 2);
	glVertex3f(0, 0, -size / 2);
	glVertex3f(0, 0, size / 2);

	glVertex3f(width / 2, 0, -size / 2);
	glVertex3f(width / 2, height, -size / 2);
	glVertex3f(0, height, -size / 2);
	glVertex3f(0, 0, -size / 2);

	glVertex3f(width / 2, height, -size / 2);
	glVertex3f(0, height, -size / 2);
	glVertex3f(0, 0, size / 2);
	glVertex3f(width / 2, 0, size / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(width / 2, 0, size / 2);
	glVertex3f(width / 2, height, -size / 2);
	glVertex3f(width / 2, 0, -size / 2);

	glVertex3f(0, 0, size / 2);
	glVertex3f(0, height, -size / 2);
	glVertex3f(0, 0, -size / 2);
	glEnd();
}
void DrawAdjustableFillRectangularPrism(float width, float height, float size){
	glBegin(GL_QUADS);
	// Face 1 : Bottom face
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(width, 0.0f, size);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Face 2 : Left Face
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	glVertex3f(0.0f, height, size);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 : Front Face
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, height, size);
	glVertex3f(width, height, size);
	glVertex3f(width, 0.0f, size);

	// Face 4 : Right Face
	glVertex3f(width, 0.0f, size);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(width, height, 0.0f);
	glVertex3f(width, height, size);

	// Face 5 : Top Face
	glVertex3f(width, height, size);
	glVertex3f(0.0f, height, size);
	glVertex3f(0.0f, height, 0.0f);
	glVertex3f(width, height, 0.0f);

	// Face 6 : Back Face
	glVertex3f(width, height, 0.0f);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
}

/*
 * --------------------------------------------------------------------
 *							Robot Parts
 * --------------------------------------------------------------------
*/

void RobotHead()
{
	//	Jaw
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	DrawFillPyramid(0.3,-0.1);
	glPopMatrix();

	//	Face
	glPushMatrix();
	glColor3f(0.0,0.0,1.0);
	DrawFillCube(0.3);
	glPopMatrix();

	//------------------Left Eye------------------//
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(0.06, 0.17, 0.0);
	DrawFillSphere(0.05);
	glPopMatrix();

	//------------------Right Eye------------------//
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(0.24, 0.17, 0.0);
	DrawFillSphere(0.05);
	glPopMatrix();
	
	//	Hair
	//------------------Left side hair piece------------------//
	glPushMatrix();
	glColor3f(0.0, 1.0, 1.0);
	glTranslatef(-0.05, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glTranslatef(0.05, 0.3, 0.0);
	DrawFillPrism(-0.15, -0.4, 0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.5,0.0);
	glTranslatef(0.45, 0.0, 0.0);
	glTranslatef(0.0, 0.1, 0.0);
	glRotatef(90, 0.0,1.0,0.0);
	glTranslatef(0.01, 0.0, 0.0);
	glRotatef(45, 1.0,0.0,0.0);
	DrawFillLeftHalfPrism(0.2, 0.1, 0.4);
	glPopMatrix();
	
	//------------------Right side hair piece------------------//
	glPushMatrix();
	glColor3f(0.0, 1.0, 1.0);
	glTranslatef(0.35, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(-0.05, 0.3, 0.0);
	DrawFillPrism(-0.15, -0.4, 0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.5,0.0);
	glTranslatef(-0.15, 0.0, 0.0);
	glTranslatef(0.0, 0.1, 0.0);
	glRotatef(-90, 0.0,1.0,0.0);
	glTranslatef(0.08, 0.0, 0.0);
	glRotatef(45, 1.0,0.0,0.0);
	DrawFillLeftHalfPrism(0.2, 0.1, 0.4);
	glPopMatrix();

	//------------------Left Bangs------------------//
	glPushMatrix();
	glColor3f(1.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, -0.25);
	DrawFillLeftHalfPrism(0.05, -0.15, 0.1);
	glPopMatrix();
	
	//------------------Right Bangs------------------//
	glPushMatrix();
	glColor3f(1.0, 0.0, 1.0);
	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, -0.25);
	DrawFillRightHalfPrism(0.05, -0.15, 0.1);
	glPopMatrix();

	//------------------Top Hair------------------//
	glPushMatrix();
	glColor3f(0.5,0.0,0.0);
	glTranslatef(0.0, 0.3001, 0.0);
	glTranslatef(0.0, 0.0, -0.05);
	glTranslatef(-0.1, 0.0, 0.0);
	DrawFillPyramid(0.5,0.15);
	glPopMatrix();

	//------------------Hairband------------------//
	//	Left
	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.2);
	glTranslatef(-0.06,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(0.05, 0.15,0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.25);
	glTranslatef(-0.07,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(-0.05, -0.15,-0.15);
	glPopMatrix();
	
	//	Right
	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.2);
	glTranslatef(-0.06,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(0.05, 0.15,0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.25);
	glTranslatef(-0.07,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(-0.05, -0.15,-0.15);
	glPopMatrix();

	//------------------Back Hair------------------//
	// glPushMatrix();
	// glColor3f(0.0,0.0,0.5);
	// glTranslatef(0.3,0.2, 0.15);
	// DrawFillCube(0.1);
	// glPopMatrix();
	
	//	Neck
	glPushMatrix();
	glColor3f(0.0,1.0,0.0);
	glTranslatef(0.15, 0.0, 0.25);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCylinder(0.05,0.05,0.2);
	glPopMatrix();
}

void RobotBody()
{
	//glPushMatrix();
	//glScalef(0.2, 0.2, 0.2);
	//glTranslatef(1, -6, 0);
	// Body Base
	glPushMatrix();
	glScalef(1.0f, 1.75f, 0.25f);
	glTranslatef(-2.0f, -1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawFillCube(4.0f);
	glPopMatrix();

	// Chest Area
	glPushMatrix();
		glTranslatef(0, 2, 0);

		glPushMatrix();
			glTranslatef(1, 0, -1);

			// Right
			glPushMatrix();
				glColor3f(0.0, 0.0, 1.0f);
				glTranslatef(0, 2.0, 2.0f);
				//glRotatef(180, 0, 0, 1);
				glRotatef(90, 1, 0, 0);
				DrawFillPrism(2.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
				glColor3f(1, 0, 0);
				glTranslatef(0, 2.5, 2.5);
				glRotatef(180, 0, 0, 1);
				DrawFillPrism(2.0f, -1.0f, 1.0f);
			glPopMatrix();

		glPopMatrix();

		// Left
		glPushMatrix();
			glTranslatef(-1, 0, -1);

			glPushMatrix();
				glColor3f(1.0, 1.0, 0.0f);
				glTranslatef(0, 2.0, 2.0f);
				//glRotatef(180, 0, 0, 1);
				glRotatef(90, 1, 0, 0);
				DrawFillPrism(2.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
				glColor3f(1, 1, 0);
				glTranslatef(0, 2.5, 2.5);
				glRotatef(180, 0, 0, 1);
				DrawFillPrism(2.0f, -1.0f, 1.0f);
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();

	// That Miku belt thing
	
	// Front
	// Bottom
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(0, 0, 1);
	glRotatef(15, 0, 0, 1);
	glScalef(4.5, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-2.1, 0.5, 1);
	glRotatef(-15, 0, 0, 1);
	glScalef(4.1, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	// Top
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(0, 0.75, 1);
	glRotatef(15, 0, 0, 1);
	glScalef(4.5, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-2.1, 1.15, 1);
	glRotatef(-15, 0, 0, 1);
	glScalef(4.1, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	// Left
	// Bottom
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(0, 0, 1);
	glRotatef(15, 0, 0, 1);
	glScalef(4.5, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-2.1, 0.5, 1);
	glRotatef(-15, 0, 0, 1);
	glScalef(4.1, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	// Top
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(0, 0.75, 1);
	glRotatef(15, 0, 0, 1);
	glScalef(4.5, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-2.1, 1.15, 1);
	glRotatef(-15, 0, 0, 1);
	glScalef(4.1, 1, 1);
	DrawFillCube(0.5f);
	glPopMatrix();

	// Hip Base
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslatef(-2.5, -2.0, -0.3);
	glScalef(2.5, 1, 0.7);
	DrawFillCube(2.0f);
	glPopMatrix();
}

void RobotArm()
{

}

void RobotLeg()
{

}

// TBA : Other stuff like weapons, etc.

/*
 * --------------------------------------------------------------------
 *								Robot
 * --------------------------------------------------------------------
*/

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	// Projection();

	//	Transformation
	glRotatef(rSpeed, rx, ry,rz);
	
	// Draw Robot Here
	RobotHead();
	RobotBody();
	RobotArm();
	RobotLeg();
}

/*
 * --------------------------------------------------------------------
 *								WinMain
 * --------------------------------------------------------------------
*/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
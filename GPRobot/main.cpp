#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cmath>
#include "PolygonCounter.h"

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define PI 3.1429												// Value of pi
# define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))		// To calculate array size

#define WINDOW_TITLE "Graphics Programming Robot Assignment"

// Define Virtual Keycodes
#define VK_KEY_A 0x41
#define VK_KEY_B 0x42
#define VK_KEY_C 0x43
#define VK_KEY_D 0x44
#define VK_KEY_E 0x45
#define VK_KEY_F 0x46
#define VK_KEY_G 0x47
#define VK_KEY_H 0x48
#define VK_KEY_I 0x49
#define VK_KEY_J 0x4A
#define VK_KEY_K 0x4B
#define VK_KEY_L 0x4C
#define VK_KEY_M 0x4D
#define VK_KEY_N 0x4E
#define VK_KEY_O 0x4F
#define VK_KEY_P 0x50
#define VK_KEY_Q 0x51
#define VK_KEY_R 0x52
#define VK_KEY_S 0x53
#define VK_KEY_T 0x54
#define VK_KEY_U 0x55
#define VK_KEY_V 0x56
#define VK_KEY_W 0x57
#define VK_KEY_X 0x58
#define VK_KEY_Y 0x59
#define VK_KEY_Z 0x5A

#define VK_KEY_0 0x30
#define VK_KEY_1 0x31
#define VK_KEY_2 0x32
#define VK_KEY_3 0x33
#define VK_KEY_4 0x34
#define VK_KEY_5 0x35
#define VK_KEY_6 0x36
#define VK_KEY_7 0x37
#define VK_KEY_8 0x38
#define VK_KEY_9 0x39

#define VK_KEY_SPACE 0x20
#define VK_KEY_ESCAPE 0x1B

#define VK_KEY_UP 0x26
#define VK_KEY_DOWN 0x28
#define VK_KEY_LEFT 0x25
#define VK_KEY_RIGHT 0x27

#define VK_KEY_F1 0x70
#define VK_KEY_F2 0x71
#define VK_KEY_F3 0x72
#define VK_KEY_F4 0x73
#define VK_KEY_F5 0x74
#define VK_KEY_F6 0x75
#define VK_KEY_F7 0x76
#define VK_KEY_F8 0x77
#define VK_KEY_F9 0x78
#define VK_KEY_F10 0x79
#define VK_KEY_F11 0x7A
#define VK_KEY_F12 0x7B

/*
 * --------------------------------------------------------------------
 *								Variables
 * --------------------------------------------------------------------
*/

/********************** ROBOT **********************/


/********************** PROJECTION **********************/
// Variables for projection transformation
float Ptx = 0.0 , Pty = 0.0 , PtSpeed = 1.0;
float Pry =0.0 , PrSpeed=1.0;

// Variables for rotation transformation
float rx = 0, ry = 0, rz = 0, rSpeed = 0.0;

//	Variables for lightning
float ambL[3] = { 1.0, 1.0, 1.0 };	//	White color ambient light
float posA[3] = { 0.0, 1.0, 0.0 };	//	Amb light pos (0,1,0) above
float difL[3] = { 1.0, 1.0, 1.0 };	//	White color Dif light 
float posD[3] = { 1.0, 0.0, 0.0 };	//	Dif light pos (1,0,0) right
float ambM[3] = { 0.0, 0.0, 1.0 };	//	Blue color Amb material
float difM[3] = { 1.0, 0.0, 0.0 };	//	Red color Dif material
bool isLightOn = false;				//	is Light on?

// Variables for views (Orthographic and Perspective)
bool isOrtho = false;
double ONear = -10.0, OFar = 10.0;
double PNear = 1.0, PFar = 20.0;
float tx = 0.0, ty = 0.0, tz = 0.0, tSpeed = 1.0;
float rS = 0.0;

/********************** TEXTURE **********************/
// Variables for texture bitmap
BITMAP bmp;
HBITMAP hBmp = NULL;
GLuint texArr[10];
int tex;

/********************** ANIMATION **********************/
//	Weapon pickup
bool gunOn = false;
bool swordOn = false;


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
				case VK_KEY_X:
					rx = -1.0;
					ry = 0.0;
					rz = 0.0;
					rSpeed = 0.15;
					break;
				case VK_KEY_Y:
					rx = 0.0;
					ry = -1.0;
					rz = 0.0;
					rSpeed = 0.15;
					break;
				case VK_KEY_Z:
					rx = 0.0;
					ry = 0.0;
					rz = -1.0;
					rSpeed = 0.15;
					break;
				case VK_SPACE:
					rSpeed = 0.0;
					break;
				case VK_SHIFT:
					isLightOn = !isLightOn;
					break;
				case VK_KEY_W:
					posD[1] += 0.5;
					break;
				case VK_KEY_A:
					posD[2] += 0.5;
					break;
				case VK_KEY_S:
					posD[1] -= 0.5;
					break;
				case VK_KEY_D:
					posD[2] -= 0.5;
					break;
				case VK_KEY_E:
					posD[3] -= 0.5;
					break;
				case VK_KEY_Q:
					posD[3] += 0.5;
					break;
				case VK_KEY_UP:
					if (isOrtho){
						if (tz > ONear)
							tz -= tSpeed;
					}
					else {
						if (tz > ONear)
							tz -= tSpeed;
					}
					break;
				case VK_KEY_DOWN:
					if (isOrtho) {
						if (tz < OFar)
							tz += tSpeed;
					}
					else {
						if (tz < PFar)
							tz += tSpeed;
					}
					break;
				case VK_KEY_LEFT:
					if (isOrtho) {
						if (tx > ONear)
							tx -= tSpeed;
					}
					else {
						if (tx > PNear)
							tx -= tSpeed;
					}
					break;
				case VK_KEY_RIGHT:
					if (isOrtho) {
						if (tx < OFar)
							tx += tSpeed;
					}
					else {
						if (tx < PFar)
							tx += tSpeed;
					}
					break;
				case VK_KEY_J:
					Ptx -= PtSpeed;
					break;
				case VK_KEY_L:
					Ptx += PtSpeed;
					break;
				case VK_KEY_I:
					Pry += PrSpeed;
					break;
				case VK_KEY_K:
					Pry -= PrSpeed;
					break;
				case VK_KEY_P:
					isOrtho = false;
					tz = PNear;
					break;
				case VK_KEY_O:
					isOrtho = true;
					break;
				case VK_KEY_1:
					gunOn = !gunOn;
					break;
				case VK_KEY_2:
					swordOn = !swordOn;
					break;
				default:
					break;
			}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

/*
 * --------------------------------------------------------------------
 *						Windows Initialization
 * --------------------------------------------------------------------
*/

bool initPixelFormat(HDC hdc){
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

GLuint LoadTexture(LPCSTR textureFile){
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

void Projection(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glTranslatef(Ptx, Pty, 0.0);
	glRotatef(Pry, 0.0, 1.0, 0.0);

	if (isOrtho) {
		glOrtho(-10.0, 10.0, -10.0, 10.0, ONear, OFar);
	}
	else {
		gluPerspective(20.0, 1.0 /* width / height of screen */, .0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, PNear, PFar);
	}
}

/*
 * --------------------------------------------------------------------
 *							Lightning Group
 * --------------------------------------------------------------------
*/
void Lighting() 
{
	if (isLightOn)
	{
		glEnable(GL_LIGHTING);	//	Turn on lighting for whole screen
	}
	else 
	{
		glDisable(GL_LIGHTING);	//	Turn off lighting for whole screen
	}

	//Light 0 : Red color ambient light at pos(0,1,0) above sphere
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambM);
	glLightfv(GL_LIGHT0, GL_POSITION, posA);
	glEnable(GL_LIGHT0);

	//Light 1 : Green color ambient light at pos(0,1,0) above sphere
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difL);
	glLightfv(GL_LIGHT1, GL_POSITION, posD);
	glEnable(GL_LIGHT1);
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
void DrawSphere(double radius, GLenum drawStyle){
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricTexture(sphere, true);
	gluQuadricDrawStyle(sphere, drawStyle);
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
void DrawAdjustableFillCube(float width, float height, float size) {
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
void DrawCube(float x, float y, float z, float height, float width, float depth, GLenum shape)
{
	// Front face
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y + height, z);
	glEnd();

	// Right face
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Back face
	glBegin(shape);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	glEnd();

	// Left face
	glBegin(shape);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glEnd();

	// Top face
	glBegin(shape);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Bottom face
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
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
void DrawCone(double topRadius, double height, int slices, int stacks, GLenum drawStyle)
{
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricTexture(cylinder, true);
	gluQuadricDrawStyle(cylinder, drawStyle);
	gluCylinder(cylinder, topRadius, 0, height, slices, stacks);
	gluDeleteQuadric(cylinder);
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
void DrawCylinder(double topRadius, double baseRadius, int slices, int stacks, double height, GLenum drawStyle) {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, drawStyle);
	gluCylinder(cylinder, topRadius, baseRadius, height, slices, stacks);
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
void DrawPyramid(float size) {
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);
	// Face 1
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);

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

//	Draw Trapezium
void DrawTriangularPrism(float x, float y, float z, float height, float width, float depth, GLenum shape)
{
	// Front
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();

	// Left
	glBegin(shape);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y, z);
	glEnd();

	// Bottom
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y, z);
	glEnd();

	// Right
	glBegin(shape);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glEnd();

	// Back
	glBegin(shape);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glEnd();
}

//	Reference Line
inline void referenceLine(){
	// X-Axis
	glColor3f(1,0,0);
	DrawCube(0.0, 0.0, 0.0, 0.01f, 1.0f, 0.01f, GL_LINE_LOOP);
	for (int i = 0; i < 10; i++)
	{
		DrawCube(0.0 + 0.1 * i, 0.0, 0.0, 0.02f, 0.02f, 0.02f, GL_LINE_LOOP);
	}

	// Y-Axis
	glColor3f(0,1,0);
	DrawCube(0.0, 0.0, 0.0, 1.0f, 0.01f, 0.01f, GL_LINE_LOOP);
	for (int i = 0; i < 10; i++)
	{
		DrawCube(0.0, 0.0 + 0.1 * i, 0.0, 0.02f, 0.02f, 0.02f, GL_LINE_LOOP);
	}

	// Z-Axis
	glColor3f(0,0,1);
	DrawCube(0.0, 0.0, 0.0, 0.01f, 0.01f, 1.0f, GL_LINE_LOOP);
	for (int i = 0; i < 10; i++)
	{
		DrawCube(0.0, 0.0, 0.0 + 0.1 * i, 0.02f, 0.02f, 0.02f, GL_LINE_LOOP);
	}
	// Center Point
	glColor3f(1,1,0);
	DrawSphere(0.02, GLU_FILL);
}
/*
 * --------------------------------------------------------------------
 *							Robot Parts
 * --------------------------------------------------------------------
*/

void RobotHead()
{
	//-----------------Cover the whole head transformation-----------------//
	glPushMatrix();
	glRotatef(180,0,1,0);
	glTranslatef(-0.1,0.5,-0.2);
	glScalef(0.7,0.7,0.7);
	//-----------------Cover the whole head transformation-----------------//
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
	glTranslatef(-0.075,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(0.05, 0.15,0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.25);
	glTranslatef(-0.025,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(0.05, 0.15,0.15);
	glPopMatrix();
	
	//	Right
	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.2);
	glTranslatef(0.4,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(0.05, 0.15,0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glTranslatef(0.0,0.35, 0.0);
	glTranslatef(0.0,0.0, 0.25);
	glTranslatef(0.35,0.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillLeftHalfPrism(0.05, 0.15,0.15);
	glPopMatrix();

	//------------------Twin Tail------------------//
	//	Left
	glPushMatrix();
	glColor3f(0.0,0.0,0.2);
	glTranslatef(0.0, 0.2, 0.0);
	glTranslatef(0.0, 0.0, 0.2);
	glRotatef(90,0.0,1.0,0.0);
	glTranslatef(0.0, 0.0, -0.2);
	glTranslatef(0.0, 0.1, 0.0);
	DrawFillLeftHalfPrism(0.2, 0.3, 0.2);
	glPopMatrix();

	//	Right
	glPushMatrix();
	glColor3f(0.0,0.0,0.2);
	glTranslatef(0.0, 0.2, 0.0);
	glTranslatef(0.0, 0.0, 0.2);
	glRotatef(-90,0.0,1.0,0.0);
	glTranslatef(0.0, 0.0, -0.5);
	glTranslatef(0.1, 0.0, 0.0);
	glTranslatef(0.0, 0.1, 0.0);
	DrawFillLeftHalfPrism(0.2, 0.3, 0.2);
	glPopMatrix();
	
	//	Neck
	glPushMatrix();
	glColor3f(0.0,1.0,0.0);
	glTranslatef(0.15, 0.0, 0.25);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCylinder(0.05,0.05,0.2);
	glPopMatrix();
	//-----------------Cover the whole head transformation-----------------//
	glPopMatrix();
}

void RobotBody()
{
	//-----------------Cover the whole body transformation-----------------//
	glPushMatrix();
	// glScalef(4.0, 4.0, 4.0);
	//-----------------Cover the whole body transformation-----------------//
	// Body Base
	glPushMatrix();
	glColor3f(1.0,0.5,0.5);
	DrawCube(-0.15, 0.1, -0.10, 0.3, 0.3, 0.2, GL_QUADS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0,0.2,0.5);
	DrawCube(-0.125, -0.15, -0.10, 0.25, 0.25, 0.2, GL_QUADS);
	glPopMatrix();

	//	Armor
	//--------------------Left--------------------//
	glPushMatrix();
	glColor3f(0.6,0.1,0.5);
	glTranslatef(0.5,0,0);
	glRotatef(-360, 0,1,0);
	glTranslatef(-0.4,0.4,0);
	DrawFillPrism(0.1,0.05,0.1);
	glPopMatrix();

	//--------------------Right--------------------//
	glPushMatrix();
	glColor3f(0.6,0.1,0.5);
	glTranslatef(0.5,0,0);
	glRotatef(-360, 0,1,0);
	glTranslatef(-0.6,0.4,0);
	DrawFillPrism(0.1,0.05,0.1);
	glPopMatrix();
	
	// Chest Area
	glPushMatrix();
	//------------------Center Side------------------//
	//	Upper Part
	glPushMatrix();
	glColor3f(0.2,0.1,0.3);
	glTranslatef(-0.15,0.1,0.1001);
	glRotatef(90,1,0,0);
	glTranslatef(0.15,0,-0.25);
	DrawFillPrism(0.2,0.15, -0.1);
	glPopMatrix();
	
	//	Lower Part
	glPushMatrix();
	glColor3f(0.6,0.1,0.3);
	glTranslatef(-0.15,0.1,0.1001);
	glRotatef(90,1,0,0);
	glTranslatef(0.15,0,-0.1);
	DrawFillPrism(0.2,0.15, 0.2);
	glPopMatrix();

	//	Waist
	//	Front
	glPushMatrix();
	glColor3f(0.3,0.2,0.2);
	DrawCube(-0.125, 0, 0.1, 0.05, 0.25, 0.05, GL_QUADS);
	glPopMatrix();

	//`Left Side
	glPushMatrix();
	glColor3f(0.3,0.2,0.2);
	glRotatef(90, 0,1,0);
	DrawCube(-0.125, 0, 0.1, 0.05, 0.25, 0.05, GL_QUADS);
	glPopMatrix();

	//	Back Side
	glPushMatrix();
	glColor3f(0.3,0.2,0.2);
	glRotatef(180, 0,1,0);
	DrawCube(-0.125, 0, 0.1, 0.05, 0.25, 0.05, GL_QUADS);
	glPopMatrix();

	//	Right Side
	glPushMatrix();
	glColor3f(0.3,0.2,0.2);
	glRotatef(-90, 0,1,0);
	DrawCube(-0.125, 0, 0.1, 0.05, 0.25, 0.05, GL_QUADS);
	glPopMatrix();
	
	//	Skirt
	glPushMatrix();
	glColor3f(0.6,0.2,0.5);
	glRotatef(-90,1,0,0);
	glTranslatef(0, 0.0, -0.2);
	DrawFillCylinder(0.25, 0.15, 0.15);
	glPopMatrix();

	//	Pelvis
	//	Left
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glTranslatef(0.1,-0.15, 0);
	glRotatef(90,1,0,0);
	glRotatef(90,0,1,0);
	glRotatef(90,0,0,1);
	DrawFillPrism(0.2,0.05, 0.15);
	glPopMatrix();

	//	Left
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glTranslatef(-0.1,-0.15, 0);
	glRotatef(90,1,0,0);
	glRotatef(90,0,1,0);
	glRotatef(90,0,0,1);
	DrawFillPrism(0.2,0.05, -0.15);
	glPopMatrix();
	
	//-----------------Cover the whole body transformation-----------------//
	glPopMatrix();
}

void RobotArm(){
	//-----------------Cover the whole arm transformation-----------------//
	glPushMatrix();
	// glScalef(4.0, 4.0, 4.0);
	//-----------------Cover the whole arm transformation-----------------//
	//------------------Left------------------//
	//	Upper Armor
	glPushMatrix();
	glColor3f(0.1,0.1,0.5);
	glTranslatef(0.5,0,0);
	glRotatef(-90, 0,1,0);
	glTranslatef(0,0.4,0.2);
	DrawFillPrism(0.15,0.15,0.3);
	glPopMatrix();
	
	//	Shoulder
	glPushMatrix();
	glColor3f(0.2,0.2,0.5);
	glTranslatef(0.5,0,0);
	glRotatef(-90, 0,1,0);
	glTranslatef(0,0.4,0.2);
	DrawFillPrism(-0.15,-0.15,-0.3);
	glPopMatrix();

	//-----------------Cover the left arm transformation-----------------//
	glPushMatrix();
	//-----------------Cover the left arm transformation-----------------//
	//	Arm Upper
	glPushMatrix();
	glColor3f(0.5,0.5,0);
	DrawCube(0.2, 0.09, -0.05, 0.3, 0.1, 0.1, GL_QUADS);
	glPopMatrix();

	//	Arm Center
	glPushMatrix();
	glColor3f(0.1,0.1,0);
	DrawCube(0.2, -0.01, -0.05, 0.1, 0.1, 0.1, GL_QUADS);
	glPopMatrix();

	//	Arm Bottom
	glPushMatrix();
	glColor3f(0.2,0.5,0);
	DrawCube(0.2, -0.21, -0.05, 0.2, 0.1, 0.1, GL_QUADS);
	glPopMatrix();
	//	Left Side
	glPushMatrix();
	glColor3f(0.1,0.2,0.5);
	glTranslatef(0.312, -0.21, 0);
	glRotatef(90,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	//	Back Side
	glPushMatrix();
	glColor3f(0.1,0.2,0.2);
	glTranslatef(0.25, -0.21, -0.07);
	glRotatef(-180,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	//	Front Side
	glPushMatrix();
	glColor3f(0.1,0.5,0.8);
	glTranslatef(0.25, -0.21, 0.07);
	glRotatef(-360,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	//	Right Side
	glPushMatrix();
	glColor3f(1.0,1.0,0.5);
	glTranslatef(0.185, -0.21, 0);
	glRotatef(-90,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();

	//	Hands
	
	
	//-----------------Cover the left arm transformation-----------------//
	glPopMatrix();
	
	//------------------Right------------------//
	//	Upper Armor
	glPushMatrix();
	glColor3f(0.1,0.1,0.5);
	glTranslatef(0.5,0,0);
	glRotatef(90, 0,1,0);
	glTranslatef(0,0.4,-0.8);
	DrawFillPrism(0.15,0.15,0.3);
	glPopMatrix();
	
	//	Shoulder
	glPushMatrix();
	glColor3f(0.2,0.2,0.5);
	glTranslatef(0.5,0,0);
	glRotatef(90, 0,1,0);
	glTranslatef(0,0.4,-0.8);
	DrawFillPrism(-0.15,-0.15,-0.3);
	glPopMatrix();

	//	Arm Upper
	glPushMatrix();
	glColor3f(0.5,0.5,0);
	DrawCube(-0.3, 0.09, -0.05, 0.3, 0.1, 0.1, GL_QUADS);
	glPopMatrix();

	//	Arm Center
	glPushMatrix();
	glColor3f(0.1,0.1,0);
	DrawCube(-0.3, -0.01, -0.05, 0.1, 0.1, 0.1, GL_QUADS);
	glPopMatrix();

	//	Arm Bottom
	glPushMatrix();
	glColor3f(0.2,0.5,0);
	DrawCube(-0.3, -0.21, -0.05, 0.2, 0.1, 0.1, GL_QUADS);
	glPopMatrix();
	//	Right Side
	glPushMatrix();
	glColor3f(0.1,0.2,0.5);
	glTranslatef(-0.312, -0.21, 0);
	glRotatef(-90,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	//	Back Side
	glPushMatrix();
	glColor3f(0.1,0.2,0.2);
	glTranslatef(-0.25, -0.21, -0.07);
	glRotatef(-180,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	//	Front Side
	glPushMatrix();
	glColor3f(0.1,0.5,0.8);
	glTranslatef(-0.25, -0.21, 0.07);
	glRotatef(-360,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	//	Left Side
	glPushMatrix();
	glColor3f(1.0,1.0,0.5);
	glTranslatef(-0.185, -0.21, 0);
	glRotatef(90,0,1,0);
	DrawFillPrism(0.1,0.2,0.025);
	glPopMatrix();
	
	//-----------------Cover the whole arm transformation-----------------//
	glPopMatrix();
}

void RobotLeg(){
	//-----------------Cover the whole leg transformation-----------------//
	glPushMatrix();
	// glScalef(4.0, 4.0, 4.0);
	//-----------------Cover the whole leg transformation-----------------//
	//------------------Left Leg------------------//
	//	Thigh
	glPushMatrix();
	glColor3f(1.0,0.5,0.5);
	DrawCube(0.05, -0.35, -0.10, 0.2, 0.1, 0.2, GL_QUADS);
	glPopMatrix();
	
	//	Left Side
	glPushMatrix();
	glColor3f(0.9,0.1,0.5);
	glTranslatef(0.165, -0.15, 0);
	glRotatef(90,0,1,0);
	glRotatef(-180,1,0,0);
	DrawFillPrism(0.2,0.2,-0.02);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9,0.1,0.5);
	glTranslatef(0.04, -0.15, 0);
	glRotatef(-90,0,1,0);
	glRotatef(-180,1,0,0);
	DrawFillPrism(0.2,0.2,-0.02);
	glPopMatrix();
	
	//	Knee
	glPushMatrix();
	glColor3f(1.0,0.8,0.5);
	DrawCube(0.05, -0.55, -0.10, 0.2, 0.1, 0.2, GL_QUADS);
	glPopMatrix();

	//	Ankle
	glPushMatrix();
	glColor3f(0.8,0.5,0.5);
	DrawCube(0.05, -0.85, -0.10, 0.3, 0.1, 0.2, GL_QUADS);
	glPopMatrix();

	//	Left Side
	glPushMatrix();
	glColor3f(0.9,0.1,0.5);
	glTranslatef(0.175, -0.85, 0);
	glRotatef(90,0,1,0);
	DrawFillPrism(0.2,0.3,0.05);
	glPopMatrix();

	//	Feet
	glPushMatrix();
	glColor3f(0.3,0.5,0.5);
	DrawCube(0.05, -0.85, 0.10, 0.05, 0.1, 0.1, GL_QUADS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1,0.1,0.5);
	glTranslatef(0.1, -0.8, 0.15);
	DrawFillPrism(0.1,0.1,0.1);
	glPopMatrix();
	
	//------------------Right Leg------------------//
	//	Thigh
	glPushMatrix();
	glColor3f(1.0,0.5,0.5);
	DrawCube(-0.15, -0.35, -0.10, 0.2, 0.1, 0.2, GL_QUADS);
	glPopMatrix();

	//	Right Side
	glPushMatrix();
	glColor3f(0.9,0.1,0.5);
	glTranslatef(-0.165, -0.15, 0);
	glRotatef(-90,0,1,0);
	glRotatef(-180,1,0,0);
	DrawFillPrism(0.2,0.2,-0.02);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9,0.1,0.5);
	glTranslatef(-0.04, -0.15, 0);
	glRotatef(90,0,1,0);
	glRotatef(-180,1,0,0);
	DrawFillPrism(0.2,0.2,-0.02);
	glPopMatrix();

	//	Knee
	glPushMatrix();
	glColor3f(1.0,0.8,0.5);
	DrawCube(-0.15, -0.55, -0.10, 0.2, 0.1, 0.2, GL_QUADS);
	glPopMatrix();

	//	Ankle
	glPushMatrix();
	glColor3f(0.8,0.5,0.5);
	DrawCube(-0.15, -0.85, -0.10, 0.3, 0.1, 0.2, GL_QUADS);
	glPopMatrix();

	//	Right Side
	glPushMatrix();
	glColor3f(0.9,0.1,0.5);
	glTranslatef(-0.175, -0.85, 0);
	glRotatef(-90,0,1,0);
	DrawFillPrism(0.2,0.3,0.05);
	glPopMatrix();

	//	Feet
	glPushMatrix();
	glColor3f(0.3,0.5,0.5);
	DrawCube(-0.15, -0.85, 0.10, 0.05, 0.1, 0.1, GL_QUADS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1,0.1,0.5);
	glTranslatef(-0.1, -0.8, 0.15);
	DrawFillPrism(0.1,0.1,0.1);
	glPopMatrix();
	//-----------------Cover the whole body transformation-----------------//
	glPopMatrix();
}

//	EX & XT Version
void RobotBody1(){
	//-----------------Cover the whole body transformation-----------------//
	glPushMatrix();
	glTranslatef(-0.1,0.5,0);
	glScalef(0.1, 0.1, 0.1);
	glTranslatef(1, -6, 0);
	//-----------------Cover the whole body transformation-----------------//
	// Body Base
	glPushMatrix();
	glScalef(1.0f, 1.75f, 0.25f);
	glTranslatef(-2.0f, -1.0f, 0.0f);
	glTranslatef(0.5,0,0);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawCube(3.0f,3.0,3.0f, 3.0f, 3.0f,5.0f, GL_QUADS);
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
	//-----------------Cover the whole body transformation-----------------//
	glPopMatrix();
}
void RobotArm1()
{
	//------------------Left Hand------------------//
	glPushMatrix();
	glTranslatef(-0.6, 0.5, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.65, 0.5, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0.5, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0.4, 0.25);
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.2, 0.25);
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCylinder(0.05, 0.05, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.1, 0.25);
	glColor3f(0.0, 1.0, 1.5);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFullSquare(0.05, 0.05, 0.2, 0.2);
	glPopMatrix();


	//------------------Right Hand------------------//
	glPushMatrix();
	glTranslatef(0.3, 0.5, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.5, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0.5, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0.4, 0.25);
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0.2, 0.25);
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFillCylinder(0.05, 0.05, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0.1, 0.25);
	glColor3f(0.0, 1.0, 1.5);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFullSquare(0.05, 0.05, 0.2, 0.2);
	glPopMatrix();
}
void RobotLeg1(){
	//------------------Left Feet------------------//
	glPushMatrix();
	glTranslatef(-0.3, 0.05, 0.25);
	glColor3f(2.5, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFullCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, 0.05, 0.25);
	glColor3f(2.5, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFullCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, 0.05, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	DrawFillPyramid(0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, 0.1, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawFullCube(0.2);
	glPopMatrix();

	//------------------Left Leg------------------//
	glPushMatrix();	
	glTranslatef(-0.35, 0.0, 0.0);
	glTranslatef(0.0, -0.6, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//DrawFullTriangle(-0.08, -0.11, -0.08, -0.01, -0.001, -0.01);
	DrawFillPyramid(0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.35, 0.0, 0.0);
	glTranslatef(0.0, -0.7, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//DrawFullTriangle(-0.08, -0.11, -0.08, -0.01, -0.001, -0.01);
	DrawFillPyramid(0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.35, 0.0, 0.0);
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//DrawFullTriangle(-0.08, -0.11, -0.08, -0.01, -0.001, -0.01);
	DrawFillPyramid(0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 1.0);
	//DrawAdjustableFillCube(0.2, 0.4, 0.2);
	DrawFullCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glTranslatef(0.0, -0.9, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 1.0);
	//DrawAdjustableFillCube(0.2, 0.4, 0.2);
	DrawFullCube(0.2);
	glPopMatrix();


	//------------------Left Small Leg------------------//
	glPushMatrix();	//Pyramid 1
	glTranslatef(-0.5, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glColor3f(0.0, 1.0, 0.0);
	DrawFillPyramid(0.2, 0.1);
	glPopMatrix();

	glPushMatrix();	//Triangle 1
	glTranslatef(-0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.38, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	glPushMatrix();	//Triangle 2
	glTranslatef(-0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.38, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	glPushMatrix();	//Pyramid 2
	glTranslatef(-0.3, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(0.0, 1.0, 0.0);
	DrawFillPyramid(0.2, 0.1);
	glPopMatrix();

	glPushMatrix();	//Triangle 3
	glTranslatef(-0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.62, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	glPushMatrix();	//Triangle 4
	glTranslatef(-0.28, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 1.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	
	//------------------Left Thigh Leg------------------//
	glPushMatrix();
	glTranslatef(-0.5, -0.2, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawAdjustableFillCube(0.2, 0.4, 0.2);
	glPopMatrix();


	//------------------Right Feet------------------//
	glPushMatrix();
	glTranslatef(0.2, -1.0, 0.25);
	glColor3f(0.0, 0.9, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawAdjustableFillCube(0.2, 0.4, 0.2);
	glPopMatrix();

	//------------------Right Leg------------------//
	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glTranslatef(0.0, -0.6, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//DrawFullTriangle(-0.08, -0.11, -0.08, -0.01, -0.001, -0.01);
	DrawFillPyramid(0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glTranslatef(0.0, -0.7, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//DrawFullTriangle(-0.08, -0.11, -0.08, -0.01, -0.001, -0.01);
	DrawFillPyramid(0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0.0, 0.0);
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//DrawFullTriangle(-0.08, -0.11, -0.08, -0.01, -0.001, -0.01);
	DrawFillPyramid(0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 1.0);
	//DrawAdjustableFillCube(0.2, 0.4, 0.2);
	DrawFullCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glTranslatef(0.0, -0.9, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 1.0);
	//DrawAdjustableFillCube(0.2, 0.4, 0.2);
	DrawFullCube(0.2);
	glPopMatrix();


	//------------------Right Small Leg------------------//
	glPushMatrix();	//Pyramid 1
	glTranslatef(0.0, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glColor3f(0.0, 1.0, 0.0);
	DrawFillPyramid(0.2, 0.1);
	glPopMatrix();

	glPushMatrix();	//Triangle 1
	glTranslatef(0.1, 0.0, 0.0);
	glTranslatef(0.0, -0.38, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	glPushMatrix();	//Triangle 2
	glTranslatef(0.1, 0.0, 0.0);
	glTranslatef(0.0, -0.38, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	glPushMatrix();	//Pyramid 2
	glTranslatef(0.2, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 0.0, 0.0, 1.0);
	glColor3f(0.0, 1.0, 0.0);
	DrawFillPyramid(0.2, 0.1);
	glPopMatrix();

	glPushMatrix();	//Triangle 3
	glTranslatef(0.1, 0.0, 0.0);
	glTranslatef(0.0, -0.62, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	glPushMatrix();	//Triangle 4
	glTranslatef(0.22, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glRotatef(180, 1.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	DrawFullTriangle(-0.1, -0.12, -0.1, -0.02, -0.0001, -0.02);
	glPopMatrix();

	//------------------Right Thigh Leg------------------//
	glPushMatrix();
	glTranslatef(0.0, -0.2, 0.25);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawAdjustableFillCube(0.2, 0.4, 0.2);
	glPopMatrix();
}

// TBA : Other stuff like weapons, etc.
//	Weapon: Loudspeaker
void LoudSpeaker()
{
	glPushMatrix();
	glRotatef(90,0,0,1);
	glRotatef(180,1,0,0);
	glScalef(0.3,0.3,0.3);
	//-----------------Cover the whole head transformation-----------------//
	//	Outer
	glPushMatrix();
	glColor3f(0.0,0.3,1.0);
	DrawFillCylinder(0.1,0.3,0.5);
	glPopMatrix();
	
	//	Inner
	glPushMatrix();
	glColor3f(1.0,0.3,1.0);
	glTranslatef(0,0,-0.1);
	DrawFillCylinder(0.1,0.1, 0.6);
	glPopMatrix();

	//	Front
	glPushMatrix();
	glColor3f(1.0,1.0,0.0);
	glTranslatef(0,0,0.5);
	DrawFillSphere(0.1);
	glPopMatrix();

	//	Handle
	glPushMatrix();
	glColor3f(1.0,1.0,0.0);
	glRotatef(-90,0,1.0,0);
	DrawFillCylinder(0.1,0.05, 0.3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0,1.0,0.0);
	glRotatef(-90,0,1.0,0);
	glTranslatef(0.0,0,0.3);
	DrawFillSphere(0.05);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0,1.0,0.0);
	glTranslatef(0.0,0,-0.3);
	DrawFillCylinder(0.05,0.1,0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0,0.0,1.0);
	glTranslatef(0.0,0,-0.3);
	DrawFillSphere(0.05);
	glPopMatrix();
	//-----------------Cover the whole loudspeaker transformation-----------------//
	glPopMatrix();
}
//	Weapon: Green Onions
void OnionSword()
{
	glPushMatrix();
	glRotatef(90,0,0,1);
	glRotatef(180,1,0,0);
	glScalef(0.7,0.7,0.7);
	//-----------------Cover the whole onion sword transformation-----------------//
	//	Handle
	glPushMatrix();
	glColor3f(0.0,0.3,0.3);
	DrawFillCylinder(0.03,0.03,0.7);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.1,0.3);
	glTranslatef(0,0.0,0.7);
	DrawFillSphere(0.03);
	glPopMatrix();

	//	Upper part:Left
	glPushMatrix();
	glColor3f(0.0,0.3,0.1);
	glRotatef(-165,1,0,0);
	DrawFillCylinder(0.03,0.03,0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.1,0.1);
	glRotatef(-165,1,0,0);
	glTranslatef(0,0,0.4);
	DrawFillCone(0.03,0.1);
	glPopMatrix();

	//	Upper part:Right
	glPushMatrix();
	glColor3f(0.0,0.3,0.5);
	glRotatef(165,1,0,0);
	DrawFillCylinder(0.03,0.03,0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.1,0.1);
	glRotatef(165,1,0,0);
	glTranslatef(0,0,0.2);
	DrawFillCone(0.03,0.1);
	glPopMatrix();
	//-----------------Cover the whole onion sword transformation-----------------//
	glPopMatrix();
}
//	Weapon Trigger
void WeaponTrigger()
{
	if(gunOn)
	{
		LoudSpeaker();
	}
	else if(swordOn)
	{
		OnionSword();
	}
}
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

	referenceLine();

	//	Transformation
	glRotatef(rSpeed, rx, ry,rz);
	// Draw Robot Here

	
	RobotHead();
	
	RobotBody();
	RobotArm();
	RobotLeg();
	WeaponTrigger();
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
		Lighting();
		// Projection();
		Display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
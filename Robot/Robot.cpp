#include <Windows.h>
#include <cmath>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#define M_PI 3.14159265358979323846////
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

// change mode
bool isHeadMovement = false;
bool isHandMovement = true;
bool isLegMovement = false;
bool isBodyMovement = false;
bool defaultMode = false;

//Camera
float Tx = 0, Tz = 0, TSpeed = 1.0;        //translate z-axis with TSpeed
float PTx = 0, PTy = 0, PTz = -0.6, PTSpeed = 0.1;            //Translate for the projection
bool isOrtho = true;
float ONear = -100.0, OFar = 100.0;  //Ortho near and far
float PNear = -50, PFar = 50;    //Perspective near and far
float radius1 = 3.0;               //Radius of sphere 1
float PRy = 0.0, PRx = 0.0, PRSpeed = 1.0;    //Rotation, Ry for projection

//Lighting
float angle = 0;

//lightSource
float difpos[3] = { 0,30,-80 };//diffuse light pos
float dif[3] = { 1.0,0.0,0.0 };//red color difuse light
float difM[3] = { 0.0,0.0,1.0 };//blue color dif material
bool isLightOn = false;//is light on?

float black[][3] = {
		{0.3, 0.3, 0.3},  // Red
		{0.7, 0.7, 0.7},  // Green
		{0.75, 0.75, 0.75},  // Blue
};

float black1[][3] = {
		{0.9, 0.3, 0.3},  // Red
		{0.9, 0.7, 0.7},  // Green
		{0.9, 0.75, 0.75},  // Blue
};

float black2[][3] = {
		{0.3, 0.9, 0.3},  // Red
		{0.7, 0.9, 0.7},  // Green
		{0.75, 0.9, 0.75},  // Blue
};

float black3[][3] = {
		{0.3, 0.3, 0.8},  // Red
		{0.7, 0.7, 0.8},  // Green
		{0.75, 0.75, 0.8},  // Blue
};

float color[3][3] = {
	{black[0][0], black[0][1], black[0][2]},
	{black[1][0], black[1][1], black[1][2]},
	{black[2][0], black[2][1], black[2][2]}
};
int countColor = 0;
int textureCount = 1;

// arms
float handSpeed = 1.0;
float wholeArmAngleL = 0, forearmAngleL = 0;
float wholeArmAngleR = 0, forearmAngleR = 0;
bool isWeaponOut = false, isSwordOut = false;

//Head
float headAngle = 0;

//Body Movement
float r = 0, bodyR;  //Angle
float bodySpeed = 2;
bool front = true, back = false, frontB = true, backB = false;

//Leg
float legAngle = 0;
float legAngle2 = 0;
float legAngle3 = 0;

bool isChangeTexture = false;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_CAPITAL) {
			isChangeTexture = !isChangeTexture;
			if (isChangeTexture) {
				isHandMovement = false;
			}
		}
		else if (wParam == '9') {
			countColor++;

			if (countColor >= 4) {
				countColor = 0;
			}
			
			if (countColor == 0) {
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						color[i][j] = black1[i][j];
					}
				}
			}
			if (countColor == 1) {
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						color[i][j] = black[i][j];
					}
				}
			}
			else if (countColor == 2) {
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						color[i][j] = black2[i][j];
					}
				}
			}
			else if (countColor == 3) {
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						color[i][j] = black3[i][j];
					}
				}
			}
		}
		else if (wParam == VK_SPACE)
		{
			isLightOn = !isLightOn; // on/off light
			/*difpos[0] = 50.0;
			difpos[1] = 50.0;
			difpos[2] = 50.0;*////////////////////////////////////////////////////

		}
		/*
		else if (wParam == VK_UP) {     //Press UP arrow key
			if (isOrtho) {
				if (Tz > ONear) {
					Tz -= TSpeed;
				}
			}
			else {
				if (Tz > PNear + radius1) {  //(Tz > PNear + radius1)
					Tz -= TSpeed;
				}
			}
		}
		else if (wParam == VK_DOWN) {   //Press DOWN arrow key
			if (isOrtho) {
				if (Tz < OFar) {
					Tz += TSpeed;
				}
			}
			else {
				if (Tz < PFar) {
					Tz += TSpeed;
				}
			}
		}
		else if (wParam == VK_LEFT) {  //Press LEFT arrow key
			
			Tx -= TSpeed;              //Move the model to the left
		}
		else if (wParam == VK_RIGHT) { //Press RIGHT arrow key
			
			Tx += TSpeed;              //Move the model to the right
		}*/
		else if (wParam == VK_UP) // press up
		{
			difpos[1] += 5; // Update y position of diffuse light
		}
		else if (wParam == VK_DOWN) // press down
		{
			difpos[1] -= 5; // Update y position of diffuse light
		}
		else if (wParam == VK_LEFT)
		{ // press left
			angle -= 90;
			difpos[0] -= 5; // Update x position of diffuse light
		}
		else if (wParam == VK_RIGHT)
		{ // press right
			angle += 90;
			difpos[0] += 5; // Update x position of diffuse light
		}
		else if (wParam == '0') {
			defaultMode = true;
			isHeadMovement = false;
			isHandMovement = false;
			isLegMovement = false;
			isBodyMovement = false;
		}
		else if (wParam == '1') {
		if (isHandMovement) {
			isWeaponOut = true; // weapon on
		}
		if (isChangeTexture) {
			textureCount = 1;
		}
		}
		else if (wParam == '2') {
		if (isHandMovement) {
			if (isWeaponOut) {
				isSwordOut = true; // sword on
			}
		}
		if (isChangeTexture) {
			textureCount = 2;
		}
		}
		else if (wParam == '3') {
		if (isHandMovement) {
			isWeaponOut = false;
			isSwordOut = false;
		}
		if (isChangeTexture) {
			textureCount = 3;
		}
		}
		else if (wParam == '4') {
		if (isChangeTexture) {
			textureCount = 4;
		}
		}
		else if (wParam == 'A') {
			PTx += PTSpeed;             //Move projection to right
		}
		else if (wParam == 'B')
		{
			if (isLegMovement) {
				if (legAngle >= 0) {
					legAngle -= 10;
				}
			}
			if (isHandMovement)
			{
				if (wholeArmAngleL > 0) {
					wholeArmAngleL -= handSpeed;
				}
			}
			}
		else if (wParam == 'C')
		{
			if (isLegMovement) {
				if (legAngle2 <= 0) {
					legAngle2 += 10;
				}
			}

			if (isOrtho) {
				if (PTz > ONear) {
					PTz -= TSpeed;
				}
			}
			else {
				if (PTz > PNear) {  //(Tz > PNear + radius1)
					PTz -= TSpeed;
				}
			}
			}
		else if (wParam == 'D') {
			PTx -= PTSpeed;             //Move projection to left

			if (isOrtho) {
				if (PTz < OFar) {
					PTz += TSpeed;
				}
			}
			else {
				if (PTz < PFar) {
					PTz += TSpeed;
				}
			}
		}
		else if (wParam == 'E') {

		}
		else if (wParam == 'F')
		{
			if (defaultMode) {
				difpos[2] += 5; // Update z position of diffuse light
			}
			if (isHandMovement)
			{
				if (wholeArmAngleR < 45) {
					wholeArmAngleR += handSpeed;
				}
			}
		}
		else if (wParam == 'G') {
			if (isHandMovement)
			{
				if (wholeArmAngleR > 0) {
					wholeArmAngleR -= handSpeed;
				}
			}
		}
		else if (wParam == 'H') {
			isHandMovement = true;

			defaultMode = false;
			isHeadMovement = false;
			isLegMovement = false;
			isBodyMovement = false;
		}
		else if (wParam == 'I') {
			isBodyMovement = true;

			defaultMode = false;
			isHeadMovement = false;
			isHandMovement = false;
			isLegMovement = false;
		}
		else if (wParam == 'J') {
			if (isHandMovement)
			{
				if (forearmAngleL < 45) {
					forearmAngleL += handSpeed;
				}
			}
			else if (isBodyMovement) {
				if (front) {
					r -= bodySpeed;
					wholeArmAngleL -= bodySpeed;
					wholeArmAngleR -= bodySpeed;

					if (r <= -20) {
						back = true;
						front = false;
					}
				}

				if (back) {
					r += bodySpeed;
					wholeArmAngleL += bodySpeed;
					wholeArmAngleR += bodySpeed;

					if (r >= 20) {
						back = false;
						front = true;
					}
				}

				if (frontB) {
					bodyR -= (bodySpeed - 1.5);

					if (bodyR <= -5) {
						backB = true;
						frontB = false;
					}
				}

				if (backB) {
					bodyR += (bodySpeed - 1.5);

					if (bodyR >= 5) {
						backB = false;
						frontB = true;
					}
				}
			}
		}
		else if (wParam == 'K') {
			isLegMovement = true;

			defaultMode = false;
			isHeadMovement = false;
			isHandMovement = false;
			isBodyMovement = false;
			}
		else if (wParam == 'L') {
		if (isOrtho == false) {
			if (PRy >= 26) {
				PRSpeed = 26;
			}else
				PRy += PRSpeed;
		}else
			PRy += PRSpeed;
				
				if (isHeadMovement) { headAngle += 10; }//Rotate along y-axis anticlockwise for projection
				}
		else if (wParam == 'M')
		{
			if (isLegMovement) {
				if (legAngle3 >= -30)
				{
					legAngle3 -= 10;
				}
			}
			if (isHandMovement)
			{
				if (wholeArmAngleL < 45) {
					wholeArmAngleL += handSpeed;
				}
			}
		}
		else if (wParam == 'N')
		{
			if (defaultMode) {
				difpos[2] -= 5; // Update z position of diffuse light
			}

			if (isHandMovement)
			{
				if (forearmAngleL > 0) {
					forearmAngleL -= handSpeed;
				}
			}
			}
		else if (wParam == 'O') {    //Press 'O' key
				isOrtho = true;
				//Tz = 0;
				}
		else if (wParam == 'P') {   //Press 'P' key
					isOrtho = false;
					//Tz = PNear + radius1;   //So the sphere will not otu of the perspective
					}
		else if (wParam == 'Q') {
			if (PRx >= 0) {
				PRx = 0;
			}
			else
				PRx += PRSpeed;            //Rotate along x-axis anticlockwise for projection
		}
		else if (wParam == 'R') {
          //Rotate along y-axis clockwise for projection
			if (isOrtho == false) {
				if (PRy <= -26) {
					PRSpeed = -26;
				}
				else
					PRy -= PRSpeed;
			}else
				PRy -= PRSpeed;

			if (isHeadMovement) { headAngle -= 10; }
}
		else if (wParam == 'S') {
			PTy += PTSpeed;             //Move projection to down
		}
		else if (wParam == 'T') {
			if (isHandMovement)
			{
				if (forearmAngleR < 45) {
					forearmAngleR += handSpeed;
				}
			}
		}
		else if (wParam == 'U') {
			if (PRx <= -90) {
				PRx = -90;
			}else	
				PRx -= PRSpeed;            //Rotate along x-axis clockwise for projection
		}
		else if (wParam == 'V') {
			if (isHandMovement)
			{
				if (forearmAngleR > 0) {
					forearmAngleR -= handSpeed;
				}
			}
			else if (isLegMovement) {
				if (legAngle3 <= 0) {
					legAngle3 += 10;
				}
			}
		}
		else if (wParam == 'W') {       //mnbbn
			PTy -= PTSpeed;             //Move projection to up
		}
		else if (wParam == 'X')
		{
			if (isLegMovement) {
				if (legAngle <= 70) {
					legAngle += 10;
				}
			}
		}
		else if (wParam == 'Y') {
			isHeadMovement = true;

			defaultMode = false;
			isHandMovement = false;
			isLegMovement = false;
			isBodyMovement = false;
		}
		else if (wParam == 'Z') {
			if (isLegMovement)
			{
				if (legAngle2 >= -70) {
					legAngle2 -= 10;
				}
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

void projection() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);    //Refer to the projection matrix
	glLoadIdentity();				//Reset the projection matrix
	glTranslatef(PTx, PTy, PTz);    //Translation for projection matrix //Move world frame
	glRotatef(PRx, 1.0, 0.0, 0.0);       //Rotation for projection matrix
	glRotatef(PRy, 0.0, 1.0, 0.0);       //Rotation for projection matrix

	if (isOrtho) {
		glOrtho(-100.0, 100.0, -100.0, 100.0, ONear, OFar);  //Ortho, default view - 100 cube
	}
	else {
		gluPerspective(100, 1.0, 1, 100);
		glTranslatef(0, 0, -100);
		//glFrustum(-100.0, 100.0, -100.0, 100, 1, 200);  //Perspective
	}


}

GLuint loadTexture(LPCSTR filename) {
	BITMAP BMP;
	HBITMAP hBMP = NULL;
	GLuint texture = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);
	return texture;
}

//------------------------------------------------------ShengMingFunction
void drawCubeS(float size, double R, double G, double B)
{

	glBegin(GL_QUADS);
	// Face 1 : Bottom
	glColor3f(R, G, B);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	// Face 2 : Left
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, size, 0.0f);
	// Face 3 : Back
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, size, 0.0f);
	// Face 4 : Right
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);
	// Face 5 : Front
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	// Face 6 : Top
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();

}
void drawCuboidS(float width, float height, float depth, double R, double G, double B)
{
	glBegin(GL_QUADS);
	// Face 1 : Bottom
	glColor3f(R, G, B);
	glVertex3f(0.0f, 0.0f, depth);
	glVertex3f(width, 0.0f, depth);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	// Face 2 : Left
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, depth);
	glVertex3f(0.0f, height, depth);
	glVertex3f(0.0f, height, 0.0f);
	// Face 3 : Back
	glVertex3f(0.0f, height, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(width, height, 0.0f);
	// Face 4 : Right
	glVertex3f(width, height, 0.0f);
	glVertex3f(width, 0.0f, 0.0f);
	glVertex3f(width, 0.0f, depth);
	glVertex3f(width, height, depth);
	// Face 5 : Front
	glVertex3f(width, height, depth);
	glVertex3f(width, 0.0f, depth);
	glVertex3f(0.0f, 0.0f, depth);
	glVertex3f(0.0f, height, depth);
	// Face 6 : Top
	glVertex3f(0.0f, height, depth);
	glVertex3f(width, height, depth);
	glVertex3f(width, height, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	glEnd();
}
void drawPyramidS(float size)
{

	glBegin(GL_TRIANGLES);
	// Base
	glColor3f(1, 1, 1);  // Red
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);
	glVertex3f(0.0f, 0.0f, size);

	// Front face
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);

	// Right face
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);

	// Back face
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Left face
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);

	glEnd();
}
void drawWireFrameCubeS(float size) {
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);

	// Bottom face
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);
	glVertex3f(0.0f, 0.0f, size);

	glEnd();

	glBegin(GL_LINE_LOOP);

	// Top face
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, size, size);
	glVertex3f(0.0f, size, size);

	glEnd();

	glBegin(GL_LINES);

	// Connect top and bottom faces
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);

	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, size, 0.0f);

	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);

	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);

	glEnd();
}
void drawSphereS(double r, double R, double G, double B) {

	glColor3f(R, G, B);
	glLineWidth(1);
	GLUquadricObj* sphere = NULL; //quadric obj pointer
	sphere = gluNewQuadric();     //create quadric obj
	//gluQuadricDrawStyle(sphere, GLU_LINE);//set draw style
	gluQuadricTexture(sphere, true);
	gluSphere(sphere, r, 30, 30);//draw sphere
	gluDeleteQuadric(sphere);       //delete quadric obj pointer/clear up memory
}
void drawCylinderS(double br, double tr, double h, double R, double G, double B) {
	glColor3f(R, G, B);
	glLineWidth(1);
	GLUquadricObj* cylinder = NULL; //quadric obj pointer
	cylinder = gluNewQuadric();     //create quadric obj
	gluQuadricDrawStyle(cylinder, GLU_LINE);//set draw style
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, br, tr, h, 30, 30);//draw sphere
	gluDeleteQuadric(cylinder);       //delete quadric obj pointer/clear up memory

}
void drawConeS(double tr, double h) {
	glLoadIdentity();
	glTranslatef(0, -0.5, 0);
	glColor3f(0.73, 0.48, 0.34);
	glLineWidth(1);
	glRotatef(-90, 1, 0, 0);
	GLUquadricObj* cone = NULL; //quadric obj pointer
	cone = gluNewQuadric();     //create quadric obj
	//gluQuadricDrawStyle(cone, GLU_LINE);//set draw style
	gluQuadricTexture(cone, true);
	gluCylinder(cone, 0, tr, h, 30, 30);//draw sphere
	gluDeleteQuadric(cone);       //delete quadric obj pointer/clear up memory

}
void drawDiskS(double inr, double outr) {
	glLineWidth(1);
	GLUquadricObj* disk = NULL; //quadric obj pointer
	disk = gluNewQuadric();     //create quadric obj
	gluQuadricDrawStyle(disk, GLU_FILL);//set draw style
	gluQuadricTexture(disk, true);
	gluDisk(disk, inr, outr, 30, 30);//draw sphere
	gluDeleteQuadric(disk);       //delete quadric obj pointer/clear up memory
}
void drawSphereWithoutGLUS()
{
	const float PI = 3.141592f;
	GLfloat x, y, z, sliceA, stackA;
	GLfloat radius = 0.5;
	int sliceNo = 30, stackNo = 30;

	for (sliceA = 0.0; sliceA < PI; sliceA += PI / sliceNo)
	{
		glBegin(GL_LINE_STRIP);
		for (stackA = 0.0; stackA < PI; stackA += PI / stackNo)
		{
			x = radius * cos(stackA) * sin(sliceA);
			y = radius * sin(stackA) * sin(sliceA);
			z = radius * cos(sliceA);
			glVertex3f(x, y, z);
			x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = radius * cos(sliceA + PI / sliceNo);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}
//---------------------------------------------------------------------ShengMingFunctionEnd

//---------------------------------------WanCheeFunction
void drawSphere(double radius) {

	GLUquadricObj* sphere = NULL;						//quadric obj pointer
	sphere = gluNewQuadric();								//create the quadric obbj
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, true);
	gluSphere(sphere, radius, 30, 30);						//draw shpere
	gluDeleteQuadric(sphere);								//delete quadric obj pointer / free up memory
}
void drawCube(float size, float x, float y, float z) {
	glBegin(GL_QUADS);

	// Front face
	glVertex3f(-size + x, -size + y, size + z);
	glVertex3f(size + x, -size + y, size + z);
	glVertex3f(size + x, size + y, size + z);
	glVertex3f(-size + x, size + y, size + z);

	// Back face
	glVertex3f(-size + x, -size + y, -size + z);
	glVertex3f(size + x, -size + y, -size + z);
	glVertex3f(size + x, size + y, -size + z);
	glVertex3f(-size + x, size + y, -size + z);

	// Left face
	glVertex3f(-size + x, -size + y, size + z);
	glVertex3f(-size + x, size + y, size + z);
	glVertex3f(-size + x, size + y, -size + z);
	glVertex3f(-size + x, -size + y, -size + z);

	// Right face
	glVertex3f(size + x, -size + y, size + z);
	glVertex3f(size + x, size + y, size + z);
	glVertex3f(size + x, size + y, -size + z);
	glVertex3f(size + x, -size + y, -size + z);

	// Top face
	glVertex3f(-size + x, size + y, size + z);
	glVertex3f(size + x, size + y, size + z);
	glVertex3f(size + x, size + y, -size + z);
	glVertex3f(-size + x, size + y, -size + z);

	// Bottom face
	glVertex3f(-size + x, -size + y, size + z);
	glVertex3f(size + x, -size + y, size + z);
	glVertex3f(size + x, -size + y, -size + z);
	glVertex3f(-size + x, -size + y, -size + z);

	glEnd();
}
void drawCone(GLenum drawStyle, double topr, double height, int slices, int stacks) {
	GLUquadricObj* cone = NULL;
	cone = gluNewQuadric();
	// gluQuadricDrawStyle(cone, GLU_LINE); /
	gluQuadricDrawStyle(cone, drawStyle);
	gluQuadricTexture(cone, true);
	gluCylinder(cone, 0, topr, height, slices, stacks);	//if one radius make it 0.0, a cone is made
	gluDeleteQuadric(cone);
}
void drawRectangle(float x, float y, float z, float width, float height, float depth) {
	glBegin(GL_QUADS);
	//glTexCoord2f();
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();
}
void drawCylinder(double baseR, double topR, double height) {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, baseR, topR, height, 30, 30);	//if one radius make it 0.0, a cone is made
	gluDeleteQuadric(cylinder);
}
void drawTriangle(float x, float y, float length) {
	glBegin(GL_TRIANGLES); 
	glVertex3f(x, y + length, 0.0f); // Vertex 1
	glVertex3f(x - length, y - length, 0.0f); // Vertex 2
	glVertex3f(x + length, y - length, 0.0f); // Vertex 3
	glEnd();
}
void drawRightTriangle(float x, float y, float length) {
	glBegin(GL_TRIANGLES);
	glVertex3f(x, y, 0.0f); // Vertex 1
	glVertex3f(x - length, y - length, 0.0f); // Vertex 2
	glVertex3f(x + length, y - length, 0.0f); // Vertex 3
	glEnd();
}

//-------------------------------------JenFunction
void sphere(GLenum style, double R, double G, double B, float radius, float sX, float sY, float sZ, float rotateDegree, float rX, float rY, float rZ) {
	glPushMatrix();
	glColor3f(R, G, B);
	glRotatef(rotateDegree, rX, rY, rZ);
	glScalef(sX, sY, sZ);
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, style);
	gluQuadricTexture(sphere, true);
	gluSphere(sphere, radius, 30, 10);
	gluDeleteQuadric(sphere);
	glPopMatrix();
}
void cylinder(GLenum style, float baserad, float toprad, float height, float rotateDegree, float rX, float rY, float rZ) {
	glPushMatrix();
	glRotatef(rotateDegree, rX, rY, rZ);
	glLineWidth(1.0);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, style);
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, baserad, toprad, height, 30, 10);
	gluDeleteQuadric(cylinder);
	glPopMatrix();
}
void cylinderJ(GLenum style, double R, double G, double B, float baserad, float toprad, float height, float rotateDegree, float rX, float rY, float rZ) {
	glPushMatrix();
	glColor3f(R, G, B);
	glRotatef(rotateDegree, rX, rY, rZ);
	glLineWidth(1.0);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, style);
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, baserad, toprad, height, 30, 10);
	gluDeleteQuadric(cylinder);
	glPopMatrix();
}
void drawCubeJ(double R, double G, double B, float size, float x, float y, float z) {
	glPushMatrix();
	glColor3f(R, G, B);
	glBegin(GL_QUADS);

	// Front face
	glVertex3f(-size * x, -size * y, size * z);
	glVertex3f(size * x, -size * y, size * z);
	glVertex3f(size * x, size * y, size * z);
	glVertex3f(-size * x, size * y, size * z);

	// Back face
	glVertex3f(-size * x, -size * y, -size * z);
	glVertex3f(size * x, -size * y, -size * z);
	glVertex3f(size * x, size * y, -size * z);
	glVertex3f(-size * x, size * y, -size * z);

	// Left face
	glVertex3f(-size * x, -size * y, size * z);
	glVertex3f(-size * x, size * y, size * z);
	glVertex3f(-size * x, size * y, -size * z);
	glVertex3f(-size * x, -size * y, -size * z);

	// Right face
	glVertex3f(size * x, -size * y, size * z);
	glVertex3f(size * x, size * y, size * z);
	glVertex3f(size * x, size * y, -size * z);
	glVertex3f(size * x, -size * y, -size * z);

	// Top face
	glVertex3f(-size * x, size * y, size * z);
	glVertex3f(size * x, size * y, size * z);
	glVertex3f(size * x, size * y, -size * z);
	glVertex3f(-size * x, size * y, -size * z);

	// Bottom face
	glVertex3f(-size * x, -size * y, size * z);
	glVertex3f(size * x, -size * y, size * z);
	glVertex3f(size * x, -size * y, -size * z);
	glVertex3f(-size * x, -size * y, -size * z);

	glEnd();
	glPopMatrix();
}

void lighting() {
	if (isLightOn) {
		glEnable(GL_LIGHTING);          //Enable lighting for the scene
	}
	else {
		glDisable(GL_LIGHTING);         //Disable lighting for the scene
	}

	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_POSITION, difpos);
	glEnable(GL_LIGHT1);
}

void drawHead(float x, float y, float z)
{
	glPushMatrix();
	glScalef(x, y, z);
	//head
	//drawSphere(0.5);
	drawSphereS(0.5, color[2][0], color[2][1], color[2][2]);

	//helmet
	glPushMatrix();
	glTranslatef(-0.42, -0.08, -0.25);
	//glRotatef(-30, 0,0 , 1);
	drawCuboidS(0.1, 0.75, 0.1, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.42, -0.08, -0.25);
	//glRotatef(30, 0, 0, 1);
	drawCuboidS(-0.1, 0.75, 0.1, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.2, -0.2, -0.45);
	glScalef(1, 0.5, 1);
	glRotatef(18, 0, 0, 1);
	drawCuboidS(0.05, 0.75, 0.5, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, -0.2, -0.45);
	glScalef(1, 0.5, 1);
	glRotatef(-18, 0, 0, 1);
	drawCuboidS(-0.05, 0.75, 0.5, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.01, -0.1, -0.55);
	glScalef(1, 0.5, 1);
	glRotatef(30, 0, 0, 1);
	drawCuboidS(0.05, 0.75, 0.5, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.01, -0.1, -0.55);
	glScalef(1, 0.5, 1);
	glRotatef(-30, 0, 0, 1);
	drawCuboidS(-0.05, 0.75, 0.5, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.2, -0.4, -0.44);
	glScalef(1, 0.5, 1);
	drawCuboidS(0.05, 0.75, 0.5, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, -0.4, -0.44);
	glScalef(1, 0.5, 1);
	drawCuboidS(-0.05, 0.75, 0.5, color[2][0], color[2][1], color[2][2]);
	glPopMatrix();

	glPushMatrix();
	glScalef(1.7, 1, 1);
	glTranslatef(0, 0.27, -0.35);
	drawSphereS(0.12, color[0][0], color[0][1], color[0][2]);
	glPopMatrix();

	glPushMatrix();
	glScalef(1.3, 1.3, 1.3);
	glTranslatef(-0.15, 0.23, -0.2);
	drawSphereS(0.12, color[0][0], color[0][1], color[0][2]);
	glPopMatrix();

	glPushMatrix();
	glScalef(1.3, 1.3, 1.3);
	glTranslatef(0.15, 0.23, -0.2);
	drawSphereS(0.12, color[0][0], color[0][1], color[0][2]);
	glPopMatrix();

	//left ear
	glPushMatrix();
	glTranslatef(0.35, 0.05, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(10, -1, 0, 0);
	drawCylinderS(0.2, 0.17, 0.2, color[2][0], color[2][1], color[2][2]);
	glTranslatef(0, 0, 0.12);
	drawSphereS(0.18, color[0][0], color[0][1], color[0][2]);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(-0.35, 0.05, 0);
	glRotatef(90, 0, -1, 0);
	glRotatef(10, -1, 0, 0);
	drawCylinderS(0.2, 0.17, 0.2, color[2][0], color[2][1], color[2][2]);
	glTranslatef(0, 0, 0.12);
	drawSphereS(0.18, color[0][0], color[0][1], color[0][2]);
	glPopMatrix();
	//face
	glPushMatrix();
	glTranslatef(0, 0, -0.02);
	drawSphereS(0.49, color[0][0], color[0][1], color[0][2]);

	//forehead
	glTranslatef(0, 0.04, 0);
	drawSphereS(0.49, color[2][0], color[2][1], color[2][2]);

	// Draw a power diamond on the forehead
	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glColor3f(color[1][0], color[1][1], color[1][2]);  // Black color for the lines
	glLineWidth(2.0);    // Set line width (adjust as needed)
	glBegin(GL_QUADS);
	glVertex3f(0, 0.07, -0.47);   // Top point
	glVertex3f(0.05, 0, -0.48);    // Right point
	glVertex3f(0, -0.07, -0.495);  // Bottom point
	glVertex3f(-0.05, 0, -0.48);   // Left point
	glEnd();
	glPopMatrix();

	//froorehead Helmet
	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glTranslatef(0, 0, -0.3);  // Adjust the translation based on the forehead position
	drawCylinderS(0.4, 0.46, 0.1, color[0][0], color[0][1], color[0][2]);  // Adjust the inner and outer radius as needed
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}

void drawRobotLeg(float x, float y, float z)
{
	glPushMatrix();
	glScalef(x, y, z);

	//Thigh
	//top sphere
	glTranslatef(0, 0.9, 0);
	glRotatef(legAngle, 1, 0, 0);
	glTranslatef(0, -0.9, 0);
	glPushMatrix();
	glTranslatef(0, 0.9, 0);
	glScalef(1, 1.5, 1);
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 0.13, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	cylinder(GL_FILL, 0.08, 0.13, 0.6, -90, 1, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.27, 0);
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 0.08, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();

	//connect T&J1
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.2);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	cylinder(GL_FILL, 0.04, 0.04, 0.1, 0, 0, 0, 0);
	glPopMatrix();

	//Decoration
	glPushMatrix();   
	glTranslatef(0, 0.9, -0.06);
	glRotatef(45, 0, 0, 1);
	glRotatef(8, 1, 0, 0);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	drawCube(0.08, 0, 0, 0);   
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 0.79, -0.12);
	glRotatef(225, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	drawRightTriangle(0, 0, 0.08);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.115, 0.79, -0.12);
	glRotatef(-225, 0, 0, 1);
	glRotatef(15, 1, -1, 0);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	drawRightTriangle(0, 0, 0.08);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.035, 0.7, -0.03);
	glScalef(0.6, 1.5, 1.1);
	glRotatef(-7.5, 1, 0, 0);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.06, 0.7, -0.04);
	glScalef(0.6, 1.5, 1);
	glRotatef(-15, 1, -1, 0);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.06, 0.55, -0.02);
	glScalef(0.6, 0.8, 1);
	glRotatef(-15, 1, -1, 0);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.025, 0.59, -0.12);
	glRotatef(45, 0, 0, 1);
	glRotatef(-10, 1, 0, 0);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	drawRightTriangle(0, 0, 0.08);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.05, 0.62, -0.03);
	glRotatef(4, 0, 0, 1);
	glScalef(1, 1.7, 0.5);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.02, 0.4, -0.03);
	glRotatef(4, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.04, 0.4, 0.02);
	glRotatef(4, 0, 0, 1);
	glRotatef(20, 0, 1, 0);
	glScalef(1, 1, 0.5);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.06, 0.62, 0.02);
	glRotatef(4, 0, 0, 1);
	glRotatef(20, 0, 1, 0);
	glScalef(1, 1.7, 0.5);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	drawCube(0.08, 0, 0, 0);
	glPopMatrix();

	// Joint 1

	glTranslatef(0, 0.15, 0);
	glRotatef(legAngle2, 1, 0, 0);
	glTranslatef(0, -0.15, 0);
	glPushMatrix();
	glPushMatrix();
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 0.07, 1.0, 1.0, 1.0, 0, 0, 0, 0);

	glPopMatrix();

	//connect J1&C
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.1);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	cylinder(GL_FILL, 0.04, 0.04, 0.1, 0, 0, 0, 0);
	glPopMatrix();

	// Calf
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 0.07, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.52, 0);
	cylinder(GL_FILL, 0.04, 0.07, 0.44, -90, 1, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.53, 0);
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 0.04, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();

	//connect C&J2
	glPushMatrix();
	glTranslatef(0, -0.6, 0);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	cylinder(GL_FILL, 0.02, 0.02, 0.1, -90, 1, 0, 0);
	glPopMatrix();

	// Joint 2
	glTranslatef(0, -0.55, 0);
	glRotatef(legAngle3, 1, 0, 0);
	glTranslatef(0, 0.55, 0);
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, -0.6, 0);
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 0.04, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();

	//connect C&J2
	glPushMatrix();
	glTranslatef(0, -0.7, 0);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	cylinder(GL_FILL, 0.02, 0.02, 0.1, -90, 1, 0, 0);
	glPopMatrix();

	// Foot
	glPushMatrix();
	glScalef(0.6, 1, 1);
	glPushMatrix();
	glTranslatef(0, -0.55, -0.1);
	glScalef(1, 0.3, 1.42);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	drawCube(0.1, 0, -0.7, 0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	glVertex3f(0.035, -0.73, -0.01);
	glVertex3f(0.035, -0.73, -0.14);
	glVertex3f(0.035, -0.67, -0.01);

	glVertex3f(-0.035, -0.73, -0.01);
	glVertex3f(-0.035, -0.73, -0.14);
	glVertex3f(-0.035, -0.64, -0.01);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	glVertex3f(0.035, -0.64, -0.01);
	glVertex3f(-0.035, -0.64, -0.01);
	glVertex3f(-0.035, -0.73, -0.14);
	glVertex3f(0.035, -0.73, -0.14);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.6, 1, 1);
	glPushMatrix();
	glColor3f(color[2][0], color[2][1], color[2][2]);
	glScalef(1.2, 0.5, 0.5);
	drawCube(0.05, 0, -0.76, 0);
	glPopMatrix();
	glPopMatrix();

	//Connect leg with body

	glPushMatrix();
	glColor3f(color[0][0], color[0][1], color[0][2]);
	glScalef(1, 1.4, 1);
	glTranslatef(-0.03, 0.4, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.07, 0.07, 0.05);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1.4, 1);
	glTranslatef(-0.03, 0.4, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDiskS(0, 0.07);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1.4, 1);
	glTranslatef(0.02, 0.4, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDiskS(0, 0.07);
	glPopMatrix();

	//Decoration
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glScalef(1.1, 1.1, 1);

	glPushMatrix();
	glScalef(1, 1, 0.8);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	glTranslatef(-0.07, 0.51, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(103, 0.0, 1.0, 0.0);
	drawCylinder(0.07, 0.10, 0.04);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.07, 0.51, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(103, 0.0, 1.0, 0.0);
	drawDiskS(0, 0.07);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0.7);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	glTranslatef(-0.075, 0.51, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(103, 0.0, 1.0, 0.0);
	drawCylinder(0.05, 0.05, 0.04);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.075, 0.51, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(103, 0.0, 1.0, 0.0);
	drawDiskS(0, 0.05);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void body() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	//Draft
	glPushMatrix();
	glScalef(2.0, 2.0, 2.0);

	//Xiongbu
	glPushMatrix();
	glTranslatef(3.56, 0.35, 13.06);
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 3.0, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.56, 0.35, 13.06); 
	sphere(GL_FILL, color[2][0], color[2][1], color[2][2], 3.0, 1.0, 1.0, 1.0, 0, 0, 0, 0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.6, 0.35, 13.06);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(10.383, 0.0, 0.0, 1.0);
	cylinder(GLU_FILL, 2, 3, 5.46, 90, 0, 1, 0);
	drawCylinder(2, 3, 5.46);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.6, 0.35, 13.06);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(10.383, 0.0, 0.0, 1.0);
	drawCylinder(2, 3, 5.46);
	glPopMatrix();

	//Inside
	glColor3f(color[0][0], color[0][1], color[0][2]);
	glPushMatrix();
	glScalef(1, 1, 0.7);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0 , -15 , -9);
	drawCylinder(2.7, 5.0, 10);
	glPopMatrix();

	//Outside
	glColor3f(color[1][0], color[1][1], color[1][2]);
	glPushMatrix();
	glTranslatef(-2.8, -2, 12);
	glRotatef(20, 0.0, 1.0, 0.0);
	glRotatef(20, 0.0, 0.0, 1.0);
	glScalef(0.8, 1.0, 1.0);
	drawCube(2, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.8, -2, 12);
	glRotatef(-20, 0.0, 1.0, 0.0);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glScalef(0.8, 1.0, 1.0);
	drawCube(2, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(10, 1.0, 0.0, 0.0);
	drawRectangle(0.25, -4.2, 13.8, 3.0, 3.5, 0);
	drawRectangle(-3.25, -4.2, 13.8, 3.0, 3.5, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.25, -4, 13.8);
	glVertex3f(1.0, -4, 13.8);
	glVertex3f(1.0, -5, 13.8);

	glVertex3f(-0.25, -4, 13.8);
	glVertex3f(-1.0, -4, 13.8);
	glVertex3f(-1.0, -5, 13.8);

	glVertex3f(0, -7, 13.8);
	glVertex3f(1.0, -6, 13.8);
	glVertex3f(1.0, -7, 13.8);

	glVertex3f(0, -7, 13.8);
	glVertex3f(-1.0, -6, 13.8);
	glVertex3f(-1.0, -7, 13.8);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.7, -4, 13.8);
	glVertex3f(2, -4, 13.8);
	glVertex3f(2, -7, 13.8); 
	glVertex3f(0.7, -7, 13.8);

	glVertex3f(-0.7, -4, 13.8);
	glVertex3f(-2, -4, 13.8);
	glVertex3f(-2, -7, 13.8);
	glVertex3f(-0.7, -7, 13.8);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(10, 0, 0);
	glPushMatrix();
	glTranslatef(1.1, -6, 0.45);
	glScalef(1, 0.4, 0.7);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -15, -9);
	drawCylinder(1.9, 2.5, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.1, -6, 0.45);
	glScalef(1, 0.4, 0.7);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -15, -9);
	drawCylinder(1.9, 2.5, 10);
	glPopMatrix();
	glPopMatrix();

	//Back
	glPushMatrix();
	glScalef(1, 1, 0.6);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -17, -4.4);
	drawCylinder(4.5, 6.0, 7);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.7, 0.7, 0.3);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -32, -14);
	drawCylinder(4.5, 6.0, 6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -3, 9.5);
	glRotatef(-7.6, 1.0, 0.0, 0.0);
	glScalef(0.8, 1.8, 0.8);
	drawCube(2.5, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0.7);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -15, 2.6);
	drawCylinder(5.6, 4.5, 2.5);
	glPopMatrix();

	//Middle body
	glPushMatrix();
	glScalef(1, 1, 0.7);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -15, -14.5);
	drawCylinder(5, 3.2, 5.2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0.6);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -17.5, 5.1);
	drawCylinder(5.2, 0.6, 1.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(color[1][0], color[1][1], color[1][2]);
	glScalef(1, 2, 0.7);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, -15, -11.1);
	drawCylinder(1.5, 5, 4);
	glPopMatrix();

	//Hand connect area - Right
	glPushMatrix();
	glTranslatef(4, 3.3, 10);

	glColor3f(color[0][0], color[0][1], color[0][2]);
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(2.5, 2.5, 2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDiskS(0, 2.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDiskS(0, 2.5);
	glPopMatrix();

	glPopMatrix();

	//Hand connect area - Left
	glPushMatrix();
	glTranslatef(-5.9, 3.3, 10);

	glColor3f(color[0][0], color[0][1], color[0][2]);
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(2.5, 2.5, 2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDiskS(0, 2.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawDiskS(0, 2.5);
	glPopMatrix();

	glPopMatrix();

	glColor3f(color[0][0], color[0][1], color[0][2]);
	glPushMatrix();
	glTranslatef(0, 6, 10.5);
	glRotatef(-90, 1.0, 0.0, 0.0);
	drawCylinder(1, 1, 4);
	glPopMatrix();
	
	glPopMatrix();
}

void handle() {
	// main
	cylinderJ(GL_FILL, 0.6, 0.6, 0.6, 0.025, 0.035, 0.25, 90, 1.0, 0, 0.0);
	// deco
	// right
	glPushMatrix();
	glTranslatef(-0.005, -0.25, -0.03);
	glScalef(1, 1.5, 1);
	drawPyramidS(0.05);
	glPopMatrix();
	// left
	glPushMatrix();
	glTranslatef(-0.048, -0.25, -0.03);
	glScalef(1, 1.5, 1);
	drawPyramidS(0.05);
	glPopMatrix();
	// front
	glPushMatrix();
	glTranslatef(-0.0215, -0.25, 0);
	glScalef(1, 1.5, 1);
	drawPyramidS(0.05);
	glPopMatrix();
	// back
	glPushMatrix();
	glTranslatef(-0.0215, -0.25, -0.05);
	glScalef(1, 1.5, 1);
	drawPyramidS(0.05);
	glPopMatrix();
}

void weapon() {
	// handle
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	handle();
	glPopMatrix();

	// base
	glPushMatrix();
	if (isWeaponOut) {
		glTranslatef(0, 0, 0);
		cylinderJ(GL_FILL, 0.5, 0.5, 0.5, 0.06, 0.06, 0.01, 90, 1.0, 0, 0.0);
	}
	else {
		glTranslatef(0, 0, 0);
		cylinderJ(GL_FILL, 0.5, 0.5, 0.5, 0, 0, 0.01, 90, 1.0, 0, 0.0);
	}
	glPopMatrix();

	// sword
	glPushMatrix();
	if (isSwordOut) {
		glTranslatef(0, -0.012, 0);
		cylinderJ(GL_FILL, 0.2, 0.2, 0.8, 0.025, 0.025, 0.95, 90, 1.0, 0, 0.0);
	}
	else {
	}
	glPopMatrix();

	// attachment
	glPushMatrix();
	if (isWeaponOut) {
		glTranslatef(0.08, 0.125, 0);
		drawCubeJ(0.9, 0.9, 0.9, 1, 0.05, 0.01, 0.01);
	}
	else {
	}
	glPopMatrix();
}

void arm() {
	// arm top skeleton
	glPushMatrix();
	glRotatef(90, 1.0, -0.2, 0.0);
	glTranslatef(0, 0, 0.22);
	drawCubeJ(color[2][0], color[2][1], color[2][2], 0.05, 0.5, 0.5, 4);
	glPopMatrix();
	// arm top
	glPushMatrix();
	glRotatef(90, 1.0, -0.2, 0.0);
	cylinderJ(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.055, 0.045, 0.35, 90, 1.0, -0.2, 0.0);
	glPopMatrix();	
	// joints
	glPushMatrix();
	glTranslatef(-0.075, -0.385, 0.0);
	sphere(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.045, 1.0, 1.0, 1.0, 0, 1.0, 1.0, 1.0);
	glPopMatrix();
	// deltoids

	//GLuint texture;
	//glEnable(GL_TEXTURE_2D);
	//texture = loadTexture("metal.bmp");
	glPushMatrix();
	glTranslatef(-0.01, -0.06, 0.0);
	sphere(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.094, 0.7, 1.3, 0.7, 347, 0, 0, 1);
	glPopMatrix();

	//glDeleteTextures(1, &texture);
	//glDisable(GL_TEXTURE_2D);
	// biceps
	glPushMatrix();
	glTranslatef(-0.033, -0.185, 0.0);
	sphere(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.089, 0.65, 2.1, 0.65, 349.5, 0, 0, 1);
	glPopMatrix();
}

void forearm() {
	// forearm skeleton
	glPushMatrix();
	glRotatef(90, 1.0, -0.22, 0.0);
	glTranslatef(0.01, 0.0, 0.2);
	drawCubeJ(color[1][0], color[1][1], color[1][2], 0.05, 0.3, 0.3, 4);
	glPopMatrix();
	// forearm
	glPushMatrix();
	glTranslatef(-0.009, -0.065, 0.0);
	cylinderJ(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.0425, 0.024, 0.25, 90, 1.0, -0.23, 0.0);
	glPopMatrix();
	// muscle
	glPushMatrix();
	glTranslatef(-0.015, -0.08, 0.0);
	sphere(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.105, 0.42, 1, 0.42, 346, 0, 0, 1);
	glPopMatrix();
	// joints
	glPushMatrix();
	glTranslatef(-0.073, -0.352, 0.0);
	sphere(GLU_FILL, color[2][0], color[2][1], color[2][2], 0.025, 1.0, 1.0, 1.0, 0, 1.0, 1.0, 1.0);
	glPopMatrix();
}

void palm() {
	glPushMatrix();
	drawCubeJ(color[2][0], color[2][1], color[2][2], 1, 0.07, 0.08, 0.02);
	glPopMatrix();
}

void finger(int rotate, float thicc, float length) {
	glPushMatrix();
	glRotatef(rotate, 1, -1, 0);
	drawCubeJ(color[1][0], color[1][1], color[1][2], 1, thicc, length, 0.01);
	glPopMatrix();
}

void fingers() {
	glPushMatrix();
	glTranslatef(-0.05, -0.11, 0);
	finger(0, 0.016, 0.03);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.05, -0.165, 0);
	finger(0, 0.014, 0.025);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.11, 0);
	finger(0, 0.016, 0.03);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -0.165, 0);
	finger(0, 0.014, 0.025);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.05, -0.11, 0);
	finger(0, 0.016, 0.03);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.05, -0.165, 0);
	finger(0, 0.014, 0.025);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glTranslatef(0.03, -0.11, 0);
	finger(0, 0.016, 0.04);
	glPopMatrix();
}

void hand() {
	glPushMatrix();
	// palm
	glScalef(0.6, 0.6, 0.6);
	glPushMatrix();
	glRotatef(75, 0, -1, 0);
	palm();
	glPopMatrix();
	// fingers
	glPushMatrix();
	glRotatef(75, 0, -1, 0);
	fingers();
	glPopMatrix();
	glPopMatrix();
}

void leftforearm() {
	glPushMatrix();
	glTranslatef(-0.175, -0.67, 0.0025);
	hand();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1, -0.25, 0.0);
	forearm();
	glPopMatrix();
	// weapon
	if (isWeaponOut) {
		glPushMatrix();
		glTranslatef(-0.24, -0.5, 0);
		glScalef(0.7, 0.7, 0.7);
		glRotatef(15, 0, 0, -1);
		weapon();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(-0.15, -0.5, 0);
		glScalef(0.7, 0.7, 0.7);
		glRotatef(15, 0, 0, -1);
		weapon();
		glPopMatrix();
	}
}

void rightforearm() {
	glPushMatrix();
	glTranslatef(-0.1, -0.25, 0.0);
	forearm();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.175, -0.67, 0.0025);
	hand();
	glPopMatrix();
	glPopMatrix();
}

void leftarm() {
	glPushMatrix();
	glTranslatef(0.0, 0.2, 0.0);
	arm();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.18, 0);
	glRotatef(forearmAngleL, 1, 0, 0);
	glTranslatef(0, 0.18, 0);
	leftforearm();
	glPopMatrix();

}

void rightarm() {
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0.0, 0.2, 0.0);
	arm();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.18, 0);
	glRotatef(-forearmAngleR, 1, 0, 0);
	glTranslatef(0, 0.18, 0);
	rightforearm();
	glPopMatrix();
}

void botharms() {
	glPushMatrix();
	glTranslatef(0, 0.3, 0);

	glPushMatrix();
	glTranslatef(-0.2, 0.2, 0);
	glRotatef(wholeArmAngleL, 0, 0, -1);
	glTranslatef(0.2, -0.2, 0);
	glPushMatrix();
	glTranslatef(-0.2, 0.0, 0.0);
	leftarm();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.18, 0.2, 0);
	glRotatef(-wholeArmAngleR, 0, 0, -1);
	glTranslatef(-0.18, -0.2, 0);
	glPushMatrix();
	glTranslatef(0.2, 0.0, 0.0);
	rightarm();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void robot() {
	//Robot
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);

	//Robot Body
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glRotatef(bodyR, 1.0, 1.0, 0.0);
	glTranslatef(0, 29, -21);
	glColor3f(1.0, 0.0, 0.0);
	body();
	glPopMatrix();

	//Robot Head   
	glPushMatrix();
	glRotatef(bodyR, 1.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(180 + headAngle, 0, 1, 0);
	glScalef(70, 95, 80);
	glTranslatef(0, 0.6, 0);
	drawHead(0.25, 0.25, 0.25);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	//Robot Left Leg
	glPushMatrix();
	glRotatef(-r, 1, 0, 0);   //Move
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glScalef(100, 100, 100);
	glTranslatef(-0.1, -0.6, 0);
	drawRobotLeg(0.6, 0.55, 0.6);
	glPopMatrix();

	glPopMatrix();

	
	//Robot Left Leg
	glPushMatrix();
	glRotatef(r, 1, 0, 0);   //Move
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glScalef(-100, 100, 100);
	glTranslatef(-0.1, -0.6, 0);
	drawRobotLeg(0.6, 0.55, 0.6);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glScalef(70, 70, 70);
	glTranslatef(0, 0.03, 0);
	glRotatef(180, 0, 1, 0);
	angle += 0.05;
	botharms();
	//weapon();
	glPopMatrix();

	glPopMatrix();  //---------Robot
}

void background() {
	glPushMatrix();
	drawSphere(150);
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//Projection
	projection();                    

	glMatrixMode(GL_MODELVIEW);       //refer to the model view matrix
	glLoadIdentity();
	//glTranslatef(Tx, 0.0, Tz);       //translate model along the z-axis

	lighting();

	//glRotatef(0.1, 1.0, 1.0, 1.0);
	//glColor3f(0.0, 0.0, 1.0);       //Blue sphere
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambM);     //red color ambient material
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);     //red color ambient material
	
	GLuint robottexture;
	glEnable(GL_TEXTURE_2D);
	if (textureCount == 1) {
		robottexture = loadTexture("");
	}
	else if (textureCount == 2) {
		robottexture = loadTexture("metal.bmp");
	}
	else if (textureCount == 3) {
		robottexture = loadTexture("sunset.bmp");
	}
	else if (textureCount == 4) {
		robottexture = loadTexture("icecream.bmp");
	}
	glPushMatrix();
	glTranslatef(0, 0, 20);
	robot();
	glPopMatrix();
	glDeleteTextures(1, &robottexture);
	glDisable(GL_TEXTURE_2D);

	GLuint bgtexture;
	glEnable(GL_TEXTURE_2D);
	robottexture = loadTexture("sky.bmp");
	background();
	glDeleteTextures(1, &bgtexture);
	glDisable(GL_TEXTURE_2D);

}


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
//--------------------------------------------------------------------

//--------------------------------------------------------------------

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
		CW_USEDEFAULT, CW_USEDEFAULT, 1080, 1080,
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

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
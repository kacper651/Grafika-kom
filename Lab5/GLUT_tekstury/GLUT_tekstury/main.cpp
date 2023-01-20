#include <windows.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>
using namespace std;

struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat xu;
	GLfloat xv;
	GLfloat yu;
	GLfloat yv;
	GLfloat zu;
	GLfloat zv;
};

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

int model = 1;
int draw = 1;

static GLfloat theta_x = 0.0;   // k�t obrotu obiektu
static GLfloat theta_y = 0.0;
static GLfloat theta_z = 0.0;

static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
							   // 0 - nie naci�ni�to �adnego klawisza
							   // 1 - naci�ni�ty zosta� lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
									  // i poprzedni� kursora myszy
static int delta_y = 0;
static int delta_z = 0;

int N = 25;

Point** createSquareMatrix()
{
	Point** matrix = new Point * [N];
	for (int i = 0; i < N; i++)
		matrix[i] = new Point[N];

	return matrix;
}

GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{
	// Struktura dla nag��wka pliku  TGA

#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE* pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte* pbitsperpixel = NULL;

	// Warto�ci domy�lne zwracane w przypadku b��du

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	fopen_s(&pFile, FileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Przeczytanie nag��wka pliku 

	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	// Odczytanie szeroko�ci, wysoko�ci i g��bi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	// Sprawdzenie, czy g��bia spe�nia za�o�one warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	// Obliczenie rozmiaru bufora w pami�ci

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Alokacja pami�ci dla danych obrazu

	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}

	// Ustawienie formatu OpenGL

	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};

	fclose(pFile);

	return pbitsperpixel;
}

void Egg() 
{
	Point** matrix = createSquareMatrix();

	for (int i = 0; i < N; i++) {

		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {

			float v = ((float)j) / (N - 1);

			matrix[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
			matrix[i][j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			matrix[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);

			matrix[i][j].xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * cos(M_PI * v);
			matrix[i][j].xv = M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(M_PI * v);
			matrix[i][j].yu = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
			matrix[i][j].yv = 0;
			matrix[i][j].zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * sin(M_PI * v);
			matrix[i][j].zv = -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);

		}
	}

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N - 1; j++) {

			GLfloat lengthV1 = sqrt(pow(matrix[i][j].yu * matrix[i][j].zv - matrix[i][j].zu * matrix[i][j].yv, 2) +
				pow(matrix[i][j].zu * matrix[i][j].xv - matrix[i][j].xu * matrix[i][j].zv, 2) +
				pow(matrix[i][j].xu * matrix[i][j].yv - matrix[i][j].yu * matrix[i][j].xv, 2));

			GLfloat lengthV2 = sqrt(pow(matrix[i + 1][j].yu * matrix[i + 1][j].zv - matrix[i + 1][j].zu * matrix[i + 1][j].yv, 2) +
				pow(matrix[i + 1][j].zu * matrix[i + 1][j].xv - matrix[i + 1][j].xu * matrix[i + 1][j].zv, 2) +
				pow(matrix[i + 1][j].xu * matrix[i + 1][j].yv - matrix[i + 1][j].yu * matrix[i + 1][j].xv, 2));

			GLfloat lengthV3 = sqrt(pow(matrix[i][j + 1].yu * matrix[i][j + 1].zv - matrix[i][j + 1].zu * matrix[i][j + 1].yv, 2) +
				pow(matrix[i][j + 1].zu * matrix[i][j + 1].xv - matrix[i][j + 1].xu * matrix[i][j + 1].zv, 2) +
				pow(matrix[i][j + 1].xu * matrix[i][j + 1].yv - matrix[i][j + 1].yu * matrix[i][j + 1].xv, 2));


			GLfloat lengthV4 = sqrt(pow(matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].zv - matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].yv, 2) +
				pow(matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].xv - matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].zv, 2) +
				pow(matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].yv - matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].xv, 2));

			GLfloat v1[3] = { (matrix[i][j].yu * matrix[i][j].zv - matrix[i][j].zu * matrix[i][j].yv) / lengthV1,
							(matrix[i][j].zu * matrix[i][j].xv - matrix[i][j].xu * matrix[i][j].zv) / lengthV1,
							(matrix[i][j].xu * matrix[i][j].yv - matrix[i][j].yu * matrix[i][j].xv) / lengthV1 };

			GLfloat v2[3] = { (matrix[i + 1][j].yu * matrix[i + 1][j].zv - matrix[i + 1][j].zu * matrix[i + 1][j].yv) / lengthV2,
								(matrix[i + 1][j].zu * matrix[i + 1][j].xv - matrix[i + 1][j].xu * matrix[i + 1][j].zv) / lengthV2,
								(matrix[i + 1][j].xu * matrix[i + 1][j].yv - matrix[i + 1][j].yu * matrix[i + 1][j].xv) / lengthV2 };

			GLfloat v3[3] = { (matrix[i][j + 1].yu * matrix[i][j + 1].zv - matrix[i][j + 1].zu * matrix[i][j + 1].yv) / lengthV3,
								(matrix[i][j + 1].zu * matrix[i][j + 1].xv - matrix[i][j + 1].xu * matrix[i][j + 1].zv) / lengthV3,
								(matrix[i][j + 1].xu * matrix[i][j + 1].yv - matrix[i][j + 1].yu * matrix[i][j + 1].xv) / lengthV3 };

			GLfloat v4[3] = { (matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].zv - matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].yv) / lengthV4,
								(matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].xv - matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].zv) / lengthV4,
								(matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].yv - matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].xv) / lengthV4 };

			if (i < N / 2) {
				v1[0] *= -1; v1[1] *= -1; v1[2] *= -1;
				v2[0] *= -1; v2[1] *= -1; v2[2] *= -1;
				v3[0] *= -1; v3[1] *= -1; v3[2] *= -1;
				v4[0] *= -1; v4[1] *= -1; v4[2] *= -1;
			}

			if (i < N / 2) {
				glBegin(GL_TRIANGLES);

				glNormal3fv(v4);
				glTexCoord2f(matrix[i + 1][j + 1].x / 2.5, matrix[i + 1][j + 1].y / 2.5);
				glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

				glNormal3fv(v2);
				glTexCoord2f(matrix[i + 1][j].x / 2.5, matrix[i + 1][j].y / 2.5);
				glVertex3f(matrix[i + 1][j].x, 5 - matrix[i + 1][j].y, matrix[i + 1][j].z);

				glNormal3fv(v1);
				glTexCoord2f(matrix[i][j].x / 2.5, matrix[i][j].y / 2.5);
				glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);

				glEnd();

				glBegin(GL_TRIANGLES);

				glNormal3fv(v3);
				glTexCoord2f(matrix[i][j + 1].x / 2.5, matrix[i][j + 1].y / 2.5);
				glVertex3f(matrix[i][j + 1].x, 5 - matrix[i][j + 1].y, matrix[i][j + 1].z);


				glNormal3fv(v4);
				glTexCoord2f(matrix[i + 1][j + 1].x / 2.5, matrix[i + 1][j + 1].y / 2.5);
				glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

				glNormal3fv(v1);
				glTexCoord2f(matrix[i][j].x / 2.5, matrix[i][j].y / 2.5);
				glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);

				glEnd();
			}
			else {
				glBegin(GL_TRIANGLES);

				glNormal3fv(v1);
				glTexCoord2f(matrix[i][j].x / 2.5, matrix[i][j].y / 2.5);
				glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);

				glNormal3fv(v2);
				glTexCoord2f(matrix[i + 1][j].x / 2.5, matrix[i + 1][j].y / 2.5);
				glVertex3f(matrix[i + 1][j].x, 5 - matrix[i + 1][j].y, matrix[i + 1][j].z);

				glNormal3fv(v4);
				glTexCoord2f(matrix[i + 1][j + 1].x / 2.5, matrix[i + 1][j + 1].y / 2.5);
				glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

				glEnd();

				glBegin(GL_TRIANGLES);

				glNormal3fv(v4);
				glTexCoord2f(matrix[i + 1][j + 1].x / 2.5, matrix[i + 1][j + 1].y / 2.5);
				glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

				glNormal3fv(v3);
				glTexCoord2f(matrix[i][j + 1].x / 2.5, matrix[i][j + 1].y / 2.5);
				glVertex3f(matrix[i][j + 1].x, 5 - matrix[i][j + 1].y, matrix[i][j + 1].z);

				glNormal3fv(v1);
				glTexCoord2f(matrix[i][j].x / 2.5, matrix[i][j].y / 2.5);
				glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);

				glEnd();
			}
		}
	}
}

void Triangle() {

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-5.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glTexCoord2f(0.75f, 1.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();

}

void Pyramid() {

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, -5.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-5.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, -5.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-5.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, -5.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-5.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, -5.0f, 0.0f);
	glEnd();
}

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora // jako pozycji poprzedniej
		y_pos_old = y;

		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora // jako pozycji poprzedniej
		y_pos_old = y;
		z_pos_old = x;
		status = 2;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else

		status = 0;          // nie zosta� wci�ni�ty �aden klawisz
}

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;
	delta_z = x - x_pos_old;

	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	z_pos_old = x;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Axes();
	glColor3f(1.0f, 1.0f, 1.0f);

	if (status == 1)
	{
		theta_x += delta_x * pix2angle;
		theta_y += delta_y * pix2angle;
	}

	if (status == 2 && model == 1) {

		theta_z += delta_z * pix2angle;

		if (theta_z > 5.0) {
			theta_z = 5.0;
		}
		else if (theta_z < -20.0) {
			theta_z = -20.0;
		}

		glTranslatef(0.0, 0.0, theta_z);

	}

	glRotatef(theta_x, 0.0, 1.0, 0.0);
	glRotatef(theta_y, 1.0, 0.0, 0.0);

	if (draw == 1) {
		Egg();
	}
	else if (draw == 2) {
		Pyramid();
	}
	else if (draw == 3) {
		Triangle();
	}
	else if (draw == 4) {
		glutSolidTeapot(3.0);
	}

	glFlush();
	glutSwapBuffers();

}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') draw = 1;
	if (key == '2') draw = 2;
	if (key == '3') draw = 3;
	if (key == '4') draw = 4;
	if (N > 1 && key == '-') N -= 1;
	if (key == '=') N += 1;
	if (key == 'z')
		exit(0);

	RenderScene();
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLbyte* pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	//  Definicja materia�u z jakiego zrobiony jest czajnik
	//  i definicja �r�d�a �wiat�a

	// Definicja materia�u z jakiego zrobiony jest czajnik

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 0.25 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               

	GLfloat mat_shininess = { 100.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni

	// Definicja �r�d�a �wiat�a

	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	// po�o�enie �r�d�a

	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.05 };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.001 };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a
	// Ustawienie patrametr�w materia�u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	// Ustawienie parametr�w �r�d�a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie opcji systemu o�wietlania sceny

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora

	// Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany

	glEnable(GL_CULL_FACE);

	//  Przeczytanie obrazu tekstury z pliku

	pBytes = LoadTGAImage("texture.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	// Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	// Zwolnienie pami�ci

	free(pBytes);

	// W��czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Okre�lenie sposobu nak�adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 100.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mainMessage()
{
	cout << "Program opisujacy teksturowanie w OpenGL\n\n";

	cout << "Sterowanie:\n"
		<< "'3' -> przelaczenie na widok trojkata\n"
		<< "'2' -> przelaczenie na widok piramidy\n"
		<< "'1' -> przelaczenie na widok jajka\n"
		<< "Lewy przycisk myszy & przeciagniecie -> obrot narysowanego obiektu\n"
		<< "Prawy przycisk myszy & przeciagniecie -> przyblizenie/oddalenie narysowanego obiektu\n"
		<< "'-'/'=' -> zmniejszenie/zwiekszenie zlozonosci jajka\n\n";

	cout << "Aby zamknac program nacisnij 'z' na oknie aplikacji.\n";
}

void main(void)
{
	mainMessage();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Tekstury");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keys);
	glutMainLoop();
}
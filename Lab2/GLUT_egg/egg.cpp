#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

typedef float point3[3];

struct Point3 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

const int N = 20;
int model = 1;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
Point3 colors[N][N];

float RandColor()
{
    return rand() % 101 * 0.01;
}

void fillColors()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            colors[i][j].x = RandColor();
            colors[i][j].y = RandColor();
            colors[i][j].z = RandColor();
        }
    }
}


void Egg()
{
	Point3 square_matrix[N][N];

	for (int i = 0; i < N; i++) {

		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {

			float v = ((float)j) / (N - 1);
            square_matrix[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
            square_matrix[i][j].y = 5 - (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
            square_matrix[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);            
		}
	}

	switch (model) {
	case 1:
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {

				glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);

				glBegin(GL_POINTS);
				glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
				glEnd();

			}
		}
		break;
	case 2:
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++) {

				glBegin(GL_LINES);
                glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
				glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
				glVertex3f(square_matrix[i + 1][j].x, square_matrix[i + 1][j].y, square_matrix[i + 1][j].z);
				glEnd();

				glBegin(GL_LINES);
                glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
				glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
				glVertex3f(square_matrix[i][j + 1].x, square_matrix[i][j + 1].y, square_matrix[i][j + 1].z);
				glEnd();
			}
		}
		break;
	case 3:
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++) {

				glBegin(GL_TRIANGLE_STRIP);
                glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
				glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
				glVertex3f(square_matrix[i + 1][j].x, square_matrix[i + 1][j].y, square_matrix[i + 1][j].z);
                glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
				glVertex3f(square_matrix[i + 1][j + 1].x, square_matrix[i + 1][j + 1].y, square_matrix[i + 1][j + 1].z);
				glEnd();

                glBegin(GL_TRIANGLE_STRIP);
                glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                glVertex3f(square_matrix[i][j + 1].x, square_matrix[i][j + 1].y, square_matrix[i][j + 1].z);
                glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
                glVertex3f(square_matrix[i + 1][j + 1].x, square_matrix[i + 1][j + 1].y, square_matrix[i + 1][j + 1].z);
                glEnd();
			}
		}
		break;
    case 4:
        glColor3f(0.5, 1, 0.5);
        glutWireTeapot(3.0);
        break;
	}
}


void Axes(void)
{
    point3  x_min = { -5.0, 0.0, 0.0 };
    point3  x_max = { 5.0, 0.0, 0.0 }; 

    point3  y_min = { 0.0, -5.0, 0.0 };
    point3  y_max = { 0.0,  5.0, 0.0 };   

    point3  z_min = { 0.0, 0.0, -5.0 };
    point3  z_max = { 0.0, 0.0,  5.0 };
    
    glColor3f(1.0f, 0.0f, 0.0f);  
    glBegin(GL_LINES); 
    glVertex3fv(x_min);
    glVertex3fv(x_max);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  
    glBegin(GL_LINES);  

    glVertex3fv(y_min);
    glVertex3fv(y_max);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);  
    glBegin(GL_LINES); 

    glVertex3fv(z_min);
    glVertex3fv(z_max);
    glEnd();
}


void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

    glLoadIdentity();

    Axes();

    glRotatef(theta[0], 1.0, 0.0, 0.0);

    glRotatef(theta[1], 0.0, 1.0, 0.0);

    glRotatef(theta[2], 0.0, 0.0, 1.0);

    Egg();
    
    glColor3f(1.0f, 1.0f, 1.0f);
       
    glFlush();    

    glutSwapBuffers();
}


void keys(unsigned char key, int x, int y)
{
    if (key == 'p') model = 1;
    if (key == 'w') model = 2;
    if (key == 's') model = 3;
    if (key == '1') model = 4;

    RenderScene();
}


void spinEgg()
{
    theta[0] -= 0.01;
    if (theta[0] > 360.0) theta[0] -= 360.0;

    theta[1] -= 0.01;
    if (theta[1] > 360.0) theta[1] -= 360.0;

    theta[2] -= 0.01;
    if (theta[2] > 360.0) theta[2] -= 360.0;

    glutPostRedisplay();
}


void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    GLfloat AspectRatio;
    
    if (vertical == 0)  
        vertical = 1;
    glViewport(0, 0, horizontal, vertical);
     
    glMatrixMode(GL_PROJECTION);
     
    glLoadIdentity();
              
    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
           
    if (horizontal <= vertical)
        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else
        glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);

    glMatrixMode(GL_MODELVIEW);                                   

    glLoadIdentity();    
}


void main(void)
{
    srand(time(NULL));

    fillColors();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 600);

    glutCreateWindow("GLUT_Lab2");

    glutKeyboardFunc(keys);

    glutIdleFunc(spinEgg);

    glutDisplayFunc(RenderScene);
    
    glutReshapeFunc(ChangeSize);
    
    MyInit();
    
    glEnable(GL_DEPTH_TEST);
  
    glutMainLoop();
}

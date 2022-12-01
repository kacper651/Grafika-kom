#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;


typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };   // inicjalizacja po³o¿enia obserwatora

static GLfloat thetaX = 0.0;   // k¹t obrotu obiektu
static GLfloat thetaY = 0.0;   // k¹t obrotu obiektu
static GLfloat thetaZ = 0.0;   // k¹t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
                               // 0 - nie naciœniêto ¿adnego klawisza
                               // 1 - naciœniêty zostaæ lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;       // poprzednia pozycja kursora myszy
static int z_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_y = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_z = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
                                      // i poprzedni¹ kursora myszy

typedef float point3[3];

struct Point3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct RGBPoint
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

int N = 20;
int model = 1;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };


float RandColor()
{
    return rand() % 101 * 0.01;
}


void Egg()
{
    Point3** square_matrix = new Point3*[N];
    for (int i = 0; i < N; i++)
        square_matrix[i] = new Point3[N];

    RGBPoint** colors = new RGBPoint* [N];
    for (int i = 0; i < N; i++)
        colors[i] = new RGBPoint[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            colors[i][j].r = RandColor();
            colors[i][j].g = RandColor();
            colors[i][j].b = RandColor();
        }
    }

    for (int i = 0; i < N; i++) 
    {

        float u = ((float)i) / (N - 1);

        for (int j = 0; j < N; j++) 
        {

            float v = ((float)j) / (N - 1);
            square_matrix[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
            square_matrix[i][j].y = 5 - (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
            square_matrix[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);
        }
    }

    switch (model) 
    {
    case 1:
        for (int i = 0; i < N; i++) 
        {
            for (int j = 0; j < N; j++) 
            {

                glColor3f(colors[i][j].r, colors[i][j].g, colors[i][j].b);

                glBegin(GL_POINTS);
                glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glEnd();

            }
        }
        break;
    case 2:
        for (int i = 0; i < N - 1; i++)
        {
            for (int j = 0; j < N - 1; j++) 
            {

                glBegin(GL_LINES);
                glColor3f(colors[i][j].r, colors[i][j].g, colors[i][j].b);
                glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i + 1][j].r, colors[i + 1][j].g, colors[i + 1][j].b);
                glVertex3f(square_matrix[i + 1][j].x, square_matrix[i + 1][j].y, square_matrix[i + 1][j].z);
                glEnd();

                glBegin(GL_LINES);
                glColor3f(colors[i][j].r, colors[i][j].g, colors[i][j].b);
                glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i][j + 1].r, colors[i][j + 1].g, colors[i][j + 1].b);
                glVertex3f(square_matrix[i][j + 1].x, square_matrix[i][j + 1].y, square_matrix[i][j + 1].z);
                glEnd();
            }
        }
        break;
    case 3:
        for (int i = 0; i < N - 1; i++) 
        {
            for (int j = 0; j < N - 1; j++) 
            {

                glBegin(GL_TRIANGLE_STRIP);
                glColor3f(colors[i][j].r, colors[i][j].g, colors[i][j].b);
                glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i + 1][j].r, colors[i + 1][j].g, colors[i + 1][j].b);
                glVertex3f(square_matrix[i + 1][j].x, square_matrix[i + 1][j].y, square_matrix[i + 1][j].z);
                glColor3f(colors[i + 1][j + 1].r, colors[i + 1][j + 1].g, colors[i + 1][j + 1].b);
                glVertex3f(square_matrix[i + 1][j + 1].x, square_matrix[i + 1][j + 1].y, square_matrix[i + 1][j + 1].z);
                glEnd();

                glBegin(GL_TRIANGLE_STRIP);
                glColor3f(colors[i][j].r, colors[i][j].g, colors[i][j].b);
                glVertex3f(square_matrix[i][j].x, square_matrix[i][j].y, square_matrix[i][j].z);
                glColor3f(colors[i][j + 1].r, colors[i][j + 1].g, colors[i][j + 1].b);
                glVertex3f(square_matrix[i][j + 1].x, square_matrix[i][j + 1].y, square_matrix[i][j + 1].z);
                glColor3f(colors[i + 1][j + 1].r, colors[i + 1][j + 1].g, colors[i + 1][j + 1].b);
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


// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba
// przerysowaæ scenê)

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz¹cym

    glLoadIdentity();
    // Czyszczenie macierzy bie??cej

    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // Zdefiniowanie po³o¿enia obserwatora

    if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
    {
        thetaX += delta_x * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
        thetaY += delta_y * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
    }                                  // do ró¿nicy po³o¿eñ kursora myszy

    glRotatef(thetaX, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
    glRotatef(thetaY, 1.0, 0.0, 0.0);  //obrót obiektu o nowy k¹t

    if (status == 2)
    {
        glMatrixMode(GL_PROJECTION);
        viewer[2] += delta_z * pix2angle;
        glMatrixMode(GL_MODELVIEW);
    }

    Axes();
    // Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej

    glColor3f(1.0f, 1.0f, 1.0f);
    // Ustawienie koloru rysowania na bia³y

    //glutWireTeapot(3.0);
    // Narysowanie czajnika
    Egg();

    glFlush();
    // Przekazanie poleceñ rysuj¹cych do wykonania
    glutSwapBuffers();
}


// Funkcja ustalaj¹ca stan renderowania

void keys(unsigned char key, int x, int y)
{
    if (key == 'p') model = 1;
    if (key == 'w') model = 2;
    if (key == 's') model = 3;
    if (key == '1') model = 4;
    if (N > 1)
    {
        if (key == '-') N -= 1;
    }
    if (key == '=') N += 1;
    if (key == 'z')
        exit(0);

    RenderScene();
}


void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny
}


// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0 / (float)horizontal; //przeliczenie pikseli na stopnie

    glMatrixMode(GL_PROJECTION);
    // Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bie¿¹cej

    gluPerspective(70, 1.0, 1.0, 30.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if (horizontal <= vertical)
        glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
    // Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
    // relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

    glMatrixMode(GL_MODELVIEW);
    // Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bie¿¹cej

}


// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        z_pos_old = y;
        status = 2;
        return;
    }

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora
        y_pos_old = y;                     // jako pozycji poprzedniej
        status = 1;          // wciêniêty zosta³ lewy klawisz myszy
        return;
    }
        status = 0;          // nie zosta³ wciêniêty ¿aden klawisz
}


// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
    delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy

    x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

    delta_y = y - y_pos_old;

    y_pos_old = y;

    delta_z = y - z_pos_old;

    z_pos_old = y;

    glutPostRedisplay();     // przerysowanie obrazu sceny
}


void mainMessage()
{
    cout << "Program implementuj¹cy nasluchiwanie zdarzen myszy w OpenGL\n\n";

    cout << "Sterowanie:\n"
        << "'p' -> przelaczenie na widok jajka zlozonego z punktow\n"
        << "'w' -> przelaczenie na widok siatki jajka\n"
        << "'s' -> przelaczenie na widok jajka opisanego krzywymi Beziera (z kolorami)\n"
        << "'1' -> przelaczenie na widok czajnika\n"
        << "Lewy przycisk myszy & przeciagniecie -> obrot narysowanego obiektu\n"
        << "Prawy przycisk myszy & przeciagniecie -> przyblizenie/oddalenie narysowanego obiektu\n"
        << "'-'/'=' -> zmniejszenie/zwiekszenie zlozonosci jajka\n\n";

    cout << "Aby zamknac program nacisnij 'z' na oknie aplikacji.\n";
}


// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

int main(void)
{
    srand(time(NULL));

    mainMessage();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 600);

    glutCreateWindow("Rzutowanie perspektywiczne");

    glutKeyboardFunc(keys);

    glutDisplayFunc(RenderScene);
    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
    // (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem
    // gdy zajdzie potrzeba przerysowania okna

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
    // za zmiany rozmiaru okna                       

    MyInit();
    // Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przyst¹pieniem do renderowania
    glEnable(GL_DEPTH_TEST);
    // W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

    glutMouseFunc(Mouse);
    // Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

    glutMotionFunc(Motion);
    // Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}

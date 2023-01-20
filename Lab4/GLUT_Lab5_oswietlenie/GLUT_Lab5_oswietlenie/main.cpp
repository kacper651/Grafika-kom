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

struct Point
{
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


int N = 25;
int model = 1;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };

Point** createSquareMatrix()
{
    Point** matrix = new Point * [N];
    for (int i = 0; i < N; i++)
        matrix[i] = new Point[N];

    return matrix;
}



void Egg()
{

    Point** matrix = createSquareMatrix();

    switch (model)
    {
    case 1:
        glutSolidTeapot(3.0);
        break;
    case 2:
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

                glBegin(GL_TRIANGLES);

                glNormal3fv(v1);
                glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);

                glNormal3fv(v2);
                glVertex3f(matrix[i + 1][j].x, 5 - matrix[i + 1][j].y, matrix[i + 1][j].z);

                glNormal3fv(v4);
                glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

                glEnd();

                glBegin(GL_TRIANGLES);

                glNormal3fv(v1);
                glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);

                glNormal3fv(v3);
                glVertex3f(matrix[i][j + 1].x, 5 - matrix[i][j + 1].y, matrix[i][j + 1].z);

                glNormal3fv(v4);
                glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);

                glEnd();
            }
        }
        break;
    default:
        glutSolidTeapot(3.0);
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

    Egg();

    glFlush();
    // Przekazanie poleceñ rysuj¹cych do wykonania
    glutSwapBuffers();
}


// Funkcja ustalaj¹ca stan renderowania

void keys(unsigned char key, int x, int y)
{
    if (key == '1') model = 1;
    if (key == '2') model = 2;
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

    GLfloat mat_ambient[] = { 0.25, 0.25, 0.25, 0.1 };
    // wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego             

    GLfloat mat_shininess = { 128.0 };
    // wspó³czynnik n opisuj¹cy po³ysk powierzchni

    GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };
    // po³o¿enie Ÿród³a

    GLfloat light_position1[] = { 10.0, 0.0, 0.0, 1.0 };
    // po³o¿enie Ÿród³a

    GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 0.5 };
    // sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    // sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
    // sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat light_ambient1[] = { 0.0, 0.0, 1.0, 0.5 };
    // sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse1[] = { 0.0, 0.0, 1.0, 1.0 };
    // sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular1[] = { 0.0, 0.0, 1.0, 1.0 };
    // sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat att_constant = { 1.0 };
    // sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
    // odleg³oœci od Ÿród³a

    GLfloat att_linear = { 0.05 };
    // sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
    // odleg³oœci od Ÿród³a

    GLfloat att_quadratic = { 0.001 };
    // sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
    // odleg³oœci od Ÿród³a

    // Ustawienie parametrów materia³u i Ÿród³a œwiat³a

    // Ustawienie patrametrów materia³u

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    // Ustawienie parametrów Ÿród³a

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

    // Ustawienie opcji systemu oœwietlania sceny

    glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
    glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny
    glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
    glEnable(GL_LIGHT1);     // w³¹czenie Ÿród³a o numerze 1
    glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora

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
    cout << "Program symulujacy oswietlenie w OpenGL\n\n";

    cout << "Sterowanie:\n"
        << "'2' -> przelaczenie na widok jajka opisanego krzywymi Beziera (z kolorami)\n"
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

    glutCreateWindow("Oswietlenie");

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

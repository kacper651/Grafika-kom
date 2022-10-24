/*************************************************************************************/


// Trochê bardziej skomplikowany program wykorzystuj¹cy funkcje biblioteki OpenGL



/*************************************************************************************/

#include <windows.h>
#include <time.h>
#include <gl/gl.h>

#include <gl/glut.h>

/*************************************************************************************/


// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)

GLint depth = 3;
GLfloat a = 100;
GLfloat st_deformacji = 3;

void drawCarpet(GLfloat x, GLfloat y, GLfloat a, GLint depth)
{

    if (depth > 0)
    {
        a = a / 3;
        depth -= 1;
        drawCarpet(x, y, a, depth);
        drawCarpet(x - a, y, a, depth);
        drawCarpet(x - 2 * a, y, a, depth);
        drawCarpet(x, y - a, a, depth);
        drawCarpet(x - 2 * a, y - a, a, depth);
        drawCarpet(x, y - 2 * a, a, depth);
        drawCarpet(x - a, y - 2 * a, a, depth);
        drawCarpet(x - 2 * a, y - 2 * a, a, depth);
    }

    else
    {
        GLfloat def = (rand() % 20) * st_deformacji / (10 * a);
        glBegin(GL_POLYGON);
            glColor3f(((rand() % 101) * 0.01), ((rand() % 101) * 0.01), ((rand() % 101) * 0.01));
            glVertex2f(x + def, y + def);
            glColor3f(((rand() % 101) * 0.01), ((rand() % 101) * 0.01), ((rand() % 101) * 0.01));
            glVertex2f(x + def, y - a + def);
            glColor3f(((rand() % 101) * 0.01), ((rand() % 101) * 0.01), ((rand() % 101) * 0.01));
            glVertex2f(x - a + def, y - a + def);
            glColor3f(((rand() % 101) * 0.01), ((rand() % 101) * 0.01), ((rand() % 101) * 0.01));
            glVertex2f(x - a + def, y + def);
        glEnd();
    }
    glFlush();
    // Przekazanie poleceñ rysuj¹cych do wykonania
}

void RenderScene(void)

{

    glClear(GL_COLOR_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz¹cym

    //glColor3f(0.0f, 1.0f, 0.0f);
    // Ustawienie aktualnego koloru rysowania na zielony

    drawCarpet(50, 50, a, depth);
}

/*************************************************************************************/


// Funkcja ustalaj¹ca stan renderowania




void MyInit(void)

{

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    // Kolor okna wnêtrza okna - ustawiono na szary

}

/*************************************************************************************/


// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)

    glMatrixMode(GL_PROJECTION);
    // Okreœlenie uk³adu wspó³rzêdnych obserwatora

    glLoadIdentity();
    // Okreœlenie przestrzeni ograniczaj¹cej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wspó³czynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // okreœlenie okna obserwatora.
    // Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
    // Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)


    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);



    glMatrixMode(GL_MODELVIEW);
    // Okreœlenie uk³adu wspó³rzêdnych    

    glLoadIdentity();

}

/*************************************************************************************/


// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli




int main(void)

{

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    // Ustawienie trybu wyœwietlania
    // GLUT_SINGLE - pojedynczy bufor wyœwietlania
    // GLUT_RGBA - model kolorów RGB


    glutCreateWindow("Drugi program w OpenGL");
    // Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

    glutDisplayFunc(RenderScene);
    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
    // Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
    // trzeba bêdzie przerysowaæ okno


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
    // zmiany rozmiaru okna

    MyInit();
    // Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
    // inicjalizacje konieczneprzed przyst¹pieniem do renderowania


    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/
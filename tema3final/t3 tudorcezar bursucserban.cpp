#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <iostream>
#include "glut.h"
using namespace std;

// the size of the window measured in pixels
#define dim 400
// the maximum number of iterations for the Mandelbrot set membership testing 
#define NRITER_MB 500
// the maximum value of M for the Mandelbrot set membership testing 
#define MODMAX_MB 10000000
// increments used in CMandelbrot
#define RX_MB 0.005
#define RY_MB 0.005

#define NRITER_JF 5000
// the maximum value of M for the Mandelbrot set membership testing 
#define MODMAX_JF 10000000
// increments used in CMandelbrot
#define RX_JF 0.01
#define RY_JF 0.01

unsigned char prevKey;
int nivel = 0;

class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};

class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}

    CPunct& operator=(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    void getxy(double& x, double& y)
    {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};
class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector& operator=(CVector& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    // it is used for normalizing a vector
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};


class CComplex {
public:
    CComplex() : re(0.0), im(0.0) {}
    CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
    CComplex(const CComplex& c) : re(c.re), im(c.im) {}
    ~CComplex() {}

    CComplex& operator=(const CComplex& c)
    {
        re = c.re;
        im = c.im;
        return *this;
    }

    double getRe() { return re; }
    void setRe(double re1) { re = re1; }

    double getIm() { return im; }
    void setIm(double im1) { im = im1; }

    double getModul() { return sqrt(re * re + im * im); }

    int operator==(CComplex& c1)
    {
        return ((re == c1.re) && (im == c1.im));
    }

    CComplex pow2()
    {
        CComplex rez;
        rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
        rez.im = 2.0 * re * im;
        return rez;
    }

    friend CComplex operator+(const CComplex& c1, const CComplex& c2);
    friend CComplex operator*(CComplex& c1, CComplex& c2);

    void print(FILE* f)
    {
        fprintf(f, "%.20f%+.20f i", re, im);
    }

private:
    double re, im;
};

CComplex operator+(const CComplex& c1, const CComplex& c2)
{
    CComplex rez(c1.re + c2.re, c1.im + c2.im);
    return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2)
{
    CComplex rez(c1.re * c2.re - c1.im * c2.im,
        c1.re * c2.im + c1.im * c2.re);
    return rez;
}
class CCurbaKoch
{
public:
    void segmentKoch(double lungime, int nivel, CPunct& p, CVector v)
    {
        CPunct p1;
        if (nivel == 0)
        {
            v.deseneaza(p, lungime);
        }
        else
        {
            segmentKoch(lungime / 3.0, nivel - 1, p, v);
            p1 = v.getDest(p, lungime / 3.0);
            v.rotatie(60);
            segmentKoch(lungime / 3.0, nivel - 1, p1, v);
            p1 = v.getDest(p1, lungime / 3.0);
            v.rotatie(-120);
            segmentKoch(lungime / 3.0, nivel - 1, p1, v);
            p1 = v.getDest(p1, lungime / 3.0);
            v.rotatie(60);
            segmentKoch(lungime / 3.0, nivel - 1, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v1(sqrt(3.0) / 2.0, 0.5);
        CPunct p1(-1.0, 0.0);

        CVector v2(0.0, -1.0);
        CPunct p2(0.5, sqrt(3.0) / 2.0);

        CVector v3(-sqrt(3.0) / 2.0, 0.5);
        CPunct p3(0.5, -sqrt(3.0) / 2.0);

        segmentKoch(lungime, nivel, p1, v1);
        segmentKoch(lungime, nivel, p2, v2);
        segmentKoch(lungime, nivel, p3, v3);
    }
};
class CCurbaSierpinski
{
public:
    void curbaSierpinski(double lungime, int nivel, CPunct& p, CVector& v, int angle)
    {
        if (nivel == 0)
        {
            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);
        }
        else
        {
            lungime /= 1.2;
            curbaSierpinski(lungime, nivel - 1, p, v, -angle);
            v.rotatie(angle);
            curbaSierpinski(lungime, nivel - 1, p, v, angle);
            v.rotatie(angle);
            curbaSierpinski(lungime, nivel - 1, p, v, -angle);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, 1.0);
        CPunct p(-0.8, -0.8);

        if (nivel % 2 == 0)
        {
            curbaSierpinski(lungime, nivel, p, v, 60);
        }
        else
        {
            curbaSierpinski(lungime, nivel, p, v, -60);
        }
    }
};
class CArboreBinar
{
public:
    void arboreBinar(double lungime, int nivel, CPunct& p, CVector v)
    {
        CPunct p1;
        if (nivel == 0)
        {
            v.deseneaza(p, lungime);
        }
        else
        {
            arboreBinar(lungime, nivel - 1, p, v);
            p1 = v.getDest(p, lungime);

            v.rotatie(-45);
            arboreBinar(lungime / 2.0, nivel - 1, p1, v);

            v.rotatie(90);
            arboreBinar(lungime / 2.0, nivel - 1, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 1.0);

        arboreBinar(lungime, nivel, p, v);
    }
};
class CArborePerron
{
public:
    void arborePerron(double lungime,
        int nivel,
        double factordiviziune,
        CPunct p,
        CVector v)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (nivel == 0)
        {
        }
        else
        {
            v.rotatie(30);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            v.rotatie(-90);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            p2 = p1;

            v.rotatie(-30);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(90);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            p2 = p1;

            v.rotatie(30);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(-90);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, 1.0);
        CPunct p(0.0, -1.0);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        arborePerron(lungime, nivel, 0.4, p, v);
    }
};
class CCurbaHilbert
{
public:
    void curbaHilbert(double lungime, int nivel, CPunct& p, CVector& v, int d)
    {
        if (nivel == 0)
        {
        }
        else
        {
            v.rotatie(d * 90);
            curbaHilbert(lungime, nivel - 1, p, v, -d);

            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);

            v.rotatie(-d * 90);
            curbaHilbert(lungime, nivel - 1, p, v, d);

            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);

            curbaHilbert(lungime, nivel - 1, p, v, d);

            v.rotatie(-d * 90);
            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);

            curbaHilbert(lungime, nivel - 1, p, v, -d);

            v.rotatie(d * 90);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, 1.0);
        CPunct p(0.0, 0.0);

        curbaHilbert(lungime, nivel, p, v, 1);
    }
};
void draw_square(double x, double y, double dim_cadran)
{
    //dimensiunea cadranului este latura
    double jum_cadran = dim_cadran / 2;
    //glColor3f(1, 0.1, 0.1); // red
    //glBegin(GL_QUADS);
    //glVertex2f(x-jum_cadran, y+jum_cadran);
    //glVertex2f(x+jum_cadran, y+jum_cadran);
    //glVertex2f(x+jum_cadran, y-jum_cadran);
    //glVertex2f(x-jum_cadran, y-jum_cadran);
    //glEnd();
    CVector v(1.0, 0.0);
    CPunct p(x - jum_cadran, y + jum_cadran);
    v.deseneaza(p, dim_cadran);
    p = v.getDest(p, dim_cadran);
    v.rotatie(-90);
    v.deseneaza(p, dim_cadran);
    p = v.getDest(p, dim_cadran);
    v.rotatie(-90);
    v.deseneaza(p, dim_cadran);
    p = v.getDest(p, dim_cadran);
    v.rotatie(-90);
    v.deseneaza(p, dim_cadran);
    p = v.getDest(p, dim_cadran);

}
void sierpinski_carpet(double x, double y, double dim_cadran, int nivel)
{
    if (nivel == 0)
    {
        draw_square(x, y, dim_cadran / 3);
    }
    else
    {
        double y_min = y - dim_cadran / 2;
        double x_min = x - dim_cadran / 2;
        double x_max = x + dim_cadran / 2;
        dim_cadran /= 3;
        nivel -= 1;
        sierpinski_carpet(x_min + dim_cadran / 2, y_min + dim_cadran / 2, dim_cadran, nivel);
        sierpinski_carpet(x_min + dim_cadran / 2, y_min + dim_cadran + dim_cadran / 2, dim_cadran, nivel);
        sierpinski_carpet(x_min + dim_cadran / 2, y_min + 2 * dim_cadran + dim_cadran / 2, dim_cadran, nivel);


        sierpinski_carpet(x_min + dim_cadran + dim_cadran / 2, y_min + dim_cadran / 2, dim_cadran, nivel);
        sierpinski_carpet(x_min + dim_cadran + dim_cadran / 2, y_min + dim_cadran + dim_cadran / 2, dim_cadran * 3, nivel);
        sierpinski_carpet(x_min + dim_cadran + dim_cadran / 2, y_min + 2 * dim_cadran + dim_cadran / 2, dim_cadran, nivel);

        sierpinski_carpet(x_max - dim_cadran / 2, y_min + dim_cadran / 2, dim_cadran, nivel);
        sierpinski_carpet(x_max - dim_cadran / 2, y_min + dim_cadran + dim_cadran / 2, dim_cadran, nivel);
        sierpinski_carpet(x_max - dim_cadran / 2, y_min + 2 * dim_cadran + dim_cadran / 2, dim_cadran, nivel);
    }
}
class CArborePerron_tema
{
public:
    void arborePerron_tema(double lungime,
        int nivel,
        double factordiviziune,
        CPunct p,
        CVector v)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (nivel == 0)
        {
        }
        else
        {
            //r1
            v.rotatie(45);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            p2 = p1;

            //r2
            v.rotatie(-90);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            arborePerron_tema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            //r3
            p1 = p2;
            v.rotatie(105);
            v.deseneaza(p1, lungime / 2);
            p1 = v.getDest(p1, lungime / 2);
            arborePerron_tema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            //r4
            p1 = p2;
            v.rotatie(-60);
            v.deseneaza(p1, lungime / 2);
            p1 = v.getDest(p1, lungime / 2);
            p2 = p1;


            //r5
            p1 = p2;
            v.rotatie(-90);
            v.deseneaza(p1, lungime / 2);
            p1 = v.getDest(p1, lungime / 2);
            arborePerron_tema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            //r6
            p1 = p2;
            v.rotatie(120);
            v.deseneaza(p1, lungime / 2);
            p1 = v.getDest(p1, lungime / 2);
            arborePerron_tema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 2.8);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        arborePerron_tema(lungime, nivel, 0.4, p, v);
    }
};

class CMandelbrot {
public:
    CMandelbrot()
    {
        // m.c is initialized by default with 0+0i

        m.nriter = NRITER_MB;
        m.modmax = MODMAX_MB;
    }

    CMandelbrot(CComplex& c)
    {
        m.c = c;
        m.nriter = NRITER_MB;
        m.modmax = MODMAX_MB;
    }

    ~CMandelbrot() {}

    void setmodmax(double v) { assert(v <= MODMAX_MB); m.modmax = v; }
    double getmodmax() { return m.modmax; }

    void setnriter(int v) { assert(v <= NRITER_MB); m.nriter = v; }
    int getnriter() { return m.nriter; }

    int isInMandelbrot(const CComplex& c, int max_iterations)
    {
        CComplex z(0.0, 0.0);
        int iteration = 0;

        while (iteration < max_iterations && z.getModul() <= 2.0)
        {
            z = z.pow2() + c;
            iteration++;
        }

        return iteration;
    }


    void DisplayMandelbrot(double xmin, double ymin, double xmax, double ymax)
    {
        glPushMatrix();
        glLoadIdentity();

        glBegin(GL_POINTS);
        int counter = 0;
        double n = -2 / (xmin - xmax);
        double m = -1 - n * xmin;
        for (double x = xmin; x <= xmax; x += RX_MB)
        {

            for (double y = ymin; y <= ymax; y += RY_MB)
            {

                CComplex c(x, y);
                int iterations = isInMandelbrot(c, NRITER_MB);

                if (iterations == NRITER_MB)
                {
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex3d(n * x + m, n * y + m, 0);
                    counter++;
                    if (counter % 1000 == 0)
                        printf("Drawn %d points in the set so far.\n", counter);
                }
                else
                {
                    if (iterations < 4)
                    {
                        glColor3f(0.0f, 1.0f, 0.0f);
                    }
                    else if (iterations < 15)
                    {
                        glColor3f(0.0f, 0.0f, 1.0f);
                    }
                    else
                    {
                        glColor3f(1.0f, 1.0f, 0.0f);
                    }
                    glVertex3d(n * x, n * y, 0);

                }
            }
        }

        glEnd();
        glPopMatrix();
    }


private:
    struct SDate {
        CComplex c;
        // number of iterations
        int nriter;
        // the maximum value of M
        double modmax;
    } m;
};

class CJuliaFatou {
public:
    CJuliaFatou()
    {
        // m.c is initialized by default with 0+0i

        m.nriter = NRITER_JF;
        m.modmax = MODMAX_JF;
    }

    CJuliaFatou(CComplex& c)
    {
        m.c = c;
        m.nriter = NRITER_JF;
        m.modmax = MODMAX_JF;
    }

    ~CJuliaFatou() {}

    void setmodmax(double v) { assert(v <= MODMAX_JF); m.modmax = v; }
    double getmodmax() { return m.modmax; }

    void setnriter(int v) { assert(v <= NRITER_JF); m.nriter = v; }
    int getnriter() { return m.nriter; }

    // it tests if x belongs to the Julia-Fatou set Jc
    // it returns 0 if yes, -1 for finite convergence, +1 for infinite convergence
    int isIn(CComplex& x)
    {
        int rez = 0;
        // an array for storing the values for computing z_n+1 = z_n * z_n + c
        CComplex z0, z1;

        z0 = x;
        for (int i = 1; i < m.nriter; i++)
        {
            z1 = z0 * z0 + m.c;
            if (z1 == z0)
            {
                // x does not belong to the J-F set because the 
                // iterative process converges finitely
                rez = -1;
                break;
            }
            else if (z1.getModul() > m.modmax)
            {
                // x does not belong to the J-F set because the 
                // iterative process converges infinitely
                rez = 1;
                break;
            }
            z0 = z1;
        }

        return rez;
    }

    // it displays a J-F set
    void display(double xmin, double ymin, double xmax, double ymax)
    {
        glPushMatrix();
        glLoadIdentity();

        glBegin(GL_POINTS);
        for (double x = xmin; x <= xmax; x += RX_JF)
            for (double y = ymin; y <= ymax; y += RY_JF)
            {
                CComplex z(x, y);
                int r = isIn(z);
                if (r == 0)
                {
                    glVertex3d(x, y, 0);
                }
                else if (r == -1)
                {
                }
                else if (r == 1)
                {
                }
            }
        fprintf(stdout, "STOP\n");
        glEnd();

        glPopMatrix();
    }

private:
    struct SDate {
        CComplex c;
        // number of iterations
        int nriter;
        // the maximum value of M
        double modmax;
    } m;
};

// Julia-Fatou set for z0 = 0 and c = -0.12375+0.056805i
void DisplayJF1() {
    CComplex c(-0.12375, 0.056805);
    CJuliaFatou cjf(c);

    glColor3f(1.0, 0.1, 0.1);
    cjf.setnriter(30);
    cjf.display(-0.8, -0.4, 0.8, 0.4);
}

// Julia-Fatou set for z0 = 0 and c = -0.012+0.74i
void DisplayJF2() {
    CComplex c(-0.012, 0.74);
    CJuliaFatou cjf(c);

    glColor3f(1.0, 0.1, 0.1);
    cjf.setnriter(30);
    cjf.display(-1, -1, 1, 1);
}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}
void Display6() {
    CCurbaHilbert cch;
    cch.afisare(0.05, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');

    nivel++;
}
void Display7() {
    CArborePerron cap;

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, -0.5, 0.0);
    cap.afisare(1, nivel);
    glPopMatrix();
    nivel++;
}

void Display2() {
    CArborePerron_tema cap;

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '2');

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, -0.5, 0.0);
    cap.afisare(1, nivel);
    glPopMatrix();
    nivel++;
}
void Display3()
{
    sierpinski_carpet(0, 0, 1.7, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'S');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'p');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 's');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'k');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');

    nivel++;

}
void Display4() {
    CCurbaSierpinski ccs;
    ccs.afisare(0.05, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'w');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'd');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'S');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'p');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 's');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'k');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');

    nivel++;
}
void Display8() {
    CArboreBinar cab;
    cab.afisare(1, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');

    nivel++;
}
void Display9() {
    CCurbaKoch cck;
    cck.afisare(sqrt(3.0), nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

    nivel++;
}

void Display(void)
{
    CMandelbrot mandelbrot;
    switch (prevKey)
    {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        nivel = 0;
        fprintf(stderr, "nivel = %d\n", nivel);
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.1, 0.1);
        mandelbrot.DisplayMandelbrot(-2.0, -2.0, 2.0, 2.0);
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.1, 0.1);
        Display2();
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.1, 0.1);
        Display3();
        break;
    case '4':
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.1, 0.1);
        Display4();
        break;
    case '5':
        glClear(GL_COLOR_BUFFER_BIT);
        Display6();
        break;
    case '6':
        glClear(GL_COLOR_BUFFER_BIT);
        Display7();
        break;
    case '7':
        glClear(GL_COLOR_BUFFER_BIT);
        Display8();
        break;
    case '8':
        glClear(GL_COLOR_BUFFER_BIT);
        Display9();
        break;
    case 'q':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayJF1();
        break;
    case 'w':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayJF2();
        break;

    default:
        break;
    }

    glFlush();
}


void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}
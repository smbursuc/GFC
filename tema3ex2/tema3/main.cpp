#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// the size of the window measured in pixels
#define dim 400
// the maximum number of iterations for the Mandelbrot set membership testing 
#define NRITER_MB 500
// the maximum value of M for the Mandelbrot set membership testing 
#define MODMAX_MB 10000000
// increments used in CMandelbrot
#define RX_MB 0.005
#define RY_MB 0.005

unsigned char prevKey;


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
    for (double x = xmin; x <= xmax; x += RX_MB)
    {
        for (double y = ymin; y <= ymax; y += RY_MB)
        {
            CComplex c(x, y);
            int iterations = isInMandelbrot(c, NRITER_MB);

            if (iterations == NRITER_MB)
            {
                glColor3f(1.0, 0.0, 0.0);
                glVertex3d(x, y, 0);
                counter++;
                    if (counter % 1000 == 0)
                        printf("Drawn %d points in the set so far.\n", counter);
            }
            else
            {
                if (iterations < 5)
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
                glVertex3d(x, y, 0);

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


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    CMandelbrot mandelbrot;
    switch (prevKey)
    {
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.1, 0.1);
        mandelbrot.DisplayMandelbrot(-2.0, -2.0, 2.0, 2.0);
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


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// the size of the window measured in pixels
#define dim 400
// the maximum number of iterations for the Mandelbrot set membership testing 
#define NRITER_MB 500
// the maximum value of M for the Mandelbrot set membership testing 
#define MODMAX_MB 10000000
// increments used in CMandelbrot
#define RX_MB 0.005
#define RY_MB 0.005

unsigned char prevKey;


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
    for (double x = xmin; x <= xmax; x += RX_MB)
    {
        for (double y = ymin; y <= ymax; y += RY_MB)
        {
            CComplex c(x, y);
            int iterations = isInMandelbrot(c, NRITER_MB);

            if (iterations == NRITER_MB)
            {
                glColor3f(1.0, 0.0, 0.0);
                glVertex3d(x, y, 0);
                counter++;
                    if (counter % 1000 == 0)
                        printf("Drawn %d points in the set so far.\n", counter);
            }
            else
            {
                if (iterations < 5)
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
                glVertex3d(x, y, 0);

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


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    CMandelbrot mandelbrot;
    switch (prevKey)
    {
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.1, 0.1);
        mandelbrot.DisplayMandelbrot(-2.0, -2.0, 2.0, 2.0);
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



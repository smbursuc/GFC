#include <cmath>
#include "glut.h"
#include <vector>
#include <iostream>
unsigned char prevKey;
class CartesianGrid {
public:
    CartesianGrid(int rows, int columns) : rows_(rows), columns_(columns) {}

    void draw() {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float spacing_x = 2.0f / (columns_ - 1);
        float spacing_y = 2.0f / (rows_ - 1);

        /*for (int j = 0; j < rows_; ++j) {
            for (int i = 0; i < columns_; ++i) {
                writePixel(i, j);
            }
        }*/

        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < columns_; ++i) {
            glVertex2f(-1.0f + i * spacing_x, -1.0f);
            glVertex2f(-1.0f + i * spacing_x, 1.0f);
        }
        for (int j = 0; j < rows_; ++j) {
            glVertex2f(-1.0f, -1.0f + j * spacing_y);
            glVertex2f(1.0f, -1.0f + j * spacing_y);
        }
        glEnd();
    }

    void writePixel(int i, int j) {
        if (i < 0 || i >= columns_ || j < 0 || j >= rows_) {
            return;
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float spacing_x = 2.0f / (columns_ - 1);
        float spacing_y = 2.0f / (rows_ - 1);
        float x = -1.0f + i * spacing_x;
        float y = -1.0f + j * spacing_y;

        glBegin(GL_POLYGON);

        for (int k = 0; k < 360; k += 10) {
            float angle = k * 3.14159265358979323846f / 180.0f;
            float vertex_x = x + 0.03f * cos(angle);
            float vertex_y = y + 0.03f * sin(angle);
            glVertex2f(vertex_x, vertex_y);
        }
        glEnd();
    }

    int rows() const {
        return rows_;
    }

    int columns() const {
        return columns_;
    }

private:
    int rows_;
    int columns_;
};

void AfisareSegmentDreaptaJos(int x0, int y0, int xn, int yn, std::vector<std::pair<int, int>>& M);
void AfisareSegmentDreaptaSus(int x0, int y0, int xn, int yn, std::vector<std::pair<int, int>>& M);
void AfisareCerc4(int R, std::vector<std::pair<int, int>>& M);
void AfisareCerc2(int R, std::vector<std::pair<double, double>>& M);
void IngrosareCerc(int R, std::vector<std::pair<double, double>>& M, int grosime);
void AfisarePuncteCerc3(int x, int y, std::vector<std::pair<int, int>>& M);

void display1() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    CartesianGrid cartesian_grid(16, 16);
    cartesian_grid.draw();

    std::vector<std::pair<int, int>> points1;
    AfisareSegmentDreaptaJos(0, 0, 15, 7, points1);
    for (const auto& p : points1) {
        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
    glColor3f(0, 0, 0);
    for (const auto& p : points1) {
        cartesian_grid.writePixel(p.first, p.second);
    }


    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    float spacing_x = 2.0f / (cartesian_grid.columns() - 1);
    float spacing_y = 2.0f / (cartesian_grid.rows() - 1);
    float x0 = -1.0f + 0 * spacing_x;
    float y0 = -1.0f + 0 * spacing_y;
    float xn = -1.0f + 15 * spacing_x;
    float yn = -1.0f + 7 * spacing_y;
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(xn, yn);
    glEnd();


    std::vector<std::pair<int, int>> points2;
    AfisareSegmentDreaptaSus(0, 15, 15, 10,points2);
    for (const auto& p : points2) {
        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
    glColor3f(0, 0, 0);
    for (const auto& p : points2) {
        cartesian_grid.writePixel(p.first, p.second);
    }

    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    x0 = -1.0f + 0 * spacing_x;
    y0 = -1.0f + 15 * spacing_y;
    xn = -1.0f + 15 * spacing_x;
    yn = -1.0f + 10 * spacing_y;
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(xn, yn);
    glEnd();
    glFlush();

    
}
void display2()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    CartesianGrid cartesian_grid(16, 16);
    cartesian_grid.draw();

    std::vector<std::pair<int, int>> points1;
    std::vector<std::pair<int, int>> points2;
    int r = 13;
    AfisareCerc4(r, points1);
    glBegin(GL_POINTS);
    for (const auto& p : points1) {
        if (p.first >= 0 && p.second >= 0)
        {
            cartesian_grid.writePixel(p.first, p.second);
        }
    }
    //glEnd();
    //glBegin(GL_POINTS);
    //AfisareElipsa(402, 134, points2);
    //glColor3f(0, 0, 0);
    //std::cout << "???" << std::endl;
    //int a_max = 300;
    //int b_max = 100;
    //for (const auto& p : points2) {
    //    double x_vp = 2.0 * (p.first - -a_max) / (a_max - -a_max) - 1 - 1;
    //    double y_vp = 2.0 * (p.second - -b_max) / (b_max - -b_max) - 1 - 1;
    //    std::cout << "vp(" << x_vp << ", " << y_vp << ")" << std::endl;
    //    /*double coord1 = double(p.first) / 150;
    //    double coord2 = double(p.second) / 50;*/
    //    glVertex2f(x_vp, y_vp);
    //}
    //glEnd();
    glBegin(GL_POINTS);
    std::vector<std::pair<double, double>> points3;
    int r_cerc = 23;
    AfisareCerc2(r_cerc, points3);
    IngrosareCerc(r, points3, 2);
    glColor3f(1, 0.1, 0.1); // red
    std::cout << "???" << std::endl;
    for (const auto& p : points3) {
        //cartesian_grid.writePixel(p.first, p.second);

        glVertex2f(double(p.first) / r - 1, double(p.second) / r - 1);
    }
    glEnd();


    glFlush();
}
void display(void)
{
    switch (prevKey)
    {
    case '1':
        display1();
        break;
    case '2':
        display2();
        break;
    }
}


void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Grid");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(KeyboardFunc);
    glutMainLoop();
    return 0;
}
void AfisareSegmentDreaptaJos(int x0, int y0, int xn, int yn, std::vector<std::pair<int, int>>& M)
{
    int dx = xn - x0;
    int dy = yn - y0;

    int d = 2 * dy - dx;
    int dE = 2 * dy;
    int dNE = 2 * (dy - dx);
    int x = x0, y = y0;

    M.push_back(std::make_pair(x, y));

    while (x < xn) {
        if (d <= 0) {
            d += dE;
            x++;
        }
        else {
            d += dNE;
            x++;
            y++;
        }
        M.push_back(std::make_pair(x, y));
    }
}


void AfisareSegmentDreaptaSus(int x0, int y0, int xn, int yn, std::vector<std::pair<int, int>>& M)
{
    int dx = xn - x0;
    int dy = yn - y0;
    int step_y = 1;

    if (y0 > yn) {
        dy = -dy;
        step_y = -1;
    }

    int d = 2 * dy - dx;
    int dE = 2 * dy;
    int dNE = 2 * (dy - dx);
    int x = x0, y = y0;

    M.push_back(std::make_pair(x, y));
    M.push_back(std::make_pair(x, y + 1));
    M.push_back(std::make_pair(x + 1, y));
    M.push_back(std::make_pair(x, y - 1));
    M.push_back(std::make_pair(x - 1, y));

    while (x < xn) {
        if (d <= 0) {
            d += dE;
            x++;
        }
        else {
            d += dNE;
            x++;
            y += step_y;
        }
        M.push_back(std::make_pair(x, y));
        M.push_back(std::make_pair(x, y + 1));
        M.push_back(std::make_pair(x + 1, y));
        M.push_back(std::make_pair(x, y - 1));
        M.push_back(std::make_pair(x - 1, y));
    }
}
void AfisarePuncteCerc3(int x, int y, std::vector<std::pair<int, int>>& M)
{
    //M.push_back(std::make_pair(x, y));
    //M.push_back(std::make_pair(-x, -y));
    //M.push_back(std::make_pair(-x, y));
    //M.push_back(std::make_pair(x, -y));
    if (x != y)
    {
        M.push_back(std::make_pair(y, x));
        M.push_back(std::make_pair(-y, -x));
        M.push_back(std::make_pair(-y, x));
        M.push_back(std::make_pair(y, -x));
        M.push_back(std::make_pair(y - 1, x));
        M.push_back(std::make_pair(y + 1, x));
    }
    if (x == y)
    {
        M.push_back(std::make_pair(x, y));
        M.push_back(std::make_pair(x - 1, y));
        M.push_back(std::make_pair(x + 1, y));
    }
}
void AfisareCerc4(int R, std::vector<std::pair<int, int>>& M)
{
    int x = 0, y = R;
    int d = 1 - R;
    int dE = 3, dSE = -2 * R + 5;
    M.empty();
    AfisarePuncteCerc3(x, y, M);
    while (y > x)
    {
        if (d < 0)
        {
            d += dE;
            dE += 2;
            dSE += 2;
        }
        else
        {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
        AfisarePuncteCerc3(x, y, M);
    }
}

void AfisareCerc2(int R, std::vector<std::pair<double, double>>& M)
{
    M.empty();
    double pi = atan(1.0) * 4;
    for (double t = 0; t <= pi / 2; t += pi / 2000)
    {
        M.push_back(std::make_pair(R * cos(t) + 0.5, R * sin(t) + 0.5));
    }
}


void IngrosareCerc(int R, std::vector<std::pair<double, double>>& M, int grosime)
{
    std::vector<std::pair<double, double>> PuncteAdaugate;
    for (int i = 0; i < M.size(); i++)
    {
        std::pair<double, double> p = M[i];
        double x = p.first;
        double y = p.second;
        for (int j = -grosime; j <= grosime; j++)
        {
            double gX = x + (j / 2.0) / R - 1;
            double gY = y + (j / 2.0) / R - 1;
            PuncteAdaugate.push_back(std::make_pair(gX, gY));
        }
    }
    M = PuncteAdaugate;
}

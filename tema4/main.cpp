#include <cmath>
#include "glut.h"
#include <vector>
#include <iostream>
class CartesianGrid {
public:
    CartesianGrid(int rows, int columns) : rows_(rows), columns_(columns) {}

    void draw() {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float spacing_x = 2.0f / (columns_ - 1);
        float spacing_y = 2.0f / (rows_ - 1);

        // draw the pixels at the intersections of rows and columns
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

void AfisareSegmentDreapta3(int x0, int y0, int xn, int yn, std::vector<std::pair<int, int>>& M);

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    CartesianGrid cartesian_grid(15, 15);
    cartesian_grid.draw();

    std::vector<std::pair<int, int>> points1;
    AfisareSegmentDreapta3(0, 0, 14, 7, points1);
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
    float xn = -1.0f + 14 * spacing_x; 
    float yn = -1.0f + 7 * spacing_y; 
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(xn, yn);
    glEnd();


    std::vector<std::pair<int, int>> points2;
    AfisareSegmentDreapta3(0, 14, 14, 10, points2);
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
    y0 = -1.0f + 14 * spacing_y;
    xn = -1.0f + 14 * spacing_x;
    yn = -1.0f + 10 * spacing_y;
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(xn, yn);
    glEnd();

    glFlush();
}



void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y) {

}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Grid");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
void AfisareSegmentDreapta3(int x0, int y0, int xn, int yn, std::vector<std::pair<int, int>>& M)
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

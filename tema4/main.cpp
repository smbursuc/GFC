#include <cmath>
#include "glut.h"


class CartesianGrid {
public:
    CartesianGrid(int rows, int columns) : rows_(rows), columns_(columns) {}

    void draw() {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // calculate the spacing between pixels
        float spacing_x = 2.0f / (columns_ - 1);
        float spacing_y = 2.0f / (rows_ - 1);

        // draw the pixels at the intersections of rows and columns
        for (int j = 0; j < rows_; ++j) {
            for (int i = 0; i < columns_; ++i) {
                writePixel(i, j);
            }
        }

        // draw the lines between the pixels
        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f); // set color to black
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

        // calculate the pixel position
        float spacing_x = 2.0f / (columns_ - 1);
        float spacing_y = 2.0f / (rows_ - 1);
        float x = -1.0f + i * spacing_x;
        float y = -1.0f + j * spacing_y;

        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.0f, 0.0f); // set color to black
        for (int k = 0; k < 360; k += 10) {
            float angle = k * 3.14159265358979323846f / 180.0f;
            float vertex_x = x + 0.03f * cos(angle);
            float vertex_y = y + 0.03f * sin(angle);
            glVertex2f(vertex_x, vertex_y);
        }
        glEnd();
    }

private:
    int rows_;
    int columns_;
};


void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set background to white
    glClear(GL_COLOR_BUFFER_BIT);
    CartesianGrid cartesian_grid(10, 10);
    cartesian_grid.draw();

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

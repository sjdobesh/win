// source
// https://stackoverflow.com/questions/19386509/drawing-multiple-shapes-in-opengl
#include <GL/glut.h>


void drawline(
  float from_x, float from_y,
  float to_x, float to_y
) {
    // from coordinate position
    glVertex2f(from_x, from_y);
    // to coordinate position
    glVertex2f(to_x, to_y);
}

void drawShape() {
    glColor3f(1.0, 1.0, 1.0); // color (rgb):
    glLineWidth(2.0); // set line width to 2.0

    // draw line
    glBegin(GL_LINES);
    drawline(0.25, 0.5, 0.4, 0.5);
    drawline(0.4, 0.6, 0.4, 0.5);
    drawline(0.4, 0.4, 0.4, 0.5);
    drawline(0.6, 0.5, 0.75, 0.5);
    glEnd();

    // draw triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(0.4, 0.5);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.6, 0.4);
    glEnd();
}

void render_shape() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 4.0, 0.0, 4.0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw shape one
    glPushMatrix();
    glTranslatef(1.5, 1.5, 0.0);
    drawShape();
    glPopMatrix();

    // draw shape two
    glPushMatrix();
    glTranslatef(2.5, 2.5, 0.0);
    drawShape();
    glPopMatrix();
}

#define GL_SILENCE_DEPRECATION
#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

// Windows

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

//MacOS

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <cmath>
#include <vector>

using namespace std;

// Global Variables
float angle = 90.0f;
float swingAngle;
float rotatingAngle = 0.0f;
float amplitude = 0.5f;
float amplitudeChange = 0.01;
float shift = 0.1;
float slope = 0.0f;
float angle2 = 0.0f;
float poleY = 1.0f;
float raise = 0.1f;

enum Scene {
    sceneOne,
    sceneTwo,
    sceneThree,
    sceneFour,
    sceneFive
};


struct Platform {
    float x, y, height, width;
};

struct Ball {
    float x, y, radius, segments;
};


struct Person {
    float x, y;
};

Scene currentScene = sceneOne;
Platform platform = {0, 0, 1.0, 20.0};
Ball ball = {3.0f, 2.0f, 1.0f, 50.0f};
Person persons[1] = {
    {0.0f, 1.0f}
};


void drawRectangle(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);

    glVertex2f(x, y);                  // Top-left corner
    glVertex2f(x + width, y);          // Top-right corner
    glVertex2f(x + width, y - height); // Bottom-right corner
    glVertex2f(x, y - height);         // Bottom-left corner

    glEnd();
}


void drawCircle(float x, float y, float radius, float segments) {
    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    
    glEnd();
    
}

void drawSemicircle(float x, float y, float radius, int segments)
{
    float angleStep = M_PI / segments;  // Angle increment between segments

    glBegin(GL_TRIANGLE_FAN);

    for (float i = 0.0f; i <= segments; ++i)
    {
        float angle = i * angleStep;
        float xPos = x + radius * cosf(angle);
        float yPos = y + radius * sinf(angle);
        glVertex2f(xPos, yPos);
    }

    glEnd();
}

void drawTriangle(float x, float y, float base, float height) {
    glBegin(GL_TRIANGLES);

    // Calculate the coordinates of the three vertices
    float x1 = x;
    float y1 = y;
    float x2 = x + base;
    float y2 = y;
    float x3 = x + (base / 2.0f);
    float y3 = y + height;

    // Specify the vertices of the triangle
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    glEnd();
}

void drawTriangleStrip(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLE_STRIP);

    // Specify the vertices of the triangle strip
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    glEnd();
}


void drawPlatform(const Platform &platform) {
    glBegin(GL_QUADS);

    glVertex2f(platform.x, platform.y); // Bottom left corner
    glVertex2f(platform.x + platform.width, platform.y); // Bottom right corner
    glVertex2f(platform.x + platform.width, platform.y + platform.height); // Top right corner
    glVertex2f(platform.x, platform.y + platform.height); // Top left corner

    glEnd();
}


void drawPerson(const Person &person) {
    
    glColor3f(0.0, 0.0, 0.0);
    drawTriangle(person.x, person.y, 2.0f, 2.0f);

    // head
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(person.x + 1.0f, person.y + 2.0f, 0.7f, 50.0f);

    // eyes
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(person.x + 1.0f, person.y + 2.3f, 0.2f, 50.0f);

    // beak
    glColor3f(0.0, 0.0, 0.0);
    drawTriangleStrip(person.x + 1.5f, person.y + 2.5f, person.x + 1.5f, person.y + 2.0f - 0.5f, person.x + 2.5f, person.y + 2.0f);
}

void drawWindmill() {
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex2f(5.0f, 0.0f);
    glVertex2f(9.0f, 0.0f);
    glVertex2f(9.0f, 9.0f);
    glVertex2f(7.0f, 13.0f);
    glVertex2f(5.0f, 9.0f);
    glEnd();
    
    // fans
    
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(7.0f, 11.0f, 0.0f);
    glRotatef(rotatingAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-7.0f, -11.0f, 0.0f);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(7.5f, 10.5f);
    glVertex2f(12.0f, 10.5f);
    glVertex2f(12.0f, 11.0f);
    glVertex2f(7.5f, 11.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(7.0f, 10.8f);
    glVertex2f(7.6f, 10.8f);
    glVertex2f(7.6f, 11.0f);
    glVertex2f(7.0f, 11.0f);
    glEnd();
    glPopMatrix();
    
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(7.0f, 11.0f, 0.0f);
    glRotatef(rotatingAngle + 90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-7.0f, -11.0f, 0.0f);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(7.5f, 10.5f);
    glVertex2f(12.0f, 10.5f);
    glVertex2f(12.0f, 11.0f);
    glVertex2f(7.5f, 11.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(7.0f, 10.8f);
    glVertex2f(7.6f, 10.8f);
    glVertex2f(7.6f, 11.0f);
    glVertex2f(7.0f, 11.0f);
    glEnd();
    glPopMatrix();
    
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(7.0f, 11.0f, 0.0f);
    glRotatef(rotatingAngle + 180.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-7.0f, -11.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(7.5f, 10.5f);
    glVertex2f(12.0f, 10.5f);
    glVertex2f(12.0f, 11.0f);
    glVertex2f(7.5f, 11.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(7.0f, 10.8f);
    glVertex2f(7.6f, 10.8f);
    glVertex2f(7.6f, 11.0f);
    glVertex2f(7.0f, 11.0f);
    glEnd();
    glPopMatrix();
    
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(7.0f, 11.0f, 0.0f);
    glRotatef(rotatingAngle + 270.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-7.0f, -11.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(7.5f, 10.5f);
    glVertex2f(12.0f, 10.5f);
    glVertex2f(12.0f, 11.0f);
    glVertex2f(7.5f, 11.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(7.0f, 10.8f);
    glVertex2f(7.6f, 10.8f);
    glVertex2f(7.6f, 11.0f);
    glVertex2f(7.0f, 11.0f);
    glEnd();
    glPopMatrix();
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(7.0f, 11.0f, 0.2f, 50.0f);
}

void drawPolarRose(float x, float y, float k, int numPoints) {
    glBegin(GL_POLYGON);

    float deltaTheta = 2.0f * M_PI / numPoints; // angle increment between points

    for (int i = 0; i < numPoints; i++) {
        float theta = i * deltaTheta;
        float r = sinf(k * theta); // polar rose equation
        float px = r * cosf(theta);
        float py = r * sinf(theta);
        glVertex2f(x + px, y + py);
    }

    glEnd();
}

void drawTree(float x1, float y1, float angle, int depth) {
    if (depth == 0) return;

    float x2 = x1 + cos(angle * M_PI / 180.0);
    float y2 = y1 + sin(angle * M_PI / 180.0) * 3.0;

    // Determine color based on current depth
    float red, green, blue;
    if (depth < 3) {
        red = 0.0f;
        green = 0.5f;
        blue = 0.0f;
    } else {
        red = 0.51f;
        green = 0.114f;
        blue = 0.0f;
    }
    
    glLineWidth(800.0);
    glColor3f(red, green, blue);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    // Recursively draw the two branches of the tree
    drawTree(x2, y2, angle - 10, depth - 1);
    drawTree(x2, y2, angle + 10, depth - 1);
    drawTree(x2, y2, angle - 25, depth - 1);
    drawTree(x2, y2, angle + 25, depth - 1);
}

void drawBoat(float posX, float posY) {
    // Draw the hull of the boat
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f + posX, 1.0f + posY);
    glVertex2f(3.0f + posX, 1.0f + posY);
    glVertex2f(2.0f + posX, 0.0f + posY);
    glVertex2f(0.0f + posX, 0.0f + posY);
    glEnd();
}

void drawFlag(float poleX, float poleY, float flagR, float flagG, float flagB) {
    // Draw the pole
    glColor3f(0.65f, 0.16f, 0.16f); // Set the color to a shade of brown
    glBegin(GL_LINES);
    glVertex2f(poleX, poleY);
    glVertex2f(poleX, poleY + 5.0f);
    glEnd();

    // Draw the triangular flag
    glColor3f(flagR, flagG, flagB); // Set the color as provided
    glBegin(GL_TRIANGLES);
    glVertex2f(poleX, poleY + raise);
    glVertex2f(poleX + 2.0f, poleY + 0.5f + raise);
    glVertex2f(poleX, poleY + 1.0f + raise);
    glEnd();
}



void drawSceneOne() {
    drawWindmill();
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(ball.x, ball.y, ball.radius, ball.segments);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawPerson(persons[0]);
    glColor3f(0.0f, 1.0f, 0.0f);
    
    drawPlatform(platform);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (float x = 1.0f; x <= 20.0f; x += 2.0f) {
        drawPolarRose(x, 1.0f, -4.0f, 1000);
    }
}

void drawSceneTwo() {
    drawTree(10.0, 1.0, angle, 6.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(ball.x, ball.y, ball.radius, ball.segments);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawPerson(persons[0]);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawPlatform(platform);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (float x = 1.0f; x <= 20.0f; x += 2.0f) {
        drawPolarRose(x, 1.0f, -4.0f, 1000);
    }
}

void drawSceneThree() {
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.0f, 10.0f, 0.0f);
    glRotatef(-60.0, 0.0, 0.0, 1.0);
    glTranslatef(0.0f, -10.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(ball.x, ball.y, ball.radius, ball.segments);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawPerson(persons[0]);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-20.0f, 0.0f);
    glVertex2f(30.0f, 0.0f);
    glVertex2f(30.0f, 15.0f);
    glVertex2f(-20.0f, 15.0f);
    glEnd();
    glPopMatrix();
}

void drawSceneFour() {
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(persons[0].x, persons[0].y, 0.0f);
    glRotatef(angle2 * (180.0f / M_PI), 0.0f, 0.0f, 1.0f);
    glTranslatef(-persons[0].x, -persons[0].y, 0.0f);
    
    glColor3f(1.0f, 1.0f, 1.0f); // Draw the sail of the boat
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1.0f + persons[0].x, 1.0f + persons[0].y);
    glVertex2f(3.0f + persons[0].x, 1.0f + persons[0].y);
    glVertex2f(1.0f + persons[0].x, 3.0f + persons[0].y);
    glEnd();
    
    drawPerson(persons[0]);
    
    drawBoat(persons[0].x, persons[0].y);

    glPopMatrix();
    
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(ball.x, ball.y, ball.radius, ball.segments);

    glColor3f(0.0, 0.0, 1.0); // Set color to blue
    glBegin(GL_QUAD_STRIP);
    for (float x = -10.0f; x <= 30.0f; x += 0.1f) {
    float y = amplitude * sinf(x) + 10.0f; // Adjust amplitude and frequency as needed
        glVertex2f(x + amplitude, y);
        glVertex2f(x + amplitude, -1.0f); // Draw to bottom of screen
    }
    glEnd();
}

void drawSceneFive() {
    glColor3f(0.0f, 1.0f, 0.0f);
    drawPlatform(platform);
    drawPerson(persons[0]);
    drawFlag(10.0f, 1.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(ball.x, ball.y, ball.radius, ball.segments);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentScene) {
        case sceneOne:
            drawSceneOne();
            break;
        case sceneTwo:
            drawSceneTwo();
            break;
        case sceneThree:
            drawSceneThree();
            break;
        case sceneFour:
            drawSceneFour();
            break;
        case sceneFive:
            drawSceneFive();
            break;
    }

}

void update(int value) {
    switch (currentScene) {
        case sceneOne:
            if (persons[0].x >= 25.0f)
            {
                currentScene = sceneTwo;
                persons[0].x = -6.0;
                ball.x = -3.0;
            }
            rotatingAngle += 1.0f;
            persons[0].x += 0.1;
            persons[0].y = abs(2.0f * sinf(persons[0].x)) + 1.0f;
            ball.x += 0.09;
            ball.y = abs(2.0f * sinf(ball.x)) + 2.0f;

            break;
        case sceneTwo:
            if (persons[0].x >= 25.0f)
            {
                currentScene = sceneThree;
                persons[0].x = -5.0f;
                ball.x = -2.0f;
            }
            swingAngle = sinf(persons[0].x) * 5.0f + 90.0f;
            angle = swingAngle;
            persons[0].x += 0.1;
            persons[0].y = abs(2.0f * sinf(persons[0].x)) + 1.0f;
            ball.x += 0.09;
            ball.y = abs(2.0f * sinf(ball.x)) + 2.0f;
            break;
        case sceneThree:
            if (persons[0].x >= 25.0f)
            {
                currentScene = sceneFour;
                persons[0].x = 0.0;
                ball.x = 3.0f;
            }
            persons[0].x += 0.1f;
            persons[0].y = 15.0f;
            ball.x += 0.09;
            ball.y = abs(2.0f * sinf(ball.x)) + 16.0f;
            break;
        case sceneFour:
            if (persons[0].x >= 25.0f)
            {
                currentScene = sceneFive;
                persons[0].x = -5.0f;
                ball.x = 18.0f;
            }
            amplitude += amplitudeChange;

            if (amplitude >= 0.7f) {
                amplitudeChange = -0.01f;
            } else if (amplitude <= -0.7f) {
                amplitudeChange = 0.01f;
            }
            ball.x += 0.09;
            persons[0].x += 0.1;
            persons[0].y = amplitude * sinf(persons[0].x) + 10.0f;
            ball.y = (1.5f + amplitude) * sinf(ball.x) + 10.0f;
            slope = cosf(persons[0].x); // slope of the tangent line at the current point
            angle2 = atanf(slope); // angle to the horizontal
            break;
        case sceneFive:
            if (persons[0].x >= 10.0f && raise <= 3.5f) {
                raise += 0.1;
            }

            if (persons[0].x < 15.0f) {
                persons[0].x += 0.07;
            }

            persons[0].y = 1.0f;
            ball.y = 2.0f;
            break;
    }

    glutPostRedisplay();
    glutTimerFunc(0, update, 0);
}



void display() {
    glClearColor(0.435f, 0.988f, 0.949f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 20, 0, 20, -1, 1);  // Set maximum x and y values to -10 and 10
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawScene();

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    glutCreateWindow("Pong");
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}

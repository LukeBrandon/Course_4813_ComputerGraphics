//---------------------------------------
// Program: graphs.cpp
// Purpose: Draw Excel like graphs based on data
// Author:  Luke Brandon
// Date:    January 2020
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

float brickX = 0.0;
float brickY = 0.0;
float brickZ = 0.0;
float brickXVelocity = 0.1;
float brickYVelocity = 0.1;
int brickXRotation = 20;
int brickYRotation = 20;
float clickStartX;
float clickStartY;
bool gravityEnabled = true;

const int WINDOW_DIMENSION = 500;
const int SCREEN_FPS = 30;
const float BRICK_DIMENSION = 0.2;
const int ROTATIONAL_GRAVITY = 5;
const float GRAVITY = 0.01;


// Functions to convert to object coords
float toObjX(int x){
    return (((float)x-250)/500)*2;
}

float toObjY(int y){
    return (-((float)y-250)/500)*2;
}

// Draws the brick
void drawBrick(){
    float midx = brickX;
    float midy = brickY;
    float midz = brickZ;

    float ax = midx - BRICK_DIMENSION / 2;
    float ay = midy - BRICK_DIMENSION / 2;
    float az = midz + BRICK_DIMENSION / 2;
    float bx = midx + BRICK_DIMENSION / 2;
    float by = midy - BRICK_DIMENSION / 2;
    float bz = midz + BRICK_DIMENSION / 2;
    float cx = midx + BRICK_DIMENSION / 2;
    float cy = midy + BRICK_DIMENSION / 2;
    float cz = midz + BRICK_DIMENSION / 2;
    float dx = midx - BRICK_DIMENSION / 2;
    float dy = midy + BRICK_DIMENSION / 2;
    float dz = midz + BRICK_DIMENSION / 2;
    float ex = midx - BRICK_DIMENSION / 2;
    float ey = midy - BRICK_DIMENSION / 2;
    float ez = midz - BRICK_DIMENSION / 2;
    float fx = midx + BRICK_DIMENSION / 2;
    float fy = midy - BRICK_DIMENSION / 2;
    float fz = midz - BRICK_DIMENSION / 2;
    float gx = midx + BRICK_DIMENSION / 2;
    float gy = midy + BRICK_DIMENSION / 2;
    float gz = midz - BRICK_DIMENSION / 2;
    float hx = midx - BRICK_DIMENSION / 2;
    float hy = midy + BRICK_DIMENSION / 2;
    float hz = midz - BRICK_DIMENSION / 2;

    glEnable(GL_CULL_FACE);

     // Rotate the brick
    glPushMatrix();
    glRotatef(brickXRotation, brickX, brickY, brickZ);
    glRotatef(brickYRotation, brickX, brickY, brickZ);

    // Draw 6 faces
    glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(ax, ay, az);
        glVertex3f(bx, by, bz);
        glVertex3f(cx, cy, cz);
        glVertex3f(dx, dy, dz);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(ax, ay, az);
        glVertex3f(dx, dy, dz);
        glVertex3f(hx, hy, hz);
        glVertex3f(ex, ey, ez);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(ax, ay, az);
        glVertex3f(ex, ey, ez);
        glVertex3f(fx, fy, fz);
        glVertex3f(bx, by, bz);

        glColor3f(0.0, 1.0, 1.0);
        glVertex3f(gx, gy, gz);
        glVertex3f(fx, fy, fz);
        glVertex3f(ex, ey, ez);
        glVertex3f(hx, hy, hz);
        
        glColor3f(1.0, 0.0, 1.0);
        glVertex3f(gx, gy, gz);
        glVertex3f(cx, cy, cz);
        glVertex3f(bx, by, bz);
        glVertex3f(fx, fy, fz);

        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(gx, gy, gz);
        glVertex3f(hx, hy, hz);
        glVertex3f(dx, dy, dz);
        glVertex3f(cx, cy, cz);
    glEnd();
    glPopMatrix();

    // Rotate the brick
    // glPushMatrix();
    // glRotatef(brickXRotation, brickX, brickY, brickZ);
    // glRotatef(brickYRotation, brickX, brickY, brickZ);

    // glBegin(GL_QUADS);
    //     // Draw the front
    //     glColor3f(0, 1, 0);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION, 0);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION, 0);

    //     // Draw the top
    //     glColor3f(1, 0, 0);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION, BRICK_DIMENSION);

    //     // Draw the left
    //     glColor3f(1, 1, 0);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION, 0);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION, 0);

    //     // Draw the right
    //     glColor3f(0, 1, 1);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION, BRICK_DIMENSION);

    //     // Draw the bottom
    //     glColor3f(1, 0, 1);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION, 0);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION, 0);

    //     // Draw the back
    //     glColor3f(0, 0, 1);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION, BRICK_DIMENSION);
    //     glVertex3f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION, BRICK_DIMENSION);

    // glEnd();
    // glPopMatrix();
}

// Updates the location of the brick
void updateBrick(){
    if(gravityEnabled){
        brickYVelocity -= GRAVITY;  // y velocity is updated due to gravity
        brickXRotation += ROTATIONAL_GRAVITY;
        brickYRotation += ROTATIONAL_GRAVITY;
    }

    // Bounce off the right wall
    if(brickX + BRICK_DIMENSION >= 1.0){
        brickXVelocity *= -.8;
        brickX = 1.0 - BRICK_DIMENSION;
    }

    // Bounce off of the left wall
    if(brickX - BRICK_DIMENSION <= -1.0){
        brickXVelocity *= -.8;
        brickX = -1.0 + BRICK_DIMENSION;
    }

    // Bounce off of the bottom
    if(brickY - BRICK_DIMENSION <= -1.0){
        brickYVelocity *= -.7;
        brickY = -1.0 + BRICK_DIMENSION;
    }

    // Bounce off of the top
    if(brickY + BRICK_DIMENSION >= 1.0){
        brickYVelocity *= -1.0;
        brickY = 1.0 - BRICK_DIMENSION;
    }
    
    // Update the location of the brick based on it's velocities
    brickX += brickXVelocity;
    brickY += brickYVelocity;
    brickZ = 0.0;
}

// Display callback for OpenGL
void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    updateBrick();
    drawBrick();
   
    glFlush();
}

// Idle callback for OpenGL
void idle() {
    // Program is idle
}

// Control the Frames Per Second (FPS)
void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000 / SCREEN_FPS, timer, 0);
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y){
    if(key == 'w')
        cout << "move up" << endl;
    if(key == 'a')
        cout << "move left" << endl;
    if(key == 's')
        cout << "move down" << endl;
    if(key == 'd')
        cout << "move right" << endl;

    // Redraw the screen
    glutPostRedisplay();
}

// Handle mouse input
void mouse(int button, int state, int x, int y){

    // Convert to object coordinates
    float objX = toObjX(x);
    float objY = toObjY(y);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        gravityEnabled = false;  // Disable gravity while user holds clicked

        // Draw brick where user clicks
        brickX = objX;
        brickY = objY;

        // Remember where drag started
        clickStartX = objX;
        clickStartY = objY;

        // Set velocity to 0
        brickXVelocity = 0;
        brickYVelocity = 0;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        gravityEnabled = true;// Re-enable gravity when click released

        // Draw the brick where the user lifts up
        brickX = objX;
        brickY = objY;

        // Calculate velocity based on how far pulled back
        brickXVelocity = (clickStartX - brickX) / 3;
        brickYVelocity = (clickStartY - brickY) / 3;
    }
}

// Handle mouse motion
// Draw the brick as the user drags the mouse
void mouseMotion(int x, int y){
    brickX = toObjX(x);
    brickY = toObjY(y);
}

// Init function for OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main program
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode (GLUT_DEPTH | GLUT_SINGLE| GLUT_RGB);
    glutCreateWindow("AngryBrick");

    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(1000 / SCREEN_FPS, timer, 0);

    init();
    glutMainLoop();
    return 0;
}
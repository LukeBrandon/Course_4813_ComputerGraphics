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

int brickX = 250;
int brickY = 250;
int brickXVelocity = 5;
int brickYVelocity = 15;
int clickStartX;
int clickStartY;
bool gravityEnabled = true;

const int WINDOW_DIMENSION = 500;
const int SCREEN_FPS = 30;
const int BRICK_DIMENSION = 20;
const int GRAVITY = 1;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init() {
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 500, 0, 500, -1.0, 1.0);
}

void setColor(vector<int> data){
   glColor3f(data[0], data[1], data[2]);
}

// Draws the brick
void drawBrick(){
    glBegin(GL_POLYGON);
        glVertex2f(brickX - BRICK_DIMENSION, brickY - BRICK_DIMENSION);
        glVertex2f(brickX + BRICK_DIMENSION, brickY - BRICK_DIMENSION);
        glVertex2f(brickX - BRICK_DIMENSION, brickY + BRICK_DIMENSION);
        glVertex2f(brickX + BRICK_DIMENSION, brickY + BRICK_DIMENSION);
    glEnd();
}

// Updates the location of the brick
void updateBrick(){
    if(gravityEnabled)
        brickYVelocity -= GRAVITY;  // y velocity is updated due to gravity

    // Bounce off the side walls
    if(brickX >= WINDOW_DIMENSION){
        brickXVelocity *= -.8;
        brickX = WINDOW_DIMENSION-BRICK_DIMENSION-5;
    }
    if(brickX <= 0){
        brickXVelocity *= -.8;
        brickX = BRICK_DIMENSION;
    }
    if(brickY >= WINDOW_DIMENSION){
        brickYVelocity *= -.7;
        brickY = WINDOW_DIMENSION - BRICK_DIMENSION;
    }
    if(brickY <= 0){
        brickYVelocity *= -.7;
        brickY = BRICK_DIMENSION;
    }
    
    // Update the location of the brick based on it's velocities
    brickX += brickXVelocity;
    brickY += brickYVelocity;
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

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        gravityEnabled = false;  // Disable gravity while user holds clicked

        // Draw brick where user clicks
        brickX = x;
        brickY = WINDOW_DIMENSION - y;

        // Remember where drag started
        clickStartX = x;
        clickStartY = WINDOW_DIMENSION - y;

        // Set velocity to 0
        brickXVelocity = 0;
        brickYVelocity = 0;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        gravityEnabled = true;// Re-enable gravity when click released

        // Draw the brick where the user lifts up
        brickX = x;
        brickY = WINDOW_DIMENSION - y;

        // Calculate velocity based on how far pulled back
        brickXVelocity = (clickStartX - brickX) / 5;
        brickYVelocity = (clickStartY - brickY) / 5;
    }
}

// Handle mouse motion
// Draw the brick as the user drags the mouse
void mouseMotion(int x, int y){
    brickX = x;
    brickY = WINDOW_DIMENSION - y;
}

// Main program
int main(int argc, char *argv[]) {
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode (GLUT_DEPTH | GLUT_SINGLE| GLUT_RGB);
   glutCreateWindow("Square");

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
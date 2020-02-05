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
    // y velocity is updated due to gravity
    brickYVelocity -= GRAVITY;

    // Bounce off the side walls
    if(brickX >= WINDOW_DIMENSION){
        brickXVelocity *= -1;
        brickX = WINDOW_DIMENSION;
    }
    if(brickX <= 0){
        brickXVelocity *= -1;
        brickX = 0;
    }
    if(brickY >= WINDOW_DIMENSION || brickY <= 0){
        brickYVelocity *= -.8;
        brickY = 0;
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

// Controls the time between screen redrawing
void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000/SCREEN_FPS, timer, 0);
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

    // Save the left button state
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        cout << "Clicked at: " << x << "/" << y << endl;
        brickX, clickStartX = x;
        brickY, clickStartY = WINDOW_DIMENSION - y;
        brickXVelocity = 0;
        brickYVelocity = 0;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        brickX = x;
        brickY = WINDOW_DIMENSION - y;
        brickXVelocity = (clickStartX - brickX) / 10;
        brickYVelocity = (clickStartY - brickY) / 10;
    }
}

// Handle mouse motion
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
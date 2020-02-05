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
int brickYVelocity = 10;
const int BRICK_DIMENSION = 50;
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
    brickYVelocity -= GRAVITY;

    brickX += brickXVelocity;
    brickY += brickYVelocity;

    cout << "brickX: " << brickX << " brickY: " << brickY << " yvelocity: " << brickYVelocity << endl;
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    updateBrick();
    drawBrick();
   
    glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Square");
   glutDisplayFunc(draw);
   init();
   glutMainLoop();
   return 0;
}
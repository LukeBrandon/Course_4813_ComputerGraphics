#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <random>
#include <stdio.h>
#include "libraries/libim/im_color.h"
#include "libraries/shading.cpp"

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const int NUM_CUBES = 20;
const int NUM_TEXTURES = 20;

// Used to hold data of 5 different cubes
float cubeX[NUM_CUBES];
float cubeY[NUM_CUBES];
float cubeYVelocity[NUM_CUBES];
float cubeZ[NUM_CUBES];

// Random looking starting rotation values
int cubeRotateX[NUM_CUBES];
int cubeRotateY[NUM_CUBES];
int cubeRotateZ[NUM_CUBES];

float cubeRadius[NUM_CUBES];

int cubeTextureNumber[NUM_CUBES];

const int rotateScaler = 3;
const float GRAVITY = 0.001;
const int SCREEN_FPS = 30;
const int imageDimension = 512;

// Texture variables
unsigned char textures[NUM_TEXTURES][imageDimension * imageDimension * 3];

// Used for generating random numbers
std::default_random_engine generator;
std::uniform_real_distribution<double> xDistribution(-1,1); //doubles from -1 to 1
std::uniform_real_distribution<double> yDistribution(1.1,5.0);
std::uniform_real_distribution<double> radiusDistribution(0.1,0.3); 
std::uniform_real_distribution<double> rotationDistribution(0,90); 
std::uniform_real_distribution<double> textureDistribution(0,NUM_TEXTURES-1); 

// Read color and depth images
void read_images(string color_name, im_color &color_image, int numTexture) {

    // Read color 
    color_image.ReadJpg(color_name.c_str());

    // Read in the color values
    int i = 0;
    for (int y = 0; y < imageDimension; ++y)
        for (int x = 0; x < imageDimension; ++x) {
            textures[numTexture][i++] = color_image.R.Data2D[y][x];
            textures[numTexture][i++] = color_image.G.Data2D[y][x];
            textures[numTexture][i++] = color_image.B.Data2D[y][x];
        }
}

// Draws the cube with image on it
void drawCube(int i) {

    float midx = cubeX[i];
    float midy = cubeY[i];
    float midz = cubeZ[i];
    float thisRadius = cubeRadius[i];

    float ax = midx - thisRadius / 2;
    float ay = midy - thisRadius / 2;
    float az = midz + thisRadius / 2;
    float bx = midx + thisRadius / 2;
    float by = midy - thisRadius / 2;
    float bz = midz + thisRadius / 2;
    float cx = midx + thisRadius / 2;
    float cy = midy + thisRadius / 2;
    float cz = midz + thisRadius / 2;
    float dx = midx - thisRadius / 2;
    float dy = midy + thisRadius / 2;
    float dz = midz + thisRadius / 2;
    float ex = midx - thisRadius / 2;
    float ey = midy - thisRadius / 2;
    float ez = midz - thisRadius / 2;
    float fx = midx + thisRadius / 2;
    float fy = midy - thisRadius / 2;
    float fz = midz - thisRadius / 2;
    float gx = midx + thisRadius / 2;
    float gy = midy + thisRadius / 2;
    float gz = midz - thisRadius / 2;
    float hx = midx - thisRadius / 2;
    float hy = midy + thisRadius / 2;
    float hz = midz - thisRadius / 2;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageDimension, imageDimension, 0, GL_RGB, GL_UNSIGNED_BYTE, textures[cubeTextureNumber[i]]);

    // Rotate the cube
    glPushMatrix();
        glRotatef(cubeRotateX[i], cubeX[i], cubeY[i], cubeZ[i]);
        glRotatef(cubeRotateY[i], cubeX[i], cubeY[i], cubeZ[i]);
        glRotatef(cubeRotateZ[i], cubeX[i], cubeY[i], cubeZ[i]);

    // Draw 6 faces
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3f(ax, ay, az);
        glTexCoord2f(0.0,1.0);
        glVertex3f(bx, by, bz);
        glTexCoord2f(1.0,1.0);
        glVertex3f(cx, cy, cz);
        glTexCoord2f(1.0,0.0);
        glVertex3f(dx, dy, dz);

        glTexCoord2f(0.0,0.0);
        glVertex3f(ax, ay, az);
        glTexCoord2f(0.0,1.0);
        glVertex3f(dx, dy, dz);
        glTexCoord2f(1.0,1.0);
        glVertex3f(hx, hy, hz);
        glTexCoord2f(1.0,0.0);
        glVertex3f(ex, ey, ez);

        glTexCoord2f(0.0,0.0);
        glVertex3f(ax, ay, az);
        glTexCoord2f(0.0,1.0);
        glVertex3f(ex, ey, ez);
        glTexCoord2f(1.0,1.0);
        glVertex3f(fx, fy, fz);
        glTexCoord2f(1.0,0.0);
        glVertex3f(bx, by, bz);

        glTexCoord2f(0.0,0.0);
        glVertex3f(gx, gy, gz);
        glTexCoord2f(0.0,1.0);
        glVertex3f(fx, fy, fz);
        glTexCoord2f(1.0,1.0);
        glVertex3f(ex, ey, ez);
        glTexCoord2f(1.0,0.0);
        glVertex3f(hx, hy, hz);
        
        glTexCoord2f(0.0,0.0);
        glVertex3f(gx, gy, gz);
        glTexCoord2f(0.0,1.0);
        glVertex3f(cx, cy, cz);
        glTexCoord2f(1.0,1.0);
        glVertex3f(bx, by, bz);
        glTexCoord2f(1.0,0.0);
        glVertex3f(fx, fy, fz);

        glTexCoord2f(0.0,0.0);
        glVertex3f(gx, gy, gz);
        glTexCoord2f(0.0,1.0);
        glVertex3f(hx, hy, hz);
        glTexCoord2f(1.0,1.0);
        glVertex3f(dx, dy, dz);
        glTexCoord2f(1.0,0.0);
        glVertex3f(cx, cy, cz);
    glEnd();
    glPopMatrix();
}

void updateCube(int i){
    cubeYVelocity[i] -= GRAVITY;
    cubeY[i] += cubeYVelocity[i];

    // Rotate the cube
    cubeRotateX[i] += rotateScaler;
    cubeRotateY[i] += rotateScaler;
    cubeRotateZ[i] += rotateScaler;

    if(cubeY[i] < -1.1){
        cubeX[i] = xDistribution(generator);
        cubeY[i] = yDistribution(generator);
        cubeYVelocity[i] = 0.0;
        cubeTextureNumber[i] = textureDistribution(generator);
    }
}

// Display callback for OpenGL
void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw all the cubes
    for(int i = 0 ; i < NUM_CUBES; i ++){
        updateCube(i);
        drawCube(i);
    }
    glFlush();
}

// Control the Frames Per Second (FPS)
void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000 / SCREEN_FPS, timer, 0);
}

void initCubeData(){
    // Used to hold data of NUM_CUBES different cubes
    for(int i = 0; i < NUM_CUBES; ++i){
        cubeRadius[i] = radiusDistribution(generator);
        cubeX[i] = xDistribution(generator);;
        cubeY[i] = yDistribution(generator);
        cubeYVelocity[i] = 0.0;
        cubeZ[i] = 0.0;
        cubeTextureNumber[i] = textureDistribution(generator);

        cubeRotateX[i] = rotationDistribution(generator);
        cubeRotateY[i] = rotationDistribution(generator);
        cubeRotateZ[i] = rotationDistribution(generator);
    }
}

void initTextureData(){
    im_color image;
    string textureSrcs[20];

    // Loads all the strings for the images
    for(int i = 0; i < 20; i++){
        // Cats
        if(i < 10){
            textureSrcs[i] = "./assets/cat" + to_string(i) + ".jpg";
        } else {
        //Dogs
            textureSrcs[i] = "./assets/dog" + to_string(i%10) + ".jpg";
        }
    }

    // Reads all the images into textures
    for(int i = 0; i < NUM_TEXTURES; ++i){
        read_images(textureSrcs[i], image, i);
    }
}

int main(int argc, char *argv[])
{

    initCubeData();
    initTextureData();

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("It's Raining Cats and Dogs");

    glutDisplayFunc(draw);
    glutTimerFunc(1000 / SCREEN_FPS, timer, 0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // Texture mapping initialization
    glEnable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Tells OpenGL to cull the back faces, front faces have counter clockwise winding
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glutMainLoop();

    return 0;
}

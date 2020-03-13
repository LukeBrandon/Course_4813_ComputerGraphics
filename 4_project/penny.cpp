#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <tuple>
#include <stdio.h>
#include "libraries/libim/im_color.h"
#include "libraries/shading.cpp"

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const int PENNY_IMAGE_WIDTH = 500;
const int PENNY_IMAGE_HEIGHT = 500;
const int PPL = 2;               // Pixels per line
const int PENNY_DEPTH_SCALER = -9; // Larger = less amplitude

typedef tuple<float, float, float> RGB;
typedef tuple<float, float, float> vector;

//X, Y, RGB, Normal Vector
tuple<float, float, RGB, vector> pennyImageData[PENNY_IMAGE_WIDTH / PPL][PENNY_IMAGE_HEIGHT / PPL];

//X, Y, Z
vector pennyDepthData[PENNY_IMAGE_WIDTH / PPL][PENNY_IMAGE_HEIGHT / PPL];

int rotateX = -160, rotateY = 30, rotateZ = 200;
int rotateScaler = 20;

enum modes
{
    Depth,
    Color,
    PhongShading
};

int displayMode = 0;

// Functions to convert to object coords
float toObjX(int x)
{
    return (((float)x - 250) / 500) * 2;
}
float toObjY(int y)
{
    return (-((float)y - 250) / 500) * 2;
}
float toObjZ(int z)
{
    return ((-((float)z - 250) / 500) * 2) / (float)PENNY_DEPTH_SCALER;
}

vector getNormalVector(vector a, vector b, vector c, vector d){
    // (xb-xa), (yb-ya), (zb-za)
    // (xd-xa), (yd-ya), (zd-za)

    vector v1 = {toObjX(get<0>(b) - get<0>(a)), toObjY(get<1>(b) - get<1>(a)), toObjZ(get<2>(b) - get<2>(a))};
    vector v2 = {toObjX(get<0>(d) - get<0>(a)), toObjY(get<1>(d) - get<1>(a)), toObjZ(get<2>(d) - get<2>(a))};

    vector crossProduct = {
        get<1>(v1) * get<2>(v2) - get<2>(v1) * get<1>(v2),
        get<2>(v1) * get<0>(v2) - get<0>(v1) * get<2>(v2),
        get<0>(v1) * get<1>(v2) - get<1>(v1) * get<0>(v2)
    };

    // Normalize vector 
    float amplitude = sqrt(get<0>(crossProduct)*get<0>(crossProduct) + get<1>(crossProduct)*get<1>(crossProduct));

    vector normalizedVector = {
        get<0>(crossProduct)/amplitude,
        get<1>(crossProduct)/amplitude,
        get<2>(crossProduct)/amplitude,
    };

    return normalizedVector;
}

// Read color and depth images
void read_images(string color_name, im_color &color_image,
                 string depth_name, im_float &depth_image)
{

    // Read depth image
    depth_image.ReadJpg(depth_name.c_str());

    // Get image dimensions
    int xdim = depth_image.Xdim;
    int ydim = depth_image.Ydim;

    // Read in the depth values
    for (int y = 0; y < ydim; y += PPL)
    {
        for (int x = 0; x < xdim; x += PPL)
        {
            pennyDepthData[x / PPL][y / PPL] = make_tuple(x, y, depth_image.Data2D[y][x]);
        }
    }


    // Read color image
    color_image.ReadJpg(color_name.c_str());

    // Get image dimensions
    xdim = color_image.R.Xdim;
    ydim = color_image.R.Ydim;

    // Read in the color values
    for (int y = 0; y < ydim; y = y + PPL)
        for (int x = 0; x < xdim; x = x + PPL)
        {
            RGB thisRGB = make_tuple(color_image.R.Data2D[y][x], color_image.G.Data2D[y][x], color_image.B.Data2D[y][x]);
            vector nullVector = {(float)0.0, (float)0.0, (float)0.0};
            pennyImageData[x / PPL][y / PPL] = make_tuple(x, y, thisRGB, nullVector);
        };


    // Calculate the normals of each vertex
    for (int y = 0; y < ydim - PPL; y = y + PPL)
        for (int x = 0; x < xdim - PPL; x = x + PPL)
        {
            int relativeX = x/PPL;
            int relativeY = y/PPL;
            get<3>(pennyImageData[x / PPL][y / PPL]) = getNormalVector(pennyDepthData[relativeX][relativeY], pennyDepthData[relativeX+1][relativeY], pennyDepthData[relativeX+1][relativeY+1], pennyDepthData[relativeX][relativeY+1]);
        };

}

void initPhong(){
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    init_light(GL_LIGHT0, 0, 0, -1, 1, 1, 1);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ('X'):
        rotateX += rotateScaler;
        break;
    case ('x'):
        rotateX -= rotateScaler;
        break;
    case ('Y'):
        rotateY += rotateScaler;
        break;
    case ('y'):
        rotateY -= rotateScaler;
        break;
    case ('Z'):
        rotateZ += rotateScaler;
        break;
    case ('z'):
        rotateZ -= rotateScaler;
        break;
    case ('1'):
        printf("Entering Depth Mode\n");
        displayMode= Depth;
        break;
    case ('2'):
        printf("Entering Color Mode\n");
        displayMode= Color;
        break;
    case ('3'):
        printf("Entering Phong Shading Mode\n");
        initPhong();
        displayMode= PhongShading;
        break;
    default:
        printf("INVALID INPUT\n");
        break;
    }

    glutPostRedisplay();
}

RGB getAverageColor(tuple<float, float, RGB, vector> a, tuple<float, float, RGB, vector> b, tuple<float, float, RGB, vector> c, tuple<float, float, RGB, vector> d){
    int averageR = (get<0>(get<2>(a)) + get<0>(get<2>(b)) + get<0>(get<2>(c)) + get<0>(get<2>(d)))/4;
    int averageG = (get<1>(get<2>(a)) + get<1>(get<2>(b)) + get<1>(get<2>(c)) + get<1>(get<2>(d)))/4;
    int averageB = (get<2>(get<2>(a)) + get<2>(get<2>(b)) + get<2>(get<2>(c)) + get<2>(get<2>(d)))/4;

    RGB average = make_tuple(averageR, averageG, averageB);

    return average;
}

// Draws the penny depth map
void drawPennyDepth()
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    // Draw the Penny
    glBegin(GL_LINE_STRIP);
    for (int x = 0; x < PENNY_IMAGE_WIDTH; x += PPL)
    {
        for (int y = 0; y < PENNY_IMAGE_HEIGHT; y += PPL)
        {
            vector tempDepthPenny = pennyDepthData[x / PPL][y / PPL];
            float xCoord = toObjX(get<0>(tempDepthPenny));
            float yCoord = toObjY(get<1>(tempDepthPenny));
            float zCoord = toObjZ(get<2>(tempDepthPenny));

            glVertex3f(xCoord, yCoord, zCoord);
        }
    }
    glEnd();
}

// Draws the penny in color
void drawPennyColor()
{
    glDisable(GL_LIGHTING);

    for (int x = 0; x < PENNY_IMAGE_WIDTH - PPL; x += PPL)
    {
        for (int y = 0; y < PENNY_IMAGE_HEIGHT - PPL; y += PPL)
        {
            tuple<float, float, RGB, vector> thisPoint = pennyImageData[x/PPL][y/PPL];
            RGB thisPointColor = get<2>(thisPoint);
            // Gets the average color of the polygon from average of the color of each point its composed of
            RGB averageColor = getAverageColor(thisPoint, pennyImageData[(x/PPL) + 1][y/PPL], pennyImageData[(x / PPL)+1][(y / PPL)+1], pennyImageData[x / PPL][(y / PPL)+1]);
            
            // Set the color by dividing by 255 to map 0-255 to 0.0-1.0
            glColor3f(get<0>(averageColor)/255, get<1>(averageColor)/255, get<2>(averageColor)/255);

            if(get<0>(thisPointColor) != 255.0 || get<1>(thisPointColor) != 255.0 || get<2>(thisPointColor) != 255.0){
                // Creates the vetexes of all of the points of the polygon that will be associated with this step in the model
                glBegin(GL_POLYGON);                
                    glVertex3f(toObjX(get<0>(pennyDepthData[x/PPL][y/PPL])), toObjY(get<1>(pennyDepthData[x/PPL][y/PPL])), toObjZ(get<2>(pennyDepthData[x/PPL][y/PPL])));
                    glVertex3f(toObjX(get<0>(pennyDepthData[(x/PPL)+1][y/PPL])), toObjY(get<1>(pennyDepthData[(x/PPL)+1][y/PPL])), toObjZ(get<2>(pennyDepthData[(x/PPL)+1][y/PPL])));
                    glVertex3f(toObjX(get<0>(pennyDepthData[(x/PPL)+1][(y/PPL)+1])), toObjY(get<1>(pennyDepthData[(x/PPL)+1][(y/PPL)+1])), toObjZ(get<2>(pennyDepthData[(x/PPL)+1][(y/PPL)+1])));
                    glVertex3f(toObjX(get<0>(pennyDepthData[x/PPL][(y/PPL)+1])), toObjY(get<1>(pennyDepthData[x/PPL][(y/PPL)+1])), toObjZ(get<2>(pennyDepthData[x/PPL][(y/PPL)+1])));
                glEnd();
            }

            
        }
    }
}

// Draws the penny using phong shading
void drawPennyPhongShading()
{
    init_material(Ka, Kd, Ks, 100 * Kp, 0.67, 0.43, 0.41);

    for (int x = 0; x < PENNY_IMAGE_WIDTH - PPL; x += PPL)
    {
        for (int y = 0; y < PENNY_IMAGE_HEIGHT - PPL; y += PPL)
        {
            int relativeX = x/PPL;
            int relativeY = y/PPL;
            tuple<float, float, RGB, vector> thisPoint = pennyImageData[relativeX][relativeY];
            RGB thisPointColor = get<2>(thisPoint);
            vector thisNormal = get<3>(thisPoint);

            glNormal3f(get<0>(thisNormal), get<1>(thisNormal) , get<2>(thisNormal));

            if(get<0>(thisPointColor) <= 247.0 || get<1>(thisPointColor) <= 247.0 || get<2>(thisPointColor) <= 247.0){
                // Creates the vetexes of all of the points of the polygon that will be associated with this step in the model
                glBegin(GL_POLYGON);                
                    glVertex3f(toObjX(get<0>(pennyDepthData[x/PPL][y/PPL])), toObjY(get<1>(pennyDepthData[x/PPL][y/PPL])), toObjZ(get<2>(pennyDepthData[x/PPL][y/PPL])));
                    glVertex3f(toObjX(get<0>(pennyDepthData[(x/PPL)+1][y/PPL])), toObjY(get<1>(pennyDepthData[(x/PPL)+1][y/PPL])), toObjZ(get<2>(pennyDepthData[(x/PPL)+1][y/PPL])));
                    glVertex3f(toObjX(get<0>(pennyDepthData[(x/PPL)+1][(y/PPL)+1])), toObjY(get<1>(pennyDepthData[(x/PPL)+1][(y/PPL)+1])), toObjZ(get<2>(pennyDepthData[(x/PPL)+1][(y/PPL)+1])));
                    glVertex3f(toObjX(get<0>(pennyDepthData[x/PPL][(y/PPL)+1])), toObjY(get<1>(pennyDepthData[x/PPL][(y/PPL)+1])), toObjZ(get<2>(pennyDepthData[x/PPL][(y/PPL)+1])));
                glEnd();
            }

        }
    }
}

// Display callback for OpenGL
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Rotate the penny
    glPushMatrix();
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, 1.0);

    switch (displayMode)
    {
    case (Depth):
        drawPennyDepth();
        break;
    case (Color):
        drawPennyColor();
        break;
    case (PhongShading):
        drawPennyPhongShading();
        break;
    default:
        printf("INVALID DRAWING MODE");
    }

    glPopMatrix();
    glFlush();
}

// Init function for OpenGL
void init()
{
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Penny PP");

    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Tells OpenGL to cull the back faces, front faces have counter clockwise winding
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

int main(int argc, char *argv[])
{
    std::cout << "Press x,y,z to rotate the model and X,Y,Z to go the reverse direction in each axis." << endl;
    std::cout << "Use the number keys 1, 2, and 3 to change between display modes." << endl;

    im_color color;
    im_float depth;
    read_images("./assets/penny-image.jpg", color,
                "./assets/penny-depth.jpg", depth);

    glutInit(&argc, argv);
    init();
    glutMainLoop();

    return 0;
}

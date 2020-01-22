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

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

const int AXIS_WIDTH = 50;
const int AXIS_HEIGHT = 50;
const int DRAWABLE_WIDTH = 450;
const int DRAWABLE_HEIGHT = 450;


//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init() {
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 500, 0, 500, -1.0, 1.0);
}

void drawRectangle(int x, int y, int x2, int y2 ,int x3, int y3 ,int x4, int y4){
   glBegin(GL_POLYGON);
   glColor3f(1.0, 1.0, 1.0);
   glVertex2f(x, y);
   glVertex2f(x2, y2);
   glVertex2f(x3, y3);
   glVertex2f(x4, y4);
   glEnd();
}

void drawLine(int x1, int y1, int x2, int y2){
   glBegin(GL_LINES);
      glColor3f(1.0, 1.0, 1.0);
      glVertex2f(x1,y1);
      glVertex2f(x2,y2);
   glEnd();
}

// Draws the graph's acxies and lines
void drawGraph(){

   drawLine(50,500,50,50);
   drawLine(50,50,500,50);

   // Draw vertical ticks
   int yOffset = DRAWABLE_HEIGHT/10;
   int counter = 1;
   while(counter <= 10){
      int y =  50 + (counter*yOffset);
      drawLine(45, y, 500, y);

      counter++;
   }

   // Draw horizontal ticks
   int xOffset = DRAWABLE_HEIGHT/(10 + 1);
   int cnt = 1;
   while(cnt <= 10){
      int x =  50 + (cnt*xOffset);
      drawLine(x, 50, x, 45);

      cnt++;
   }


}

// Draws the data 
void drawData(){
   // Read in command from file

   // Draw the commands

}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void draw() {
   glClear(GL_COLOR_BUFFER_BIT);

   drawGraph();
   drawData();

   glFlush();
}


// Returns true if the character is a a character and false if anything else
bool isCharacter(char character){
   if((character <= 97 && character <= 122) || (character >= 65 && character <= 90)){
      return true;
   }
   return false;
}

bool isNumber(char character){
   if(character <= 48 && character <= 57){
      return true;
   }
   return false;
}

void parseCommand(string command){
   vector<string> parsedCommand;
   string tempString;

   for(uint i = 0; i < command.length(); i++){

      // Adds current character to the tempString
      if(command.at(i) != ' '){
         tempString += command.at(i);

      // If the previous word ends in a character
      } else if(isCharacter(command.at(i))){
         cout << "adding string to parsed command" << endl;
         parsedCommand.push_back(tempString);

      // If the previous word ends in a number
      } else if(isNumber(command.at(i))){
         cout << "adding number to parsed command" << endl;

      }

   }

   parsedCommand.push_back("draw_point");

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
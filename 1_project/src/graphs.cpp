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

const int AXIS_WIDTH = 50;
const int AXIS_HEIGHT = 50;
const int DRAWABLE_WIDTH = 400;
const int DRAWABLE_HEIGHT = 400;
const int LINE_WIDTH = 50;
const int POINT_WIDTH = 50;


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


// Draws a point at position x, y with size of POINT_WIDTH
void drawPoint(vector<int> coords){
   glPointSize(coords[0]);
   glBegin(GL_POINTS);
      glVertex2f(coords[1], coords[2]);
   glEnd();
}

// Draws a line from x1, y1 to x2, y2 with width of LINE_WIDTH
void drawLine(vector<int> coords){
   glLineWidth(coords[0]);
   glBegin(GL_LINES);
      glVertex2f(coords[1],coords[2]);
      glVertex2f(coords[3],coords[4]);
   glEnd();
}

// Draws a polygon with the inputted verticies
void drawPolygon(vector<int> coords){
   glBegin(GL_POLYGON);
      for(uint i = 0; i < coords.size(); i+=2){
         glVertex2f(coords[i], coords[i+1]);
      }
   glEnd();
}

// Draws the graph's acxies and lines
void drawGraph(){
   // Set color to white
   setColor({1, 1, 1});
   int axisLineWidth = 1;

   // Draw the axies
   drawLine({axisLineWidth, 50,50,50,450});
   drawLine({axisLineWidth, 50,50,450,50});

   // Draw vertical ticks
   int yOffset = DRAWABLE_HEIGHT/10;
   int counter = 0;
   while(counter <= 10){
      int y =  50 + (counter*yOffset);
      drawLine({axisLineWidth, 45, y, 450, y});

      counter++;
   }

   // Draw horizontal ticks
   int xOffset = DRAWABLE_HEIGHT/(10 + 1);
   int cnt = 0;
   while(cnt <= 10){
      int x =  50 + (cnt*xOffset);
      drawLine({axisLineWidth, x, 50, x, 45});

      cnt++;
   }

}

// Parses the command from string back into a pair of string and vector of ints
pair<string, vector<int>> parseCommand(string command){
   pair<string, vector<int>> parsedCommand = {"", {}};
   string tempString;

   for(uint i = 0; i < command.length(); i++){

      // If the current character is a space or this is the last char in command
      if(command.at(i) == ' ' || i == command.length()-1){

         // If the previous word ends in a character
         if(isalpha(command.at(i-1))){
            parsedCommand.first = tempString;
         }

         // If the previous word ends in a number
         if(isdigit(command.at(i-1))){
            parsedCommand.second.push_back(atoi(tempString.c_str()));
         }

      // Reset the tempString
      tempString="";

      // Otherwise add the character to the tempString
      } else {
         tempString += command.at(i);
      }

   }

   // // Printing out parsed command
   // cout << parsedCommand.first;
   // for(uint i = 0; i < parsedCommand.second.size(); i++){
   //    cout << parsedCommand.second[i] << " ";
   // }

   // cout <<endl;

   return parsedCommand;
}

// Draws the data 
void drawData(){
   // Read in command from file
   ifstream infile;
   infile.open("commands.txt");

   string input;

   // Loop through all of the commands
   while(!infile.eof()){
      getline(infile, input);

      pair<string, vector<int>> command = parseCommand(input);

      if(command.first == "set_color"){
         setColor(command.second);
      } else if(command.first == "draw_point"){
         drawPoint(command.second);
      } else if(command.first == "draw_line"){
         drawLine(command.second);
      } else if(command.first == "draw_polygon") {
         drawPolygon(command.second);
      } else {

      }
   }

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
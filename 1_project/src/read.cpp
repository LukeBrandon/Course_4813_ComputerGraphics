//---------------------------------------
// Program: read.cpp
// Purpose: Read in the data and display the commands to draw the graph
// Author:  Luke Brandon
// Date:    January 2020
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h> 
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int AXIS_WIDTH = 50;
const int AXIS_HEIGHT = 50;
const int DRAWABLE_WIDTH = 450;
const int DRAWABLE_HEIGHT = 450;


// Graph type enum and strings array
enum GraphType{
    DOT_PLOT, BAR_GRAPH, LINE_GRAPH, FILLED_LINE_GRAPH
};

static const char * GraphTypeStrings[] =
{
     "Dot Plot", "Bar Graph", "Line Graph", "Filled Line Graph" 
};

// Turns a integer vector into a string with spaces between the integers
string stringifyVector(vector<int> vector){
    string result = "";

    for(uint i =0; i < vector.size(); i++)
        result += to_string(vector[i]) + " ";

    return result;
}

// Print the commands
void print(vector<pair<string, vector<int>>> commands){
    cout << "\nCommands for this graph:  "<< endl;
    for(uint i =0; i < commands.size(); i++ ){
        cout << commands[i].first << " : " << stringifyVector(commands[i].second) << endl;
    }
}

// Read in the data from the input file
void readData(vector<int> &data){
    ifstream infile;
    infile.open("input.txt");

    string input;
    getline(infile, input);

    string tempString = "";

    // Parse the data out of the single line read in from the input
    for(uint i = 0; i < input.length(); i++){
        tempString += (input.at(i));

        // If the character is a space then add the previous number to the data
        if(input.at(i) == ' ' || i == (input.length()-1)){
            data.push_back(atoi(tempString.c_str()));
            tempString = "";
        }
    }

    infile.close();
}

// Writes the commands to an output file
void writeToFile(vector<pair<string, vector<int>>> commands){
    ofstream outfile("commands.txt");

    for(uint i = 0; i < commands.size(); i++){
        outfile << commands[i].first << " " << stringifyVector(commands[i].second) << endl;
    }
}

// Creates the commands that will be printed, output, and read from the OpenGL file
void generateCommands(vector<pair<string, vector<int>>> &commands, vector<int> data, int graphType){
    int xOffset = DRAWABLE_WIDTH/(data.size() + 1);
    int yOffset = DRAWABLE_HEIGHT/50; //50 is max y value

    if(graphType == DOT_PLOT){

        // Adds commands to draw a point for each of the data points of size 10
        for(uint i = 0; i < data.size(); i++ ) {
            int x = xOffset*(i+1) + AXIS_WIDTH;
            int y = 500 - (data[i] * yOffset);
            vector<int> verticies = {20, x, y};

            commands.push_back({"draw_point", verticies});

        }

    } else if(graphType == BAR_GRAPH){
        int barThickness = 80;

        for(uint i = 0; i < data.size(); i++ ) {
            int x = xOffset*(i+1) + AXIS_WIDTH - barThickness/2;
            int y = 500 - (data[i] * yOffset);
            int x2 = x + barThickness;
            int y2 = y;
            int x3 = x2;
            int y3 = 450;
            int x4 = x;
            int y4 = 450;
            vector<int> verticies = {x, y, x2, y2, x3, y3, x4, y4};

            commands.push_back({"draw_polygon", verticies});

        }

    } else if (graphType == LINE_GRAPH){
        int prevX = -1;
        int prevY = -1;

        for(uint i = 0; i < data.size(); i++){

            int x = xOffset*(i+1) + AXIS_WIDTH;
            int y = 500 - (data[i] * yOffset);

            // Doesn't write the command for the first point
            if(i != 0){
                vector<int> verticies = {prevX, prevY, x, y};

                commands.push_back({"draw_line", verticies});
            }

            // Setting the previous for the next command
            prevX = x;
            prevY = y;

        }


    } else if (graphType == FILLED_LINE_GRAPH){
        vector<int> verticies;

        int startingX, endingX, startingY;

        // NEED TO DO
        for(uint i = 0; i < data.size(); i++ ) {
            int x = xOffset*(i+1) + AXIS_WIDTH;
            int y = 500 - (data[i] * yOffset);

            if(i == 0){
                startingX = x;
                startingY = y;
            }

            if(i  == data.size()-1)
                endingX = x;

            verticies.push_back(x);
            verticies.push_back(y);
        }

        // Vertex down to x axis
        verticies.push_back(endingX);
        verticies.push_back(450);

        // Vertex across x axis
        verticies.push_back(startingX);
        verticies.push_back(450);

        // Draws line up to starting point
        // verticies.push_back(startingX);
        // verticies.push_back(startingY);

        // Add lines straight down on either end and across the bottom
        commands.push_back({"draw_polygon", verticies});

    } else {
        cout << "ERROR: Invalid graph type was selected" << endl;
    }

    writeToFile(commands);
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
    vector<int> data;
    vector<pair<string, vector<int>>> commands;

    // Read in the data from input.txt
    readData(data);

    // Get user's input for graph type
    int graphType = -1;
    while(graphType != 3 && graphType != 2 && graphType != 1 && graphType != 0){
        cout << "0. " << GraphTypeStrings[DOT_PLOT] << endl;
        cout << "1. " << GraphTypeStrings[BAR_GRAPH] << endl;
        cout << "2. " << GraphTypeStrings[LINE_GRAPH] << endl;
        cout << "3. " << GraphTypeStrings[FILLED_LINE_GRAPH] << endl;
        cout << "Which type of graph would you like to see? --> ";
        cin >> graphType;  

        cout << "---------------------------" << endl;
    }
    cout << "Selected Graph Type: " << GraphTypeStrings[graphType] << endl;

    generateCommands(commands, data, graphType);

    print(commands);

   return 0;
}
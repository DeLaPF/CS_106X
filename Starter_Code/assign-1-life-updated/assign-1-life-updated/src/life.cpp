/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "filelib.h"
#include "gevents.h" // for mouse event detection
#include "grid.h"
#include "simpio.h"  // for getLine
#include "strlib.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Hello! Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

static void readConfigFile(Grid<int> &world) {
    ifstream infile;
    promptUserForFile(infile, "File?");
    Vector<string> lines;
    readEntireFile(infile, lines);
    infile.close();

    int i = 0;
    while (true) {
        if (lines[i][0] != '#') {
            break;
        }
        i++;
    }
    int rows = stringToInteger(lines[i++]);
    int cols = stringToInteger(lines[i++]);

    world.resize(rows, cols);

    for (int row = 0; i + row < lines.size(); row++) {
        string rowLine = lines[i + row];
        for (int col = 0; col < cols; col++) {
            if (rowLine[col] == '-') {
                world[row][col] = 0;
            } else {
                world[row][col] = 1;
            }
        }
    }
}

static void displayWorld(LifeDisplay &display, const Grid<int> &world) {
    int rows = world.numRows();
    int cols = world.numCols();

    display.setDimensions(rows, cols);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int age = world[row][col];
            display.drawCellAt(row, col, age);
        }
    }

    display.repaint();
}

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
    welcome();

    Grid<int> world;
    readConfigFile(world);
    displayWorld(display, world);

    return 0;
}

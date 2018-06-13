/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
using namespace std;

#include "console.h" // required of all files that contain the main function
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

static Grid<int> readConfigFile(string fileName) {

}

static string promptConfigFile() {
    return getLine("Please enter a config file [press enter if no Config]: ");
}

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    Grid<int> world;
    LifeDisplay display;
    display.setTitle("Game of Life");
    welcome();
    string fileName = promptConfigFile();
    if (fileName != "") {
        world = readConfigFile(fileName);
    }
    return 0;
}

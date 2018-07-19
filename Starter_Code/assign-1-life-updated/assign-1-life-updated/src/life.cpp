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
#include "gwindow.h"
#include "simpio.h"  // for getLine
#include "strlib.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

#define LONELY_THRESHOLD (1)
#define STABLE_THRESHOLD (2)
#define HOSPITABLE_THRESHOLD (3)
#define OVERCROWDED_THRESHOLD (4)
#define DEFAULT_WORLD_SIZE (25)

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

/**
 * Function: hasFile
 * -----------------
 * Checks if the user has a file.
 */
static bool hasFile() {
    if (getYesOrNo("Do you have a file in mind?")) {
        return true;
    }
    cout << "A random file will be generated for you" << endl;
    return false;
}

/**
 * Function: getAnimSpeed
 * -----------------
 * Gets the speed that the user would like to run the animation at.
 */
static int getAnimSpeed() {
    int speed;

    cout << "How fast would you like the animation to run?" << endl;
    cout << "\t(1) As fast as possible" << endl;
    cout << "\t(2) A slight pause between each new generation" << endl;
    cout << "\t(3) A slightly longer pause between each new generation" << endl;
    cout << "\t(4) Fully manual, click for each new generation" << endl;

    while (true) {
        speed = getInteger();

        if (speed >= 1 && speed <= 4) {
            break;
        } else {
            cout << "Please enter a valid number between 1 and 4" << endl;
        }
    }

    return speed;
}

/**
 * Function: readConfigFile
 * -----------------
 * Takes in a user-created-file and creates a "world" array based on its contents.
 */
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

/**
 * Function: randGenWorld
 * -----------------
 * Randomly generates a 25*25 "world".
 */
static Grid<int> randGenWorld() {
    Grid<int> world(DEFAULT_WORLD_SIZE, DEFAULT_WORLD_SIZE);
    for (int r = 0; r < DEFAULT_WORLD_SIZE; r++) {
        for (int c = 0; c < DEFAULT_WORLD_SIZE; c++) {
            int cell = rand() % 2;
            world[r][c] = cell;
        }
    }

    return world;
}

/**
 * Function: displayWorld
 * -----------------
 * Displays the "world" array.
 */
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
 * Function: isLonely
 * -----------------
 * Checks if a cell is lonely (has one or less neighbors).
 */
static bool isLonely(int neighbors) {
    if (neighbors <= LONELY_THRESHOLD) {
        return true;
    }
    return false;
}

/**
 * Function: isStable
 * -----------------
 * Checks if a cell is stable (has two neighbors).
 */
static bool isStable(int neighbors) {
    if (neighbors == STABLE_THRESHOLD) {
        return true;
    }
    return false;
}

/**
 * Function: isHospitable
 * -----------------
 * Checks if a space is hospitable (has three neighbors) for new, or old, life.
 */
static bool isHospitable(int neighbors) {
    if (neighbors == HOSPITABLE_THRESHOLD) {
        return true;
    }
    return false;
}

/**
 * Function: isOvercrowded
 * -----------------
 * Checks if a cell is overcrowded (has four or more neighbors).
 */
static bool isOvercrowded(int neighbors) {
    if (neighbors >= OVERCROWDED_THRESHOLD) {
        return true;
    }
    return false;
}

/**
 * Function: countNeighbors
 * -----------------
 * Counts the number of neighbors surrounding each given cell.
 */
static int countNeighbors(const Grid<int> &world, int r, int c) {
    int neighbors = 0;
    for (int row = r - 1; row <= r + 1; row++) {
        for (int col = c - 1; col <= c + 1; col++) {
            if (row == r && col == c) {
                continue;
            }
            if (!world.inBounds(row, col)) {
                continue;
            }

            if (world[row][col] > 0) {
                neighbors++;
            }
        }
    }

    return neighbors;
}

/**
 * Function: updateWorld
 * --------------
 * Updates the "world" array to the new generation (the new states of the cells).
 */
static Grid<int> updateWorld(const Grid<int> &world, Grid<int> &upWorld) {
    int rows = world.numRows();
    int cols = world.numCols();
    upWorld = world;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int neighbors = countNeighbors(world, r, c);
            if (isLonely(neighbors) || isOvercrowded(neighbors)) {
                upWorld[r][c] = 0;
            }
            if (isStable(neighbors)) {
                upWorld[r][c] += (world[r][c] > 0 ? 1 : 0);
            }
            if (isHospitable(neighbors)) {
                upWorld[r][c]++;
            }
        }
    }

    return upWorld;
}

/**
 * Function: runAnimation
 * -----------------
 * Runs the entire animation for the program.
 */
static void runAnimation(LifeDisplay &display, Grid<int> &world, Grid<int> &upWorld, int animSpeed) {
    bool isManual = false;
    displayWorld(display, world);

    switch (animSpeed) {
    case 1:
        animSpeed = 0;
        break;
    case 2:
        animSpeed = 50;
        break;
    case 3:
        animSpeed = 100;
        break;
    case 4:
        isManual = true;
        break;
    }

    GEvent event;
    GTimer timer(animSpeed);
    timer.start();
    while (true) {
        if (isManual) {
            event = waitForEvent(KEY_EVENT);
        } else {
            event = waitForEvent(TIMER_EVENT + MOUSE_EVENT);
        }

        if (event.getEventClass() == TIMER_EVENT || event.getEventClass() == KEY_EVENT) {
            world = updateWorld(world, upWorld);
            displayWorld(display, world);
        } else if (event.getEventType() == MOUSE_PRESSED) {
            break;
        }
    }
    timer.stop();
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
    Grid<int> upWorld;

    if (hasFile()) {
        readConfigFile(world);
    } else {
        world = randGenWorld();
    }

    runAnimation(display, world, upWorld, getAnimSpeed());

    return 0;
}

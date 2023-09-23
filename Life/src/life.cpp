// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

void welcome();
void file_input(string& file_path, ifstream& fin);
void colony_initializer(ifstream& fin, int& row, int& col, Grid<char>& curr_colony, Grid<char>& next_colony, LifeGUI& gui);
void read_colony(ifstream& fin, Grid<char>& curr_colony, LifeGUI& gui);
void display(const Grid<char>& g);
void count_cell_warpping(const Grid<char>& g, Vector<Vector<int>>& cnt);
void count_cell_nowarpping(const Grid<char>& g, Vector<Vector<int>>& cnt);
void evolution(Grid<char>& curr_colony, Grid<char>& next_colony, Vector<Vector<int>>& cnt, int frames, bool warping, LifeGUI& gui);
void menu(char& option, Grid<char>& curr_colony, Grid<char>& next_colony, Vector<Vector<int>>& cnt, bool warrping, LifeGUI& gui);
bool warping_indicator();

vector<int> dx {-1, -1, -1, 0, 1, 1, 1, 0}, dy {-1, 0, 1, 1, 1, 0, -1, -1};

int main() {
    // print welcome messages
    welcome();

    // Input colony file
    ifstream fin; // for file input
    string file_path; // input file path
    file_input(file_path, fin);

    // Initialize cell colony with date in file
    int row, col;
    Grid<char> curr_colony, next_colony;
    LifeGUI gui;
    colony_initializer(fin, row, col, curr_colony, next_colony, gui);

    // calculate the neighbors of every posiyion
    Vector<Vector<int>> cnt(row, Vector<int>(col));

    // menu
    char option;
    menu(option, curr_colony, next_colony, cnt, warping_indicator(), gui);

    cout << "Have a nice Life!" << endl;
    fin.close();
    return 0;
}

void welcome() {
    cout << "Welcome to Life game!\n"
         << "This is a simulation of the lifecycle of a bacteria colony.\n"
         << "One cell(X) lives or dies by the following rules:\n"
         << "- A location that has zero or one neighbors will be empty in the next generation. If a cell was there, it dies.\n"
         << "- A location with two neighbors is stable. If it had a cell, it still contains a cell. If it was empty, it's still empty.\n"
         << "- A location with three neighbors will contain a cell in the next generation. If it was unoccupied before, a new cell is born. If it currently contains a cell, the cell remains.\n"
         << "- A location with four or more neighbors will be empty in the next generation. If there was a cell in that location, it dies of overcrowding.\n\n";
}

void file_input(string& file_path, ifstream& fin) {
    while (true) {
        file_path = getLine("Please input the Grid file name? ");
        if (!fileExists(file_path))
            cout << "The file " << file_path << "can't be located! Please input again. \n";
        else {
            openFile(fin, file_path);
            break;
        }
    }
}

void colony_initializer(ifstream& fin, int& row, int& col, Grid<char>& curr_colony, Grid<char>& next_colony, LifeGUI& gui) {
    fin >> row >> col;
    curr_colony.resize(row, col);
    next_colony.resize(row, col);
    gui.resize(row, col);
    read_colony(fin, curr_colony, gui);
    next_colony = curr_colony;
    cout << endl;
    display(curr_colony);
}

void display(const Grid<char> &g) {
    for (int i = 0; i < g.numRows(); i ++) {
        for (int j = 0; j < g.numCols(); j ++)
            cout << g[i][j];
        cout << endl;
    }
}

void read_colony(ifstream& fin, Grid<char>& curr_colony,LifeGUI& gui) {
    for (int i = 0; i < curr_colony.numRows(); i ++)
        for (int j = 0; j < curr_colony.numCols(); j ++) {
            fin >> curr_colony[i][j];
            gui.drawCell(i, j, curr_colony[i][j] == 'X');
        }
}

void count_cell_warpping(const Grid<char>& g, Vector<Vector<int>>& cnt) {
    int row = g.numRows(), col = g.numCols();
    for (int i = 0; i < row; i ++ ) {
        for (int j = 0; j < col; j ++) {
            for (int k = 0; k < 8; k ++) {
                int x = i + dx[k];
                int y = j + dy[k];
                x = (x % row + row) % row;
                y = (y % col + col) % col;
                if (g[x][y] == 'X')
                    cnt[i][j] ++;
            }
        }
    }
}

void count_cell_nowarpping(const Grid<char>& g, Vector<Vector<int>>& cnt) {
    int row = g.numRows(), col = g.numCols();
    for (int i = 0; i < row; i ++ ) {
        for (int j = 0; j < col; j ++) {
            for (int k = 0; k < 8; k ++) {
                int x = i + dx[k];
                int y = j + dy[k];
                if (x < 0 || x >= row || y < 0 || y >= col)
                    continue;
                if (g[x][y] == 'X')
                    cnt[i][j] ++;
            }
        }
    }
}

void evolution(Grid<char>& curr_colony, Grid<char>& next_colony, Vector<Vector<int>>& cnt, int frames, bool warpping, LifeGUI& gui) {
    for (int m = 0; m < frames; m ++) {
        clearConsole();

        for (int i = 0; i < curr_colony.numRows(); i ++)
            for (int j = 0; j < curr_colony.numCols(); j ++)
                cnt[i][j] = 0;

        if (warpping)
            count_cell_warpping(curr_colony, cnt);
        else
            count_cell_nowarpping(curr_colony, cnt);

        for (int i = 0; i < curr_colony.numRows(); i ++) {
            for (int j = 0; j < curr_colony.numCols(); j ++) {
                int num = cnt[i][j];
                if (!num || num == 1) {
                    next_colony[i][j] = '-';
                    gui.drawCell(i, j, false);
                }
                else if (num == 3) {
                    next_colony[i][j] = 'X';
                    gui.drawCell(i, j, true);
                }
                else if (num >= 4) {
                    next_colony[i][j] = '-';
                    gui.drawCell(i, j, false);
                }
            }
        }
        curr_colony = next_colony;
        display(curr_colony);

        pause(50);
    }
}


bool warping_indicator() {
    char warp_option;
    cout << "Should the simulation warp around the grid (y/n)? ";
    cin >> warp_option;

    // !cin reprents that cin is fasle state -> cin == 0
    // cin.ignore(999, '\n') ignore 999 ch or until meet '\n'
    while (!cin || cin.peek() != '\n') {
        cout << "Invalid input, try again.\n";
        cin.clear();
        cin.ignore(999, '\n');
        cout << "Should the simulation warp around the grid (y/n)? ";
        cin >> warp_option;
    }
    switch (warp_option) {
    case 'y':
    case 'Y':
        return true;
        break;
    case 'n':
    case 'N':
        return false;
        break;
    default:
        cin.clear();
        cin.ignore(999, '\n');
        cout << "Invalid input, try again.\n";
        return warping_indicator();
        break;
    }
}

void menu(char& option, Grid<char>& curr_colony, Grid<char>& next_colony, Vector<Vector<int>>& cnt, bool warrping, LifeGUI& gui) {
    cout << "\nA)inmate, T)ick, Q)uit? ";
    cin >> option;
    while (!cin || cin.peek() != '\n') {
        cout << "Invalid input, try again.\n";
        cin.clear();
        cin.ignore(999, '\n');
        cout << "\nA)inmate, T)ick, Q)uit? ";
        cin >> option;
    }
    int frames;

    switch (option) {
    case 'a':
    case 'A':
        cout << "how many frames? ";
        cin >> frames;
        while (!cin || cin.peek() != '\n') {
            cout << "Invalid input, try again.\n";
            cin.clear();
            cin.ignore(999, '\n');
            cout << "how many frames? ";
            cin >> frames;
        }
        evolution(curr_colony, next_colony, cnt, frames, warrping, gui);
        menu(option, curr_colony, next_colony, cnt, warrping, gui);
        break;
    case 't':
    case 'T':
        evolution(curr_colony, next_colony, cnt, 1, warrping, gui);
        menu(option, curr_colony, next_colony, cnt, warrping, gui);
        break;
    case 'q':
    case 'Q':
        break;
    default:
        cin.clear();
        cin.ignore(999, '\n');
        cout << "Invalid input, Try, again.";
        menu(option, curr_colony, next_colony, cnt, warrping, gui);
        break;
    }
}

// renderer.cpp - An attempt at making a more reliable way of rendering text to the terminal than cout

// Idea: The console window is resized to a certain number of characters for the width and height.
//       The console window then edits specific characters (all of them) to match the contents of a similarly sized array.
//       There is no cout, only changes to characters.

#include <windows.h>

#include <iostream> // Simple debug stuff to start with.
#include <stdlib.h> // Random number
#include <ctime> // time for the random number generator
#include <string> // String for our console output

using namespace std;


struct consoleTextInfo{
    int rows;
    int columns;
};

consoleTextInfo getRows(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    consoleTextInfo x;
    x.columns = columns;
    x.rows = rows;

    return x;
}

void Resize(int width, int height){
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); 

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, width, height, TRUE);
}

// New goal: Use a 1D array of size(rows*cols) for everything

void populateArray(string& x, int rows, int cols){
    // Seed the random number generator
    
    // Populate the char array with random characters
    srand(time(NULL));
    for(int i = 0; i < rows*cols; i++){
        x = x + static_cast<char>(rand()%26 + 65);
    }

    /*
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            x[(row*cols) + col] = static_cast<char>((rand()%26) + 65);
        }
    }
    */
}

void writeScreen(string x, int ROWS, int COLS){
    DWORD dw;
    COORD here;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
    {
        printf("Invalid handle");
    }

    LPSTR s;
    here.X = 0;
    here.Y = 0;
    //s = const_cast<char*>(x.c_str());
    
    //WriteConsoleOutputCharacterA(hStdOut, s, 2025, here, &dw); // Write the character to the console window.
    
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            here.X = row; // Set the cursor's X position to the row value
            here.Y = col; // Set the cursor's Y position to the column value

            s = const_cast<char*>(x.substr((COLS * row) + col, 1).c_str());

            WriteConsoleOutputCharacterA(hStdOut, s, 1, here, &dw); // Write the character to the console window.
        }
    }
    
}

void goToLastRow(short lastRow){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = {0, lastRow};
    SetConsoleCursorPosition(h, c);
}

int main(){
    Resize(320, 240); // Resize the console window to be 640px by 480px
    // This should give us 27 rows and 75 column of characters.

    consoleTextInfo terminalDimensions = getRows();

    int ROWS = terminalDimensions.rows - 1;
    int COLS = terminalDimensions.columns;

    string x;

    // Test 4 times.
    for(int i = 0; i < 40; i++){
        populateArray(x, ROWS, COLS);
        Sleep(500);
        writeScreen(x, COLS, ROWS);
        goToLastRow(ROWS);
        x = "";
        cout << "ITERATION: " << i;
        cout.flush();
        Sleep(500);
    }
    
    // The algorithm for accessing an element at a certain "row" and "column" is:
    //          (totalColumns * row) + column
    // where totalColumns is the number of columns in each row
    //       row is the row that our element is in
    //       column is the column that our element is in

    // Now we need to move the cursor to specific positions and change the value to match the char in the same position in the array.

    

    system("pause");
    return 0;
}
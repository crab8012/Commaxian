// commaxianEngine.cpp - Contains methods for getting the game to run
#pragma once

#include <windows.h>
#include <ctime>
#include <iostream>


time_t pastTime;
// Setup everything for the game engine.
void initializeEngine(){
    std::cout << "Initializing the Commaxian Game Engine" << std::endl;

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prev_mode;
    GetConsoleMode(hInput, &prev_mode); 
    SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);

    Sleep(1000);
    pastTime = time(nullptr) * 1000; // Update the time before the loop starts.
}

// Determine time difference and wait for the remainder
void waitForTime(int steps){
    // Algorithm: Get the current system time
    //            Compare to previous system time
    //            Wait for ((1/NUM_TIME_STEPS) - (currentTime - previousTime))}
    time_t currentTime = time(nullptr) * 1000; // Get the current time
    std::cout << "C: " << currentTime << std::endl;
    time_t previousTime = pastTime; // Get the previous time
    std::cout << "P: " << previousTime << std::endl;
    pastTime = currentTime; // Update the previous time

    Sleep((1/steps) - (currentTime - previousTime));
}

// Clear the console screen
void ClearScreen(){
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE);
    if(hStdOut == INVALID_HANDLE_VALUE) return;

    // Get the number of cells in the current buffer
    if(!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire buffer with spaces
    if(!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) return;

    // Fill the buffer with the current colors and attributes
    if(!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;

    // Move the cursor home
    SetConsoleCursorPosition(hStdOut, homeCoords);
}
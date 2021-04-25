#include <bitset>

#include "commaxianEngine.cpp"
#include <vector>

using namespace std;

void gameLoop();
void processInput();
void render();

// Global Variables
int NUM_TIME_STEPS = 2; // Loops per Second
bool shouldClose = false; // Should the game quit?
int loops = 0;

vector<vector<char>> map;

long long x = 0;

int main(){
    // Print a message to show that the window is working.
    initializeEngine(); // Setup the game engine
    gameLoop(); // Start the game loop
    system("pause");
    return 0;
}

void gameLoop(){
    while(!shouldClose){ // While the program is running
        processInput();
        render();
        waitForTime(NUM_TIME_STEPS);
    }
}

// Process any user input
void processInput(){
    bool upPressed = bitset<8>(GetKeyState(0x26))[7];
    bool downPressed = bitset<8>(GetKeyState(0x28))[7];
    bool leftPressed = bitset<8>(GetKeyState(0x25))[7];
    bool rightPressed = bitset<8>(GetKeyState(0x27))[7];
    bool deletePressed = bitset<8>(GetKeyState(0x2E))[7];
    if(upPressed){
        cout << "UP: " << upPressed << endl;
    }
    if(downPressed){
        cout << "DOWN: " << downPressed << endl;
    }
    if(leftPressed){
        cout << "LEFT: " << leftPressed << endl;
    }
    if(rightPressed){
        cout << "RIGHT: " << rightPressed << endl;
    }
    if(deletePressed){
        cout << "DELETE: " << deletePressed << endl;
    }
    
}

// Display the game on the screen
void render(){
    //ClearScreen();
    cout << flush;
    cout.clear();
    cout << "update " << endl;
    cout << "Please work!" << endl;
}
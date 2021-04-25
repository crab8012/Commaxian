#include <iostream>

int main(){
    bool x;
    while(true){
        if(x){
            std::cout << "A" << std::endl;
            x = !x;
        }        else{
            std::cout << "B" << std::endl;
            x = !x;
        }
    }
    return 0;
}
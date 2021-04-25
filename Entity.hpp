// Entity.hpp - The Entity class for objects in the game
#include <vector>

class Entity{
    public:
        void updateState();
        char getRepresentation();
        Entity(std::vector<char> representations);
        Entity();

    private:
        std::vector<char> representation;
        int state = 0;

    void updateState(){
        if(state <= representation.size()){
            state++;
        }else{
            state = 0;
        }
    }

    char getRepresentation(){
        return representation[state];
    }

    Entity(std::vector<char> representations){
        representation = representations;
    }
    Entity(){
        representation = {'?'};
    }
};
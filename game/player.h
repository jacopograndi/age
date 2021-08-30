#ifndef PLAYER_H
#define PLAYER_H

#include <vector>


class Player {
    public:
    Player (int r, int g, int b) : r(r), g(g), b(b) { }
    
    std::vector<int> res { 0, 0 };
    std::vector<int> techs;
    
    int level { 0 };
    
    int r, g, b;
};

#endif
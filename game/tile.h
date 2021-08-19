#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>

#include "../umath/vec2.h"

class Tile {
    public:
    Tile() {}
    
    std::string name;
    int move_cost;
    int sight_cost;
    int range_bonus;
    float attack_bonus;
    float defence_bonus;
    
    vec2 spritebounds;
};

#endif
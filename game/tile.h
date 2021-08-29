#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>

#include "../umath/vec2.h"

class Tile {
    public:
    Tile() {}
    
    std::string name;
    int move_cost { 1 };
    int sight_cost { 1 };
    int range_bonus { 0 };
    float attack_bonus { 0 };
    float defence_bonus { 0 };
    
    vec2 spritebounds;
};

#endif
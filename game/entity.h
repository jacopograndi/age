#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>
#include <string>

#include "../umath/vec2.h"

class Ability {
    public:
    Ability();
};

class EntityInfo {
    public:
    EntityInfo() { spritebounds = vec2 { 16*6, 16 }; }
    
    std::string name;
    
    float hp;
    float attack;
    float defence;
    int range;
    float sight;
    int move;
    std::vector<Ability> abilities;
    
    vec2 spritebounds;
};

class Entity {
    public:
    Entity(int x, int y, EntityInfo &info, int owner) 
        : x(x), y(y), info(info), owner(owner) {}

    int x, y;
    bool done = false;
    EntityInfo &info;
    
    int owner;
};

#endif
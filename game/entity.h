#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>
#include <string>

#include <iostream>

#include "../umath/vec2.h"


class EntityInfo {
    public:
    EntityInfo() { spritebounds = vec2 { 16*6, 16 }; }
    
    enum Class { inf, cav, ran, sie, bld };
    Class ent_class;
    
    std::string name;
    
    int id;
    int level;
    
    float hp;
    float attack;
    float defence;
    int range;
    float sight;
    int move;
    int unit;
    std::vector<int> abilities;
    std::vector<int> build;
    std::vector<int> train_id;
    std::vector<Class> train_class;
    
    std::vector<float> prod { 0, 0 };
    std::vector<float> cost { 0, 0 };
    
    std::vector<int> adjacent;
    std::vector<int> diagonal;
    float defence_bonus { 0 };
    int upgrade { -1 };
    
    vec2 spritebounds;
};

namespace EntityClass {
    int from_string(std::string str);
}

class Entity {
    public:
    Entity(int x, int y, EntityInfo *info, int owner) 
        : x(x), y(y), info(info), owner(owner) { moved = 0; hp = 100; }
    

    bool operator==(Entity oth) { 
        return x == oth.x && y == oth.y && info->unit == oth.info->unit;
    }

    int building { 0 };
    float hp; /**/
    int x, y;
    bool done = false;
    int moved;
    EntityInfo *info;
    int fights { 0 };
    
    int owner;
};

#endif
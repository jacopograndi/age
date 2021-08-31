#ifndef TECH_H
#define TECH_H

#include <vector>
#include <unordered_map>
#include <string>

#include "../umath/vec2.h"

class TechBonus {
    public:
    float attack { 0 };
    float defence { 0 };
    int sight { 0 };
    int range { 0 };
    int move { 0 };
    std::vector<float> cost { 0, 0 };
    std::vector<float> cost_abs { 0, 0 };
    std::vector<float> prod { 0, 0 };
    int trade;
    int improved_heal;
    int improved_convert;
    int req_range { 999 };
    
    std::vector<int> aff_id;
    std::vector<int> aff_class;
    int aff_level;
    int aff_all { 0 };
    
    TechBonus operator+(const TechBonus &rhs) { 
        TechBonus b;
        b.attack = attack + rhs.attack;
        b.defence = defence + rhs.defence;
        b.sight = sight + rhs.sight;
        b.move = move + rhs.move;
        for (int i=0; i<b.cost.size(); i++) {
            b.cost[i] = cost[i] + rhs.cost[i];
            b.cost_abs[i] = cost_abs[i] + rhs.cost_abs[i];
            b.prod[i] = prod[i] + rhs.prod[i];
        }
        b.trade = trade + rhs.trade;
        b.improved_heal = improved_heal + rhs.improved_heal;
        b.improved_convert = improved_convert + rhs.improved_convert;
        return b;
    }    
    
};

class TechLookup {
    public:
    TechLookup() {}
    
    TechBonus id (int i) { return map_id[i]; }
    
    std::unordered_map<int, TechBonus> map_id;
};

class Tech {
    public:
    Tech() {}
    
    std::string name;
    int id;
    int level;
    int req_id;
    std::vector<float> cost { 0, 0 };
    
    TechBonus bonus;
    
    vec2 spritebounds { 0, 0 };
};

#endif
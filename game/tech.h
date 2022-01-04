#ifndef TECH_H
#define TECH_H

#include <vector>
#include <unordered_map>
#include <string>

#include <math.h>

#include <iostream>

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
    int trade { 0 } ;
    int improved_heal { 0 };
    int improved_convert { 0 };
    int req_range { 999 };
    
    std::vector<int> aff_id;
    std::vector<int> aff_class;
    int aff_level { -1 };
    int aff_all { 0 };
    
    TechBonus operator+(const TechBonus &rhs) { 
        TechBonus b;
        b.attack = attack + rhs.attack;
        b.defence = defence + rhs.defence;
        b.sight = sight + rhs.sight;
        b.move = move + rhs.move;
        for (int i=0; i<cost.size(); i++) {
            b.cost[i] = cost[i] + rhs.cost[i];
            b.cost_abs[i] = cost_abs[i] + rhs.cost_abs[i];
            b.prod[i] = prod[i] + rhs.prod[i];
        }
        b.trade = trade + rhs.trade;
        b.improved_heal = improved_heal + rhs.improved_heal;
        b.improved_convert = improved_convert + rhs.improved_convert;
        return b;
    }
    
    std::string to_string () {
        std::string str = "";
        if (attack != 0) { 
            int value = (int)roundf(attack*100);
            str += "Attack: " + std::to_string(value) + "%\n";
        }
        if (defence != 0) { 
            int value = (int)roundf(defence*100);
            str += "Defence: " + std::to_string(value) + "%\n";
        }
        if (sight != 0) { 
            str += "Sight: " + std::to_string(sight) + "\n";
        }
        if (range != 0) { 
            str += "Range: " + std::to_string(range) + "\n";
        }
        if (move != 0) { 
            str += "Range: " + std::to_string(range) + "\n";
        }
        if (cost[0] != 0 || cost[1] != 0) { 
            int vf = (int)roundf(cost[0]*100);
            int vg = (int)roundf(cost[1]*100);
            str += "Cost f: " + std::to_string(vf);
            str += "%, g: " + std::to_string(vg) + "%\n";
        }
        if (cost_abs[0] != 0 || cost_abs[1] != 0) { 
            int vf = (int)roundf(cost_abs[0]);
            int vg = (int)roundf(cost_abs[1]);
            str += "Cost f: " + std::to_string(vf);
            str += ", g: " + std::to_string(vg) + "\n";
        }
        if (prod[0] != 0 || prod[1] != 0) { 
            int vf = (int)roundf(prod[0]*100);
            int vg = (int)roundf(prod[1]*100);
            str += "Production f: " + std::to_string(vf);
            str += "%, g: " + std::to_string(vg) + "%\n";
        }
        if (trade != 0) { str += "Improves Trade Rate\n"; }
        if (improved_heal != 0) { str += "Improves Heal Ability\n"; }
        if (improved_convert != 0) { str += "Improves Convert Ability\n"; }
        if (aff_id.size() > 0) { 
            str += "of entitiy "; 
            for (int id : aff_id) str += std::to_string(id) + " ";
            str += "\n"; 
        }
        if (aff_class.size() > 0) { 
            str += "of class "; 
            for (int c : aff_class) str += std::to_string(c) + " ";
            str += "\n"; 
        }
        if (aff_level != -1) { 
            str += "of level " +  std::to_string(aff_level) + "\n"; 
        }
        return str;
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
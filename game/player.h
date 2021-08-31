#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "tech.h"


class Player {
    public:
    Player (int r, int g, int b, int id) : r(r), g(g), b(b), id(id) { }
    
    void pay (std::vector<float> cost) {             
        for (int i=0; i<res.size(); i++) {
            res[i] -= cost[i];
        }  
    }
    void gain (std::vector<float> gain) {             
        for (int i=0; i<res.size(); i++) {
            res[i] += gain[i];
        }  
    }
    
    bool has_tech (int id) {
        if (std::find(techs.begin(), techs.end(), id) != techs.end()) 
            return true;
        return false;
    }
    
    bool operator== (Player &oth) { return id == oth.id; }
    
    int id;
    
    std::vector<float> res { 0, 0 };
    std::vector<int> techs;
    
    TechLookup tech_lookup;
    
    int researching { -1 };
    int leveling_up { -1 };
    int level { 0 };
    
    int r, g, b;
};

#endif
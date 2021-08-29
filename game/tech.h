#ifndef TECH_H
#define TECH_H

#include <vector>
#include <string>

#include "../umath/vec2.h"

class Tech {
    public:
    Tech() {}
    
    std::string name;
    std::vector<int> cost { 0, 0 };
    
    vec2 spritebounds { 0, 0 };
};

#endif
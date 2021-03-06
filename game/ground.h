#ifndef GROUND_H
#define GROUND_H

#include "entity.h"

class Gst;

class Resource {
    public:
    Resource (int pos, int kind) : pos(pos), kind(kind) {}
   
    enum Type { gold, food };
    
    int pos, kind;
};

class Ground {
    public:
    Ground () {}
    
    int *tiles;
    
    std::vector<Resource> resources;
    
    void build (int sx, int sy);
    
    int sizex;
    int sizey;
    int at (int x, int y);
    std::vector<int> star (int pos);
    std::vector<int> move_area (Gst &gst, Entity &ent);
    std::vector<int> attack_targets (Gst &gst, Entity &ent);
    std::vector<int> heal_targets (Gst &gst, Entity &ent);
    std::vector<int> convert_targets (Gst &gst, Entity &ent);
};

#endif
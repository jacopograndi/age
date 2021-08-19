#ifndef GROUND_H
#define GROUND_H

#include "entity.h"

class Gst;

class Ground {
    public:
    Ground (int sx, int sy);
    ~Ground ();
    
    int *tiles;
    
    int sizex;
    int sizey;
    int at (int x, int y);
    std::vector<int> star (int pos);
    std::vector<int> move_area (Gst &gst, Entity ent);
};

#endif
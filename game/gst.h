#ifndef GST_H
#define GST_H

#include <vector>

#include "ground.h"
#include "entity.h"
#include "tile.h"
#include "player.h"

class Gst {
    public:
    Gst(int sx, int sy) : ground(sx, sy) {}
    
    std::vector<EntityInfo> infos;
    std::vector<Tile> tiles;
    std::vector<Entity> entities;
    Ground ground;
    
    std::vector<Player> players;
    
    int turn { 0 };
    int day { 0 };
    
    void end_day () {
        day++;
        if (day >= players.size()) {
            day = 0;
            turn++;
        }
    }
};

#endif
#include "ground.h"
#include "gst.h"

#include <iostream>
#include <algorithm>

Ground::Ground (int sx, int sy) {
    sizex = sx; sizey = sy;
    tiles = new int[sx*sy];
    for (int i=0; i<sx*sy; i++) tiles[i] = 0;
}

Ground::~Ground () {
    
}

int Ground::at(int x, int y) {
    return x+y*sizex;
}


// areas
std::vector<int> Ground::star (int pos) {
    std::vector<int> fs;
    int x = pos % sizex, y = pos / sizex;
    if (x-1 >= 0) { fs.push_back(at(x-1, y)); }
    if (x+1 < sizex) { fs.push_back(at(x+1, y)); }
    if (y-1 >= 0) { fs.push_back(at(x, y-1)); }
    if (y+1 < sizey) { fs.push_back(at(x, y+1)); }
    return fs;
}

class step { public: 
    step(int pos, int m) : pos(pos), m(m) {};
    bool operator==(step oth) { return pos==oth.pos && m==oth.m; }
    bool operator==(int p) { return pos==p; }
    int pos, m; 
};

std::vector<int> Ground::move_area (Gst &gst, Entity &ent) {
    Player &player = gst.players[ent.owner];
    int move_num = ent.info->move;
    move_num += player.tech_lookup.id(ent.info->id).move;
    
    std::vector<int> moves;
    std::vector<int> visited { at(ent.x, ent.y) };
    std::vector<step> frontier { step { at(ent.x, ent.y), move_num } };
    
    int maxcost = 99;
    if (gst.info_has_ability(ent.info, "Scout")) maxcost = 2;
    
    int iter=0;
    for (; iter<10000; iter++) {
        
        if (frontier.size() == 0) break;
        step maxf {-1, -1};
        for (step t : frontier) {
            if (t.m > maxf.m) {
                maxf.pos = t.pos;
                maxf.m = t.m;
            }
        }
        frontier.erase(std::remove(frontier.begin(), frontier.end(), maxf),
               frontier.end());
        auto forward_star = star(maxf.pos);
        for (int t : forward_star) {
            if (!(std::find(visited.begin(), visited.end(), t) != visited.end())
            && !(std::find(frontier.begin(), frontier.end(), t) != frontier.end())) {
                int movecost = gst.tiles[gst.ground.tiles[t]].move_cost;
                if (movecost > maxcost) movecost = maxcost;
                int walkedm = maxf.m - movecost;
                bool obstructed = false;
                for (Entity &e : gst.entities) {
                    if (e.owner != ent.owner && at(e.x, e.y) == t) {
                        obstructed = true;
                        break;
                    }
                }
                if (walkedm >= 0 && !obstructed) {
                    frontier.emplace_back(t, walkedm);
                    moves.push_back(t);
                }
            }
        }
        visited.push_back(maxf.pos);
    }
    
    std::cout << "iters: " << iter;
    
    return moves;
}

std::vector<int> Ground::attack_targets (Gst &gst, Entity &ent) {
    std::vector<int> attacks;
    int range = gst.get_range(ent);
    bool builds = !gst.info_has_ability(ent.info, "Units Only");
    bool units = !gst.info_has_ability(ent.info, "Buildings Only");
    for (Entity &e : gst.entities) {
        if (!units && e.info->unit == 1) continue;
        if (!builds && e.info->unit == 0) continue;
        if (e.owner != ent.owner && abs(e.x-ent.x)+abs(e.y-ent.y) <= range) {
            attacks.push_back(at(e.x, e.y));
        }
    }
    return attacks;
}
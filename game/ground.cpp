#include "ground.h"
#include "gst.h"

#include <iostream>
#include <algorithm>

void Ground::build (int sx, int sy) {
    sizex = sx; sizey = sy;
    tiles = new int[sx*sy];
    for (int i=0; i<sx*sy; i++) tiles[i] = 0;
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
    if (gst.inv->info_has_ability(ent.info, "Scout")) maxcost = 2;
    
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
                int movecost = gst.inv->tiles[tiles[t]].move_cost;
                if (movecost > maxcost) movecost = maxcost;
                int walkedm = maxf.m - movecost;
                bool obs_enemy = false, obs_friend = false;
                for (Entity &e : gst.entities) {
                    if (at(e.x, e.y) == t) {
                        if (e.owner != ent.owner) obs_enemy = true;
                        else obs_friend = true;
                        break;
                    }
                }
                if (walkedm >= 0 && !obs_enemy) {
                    frontier.emplace_back(t, walkedm);
                    if (!obs_friend) {
                        moves.push_back(t);
                    }
                }
            }
        }
        visited.push_back(maxf.pos);
    }
    
    
    return moves;
}

std::vector<int> Ground::attack_targets (Gst &gst, Entity &ent) {
    std::vector<int> attacks;
    int range = gst.get_range(ent);
    bool builds = !gst.inv->info_has_ability(ent.info, "Units Only");
    bool units = !gst.inv->info_has_ability(ent.info, "Buildings Only");
    for (Entity &e : gst.entities) {
        if (!units && e.info->unit == 1) continue;
        if (!builds && e.info->unit == 0) continue;
        int dist = abs(e.x-ent.x)+abs(e.y-ent.y);
        if (dist > 0 && e.owner != ent.owner && dist <= range) {
            attacks.push_back(at(e.x, e.y));
        }
    }
    return attacks;
}

std::vector<int> Ground::heal_targets (Gst &gst, Entity &ent) {
    std::vector<int> heals;
    int range = gst.get_range(ent);
    for (Entity &e : gst.entities) {
        if (e.info->unit == 0) continue;
        if (e.info->ent_class == EntityInfo::Class::sie) continue;
        int dist = abs(e.x-ent.x)+abs(e.y-ent.y);
        if (dist > 0 && e.owner == ent.owner && dist <= range) {
            heals.push_back(at(e.x, e.y));
        }
    }
    return heals;
}

std::vector<int> Ground::convert_targets (Gst &gst, Entity &ent) {
    std::vector<int> converts;
    int range = gst.get_range(ent);
    for (Entity &e : gst.entities) {
        if (e.info->unit == 0) continue;
        int dist = abs(e.x-ent.x)+abs(e.y-ent.y);
        if (dist > 0 && e.owner != ent.owner && dist <= range) {
            converts.push_back(at(e.x, e.y));
        }
    }
    return converts;
}
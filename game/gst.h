#ifndef GST_H
#define GST_H

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>

#include "ground.h"
#include "entity.h"
#include "tile.h"
#include "player.h"
#include "tech.h"

class Ability {
    public:
    Ability(std::string name) : name(name) {}
    std::string name;
};

class Bonus {
    public: 
    Bonus(float amt, int id, bool atk) : amt(amt), id(id), atk(atk) {}
    float amt; int id; bool atk;
    enum Id { ground, type, ability, tech };
};

class Gst {
    public:
    Gst(int sx, int sy) : ground(sx, sy) {}
    
    std::vector<Tech> techs;
    std::vector<Ability> abilities;
    std::vector<EntityInfo> infos;
    std::vector<Tile> tiles;
    std::vector<Entity> entities;
    Ground ground;
    
    std::vector<Player> players;
    
    EntityInfo* get_info (std::string name);
    EntityInfo* get_info (int id);
    bool info_has_ability (EntityInfo* info, std::string name);
    Entity& get_at (int x, int y);
    
    float get_type_bonus (Entity &atk, Entity &def);
    std::vector<Bonus> get_bonuses (Entity &atk, Entity &def);
    float get_damage (Entity &atk, Entity &def);
    bool get_first_strike (Entity &atk, Entity &def);
    void battle (Entity &atk, Entity &def);
    void clear_dead();    
    
    std::vector<int> get_possible_builds (Entity &ent);
    
    bool check_req_build(Entity &ent, EntityInfo *info);
    
    bool check_obstructed (Entity &ent);
    
    int turn { 0 };
    int day { 0 };
    
    void end_day ();
};

#endif
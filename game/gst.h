#ifndef GST_H
#define GST_H

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>

#include <random> // just for picking market trains

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
    enum Id { ground, type, ability, tech, veteran, on_bld, adjacency };
    std::string id_string () {
        switch (id) {
            case ground: return "Ground";
            case type: return "Class";
            case ability: return "Ability";
            case tech: return "Tech";
            case veteran: return "Veteran";
            case on_bld: return "On Building";
            case adjacency: return "Adjacency";
        }
    }
};

class BattleResult {
    public:
    BattleResult(float atk_hp, float def_hp) 
        : atk_hp(atk_hp), def_hp(def_hp) {}
    float atk_hp, def_hp;
};

class Gst {
    private: std::default_random_engine engine = std::default_random_engine{};
    public:
    Gst(int sx, int sy) : ground(sx, sy) { }
    
    std::vector<Tech> techs;
    std::vector<Ability> abilities;
    std::vector<EntityInfo> infos;
    std::vector<Tile> tiles;
    std::vector<Entity> entities;
    Ground ground;
    
    std::vector<Player> players;
    
    Player& get_player (int id);
    Tech* get_tech (int id);
    EntityInfo* get_info (std::string name);
    EntityInfo* get_info (int id);
    bool info_has_ability (EntityInfo* info, std::string name);
    Entity& get_at (int x, int y);
    std::vector<float> get_cost (EntityInfo *info, Player &player);
    float get_trade_rate (Player &player);
    
    int get_vet_level (Entity &ent);
    float get_type_bonus (Entity &atk, Entity &def);
    std::vector<Bonus> get_bonuses (Entity &atk, Entity &def);
    float get_damage (Entity &atk, Entity &def);
    float get_damage (Entity &atk, Entity &def, float atk_hp);
    bool get_first_strike (Entity &atk, Entity &def);
    BattleResult battle_res (Entity &atk, Entity &def);
    void battle (Entity &atk, Entity &def);
    void clear_dead();    
    int get_range(Entity &ent);
    
    std::vector<int> get_possible_trains (Entity &ent);    
    std::vector<int> get_possible_builds (Entity &ent);
    
    bool check_req_build (Entity &ent, EntityInfo *info);
    bool check_req_tech (Tech *tech, Player &player); 
    bool check_req_level (Player &player); 
    bool check_obstructed (Entity &ent);
    
    int turn { 0 };
    int day { 0 };
    
    void end_day ();
    void level_upgrade (Player &player);
    
    void update_tech_lookup (Player &player);
};

#endif
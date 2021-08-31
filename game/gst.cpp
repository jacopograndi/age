#include "gst.h"

#include <map>
#include <iostream>

Player& Gst::get_player (int id) {
    for (auto &player : players) {
        if (id == player.id) return player;
    }
}

Tech* Gst::get_tech (int id) {
    for (auto &tech : techs) {
        if (id == tech.id) return &tech;
    }
}
    
EntityInfo* Gst::get_info (std::string name) {
    for (EntityInfo &info : infos) {
        if (name == info.name) return &info;
    }
}

EntityInfo* Gst::get_info (int id) {
    for (EntityInfo &info : infos) {
        if (id == info.id) return &info;
    }
}

bool Gst::info_has_ability (EntityInfo* info, std::string name) {
    for (int ab : info->abilities) {
        if (abilities[ab].name == name) return true;
    }
    return false;
}

Entity& Gst::get_at (int x, int y) {
    for (Entity &e : entities) {
        if (e.x ==x && e.y == y) return e;
    }
}

std::vector<float> Gst::get_cost (EntityInfo *info, Player &player) {
    std::vector<float> cost = info->cost;
    for (int i=0; i<info->cost.size(); i++) {
        cost[i] *= 1+player.tech_lookup.id(info->id).cost[i];
        cost[i] += player.tech_lookup.id(info->id).cost_abs[i];
    }
    return cost;
}

float Gst::get_trade_rate (Player &player) {
    float rate = 250;
    rate -= player.tech_lookup.id(0).trade * 25;
    return rate;
}

float Gst::get_type_bonus (Entity &atk, Entity &def) {
    float b = 0;
    switch(atk.info->ent_class) {
        case EntityInfo::Class::inf: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += 1.0f/3.0f;
        if (def.info->ent_class == EntityInfo::Class::sie) b += 1.0f/3.0f;
        break;
        
        case EntityInfo::Class::cav: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += -0.5;
        if (def.info->ent_class == EntityInfo::Class::inf) b += 1.0f/3.0f;
        if (def.info->ent_class == EntityInfo::Class::ran) b += 1.0f/3.0f;
        break;
        
        case EntityInfo::Class::ran: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += -0.5f;
        break;
        
        case EntityInfo::Class::sie: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += +0.5f;
        break;
    }
    return b;
}

std::vector<Bonus> Gst::get_bonuses (Entity &atk, Entity &def) {
    std::vector<Bonus> bs;
    if (tiles[ground.tiles[ground.at(atk.x, atk.y)]].attack_bonus != 0) {
        
        bs.emplace_back(
            tiles[ground.tiles[ground.at(atk.x, atk.y)]].attack_bonus, 
            Bonus::Id::ground, true);
    }
    if (tiles[ground.tiles[ground.at(def.x, def.y)]].defence_bonus != 0) {
        bs.emplace_back(
            tiles[ground.tiles[ground.at(def.x, def.y)]].defence_bonus, 
            Bonus::Id::ground, false);
    }
        
    if (get_type_bonus(atk, def) != 0) {
        bs.emplace_back(get_type_bonus(atk, def), Bonus::Id::type, true);
    }
    if (get_type_bonus(def, atk) != 0) {
        bs.emplace_back(get_type_bonus(def, atk), Bonus::Id::type, false);
    }
    
    if (info_has_ability(atk.info, "Causes Fear")) 
        bs.emplace_back(-1.0f/3, Bonus::Id::ability, false);
    if (info_has_ability(def.info, "Causes Fear")) 
        bs.emplace_back(-1.0f/3, Bonus::Id::ability, true);
    
    if (info_has_ability(atk.info, "Anti-Cavalry")) 
        bs.emplace_back(4.0f/3, Bonus::Id::ability, true);
    if (info_has_ability(def.info, "Anti-Cavalry")) 
        bs.emplace_back(4.0f/3, Bonus::Id::ability, false);
    
    if (info_has_ability(atk.info, "Desert Charge")
    && !info_has_ability(def.info, "Desert Charge")
    && tiles[ground.tiles[ground.at(def.x, def.y)]].name == "Desert") 
        bs.emplace_back(1.0f/3, Bonus::Id::ability, true);
        
    if (info_has_ability(atk.info, "Plains Charge")
    && !info_has_ability(def.info, "Plains Charge")
    && tiles[ground.tiles[ground.at(def.x, def.y)]].name == "Plains") 
        bs.emplace_back(1.0f/3, Bonus::Id::ability, true);
        
    if (info_has_ability(atk.info, "Woodsman")
    && !info_has_ability(def.info, "Woodsman")
    && tiles[ground.tiles[ground.at(def.x, def.y)]].name == "Forest") 
        bs.emplace_back(1.0f/3, Bonus::Id::ability, true);
        
    if (info_has_ability(atk.info, "Volley") && atk.hp >= 50) 
        bs.emplace_back(1.0f/3, Bonus::Id::ability, true);
    
    if (info_has_ability(atk.info, "Frenzy")) 
        bs.emplace_back(1/atk.hp, Bonus::Id::ability, true);
    
    
    Player &player_atk = players[atk.owner];
    Player &player_def = players[def.owner];
    float tech_attack = player_atk.tech_lookup.id(atk.info->id).attack;
    if (tech_attack != 0)
        bs.emplace_back(tech_attack, Bonus::Id::tech, true);
    float tech_defence = player_def.tech_lookup.id(def.info->id).defence;
    if (tech_defence != 0)
        bs.emplace_back(tech_defence, Bonus::Id::tech, false);
    
    return bs;
}

float Gst::get_damage (Entity &atk, Entity &def, float atk_hp) {
    float atkmul = 1;
    float defmul = 1;
    
    auto bonuses = get_bonuses(atk, def);
    for (auto bonus : bonuses) {
        if (bonus.atk) { atkmul += bonus.amt; }
        else { defmul += bonus.amt; }
    }
    
    float dam = (atk.info->attack * atk_hp * atkmul) 
        / (2.0f*def.info->defence * defmul);
    
    return dam;
}

float Gst::get_damage (Entity &atk, Entity &def) {
    return get_damage(atk, def, atk.hp);
}

bool Gst::get_first_strike (Entity &atk, Entity &def) {
    bool fs { false };
    fs = info_has_ability(atk.info, "First Strike");
    return fs;
}

float clamp_hp (float hp) { 
    if (hp > 100) hp = 100; 
    if (hp < 0) hp = 0;
    return hp; 
}

BattleResult Gst::battle_res (Entity &atk, Entity &def) {
    BattleResult result { atk.hp, def.hp };
    bool first_strike_atk = info_has_ability(atk.info, "First Strike");
    bool first_strike_def = info_has_ability(def.info, "First Strike");
    bool skirmish_atk = info_has_ability(atk.info, "Skirmish");
    bool skirmish_def = info_has_ability(def.info, "Skirmish");
    bool anticav_atk = info_has_ability(atk.info, "Anti-Cavalry");
    bool anticav_def = info_has_ability(def.info, "Anti-Cavalry");
    first_strike_atk = first_strike_atk 
        || (skirmish_atk && def.info->range == 1);
    first_strike_def = first_strike_def 
        || (skirmish_def && atk.info->range == 1);
    first_strike_atk = first_strike_atk 
        || (anticav_atk && def.info->ent_class == EntityInfo::Class::cav);
    first_strike_def = first_strike_def 
        || (anticav_def && atk.info->ent_class == EntityInfo::Class::cav);
        
    int dist = abs(atk.x-def.x) + abs(atk.y-def.y);
    bool def_inrange = (dist <= get_range(def)) ? true : false;
        
    bool swap = false;
    if (first_strike_def && !first_strike_atk) swap = true;
    if (swap) {
        if (def_inrange) {
            result.atk_hp = clamp_hp(
                result.atk_hp - get_damage(def, atk, result.def_hp));
        }
        if (!info_has_ability(atk.info, "No Counter")) 
            if (result.atk_hp > 0) 
                result.def_hp = clamp_hp(
                    result.def_hp - get_damage(atk, def, result.atk_hp));
    } else {
        result.def_hp = clamp_hp(
            result.def_hp - get_damage(atk, def, result.atk_hp));
        if (!info_has_ability(def.info, "No Counter") && def_inrange) 
            if (result.def_hp > 0) 
                result.atk_hp = clamp_hp(
                    result.atk_hp - get_damage(def, atk, result.def_hp));
    }
    
    if (info_has_ability(atk.info, "Rapid Fire")) 
        if (result.def_hp > 0) 
            result.def_hp = clamp_hp(
                result.def_hp - get_damage(atk, def, result.def_hp));
    
    if (info_has_ability(def.info, "Rapid Fire") && def_inrange)
        if (result.atk_hp > 0) 
            result.atk_hp = clamp_hp(
                result.atk_hp - get_damage(def, atk, result.def_hp));
    
    if (result.atk_hp > 0 && info_has_ability(atk.info, "Zeal")) 
        result.atk_hp = clamp_hp(result.atk_hp + 20);
    if (result.def_hp > 0 && info_has_ability(def.info, "Zeal")) 
        result.def_hp = clamp_hp(result.def_hp + 20);
    
    return result;
}

void Gst::battle (Entity &atk, Entity &def) {
    std::cout << "! attack " << atk.info->name << "(hp:" << atk.hp << "), "
    << def.info->name << "(hp:" << def.hp << ") \n";
    
    auto result = battle_res(atk, def);
    atk.hp = result.atk_hp;
    def.hp = result.def_hp;
    
    std::cout << "! result " << atk.info->name << "(hp:" << atk.hp << "), "
    << def.info->name << "(hp:" << def.hp << ") \n";
    clear_dead();
}

void Gst::clear_dead() {
    auto i = std::begin(entities);
    while (i != std::end(entities)) {
        if (i->hp <= 0) {
            entities.erase(i);
        }
        else i++;
    }
}

int Gst::get_range (Entity &ent) {
    int range = ent.info->range;
    if (range > 1) {
        range += tiles[ground.tiles[ground.at(ent.x, ent.y)]].range_bonus; 
    }
    if (range < 1) range = 1;
    return range;
}

std::vector<int> Gst::get_possible_builds (Entity &ent) {
    std::vector<int> builds;
    for (int id : ent.info->build) {
        if (check_req_build(ent, get_info(id))) {
            builds.push_back(id);
        }
    }
    return builds;
}


bool Gst::check_req_build(Entity &ent, EntityInfo *info) {
    Player &player = players[ent.owner];
    if (player.level < info->level) return false;
    for (int id : info->adjacent) {
        bool adj = false;
        for (Entity &e : entities) {
            if (e.info->id == id && ent.owner == e.owner) {
                int dist = abs(e.x-ent.x) + abs(e.y-ent.y);
                if (dist == 1) {
                    adj = true;
                }
            }
        }
        if (!adj) return false;
    }
    if (info->id == 100) {
        for (Resource &r : ground.resources) {
            if (r.pos == ground.at(ent.x, ent.y))  {
                return false;
            }
        }
        int mindist = 9999;
        for (Entity &e : entities) {
            if (e.info->id == 100 && ent.owner == e.owner) {
                int dist = abs(e.x-ent.x) + abs(ent.y-e.y);
                if (dist < mindist) {
                    mindist = dist;
                }
            }
        }
        std::cout << "  " << mindist;
        if (mindist < 5) {
            return false;
        }
        return true;
    }
    if (info->id == 101) {
        for (Resource &r : ground.resources) {
            if (r.kind == Resource::Type::food
                && r.pos == ground.at(ent.x, ent.y))  {
                return true;
            }
        }
        return false;
    }
    if (info->id == 102) {
        for (Resource &r : ground.resources) {
            if (r.kind == Resource::Type::gold
                && r.pos == ground.at(ent.x, ent.y))  {
                return true;
            }
        }
        return false;
    }
    return true;
}

bool Gst::check_req_train (Entity &ent, EntityInfo *info) {
    Player &player = players[ent.owner];
    if (player.level < info->level) return false;
    return true;
}


bool Gst::check_req_tech (Tech *tech, Player &player) {
    if (player.leveling_up == 1) return false;
    if (tech->level > player.level) {
        return false;
    }
    if (tech->cost[0] > player.res[0]
        || tech->cost[1] > player.res[1] ) 
    {
        return false;
    }
    if (player.has_tech(tech->id)) {
        return false;
    }
    bool req_id = false;
    for (auto &ent : entities) {
        if (ent.owner == turn // WARNING: turn is not player.id
        && ent.info->id == tech->req_id
        && ent.building == 0) 
        {
            req_id = true;
            break;
        }
    }
    if (!req_id) {
        return false;
    }
    return true;
}

bool Gst::check_req_level (Player &player) {
    if (player.leveling_up == 1) return false;
    
    for (float v : player.res) {
        if (v <= (player.level+1)*500) return false;
    }
    
    std::map<int, int> lv_techs;
    for (int id : player.techs) lv_techs[get_tech(id)->level] ++;
    if (player.level == 0) {
        if (lv_techs[0] >= 3) return true;
    }
    if (player.level == 1) {
        if (lv_techs[1] >= 7) return true;
    }
    if (player.level == 2) {
        if (lv_techs[2] >= 11) return true;
    }
    return false;
}

bool Gst::check_obstructed (Entity &ent) {
    for (Entity &e : entities) {
        if (&ent != &e && e.x == ent.x && ent.y == e.y) return true;
    }
    return false;
}

void Gst::end_day () {
    turn++;
    if (turn >= players.size()) {
        turn = 0;
        day++;
    }
    Player &player = players[turn];
    if (player.leveling_up != -1) {
        level_upgrade(player);
        player.level ++;
        player.leveling_up = -1;
    }
    for (Entity &e : entities) {
        e.done = false;
        e.moved = 0;
        if (get_player(e.owner) == player) {
            for (int i=0; i<player.res.size(); i++) {
                player.res[i] += e.info->prod[i] * 
                    (1+player.tech_lookup.id(e.info->id).prod[i]);
            } 
            if (e.building < 0) {
                e.building++;
                if (e.building == 0) {
                    e.hp = clamp_hp(e.hp + 50); 
                }
            }
            if (e.info->unit == 1 && check_obstructed(e)) {
                e.hp = clamp_hp(e.hp + 20); 
            }
        }
    }
    if (player.researching != -1) {
        player.techs.push_back(player.researching);
        update_tech_lookup(player);
        player.researching = -1;
    }
}

void Gst::level_upgrade (Player &player) {
    for (Entity &e : entities) {
        if (get_player(e.owner) == player) {
            if (e.info->upgrade != -1 && e.info->level == player.level) {
                e.info = get_info(e.info->upgrade);
            }
        }
    }
}


void Gst::update_tech_lookup (Player &player) {
    player.tech_lookup.map_id.clear();
    for (int i : player.techs) {
        Tech *tech = get_tech(i);
        std::vector<int> ids { };
        if (tech->bonus.aff_id.size() > 0) {
            ids = tech->bonus.aff_id;
        } else {
            if (tech->bonus.aff_level != -1) {
                for (EntityInfo info : infos) {
                    if (info.level == tech->bonus.aff_level) {
                        ids.push_back(info.id);
                    }
                }
            }
            if (tech->bonus.aff_class.size() > 0) {
                for (EntityInfo info : infos) {
                    auto &cls = tech->bonus.aff_class;
                    if (std::find(cls.begin(), cls.end(), 
                        info.ent_class) != cls.end())                        
                    {
                        ids.push_back(info.id);
                    }
                }
            }
            if (tech->bonus.aff_all == 1) {
                for (EntityInfo info : infos) {
                    ids.push_back(info.id);
                }
            }
        }
        for (int id : ids) {
            player.tech_lookup.map_id[id] = 
                player.tech_lookup.map_id[id] + tech->bonus;
        }
    }
}
#include "gst.h"

#include <iostream>

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

float Gst::get_type_bonus (Entity &atk, Entity &def) {
    float b = 1;
    switch(atk.info->ent_class) {
        case EntityInfo::Class::inf: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += 1.0/3;
        if (def.info->ent_class == EntityInfo::Class::sie) b += 1.0/3;
        break;
        
        case EntityInfo::Class::cav: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += -0.5;
        if (def.info->ent_class == EntityInfo::Class::inf) b += 1.0/3;
        if (def.info->ent_class == EntityInfo::Class::ran) b += 1.0/3;
        break;
        
        case EntityInfo::Class::ran: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += -0.5;
        break;
        
        case EntityInfo::Class::sie: 
        if (def.info->ent_class == EntityInfo::Class::bld) b += +0.5;
        break;
    }
    return b;
}

std::vector<Bonus> Gst::get_bonuses (Entity &atk, Entity &def) {
    std::vector<Bonus> bs;
    bs.emplace_back(
        tiles[ground.tiles[ground.at(atk.x, atk.y)]].attack_bonus, 
        Bonus::Id::ground, true);
    bs.emplace_back(
        tiles[ground.tiles[ground.at(def.x, def.y)]].defence_bonus, 
        Bonus::Id::ground, false);
        
    bs.emplace_back(get_type_bonus(atk, def), Bonus::Id::type, true);
    bs.emplace_back(get_type_bonus(def, atk), Bonus::Id::type, false);
    
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
    
    
    return bs;
}

float Gst::get_damage (Entity &atk, Entity &def) {
    float atkmul = 1;
    float defmul = 1;
    
    auto bonuses = get_bonuses(atk, def);
    for (auto bonus : bonuses) {
        if (bonus.atk) { atkmul += bonus.amt; }
        else { defmul += bonus.amt; }
    }
    
    float dam = (atk.info->attack * atk.hp * atkmul) 
        / (2.0f*def.info->defence * defmul);
    
    return dam;
}

bool Gst::get_first_strike (Entity &atk, Entity &def) {
    bool fs { false };
    fs = info_has_ability(atk.info, "First Strike");
    return fs;
}

float clamp (float hp) { if (hp > 100) hp = 100; return hp; }

void Gst::battle (Entity &atk, Entity &def) {
    std::cout << "! attack " << atk.info->name << "(hp:" << atk.hp << "), "
    << def.info->name << "(hp:" << def.hp << ") \n";
    
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
        
    bool swap = false;
    if (first_strike_def && !first_strike_atk) swap = true;
    if (swap) {
        atk.hp = clamp(atk.hp - get_damage(def, atk));
        if (!info_has_ability(atk.info, "No Counter")) 
            if (atk.hp > 0) def.hp -= get_damage(atk, def);
    } else {
        def.hp = clamp(def.hp - get_damage(atk, def));
        if (!info_has_ability(def.info, "No Counter")) 
            if (def.hp > 0) atk.hp -= get_damage(def, atk);
    }
    
    if (info_has_ability(atk.info, "Rapid Fire")) 
        if (def.hp > 0) 
            def.hp = clamp(def.hp - get_damage(atk, def));
    
    if (info_has_ability(def.info, "Rapid Fire"))
        if (atk.hp > 0) 
            atk.hp = clamp(atk.hp - get_damage(def, atk));
    
    if (atk.hp > 0 && info_has_ability(atk.info, "Zeal")) 
        atk.hp = clamp(atk.hp + 20);
    if (def.hp > 0 && info_has_ability(def.info, "Zeal")) 
        def.hp = clamp(def.hp + 20);
    
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
    for (int id : info->adjacent) {
        bool adj = false;
        for (Entity &e : entities) {
            if (e.info->id == id) {
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
            if (e.info->id == 100) {
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
}
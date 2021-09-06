#include <iostream>
#include <fstream>
#include <vector>

#include "load.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

std::vector<Ability> load_abilities () {
    std::vector<Ability> abs;
    abs.emplace_back("Anti-Cavalry");
    abs.emplace_back("Skirmish");
    abs.emplace_back("First Strike");
    abs.emplace_back("Rapid Fire");
    abs.emplace_back("Units Only");
    abs.emplace_back("Buildings Only");
    abs.emplace_back("No Counter");
    abs.emplace_back("No Move & Attack");
    abs.emplace_back("Causes Fear");
    abs.emplace_back("Desert Charge");
    abs.emplace_back("Plains Charge");
    abs.emplace_back("Scares Horses");
    abs.emplace_back("Woodsman");
    abs.emplace_back("Volley");
    abs.emplace_back("Frenzy");
    abs.emplace_back("Zeal");
    abs.emplace_back("Scout");
    abs.emplace_back("Convert");
    abs.emplace_back("Heal");
    abs.emplace_back("Seasoned Veteran");
    return abs;
}

void load_json (Inv &inv) {    
    inv.abilities = load_abilities();

    std::ifstream file_tiles("content/tiles.json");
    json j_tiles; file_tiles >> j_tiles;
    for (auto it : j_tiles) {
        Tile tile;
        tile.name = it["name"];
        tile.move_cost = it["move_cost"];
        tile.defence_bonus = it["defence_bonus"];
        tile.range_bonus = it["range_bonus"];
        tile.spritebounds = vec2 { it["spritebounds"][0], it["spritebounds"][1] };
        inv.tiles.push_back(tile);
    }
    
    std::ifstream file_ents("content/entities.json");
    json j_ents; file_ents >> j_ents;
    for (auto it : j_ents) {
        EntityInfo ent;
        ent.id = it["id"];
        ent.name = it["name"];
        ent.level = it["level"];
        ent.range = it["range"];
        ent.move = it["move"];
        ent.attack = it["attack"];
        ent.defence = it["defence"];
        ent.sight = it["sight"];
        ent.unit = it["unit"];
        for (int i=0; i<it["prod"].size(); i++) {
            ent.prod[i] = it["prod"][i];
        }
        for (int i=0; i<it["cost"].size(); i++) {
            ent.cost[i] = it["cost"][i];
        }
        for (auto blds : it["build"]) {
            ent.build.push_back(blds);
        }
        for (auto train : it["train_id"]) {
            ent.train_id.push_back(train);
        }
        for (auto train : it["train_class"]) {
            ent.train_class.push_back((EntityInfo::Class) 
                EntityClass::from_string(train.get<std::string>()));
        }
        for (auto ad : it["adjacent"]) {
            ent.adjacent.push_back(ad);
        }
        for (auto ad : it["diagonal"]) {
            ent.diagonal.push_back(ad);
        }
        ent.ent_class = (EntityInfo::Class) EntityClass::from_string(
            it["class"].get<std::string>());
        for (auto ab : it["abilities"]) {
            int index = 0;
            for (int i=0; i<inv.abilities.size(); i++) {
                if (inv.abilities[i].name == ab) { index = i; break; }
            }
            ent.abilities.push_back(index);
        }
        ent.spritebounds = vec2 { it["spritebounds"][0], it["spritebounds"][1] };
        if (it.contains("upgrade")) { ent.upgrade = it["upgrade"]; }
        if (it.contains("defence_bonus")) { 
            ent.defence_bonus = it["defence_bonus"];
        }
        inv.infos.push_back(ent);
    }
    
    std::ifstream file_techs("content/techs.json");
    json j_techs; file_techs >> j_techs;
    for (auto it : j_techs) {
        Tech tech;
        tech.name = it["name"];
        tech.id = it["id"];
        tech.level = it["level"];
        tech.req_id = it["req_id"];
        for (int i=0; i<it["cost"].size(); i++) {
            tech.cost[i] = it["cost"][i];
        }
        auto b = it["bonus"];
        if (b != nullptr) {
            if (b.contains("attack")) { tech.bonus.attack = b["attack"]; }
            if (b.contains("defence")) { tech.bonus.defence = b["defence"]; }
            if (b.contains("sight")) { tech.bonus.sight = b["sight"]; }
            if (b.contains("move")) { tech.bonus.move = b["move"]; }
            if (b.contains("trade")) { tech.bonus.trade = b["trade"]; }
            if (b.contains("req_range")) { 
                tech.bonus.req_range = b["req_range"];
            }
            if (b.contains("improved_heal")) {
                tech.bonus.improved_heal = b["improved_heal"]; 
            }
            if (b.contains("improved_convert")) {
                tech.bonus.improved_convert = b["improved_convert"]; 
            }
            if (b.contains("cost")) tech.bonus.cost.clear();
            for (auto v : b["cost"]) { tech.bonus.cost.push_back(v); }
            if (b.contains("cost_abs")) tech.bonus.cost_abs.clear();
            for (auto v : b["cost_abs"]) { tech.bonus.cost_abs.push_back(v); }
            if (b.contains("prod")) tech.bonus.prod.clear();
            for (auto v : b["prod"]) { tech.bonus.prod.push_back(v); }
            
            for (auto v : b["aff_id"]) { tech.bonus.aff_id.push_back(v); }
            for (auto v : b["aff_class"]) { 
                tech.bonus.aff_class.push_back(EntityClass::from_string(
                    v.get<std::string>())); 
            }
            if (b.contains("aff_level")) { 
                tech.bonus.aff_level = b["aff_level"]; 
            }
            if (b.contains("aff_all")) { 
                tech.bonus.aff_all = b["aff_all"]; 
            }
        }
        inv.techs.push_back(tech);
    }
}
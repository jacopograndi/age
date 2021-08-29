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

void load_json (Gst &gst) {    
    gst.abilities = load_abilities();

    std::ifstream file_tiles("content/tiles.json");
    json j_tiles; file_tiles >> j_tiles;
    for (auto it : j_tiles) {
        Tile tile;
        tile.name = it["name"];
        tile.move_cost = it["move_cost"];
        tile.spritebounds = vec2 { it["spritebounds"][0], it["spritebounds"][1] };
        gst.tiles.push_back(tile);
    }
    
    std::ifstream file_ents("content/entities.json");
    json j_ents; file_ents >> j_ents;
    for (auto it : j_ents) {
        EntityInfo ent;
        ent.id = it["id"];
        ent.name = it["name"];
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
        for (auto train : it["train"]) {
            ent.train.push_back(train);
        }
        for (auto ad : it["adjacent"]) {
            ent.adjacent.push_back(ad);
        }
        std::string cl = it["class"];
        if (cl == "inf") ent.ent_class = EntityInfo::Class::inf;
        if (cl == "cav") ent.ent_class = EntityInfo::Class::cav;
        if (cl == "ran") ent.ent_class = EntityInfo::Class::ran;
        if (cl == "sie") ent.ent_class = EntityInfo::Class::sie;
        if (cl == "bld") ent.ent_class = EntityInfo::Class::bld;
        for (auto ab : it["abilities"]) {
            int index = 0;
            for (int i=0; i<gst.abilities.size(); i++) {
                if (gst.abilities[i].name == ab) { index = i; break; }
            }
            ent.abilities.push_back(index);
        }
        ent.spritebounds = vec2 { it["spritebounds"][0], it["spritebounds"][1] };
        gst.infos.push_back(ent);
    }
}
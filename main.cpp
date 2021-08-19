#include <iostream>
#include <fstream>
#include <vector>

#include "umath/vec2.h"

#include "graphics/graphics.h"
#include "commands/commands.h"
#include "timing/timing.h"
#include "game/entity.h"
#include "game/ground.h"
#include "game/playercontrol.h"
#include "game/gst.h"
#include "game/tile.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

// state representation
/*
    ground matrix
    . tile: {
        name
        range bonus
        travel cost
        defence bonus
        sight penalty
    }
    units list
    . unit: {
        name
        type
        attack
        defence
        life
        move points
        sight
        (morale?)
        ability list
        . abilities: [ ]
    }
    builds
    . build: {
        same as unit
        trainees list
    }
    tech
    resources (food, gold)
    trade bonus
*/

int main () {
    Graphics graphics (900, 600);
    Commands_sdl com;
    timing_sdl timing (60);
    
    Player_control control;
    
    float ts = 16;
    Gst gst (15,15);
    
    gst.players.emplace_back(255, 0, 0);
    gst.players.emplace_back(0, 0, 255);
    
    gst.ground.tiles[1] = 1;
    gst.ground.tiles[8] = 1;
    gst.ground.tiles[11] = 2;
    gst.ground.tiles[12] = 2;
    for (int i=0; i<2000; i+=37) {
        gst.ground.tiles[i%(gst.ground.sizex*gst.ground.sizey)] = 2;
        gst.ground.tiles[(i+1)%(gst.ground.sizex*gst.ground.sizey)] = 2;
    }
    for (int i=0; i<2000; i+=71) {
        gst.ground.tiles[i%(gst.ground.sizex*gst.ground.sizey)] = 1;
    }
    
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
        ent.name = it["name"];
        ent.move = it["move"];
        ent.spritebounds = vec2 { it["spritebounds"][0], it["spritebounds"][1] };
        gst.infos.push_back(ent);
    }
    
    gst.entities.emplace_back(1, 1, gst.infos[0], 0);
    gst.entities.emplace_back(2, 1, gst.infos[1], 0);
    gst.entities.emplace_back(10, 10, gst.infos[1], 1);
    
    View view (vec2 { (float)graphics.resx, (float)graphics.resy });
    
    vec2 mouselast { 0, 0 };
    
    while(true) {
        if (com.process_events()) break;
        vec2 mousepos { (float)com.mx, (float)com.my };
        vec2 mousedelta { mousepos };
        mousedelta -= mouselast;
        
        if (com.mclick[1]) {
            graphics.cam.pos += mousedelta;
        }
        
        mouselast = vec2 { (float)com.mx, (float)com.my };
        
        timing.process();
        while (timing.check_process()) {
            com.process_clicks();
            // input detection
            // turn fsm or ai
            // turn tick calculations
            /*
                economy
                tech
                upgrades
                win condition
            */ 
            view.process(gst, graphics.cam.pos, mousepos, com.mheld);
            if (com.check_key(SDL_SCANCODE_ESCAPE)) {
                view.back = 1;
            }
            
            control.process(gst, view);
            com.process_keys();
        }
        if (timing.check_render()) {
            graphics.render(gst, view);
            graphics.present();
        }
    }
	return 0;
}
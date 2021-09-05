#include <iostream>
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
#include "game/load.h"

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
    
    Inv inv;
    inv.ground.build(15,15);
    
    inv.ground.tiles[1] = 1;
    inv.ground.tiles[8] = 1;
    inv.ground.tiles[11] = 2;
    inv.ground.tiles[12] = 2;
    for (int i=0; i<2000; i+=37) {
        inv.ground.tiles[i%(inv.ground.sizex*inv.ground.sizey)] = 2;
        inv.ground.tiles[(i+1)%(inv.ground.sizex*inv.ground.sizey)] = 2;
    }
    for (int i=0; i<2000; i+=71) {
        inv.ground.tiles[i%(inv.ground.sizex*inv.ground.sizey)] = 1;
    }
    
    inv.ground.resources.emplace_back(inv.ground.at(3, 4), Resource::Type::gold);
    inv.ground.resources.emplace_back(inv.ground.at(4, 4), Resource::Type::food);
    
    Gst gst { &inv };
    
    load_json(inv);
    
    gst.players.emplace_back(255, 0, 0, 0);
    gst.players[0].res = std::vector<float> { 1500, 1500 };
    gst.players.emplace_back(0, 0, 255, 1);
    gst.players[1].res = std::vector<float> { 1500, 1500 };
    gst.players[0].level = 2;
    
    
    
    gst.entities.emplace_back(5, 1, inv.get_info("Town Center"), 0);
    gst.entities.emplace_back(6, 1, inv.get_info("Barracks"), 0);
    gst.entities.emplace_back(5, 0, inv.get_info("Stable"), 0);
    gst.entities.emplace_back(4, 1, inv.get_info("Market"), 0);
    gst.entities.emplace_back(1, 1, inv.get_info("Villager"), 0);
    gst.entities.emplace_back(2, 1, inv.get_info("Light Cavalry"), 0);
    gst.entities.emplace_back(4, 2, inv.get_info("Light Cavalry"), 1);
    gst.entities.emplace_back(10, 10, inv.get_info("Villager"), 1);
    gst.entities.emplace_back(1, 5, inv.get_info("Villager"), 1);
    gst.entities.emplace_back(2, 5, inv.get_info("Militia"), 1);
    gst.entities.emplace_back(0, 0, inv.get_info("Scout Cavalry"), 0);
    gst.entities.emplace_back(1, 0, inv.get_info("Persian War Elephants"), 0);
    gst.entities.emplace_back(0, 1, inv.get_info("Scorpions"), 1);
    gst.entities.emplace_back(1, 2, inv.get_info("Archers"), 1);
    gst.entities.emplace_back(4, 3, inv.get_info("Monks"), 0);
    
    View view (vec2 { (float)graphics.resx, (float)graphics.resy });
    
    vec2 mouselast { 0, 0 };
    
    while(true) {
        if (com.process_events()) break;
        if (com.resx != -1) { 
            graphics.change_res(com.resx, com.resy); 
            view.res = vec2 { (float)com.resx, (float)com.resy };
        }
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
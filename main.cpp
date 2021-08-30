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
    Gst gst (15,15);
    
    load_json(gst);
    
    gst.players.emplace_back(255, 0, 0);
    gst.players[0].res = std::vector<int> { 1500, 1500 };
    gst.players.emplace_back(0, 0, 255);
    gst.players[1].res = std::vector<int> { 1500, 1500 };
    
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
    
    gst.ground.resources.emplace_back(gst.ground.at(3, 4), Resource::Type::gold);
    gst.ground.resources.emplace_back(gst.ground.at(4, 4), Resource::Type::food);
    
    
    
    gst.entities.emplace_back(5, 1, gst.get_info("Town Center"), 0);
    gst.entities.emplace_back(1, 1, gst.get_info("Villager"), 0);
    gst.entities.emplace_back(2, 1, gst.get_info("Light Cavalry"), 0);
    gst.entities.emplace_back(10, 10, gst.get_info("Villager"), 1);
    gst.entities.emplace_back(1, 5, gst.get_info("Villager"), 1);
    gst.entities.emplace_back(2, 5, gst.get_info("Militia"), 1);
    gst.entities.emplace_back(0, 0, gst.get_info("Scout Cavalry"), 0);
    gst.entities.emplace_back(1, 0, gst.get_info("Persian War Elephants"), 0);
    gst.entities.emplace_back(0, 1, gst.get_info("Scorpions"), 1);
    gst.entities.emplace_back(1, 2, gst.get_info("Archers"), 1);
    
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
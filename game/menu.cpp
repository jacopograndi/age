#include <iostream>

#include "menu.h"

void Menu::close () {
    active = false;
}

void Menu::open (vec2 res) {
    over = -1;
    active = true;
    pos = vec2 { (float)res.x, (float)res.y };
    float height = options.size() * 20;
    size = vec2 { 150, height+10 };
    pos *= 0.5f;
    pos -= size/2;
}

int Menu::mouse_option (vec2 mouse) {
    int i=0;
    for (Option opt : options) {
        vec2 off { 0, 5.0f + i*20 };
        vec2 sizeopt { 150, 20 };
        off += pos;
        if (off.x < mouse.x && mouse.x < off.x+sizeopt.x 
        && off.y < mouse.y && mouse.y < off.y+sizeopt.y ) {
            return opt.id;
        }
        i++;
    }
    return -1;
}


void Menu_tech::open (vec2 res) {
    over = -1;
    active = true;
    pos = vec2 { (float)res.x, (float)res.y };
    
    tech_opt_ordered.clear();
    tech_opt_ordered.emplace_back();
    tech_opt_ordered.emplace_back();
    tech_opt_ordered.emplace_back();
    tech_opt_ordered.emplace_back();
    
    for (OptionTech opt : tech_options) {
        tech_opt_ordered[opt.tech->level].emplace_back(opt);
    }
    int maxsize = 0;
    for (auto v : tech_opt_ordered) { 
        maxsize = maxsize < v.size() ? v.size() : maxsize;
    }
    
    float width = 150 * tech_opt_ordered.size();
    float height = maxsize * 10;
    size = vec2 { width, height+20 };
    pos *= 0.5f;
    pos -= size/2;
}

int Menu_tech::mouse_option (vec2 mouse) {
    float x = 0, y = 0;
    for (auto v : tech_opt_ordered) {
        for (auto opt : v) {
            vec2 off { x*150, 10.0f + y*10 };
            vec2 sizeopt { 150, 10 };
            off += pos;
            if (off.x < mouse.x && mouse.x < off.x+sizeopt.x 
            && off.y < mouse.y && mouse.y < off.y+sizeopt.y ) {
                return opt.tech->id;
            }
            y++;
        }
        x++; y=0;
    }
    return -1;
}
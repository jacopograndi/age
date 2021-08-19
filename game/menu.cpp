#include "menu.h"

void Menu::close () {
    active = false;
}

void Menu::open (vec2 res) {
    active = true;
    pos = vec2 { (float)res.x, (float)res.y };
    float height = options.size() * 20;
    size = vec2 { 120, height+10 };
    pos *= 0.5f;
    pos -= size/2;
}

int Menu::mouse_option (vec2 mouse) {
    int i=0;
    for (Option opt : options) {
        vec2 off { 10, 10.0f + i*20 };
        vec2 sizeopt { 100, 20 };
        off += pos;
        if (off.x < mouse.x && mouse.x < off.x+sizeopt.x 
        && off.y < mouse.y && mouse.y < off.y+sizeopt.y ) {
            return opt.id;
        }
        i++;
    }
    return -1;
}
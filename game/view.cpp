#include "view.h"

void View::process (Gst &gst, vec2 cam, vec2 mouse, int *mheld) {
    Ground &gr = gst.ground;
    std::vector<Entity> &entities = gst.entities;
    
    vec2 absmouse { mouse }; 
    absmouse -= cam;

    cursor_ground = -1;
    cursor_entity = -1;
    back = -1;
    opt = -1;
    
    if (mheld[0] == 1) {
        bool found = false;  
        
        if (moves.size() > 0 && !found) {
            for (int i=0; i<moves.size() && !found; i++) {
                int x = moves[i] % gr.sizex;
                int y = moves[i] / gr.sizex;
                vec2 pos { (float)x*32, (float)y*32 };
                if (pos.x < absmouse.x && absmouse.x < pos.x+32 
                && pos.y < absmouse.y && absmouse.y < pos.y+32) 
                {
                    cursor_ground = moves[i];
                    found = true;
                }
            }
        }

        if (menu_unit.active && !found) {
            int selected = menu_unit.mouse_option(mouse);
            if (selected != -1) {
                opt = selected;
                found = true;
            } else {
                back = 1;
                found = 1;
            }
        }

        if (menu_day.active && !found) {
            int selected = menu_day.mouse_option(mouse);
            if (selected != -1) {
                opt = selected;
                found = true;
            } else {
                back = 1;
                found = 1;
            }
        }
        
        for (int i=0; i<entities.size() && !found; i++) {
            if (entities[i].done) continue;
            if (entities[i].owner != gst.day) continue;
            vec2 pos { (float)entities[i].x*32, (float)entities[i].y*32 };
            if (pos.x < absmouse.x && absmouse.x < pos.x+32 
            && pos.y < absmouse.y && absmouse.y < pos.y+32) 
            {
                cursor_entity = i;
                found = true;
            }
        }
        for (int y=0; y<gr.sizey && !found; y++) {
            for (int x=0; x<gr.sizex && !found; x++) {
                vec2 pos { (float)x*32, (float)y*32 };
                if (pos.x < absmouse.x && absmouse.x < pos.x+32 
                && pos.y < absmouse.y && absmouse.y < pos.y+32) 
                {
                    cursor_ground = x+y*gr.sizex;
                    found = true;
                }
            }
        }
    }
}
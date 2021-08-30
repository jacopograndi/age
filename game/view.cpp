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
    hover_ground = -1;
    
    bool hfound = false;
    for (int y=0; y<gr.sizey && !hfound; y++) {
        for (int x=0; x<gr.sizex && !hfound; x++) {
            vec2 pos { (float)x*32, (float)y*32 };
            if (pos.x < absmouse.x && absmouse.x <= pos.x+32 
            && pos.y < absmouse.y && absmouse.y <= pos.y+32) 
            {
                hover_ground = x+y*gr.sizex;
                hfound = true;
            }
        }
    }
    
    if (menu_train.active) {
        menu_train.over = menu_train.mouse_option(mouse);
    }
    if (menu_build.active) {
        menu_build.over = menu_build.mouse_option(mouse);
    }
    if (menu_unit.active) {
        menu_unit.over = menu_unit.mouse_option(mouse);
    }
    if (menu_day.active) {
        menu_day.over = menu_day.mouse_option(mouse);
    }
    if (menu_tech.active) {
        menu_tech.over = menu_tech.mouse_option(mouse);
    }
    
    if (mheld[0] == 1) {
        bool found = false;  
        
        if (moves.size() > 0 && !found) {
            for (int i=0; i<moves.size() && !found; i++) {
                int x = moves[i] % gr.sizex;
                int y = moves[i] / gr.sizex;
                int valid = 1;
                for (Entity &e : gst.entities) {
                    if (e.x == x && e.y == y && e.info->unit == 1) {
                        valid = 0;
                    }
                }
                if (!valid) continue;
                vec2 pos { (float)x*32, (float)y*32 };
                if (pos.x < absmouse.x && absmouse.x <= pos.x+32 
                && pos.y < absmouse.y && absmouse.y <= pos.y+32) 
                {
                    cursor_ground = moves[i];
                }
            }
            found = true;
        }
        
        if (attacks.size() > 0 && !found) {
            for (int i=0; i<attacks.size() && !found; i++) {
                int x = attacks[i] % gr.sizex;
                int y = attacks[i] / gr.sizex;
                vec2 pos { (float)x*32, (float)y*32 };
                if (pos.x < absmouse.x && absmouse.x <= pos.x+32 
                && pos.y < absmouse.y && absmouse.y <= pos.y+32) 
                {
                    cursor_ground = attacks[i];
                }
            }
            found = true;
        }

        if (menu_train.active && !found) {
            int selected = menu_train.mouse_option(mouse);
            if (selected != -1) {
                opt = selected; found = true;
            } else {
                back = 1; found = 1;
            }
        }

        if (menu_build.active && !found) {
            int selected = menu_build.mouse_option(mouse);
            if (selected != -1) {
                opt = selected; found = true;
            } else {
                back = 1; found = 1;
            }
        }

        if (menu_unit.active && !found) {
            int selected = menu_unit.mouse_option(mouse);
            if (selected != -1) {
                opt = selected; found = true;
            } else {
                back = 1; found = 1;
            }
        }

        if (menu_day.active && !found) {
            int selected = menu_day.mouse_option(mouse);
            if (selected != -1) {
                opt = selected; found = true;
            } else {
                back = 1; found = 1;
            }
        }

        if (menu_tech.active && !found) {
            int selected = menu_tech.mouse_option(mouse);
            if (selected != -1) {
                opt = selected; found = true;
            } else {
                back = 1; found = 1;
            }
        }
        
        for (int i=0; i<entities.size() && !found; i++) {
            if (entities[i].done) continue;
            if (entities[i].owner != gst.turn) continue;
            vec2 pos { (float)entities[i].x*32, (float)entities[i].y*32 };
            if (pos.x < absmouse.x && absmouse.x <= pos.x+32 
            && pos.y < absmouse.y && absmouse.y <= pos.y+32) 
            {
                cursor_entity = i;
                if (entities[i].info->unit == 1) {
                    found = true;
                }
            }
        }
        if (cursor_entity != -1) found = true;
        
        for (int y=0; y<gr.sizey && !found; y++) {
            for (int x=0; x<gr.sizex && !found; x++) {
                vec2 pos { (float)x*32, (float)y*32 };
                if (pos.x < absmouse.x && absmouse.x <= pos.x+32 
                && pos.y < absmouse.y && absmouse.y <= pos.y+32) 
                {
                    cursor_ground = x+y*gr.sizex;
                    found = true;
                }
            }
        }
    }
}
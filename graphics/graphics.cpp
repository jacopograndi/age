#include <algorithm>
#include <iostream>
#include <string>

#include <math.h>

#include "graphics.h"
#include "../game/menu.h"
#include "../game/tile.h"
#include "../game/constants.h"

Graphics::~Graphics () {
}

void Graphics::present () {
    backend.present();
}

void render_ent (Graphics *graphics, Gst &gst, Entity &ent, 
    vec2 pos, int dflag) 
{
    Player &player = gst.players[ent.owner];
    float bound_y = ent.info->spritebounds.y;
    if (ent.info->unit == 0) { bound_y = player.level*16+16; }
    int done = 0;
    if (dflag == 1) done = 512;
    graphics->backend.render_sprite(
        (int)ent.info->spritebounds.x,
        (int)bound_y+done, 16, 16,
        (int)pos.x, (int)pos.y, 32, 32
    );
    if (dflag == 0) {
        graphics->backend.render_sprite(
            (int)ent.info->spritebounds.x,
            (int)bound_y+768, 16, 16,
            (int)pos.x, (int)pos.y, 32, 32,
            player.r, player.g, player.b
        );
    }
}

void render_ents (Graphics *graphics, Gst &gst, int unit) {
    std::vector<Entity> &entities = gst.entities;
    for (int i=0; i<entities.size(); i++) {
        if (entities[i].info->unit != unit) continue;
        Player &player = gst.players[entities[i].owner];
        int dflag = 0;
        if (unit) dflag = entities[i].done;
        vec2 pos {
            graphics->cam.pos.x + entities[i].x*32, 
            graphics->cam.pos.y + entities[i].y*32
        };
        render_ent(graphics, gst, entities[i], pos, dflag);
    }
}

void render_menu (Graphics *graphics, Gst &gst, Menu &menu) {
    vec2 res { (float)graphics->resx, (float)graphics->resy };
    if (menu.active) {
        vec2 pos { res };
        float height = menu.options.size() * 20;
        vec2 size { 120, height+10 };
        pos *= 0.5f;
        pos -= size/2;
        graphics->backend.render_rect(
            255,255,255,255,
            menu.pos.x,menu.pos.y,menu.size.x,menu.size.y
        );
        float acc = 0;
        for (Option opt : menu.options) {
            int r=0, g=0, b=0;
            if (opt.id == menu.over) {
                Player &player = gst.players[gst.turn];
                r = player.r; g = player.g; b = player.b;
            }
            graphics->backend.txt.render_text(
                opt.name, menu.pos + vec2 {10, 10 + acc}, r, g, b);
            float width = graphics->backend.txt.get_width(opt.name);
            if (opt.cost.size() > 0) {
                graphics->backend.txt.render_text(
                    std::to_string((int)roundf(opt.cost[0])) + "f", 
                    menu.pos + vec2 { width + 20, 10 + acc });
                graphics->backend.txt.render_text(
                    std::to_string((int)roundf(opt.cost[1])) + "g", 
                    menu.pos + vec2 { width + 50, 10 + acc });
            }
            acc += 20;
        }
    }
}

void render_menu_tech (Graphics *graphics, Gst &gst, View &view) {
    vec2 res { (float)graphics->resx, (float)graphics->resy };
    if (view.menu_tech.active) {
        vec2 pos { res };
        float height = view.menu_tech.options.size() * 20;
        vec2 size { 120, height+10 };
        pos *= 0.5f;
        pos -= size/2;
        graphics->backend.render_rect(
            255,255,255,255,
            view.menu_tech.pos.x,view.menu_tech.pos.y,
            view.menu_tech.size.x,view.menu_tech.size.y
        );
        
        float x = 0, y = 0;
        for (auto v : view.menu_tech.tech_opt_ordered) {
            for (auto opt : v) {
                int r=0, g=0, b=0;
                Player &player = gst.players[gst.turn];
                if (opt.tech->id == view.menu_tech.over) {
                    r = player.r; g = player.g; b = player.b;
                }
                if (!gst.check_req_tech(opt.tech, player)) {
                    r = 100; g = 100; b = 100;
                }
                if (player.has_tech(opt.tech->id)) {
                    r = constants::col_gud_r; 
                    g = constants::col_gud_g; 
                    b = constants::col_gud_b;
                }
                graphics->backend.txt.render_text(opt.name, 
                    view.menu_tech.pos + vec2 {10 + x, 10 + y}, r, g, b);
                float width = graphics->backend.txt.get_width(opt.name);
                /*
                if (opt.tech->cost.size() > 0) {
                    graphics->backend.txt.render_text(
                        std::to_string (opt.tech->cost[0]) + "f", 
                        view.menu_tech.pos + vec2 { width + 20 + x, 10 + y });
                    graphics->backend.txt.render_text(
                        std::to_string (opt.tech->cost[1]) + "g", 
                        view.menu_tech.pos + vec2 { width + 50 + x, 10 + y });
                }*/
                y += 10;
            }
            x += 150; y = 0;
        }
    }
}


int get_entity_info_height (Entity &ent) {
    return 155+10*ent.info->abilities.size();
}

void render_entity_info (Graphics *graphics, Gst &gst, vec2 pos, int i) {
    Entity &ent = gst.entities[i];
    Player &player = gst.players[ent.owner];
    auto &tech = player.tech_lookup;
    int id = ent.info->id;
    int w = 200, h = get_entity_info_height(ent);
    
    graphics->backend.render_rect (0,0,0,255,
        (int)pos.x,(int)pos.y,w,h);
    graphics->backend.render_rect (255,255,255,255,
        (int)pos.x+1,(int)pos.y+1,w-2,h-2);
    
    graphics->backend.render_rect (
        0,0,0,255, (int)pos.x+w-32-10-1, (int)pos.y+10-1,34,34);
    graphics->backend.render_rect (
        255,255,255,255, (int)pos.x+w-32-10, (int)pos.y+10,32,32);
    
    render_ent(graphics, gst, ent, vec2 { w-32-10.0f, 10 } + pos, 0);
    
    graphics->backend.txt.render_text(ent.info->name, pos + vec2 { 10, 10 });
    graphics->backend.txt.render_text("Attack", pos + vec2 { 10, 30 });
    float attack = ent.info->attack * (1+tech.id(id).attack);
    graphics->backend.txt.render_text(
        std::to_string((int)roundf(attack)), pos + vec2 { 90, 30 });
    graphics->backend.txt.render_text("Defence", pos + vec2 { 10, 45 });
    float defence = ent.info->defence * (1+tech.id(id).defence);
    graphics->backend.txt.render_text(
        std::to_string((int)roundf(defence)), pos + vec2 { 90, 45 });
    graphics->backend.txt.render_text("Move", pos + vec2 { 10, 60 });
    int move = ent.info->move + tech.id(id).move;
    graphics->backend.txt.render_text(
        std::to_string(move), pos + vec2 { 90, 60 });
    graphics->backend.txt.render_text("Range", pos + vec2 { 10, 75 });
    int range = ent.info->range + tech.id(id).range;
    graphics->backend.txt.render_text(
        std::to_string(range), pos + vec2 { 90, 75 });
    graphics->backend.txt.render_text("Sight", pos + vec2 { 10, 90 });
    int sight = ent.info->sight + tech.id(id).sight;
    graphics->backend.txt.render_text(
        std::to_string(sight), pos + vec2 { 90, 90 });
    graphics->backend.txt.render_text("Health", pos + vec2 { 10, 105 });
    graphics->backend.txt.render_text(
        std::to_string((int)roundf(ent.hp)), pos + vec2 { 90, 105 });
    graphics->backend.txt.render_text("Class", pos + vec2 { 10, 120 });
    std::string ent_class = "";
    switch (ent.info->ent_class) {
        case EntityInfo::Class::inf: ent_class = "Infantry"; break;
        case EntityInfo::Class::cav: ent_class = "Cavalry"; break;
        case EntityInfo::Class::ran: ent_class = "Ranged"; break;
        case EntityInfo::Class::sie: ent_class = "Siege"; break;
        case EntityInfo::Class::bld: ent_class = "Building"; break;
    }
    graphics->backend.txt.render_text(ent_class, pos + vec2 { 90, 120 });
    graphics->backend.txt.render_text("Abilities", pos + vec2 { 10, 135 });
    std::string abname;
    for (int s=0; s<ent.info->abilities.size(); s++) {
        abname = gst.abilities[ent.info->abilities[s]].name;
        graphics->backend.txt.render_text(abname, 
            pos + vec2 { 90, 135+s*10.0f });
    }
}

void render_tile_info (Graphics *graphics, Gst &gst, vec2 pos, int i) {
    int x = i % gst.ground.sizex;
    int y = i / gst.ground.sizex;
    Tile &tile = gst.tiles[gst.ground.tiles[gst.ground.at(x,y)]];
    
    int w = 200, h = 95;
    graphics->backend.render_rect (
        0,0,0,255,
        (int)pos.x,(int)pos.y,w,h
    );
    graphics->backend.render_rect (
        255,255,255,255,
        (int)pos.x+1,(int)pos.y+1,w-2,h-2
    );
    
    graphics->backend.render_sprite (
        (int)tile.spritebounds.x,
        (int)tile.spritebounds.y-16, 16, 32, 
        (int)pos.x + w-32-10, 
        (int)pos.y + 10-32, 32, 64
    );

    graphics->backend.txt.render_text(tile.name, pos + vec2 { 10, 10 });
    graphics->backend.txt.render_text("Move", pos + vec2 { 10, 30 });
    graphics->backend.txt.render_text(
        std::to_string(tile.move_cost), pos + vec2 { 90, 30 });
    graphics->backend.txt.render_text("Sight", pos + vec2 { 10, 45 });
    graphics->backend.txt.render_text(
        std::to_string(tile.sight_cost), pos + vec2 { 90, 45 });
    graphics->backend.txt.render_text("Defence Bonus", pos + vec2 { 10, 60 });
    graphics->backend.txt.render_text(
        std::to_string((int)roundf(tile.defence_bonus*100))+"%", pos + vec2 { 90, 60 });
    graphics->backend.txt.render_text("Range Bonus", pos + vec2 { 10, 75 });
    graphics->backend.txt.render_text(
        std::to_string(tile.range_bonus), pos + vec2 { 90, 75 });
}

int render_attack_info_bonus (Graphics *graphics, Gst &gst, vec2 pos, 
    Entity &atk, Entity &def, bool attack, int rtl) 
{
    float margin_amt = constants::menu_attack_margin_amt;
    float margin_arrow = constants::menu_attack_margin_arrow;
    float margin_mod = constants::menu_attack_margin_mod;
    
    std::vector<Bonus> bonuses;
    if (attack) { bonuses = gst.get_bonuses(atk, def); }
    else { bonuses = gst.get_bonuses(def, atk); }
    
    float value = attack ? atk.info->attack : atk.info->defence;
    
    {
        std::string nlabel = attack ? "Attack" : "Defence";
        float labw = graphics->backend.txt.get_width(nlabel);
        graphics->backend.txt.render_text(nlabel, 
            pos + vec2 { -labw*rtl, 0 });
    } {
        std::string nlabel = std::to_string((int)roundf(value));
        float labw = graphics->backend.txt.get_width(nlabel);
        graphics->backend.txt.render_text(nlabel, 
            pos + vec2 { margin_amt*(1-rtl*2)-labw*rtl, 0 });
    }
    
    int bonusnum = 0; float atk_mod = 1;
    for (int s=0; s<bonuses.size(); s++) {
        Bonus &b = bonuses[s];
        if (b.atk == attack) {
            float idw = graphics->backend.txt.get_width(b.id_string());
            graphics->backend.txt.render_text(
                b.id_string(), pos + vec2 { -idw*rtl, 10.0f+bonusnum*10 });
            std::string amt;
            if (b.amt > 0) amt += "+";
            amt += std::to_string((int)roundf(b.amt*100)) + "%";
            float labw = graphics->backend.txt.get_width(amt);
            graphics->backend.txt.render_text(
                amt, pos + vec2 { 
                    margin_amt*(1-rtl*2)-labw*rtl, 
                    10.0f+bonusnum*10 });
            atk_mod += b.amt;
            bonusnum ++;
        }
    }
    
    if (bonusnum > 0) {
        {
            std::string label = rtl ? "<-" : "->";
            float labw = graphics->backend.txt.get_width(label);
            graphics->backend.txt.render_text(label, 
                pos + vec2 { margin_arrow*(1-rtl*2)-labw*rtl, 0 });
        }
        {
            std::string label = std::to_string((int)roundf(value * atk_mod));
            float labw = graphics->backend.txt.get_width(label);
            graphics->backend.txt.render_text(label, 
                pos + vec2 { margin_mod*(1-rtl*2)-labw*rtl, 0 });
        }
    }
    
    return bonusnum*10;
}


void render_attack_info (Graphics *graphics, Gst &gst, vec2 pos, int i, int j) {
    float margin_amt = constants::menu_attack_margin_amt;
    float margin_arrow = constants::menu_attack_margin_arrow;
    float margin_mod = constants::menu_attack_margin_mod;
    
    Entity &atk = gst.entities[i];
    Entity &def = gst.entities[j];
    int w = 300, h = 250;
    
    graphics->backend.render_rect (0,0,0,255,
        (int)pos.x,(int)pos.y,w,h);
    graphics->backend.render_rect (255,255,255,255,
        (int)pos.x+1,(int)pos.y+1,w-2,h-2);
    
    graphics->backend.render_rect (
        0,0,0,255, (int)pos.x+10-1, (int)pos.y+35-1,34,34);
    graphics->backend.render_rect (
        255,255,255,255, (int)pos.x+10, (int)pos.y+35,32,32);
    graphics->backend.render_sprite (
        (int)atk.info->spritebounds.x,
        (int)atk.info->spritebounds.y, 16, 16, 
        (int)pos.x + 10, 
        (int)pos.y + 35, 32, 32
    );
    {
        Tile &tile = gst.tiles[gst.ground.tiles[gst.ground.at(atk.x,atk.y)]];
        graphics->backend.render_sprite (
            (int)tile.spritebounds.x,
            (int)tile.spritebounds.y-16, 16, 32, 
            (int)pos.x + 52, 
            (int)pos.y + 35-32, 32, 64
        );
    }
    
    graphics->backend.render_rect (
        0,0,0,255, (int)pos.x+w-42-1, (int)pos.y+35-1,34,34);
    graphics->backend.render_rect (
        255,255,255,255, (int)pos.x+w-42, (int)pos.y+35,32,32);
    graphics->backend.render_sprite (
        (int)def.info->spritebounds.x,
        (int)def.info->spritebounds.y, 16, 16, 
        (int)pos.x + w-42, 
        (int)pos.y + 35, 32, 32
    );
    {
        Tile &tile = gst.tiles[gst.ground.tiles[gst.ground.at(def.x,def.y)]];
        graphics->backend.render_sprite (
            (int)tile.spritebounds.x,
            (int)tile.spritebounds.y-16, 16, 32, 
            (int)pos.x + w-84, 
            (int)pos.y + 35-32, 32, 64
        );
    }
    
    graphics->backend.txt.render_text(atk.info->name, pos + vec2 { 10, 10 });
    {
        int txtwidth = graphics->backend.txt.get_width(def.info->name);
        graphics->backend.txt.render_text(def.info->name, 
            pos + vec2 { w-10.0f-txtwidth, 10 });
    }
    
    BattleResult result = gst.battle_res(atk, def);
    std::vector<std::string> sres = { "++", "+", "=", "-", "--" };
    float atk_loss = atk.hp-result.atk_hp;
    float def_loss = def.hp-result.def_hp;
    float est_atk = def_loss - atk_loss;
    float est_def = atk_loss - def_loss;
    int atk_res = 2;
    int def_res = 2;
    if (est_atk > 40) { atk_res = 0; }
    else if (est_atk > 5) { atk_res = 1; }
    if (est_atk < -40) { atk_res = 4; }
    else if (est_atk < -5) { atk_res = 3; }
    if (est_def > 40) { def_res = 0; }
    else if (est_def > 5) { def_res = 1; }
    if (est_def < -40) { def_res = 4; }
    else if (est_def < -5) { def_res = 3; }
    
    graphics->backend.txt.render_text("Health", pos + vec2 { 10, 80 });
    graphics->backend.txt.render_text(
        std::to_string((int)roundf(atk.hp)), 
        pos + vec2 { 10+margin_amt, 80 });
    graphics->backend.txt.render_text("->", 
        pos + vec2 { 10+margin_arrow, 80 });
    graphics->backend.txt.render_text(
        std::to_string((int)roundf(result.atk_hp)), 
        pos + vec2 { 10+margin_mod, 80 });
    graphics->backend.txt.render_text(
        sres[atk_res],
        pos + vec2 { 10+margin_mod+20, 80 });
    {
        int txtwidth = graphics->backend.txt.get_width("Health");
        graphics->backend.txt.render_text("Health", 
            pos + vec2 { w-10.0f-txtwidth, 80 });
        std::string label = std::to_string((int)roundf(def.hp));
        int txtwidthlabel = graphics->backend.txt.get_width(label);
        graphics->backend.txt.render_text(label, 
            pos + vec2 { w-10.0f-margin_amt-txtwidthlabel, 80 });
    } {
        int txtwidth = graphics->backend.txt.get_width("<-");
        graphics->backend.txt.render_text("<-", 
            pos + vec2 { w-10.0f-margin_arrow-txtwidth, 80 });
    } {
        std::string label = std::to_string((int)roundf(result.def_hp));
        int txtwidth = graphics->backend.txt.get_width(label);
        graphics->backend.txt.render_text(label, 
            pos + vec2 { w-10.0f-margin_mod-txtwidth, 80 });
    } {
        int txtwidth = graphics->backend.txt.get_width(sres[def_res]);
        graphics->backend.txt.render_text(sres[def_res], 
            pos + vec2 { w-10.0f-margin_mod-20-txtwidth, 80 });
    }
    
    {
        int batkh = render_attack_info_bonus(graphics, gst, 
            pos + vec2 { 10, 95 }, atk, def, true, 0);
        int bdefh = render_attack_info_bonus(graphics, gst, 
            pos + vec2 { 10, 95 + batkh + 15.0f }, atk, def, false, 0);
    } {
        int batkh = render_attack_info_bonus(graphics, gst, 
            pos + vec2 { w-10.0f, 95 }, def, atk, true, 1);
        int bdefh = render_attack_info_bonus(graphics, gst, 
            pos + vec2 { w-10.0f, 95 + batkh + 15.0f }, def, atk, false, 1);
    }
}

void Graphics::render (Gst &gst, View &view) 
{
    Ground &gr = gst.ground;
    std::vector<Entity> &entities = gst.entities;
    vec2 res { (float)resx, (float)resy };
    
    for (int y=0; y<gr.sizey; y++) {
        for (int x=0; x<gr.sizex; x++) {
            Tile &tile = gst.tiles[gr.tiles[gr.at(x,y)]];
            backend.render_sprite(
                (int)tile.spritebounds.x,
                (int)tile.spritebounds.y, 16, 16, 
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
    
    for (Resource res : gst.ground.resources) {
        int x = res.pos % gr.sizex;
        int y = res.pos / gr.sizex;
        backend.render_sprite(
            16*res.kind, 16*2, 16, 16, 
            (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
        );
    }
    
    render_ents(this, gst, 0);
    render_ents(this, gst, 1);
    
    /* overlay */
    for (int y=0; y<gr.sizey; y++) {
        for (int x=0; x<gr.sizex; x++) {
            Tile &tile = gst.tiles[gr.tiles[gr.at(x,y)]];
            backend.render_sprite(
                (int)tile.spritebounds.x,
                (int)tile.spritebounds.y-16, 16, 16, 
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32-32, 32, 32
            );
        }
    }
    
    if (view.selected_entity != -1) {
        int i = view.selected_entity;
        Entity &ent = entities[i];
        Player &player = gst.get_player(ent.owner);
        float bound_y = ent.info->spritebounds.y;
        if (ent.info->unit == 0) { bound_y = player.level*16+16; }
        vec2 pos { (float)ent.x*32, (float)ent.y*32 };
        backend.render_sprite (
            (int)ent.info->spritebounds.x,
            (int)bound_y+256, 16, 16, 
            (int)cam.pos.x + (int)pos.x, 
            (int)cam.pos.y + (int)pos.y, 32, 32
        );
    } 
    
    if (view.selected_ground != -1) {
        int x = view.selected_ground % gr.sizex;
        int y = view.selected_ground / gr.sizex;
        vec2 pos { (float)x*32, (float)y*32 };
        Tile &tile = gst.tiles[gr.tiles[gr.at(x,y)]];
        backend.render_sprite(
            (int)tile.spritebounds.x,
            (int)tile.spritebounds.y+256-16, 16, 32, 
            (int)cam.pos.x + (int)pos.x, 
            (int)cam.pos.y + (int)pos.y-32, 32, 64
        );
    } 
    
    for (int i=0; i<entities.size(); i++) {
        Entity &ent = entities[i];
        if (ent.info->unit == 1) {
            backend.render_rect(
                0, 0, 0, 255,
                (int)cam.pos.x + (int)ent.x*32+2, 
                (int)cam.pos.y + (int)ent.y*32+30, 28, 2
            );
            int amt = 28 * (entities[i].hp / 100);
            Player &player = gst.get_player(ent.owner);
            backend.render_rect(
                player.r, player.g, player.b, 255,
                (int)cam.pos.x + (int)ent.x*32+2, 
                (int)cam.pos.y + (int)ent.y*32+30, amt, 2
            );
        }
    }
    
    render_menu(this, gst, view.menu_unit);
    render_menu(this, gst, view.menu_day);
    render_menu(this, gst, view.menu_build);
    render_menu(this, gst, view.menu_train);
    render_menu(this, gst, view.menu_trade);
    render_menu(this, gst, view.menu_age_up);
    render_menu_tech(this, gst, view);
    
    if (view.moves.size() > 0) {
        for (int m : view.moves) {
            int x = m % gr.sizex; int y = m / gr.sizex;
            backend.render_rect(
                0, 120, 255, 100,
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
    
    if (view.attacks.size() > 0) {
        for (int m : view.attacks) {
            int x = m % gr.sizex; int y = m / gr.sizex;
            backend.render_rect(
                255, 120, 0, 100,
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
    
    if (view.heals.size() > 0) {
        for (int m : view.heals) {
            int x = m % gr.sizex; int y = m / gr.sizex;
            backend.render_rect(
                0, 200, 0, 100,
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
    
    if (view.converts.size() > 0) {
        for (int m : view.converts) {
            int x = m % gr.sizex; int y = m / gr.sizex;
            backend.render_rect(
                200, 0, 200, 100,
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
    
    // top bar
    vec2 pos { 0,0 };
    backend.render_rect (
        255,255,255,255,
        (int)pos.x,(int)pos.y,(int)res.x, 30
    );
    Player &player = gst.players[gst.turn];
    backend.render_rect (
        player.r, player.g, player.b, 255,
        (int)pos.x+5,(int)pos.y+5, 20, 20
    );
    std::string txtfood = std::to_string((int)roundf(player.res[0])) + "f";
    std::string txtgold = std::to_string((int)roundf(player.res[1])) + "g";
    backend.txt.render_text(
        txtfood, pos + vec2 { -backend.txt.get_width(txtfood) + res.x/2-10, 10 }
    );
    backend.txt.render_text(txtgold, pos + vec2 { res.x/2+10, 10 });
    std::string txtres = "Researching: ";
    if (player.researching != -1) { 
        txtres += gst.get_tech(player.researching)->name; 
    } else { txtres += "None"; }
    float reswidth = backend.txt.get_width(txtres);
    backend.txt.render_text(txtres, pos + vec2 { res.x-reswidth-10, 10 });
    
    // low bar
    backend.render_rect (
        255,255,255,255,
        0,(int)res.y-30,(int)res.x, 30
    );
    
    // info stack box
    int info_ground = -1;
    if (view.hover_ground != -1) { info_ground = view.hover_ground; }
    if (view.selected_entity != -1) { 
        int i = view.selected_entity;
        info_ground = gst.ground.at(entities[i].x, entities[i].y);
    }
    if (view.selected_ground != -1) { info_ground = view.selected_ground; }
    
    if (info_ground != -1) {
        int unit = -1;
        int bld = -1;
        int x = info_ground % gr.sizex;
        int y = info_ground / gr.sizex;
        for (int i=0; i < gst.entities.size(); i++) {
            Entity &ent = gst.entities[i];
            if (ent.x == x && ent.y == y) {
                if (ent.info->unit) { unit = i; }
                else { bld = i; }
            }
        }
        
        float hoff = -95;
        if (bld != -1) hoff -= get_entity_info_height(gst.entities[bld]);
        if (unit != -1) hoff -= get_entity_info_height(gst.entities[unit]);
        
        if (unit != -1) {
            render_entity_info(this, gst, vec2 { 0, res.y-30+hoff }, unit);
            hoff += get_entity_info_height(gst.entities[unit]);
        }
        if (bld != -1) {
            render_entity_info(this, gst, vec2 { 0, res.y-30+hoff }, bld);
            hoff += get_entity_info_height(gst.entities[bld]);
        }
        render_tile_info(this, gst, vec2 { 0, res.y-30+hoff }, info_ground);
    } 
    
    if (view.attacks.size() > 0 && view.hover_ground != -1) {
        int def = -1;
        int x = view.hover_ground % gr.sizex;
        int y = view.hover_ground / gr.sizex;
        for (int i=0; i < gst.entities.size(); i++) {
            Entity &ent = gst.entities[i];
            if (ent.x == x && ent.y == y && ent.owner != gst.turn) {
                def = i; break;
            }
        }
        if (def != -1) {
            render_attack_info(this, gst, vec2 { res.x/2-175, res.y/2-125 }, 
                view.selected_entity, def);
        }
    }
}


Graphics_sdl::Graphics_sdl (int resx, int resy) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    window = SDL_CreateWindow("age", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, resx, resy, SDL_WINDOW_SHOWN 
        | SDL_WINDOW_RESIZABLE);
    screenSurface = SDL_GetWindowSurface(window);

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    
    load_sheet();
    
    txt.gRenderer = rend;
}

Graphics_sdl::~Graphics_sdl () {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Renderer* Graphics_sdl::get_renderer () { return rend; }

void Graphics_sdl::present () {    
    SDL_RenderPresent(rend);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
}

void Graphics_sdl::load_sheet () {
    SDL_Surface* surf = SDL_LoadBMP("content/sprites.bmp");
    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf ->format,255,0,255));
	if (!surf) {
		std::cout << "Error loading image: " << SDL_GetError() << std::endl;
		system("pause");
		return;
	}
    txsprites = SDL_CreateTextureFromSurface(rend, surf);
    SDL_FreeSurface(surf);
    
    SDL_Surface* surftext = SDL_LoadBMP("content/gf.bmp");
    SDL_SetColorKey(surftext, SDL_TRUE, SDL_MapRGB(surftext ->format,255,0,255));
	if (!surftext) {
		std::cout << "Error loading image: " << SDL_GetError() << std::endl;
		system("pause");
		return;
	}
    txt.tex = SDL_CreateTextureFromSurface(rend, surftext);
    SDL_FreeSurface(surftext);
}

void Graphics_sdl::render_sprite (
    int x, int y, int w, int h, 
    int u, int v, int s, int t)
{
    SDL_Rect srcRect = { x, y, w, h };
    SDL_Rect dstRect = { u, v, s, t };
    SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
}

void Graphics_sdl::render_sprite (
    int x, int y, int w, int h, 
    int u, int v, int s, int t,
    int r, int g, int b)
{
    SDL_SetTextureColorMod(txsprites, r, g, b);
    render_sprite(x, y, w, h, u, v, s, t);
    SDL_SetTextureColorMod(txsprites, 255, 255, 255);
}

void Graphics_sdl::render_rect (
    int r, int g, int b, int a,
    int x, int y, int w, int h)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(rend, &rect);
}



Graphics_sdl_text::Graphics_sdl_text () {
    for (int i=0; i<128; char_width[i++]=5);
    char_width['('] = 2; char_width[')'] = 2;
    char_width['['] = 2; char_width[']'] = 2;
    char_width['{'] = 3; char_width['}'] = 3;
    char_width[','] = 1; char_width['-'] = 5; char_width['.'] = 1; 
    char_width['/'] = 4; char_width['!'] = 1;
    char_width[':'] = 1; char_width[';'] = 1;
    char_width['%'] = 4;
    char_width['<'] = 3; char_width['>'] = 3; char_width['='] = 4;
    char_width['I'] = 1;
    char_width['f'] = 4; char_width['i'] = 1; char_width['j'] = 3;
    char_width['l'] = 1; char_width['k'] = 4; char_width['t'] = 4;
}

int Graphics_sdl_text::get_width (std::string str) {
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) { 
        width += char_width[str[i]]; 
        if (str[i+1]!='\0') width++;
    }
    return width;
}

void Graphics_sdl_text::render_text (std::string str, vec2 off) {
    render_text(str, off, 0, 0, 0);
}

void Graphics_sdl_text::render_text (std::string str, vec2 off,
        int r, int g, int b) 
{
    SDL_SetTextureColorMod(tex, r, g, b);
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*6+1, (char_i/32)*12+1, 5, 11 };
        SDL_Rect dstRect = { (int)off.x+width, (int)off.y, 5, 11 };
        SDL_RenderCopy(gRenderer, tex, &srcRect, &dstRect);
        width += char_width[char_i]+1;
    }
}

/*
void render_text_scaled (SDL_Renderer* rend, char str[], 
    float off[], txtd *t, float scale) 
{
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*6+1, (char_i/32)*12+1, 5, 11 };
        SDL_Rect dstRect = { off[0]+width, off[1], 5*scale, 11*scale };
        SDL_RenderCopy(rend, t->tex, &srcRect, &dstRect);
        width += t->cw[char_i]*scale+1*scale;
    }
}*/
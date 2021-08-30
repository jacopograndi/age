#include <iostream>
#include <string>

#include "playercontrol.h"
#include "entity.h"

void open_unit_menu (Gst &gst, View &view, Fsm &fsm, int p) {
    Entity &ent = gst.entities[view.selected_entity];
    view.menu_unit.options.clear();
    if (ent.info->unit == 1) {
        if (gst.ground.move_area(gst, ent).size() > 0 
            && ent.moved == 0) 
        {
            view.menu_unit.options.emplace_back("Move", Menu_unit::Opts::move);
        }
        if (gst.ground.attack_targets(gst, ent).size() > 0
            && (!gst.info_has_ability(ent.info, "No Move & Attack") 
                || ent.moved == 0)) {
            view.menu_unit.options.emplace_back("Attack", Menu_unit::Opts::attack);
        }
        if (ent.info->build.size() > 0
            && !gst.check_obstructed(ent)
            && gst.get_possible_builds(ent).size() > 0) 
        {
            view.menu_unit.options.emplace_back("Build", Menu_unit::Opts::build);
        }
        view.menu_unit.options.emplace_back("Done", Menu_unit::Opts::done);
    } else {
        if (ent.info->train.size() 
            && !gst.check_obstructed(ent))
        {
            view.menu_unit.options.emplace_back("Train", Menu_unit::Opts::train);
        }
        if (ent.info->id == 100 
            || ent.info->id == 101) {
            view.menu_unit.options.emplace_back("Trade", Menu_unit::Opts::trade);
        }
        if (ent.info->id == 100) {
            view.menu_unit.options.emplace_back("Age Up", Menu_unit::Opts::age_up);
        }
    }
    view.menu_unit.open(view.res);
}

Player_control::Player_control () {   
    fsm.arcs.emplace_back(
        select, sel_ground, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.selected_ground = p;
            std::cout << "selected ground " << p << "\n";
            view.menu_day.options.clear();
            view.menu_day.options.emplace_back("End Day", Menu_day::Opts::end_day);
            view.menu_day.options.emplace_back("Research", Menu_day::Opts::tech);
            view.menu_day.options.emplace_back("Empire Review", Menu_day::Opts::empire_review);
            view.menu_day.options.emplace_back("Scoring", Menu_day::Opts::scoring);
            view.menu_day.open(view.res);
            return menu_day;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_day, opt, Menu_day::Opts::end_day,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_day.close();
            view.selected_ground = -1;
            gst.end_day();
            std::cout << "end day " << p << "\n";
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_day, back, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) {
            view.menu_day.close();
            view.selected_ground = -1; 
            return select;
        }
    );   
    fsm.arcs.emplace_back(
        menu_day, opt, Menu_day::Opts::tech,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_day.close();
            view.menu_tech.tech_options.clear();
            for (Tech &tech : gst.techs) {
                view.menu_tech.tech_options.emplace_back(tech.name, &tech);
            }
            view.menu_tech.open(view.res);
            std::cout << "tech screen " << "\n";
            return menu_tech;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_tech, opt, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_tech.close();
            view.selected_ground = -1; 
            std::cout << "selected tech " << p << "\n";
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_tech, back, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_tech.close();
            view.selected_ground = -1; 
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        select, sel_unit, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.selected_entity = p;
            open_unit_menu(gst, view, fsm, p);
            std::cout << "selected unit " << p << "\n";
            return menu_unit;
        }
    );  
    fsm.arcs.emplace_back(
        menu_unit, back, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.selected_entity = -1;
            view.menu_unit.close();
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_unit, opt, Menu_unit::Opts::train,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_unit.close();
            std::cout << "train " << p << "\n";
            Entity &ent = gst.entities[view.selected_entity];
            view.menu_train.options.clear();
            for (int id : ent.info->train) {
                std::cout << id << " " << gst.get_info(id)->name << "\n";
                EntityInfo *info = gst.get_info(id);
                Option opt { info->name, id };
                opt.cost = info->cost;
                view.menu_train.options.push_back(opt);
            }
            view.menu_train.open(view.res);
            return menu_train;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_train, opt, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_train.close();
            Entity &ent = gst.entities[view.selected_entity];
            ent.done = true;
            Entity entb { ent.x, ent.y, gst.get_info(p), ent.owner };
            entb.building = -1;
            entb.done = true;
            entb.hp = 50;
            gst.entities.push_back(entb);
            Player &player = gst.players[gst.turn];
            for (int i=0; i<player.res.size(); i++) {
                player.res[i] -= entb.info->cost[i];
            } 
            view.selected_entity = -1;
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_unit, opt, Menu_unit::Opts::build,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_unit.close();
            std::cout << "build " << p << "\n";
            Entity &ent = gst.entities[view.selected_entity];
            view.menu_build.options.clear();
            for (int id : ent.info->build) {
                EntityInfo *info = gst.get_info(id);
                if(!gst.check_req_build(ent, info)) continue;
                std::cout << id << " " << gst.get_info(id)->name << "\n";
                Option opt { info->name, id };
                opt.cost = info->cost;
                view.menu_build.options.push_back(opt);
            }
            view.menu_build.open(view.res);
            return menu_build;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_build, opt, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_build.close();
            std::cout << "building " << p << "\n";
            Entity &ent = gst.entities[view.selected_entity];
            ent.done = true;
            Entity entb { ent.x, ent.y, gst.get_info(p), ent.owner };
            entb.building = -1;
            entb.done = true;
            entb.hp = 50;
            gst.entities.push_back(entb);
            Player &player = gst.players[gst.turn];
            for (int i=0; i<player.res.size(); i++) {
                player.res[i] -= entb.info->cost[i];
            } 
            view.selected_entity = -1;
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_unit, opt, Menu_unit::Opts::move,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_unit.close();
            std::cout << "move " << p << "\n";
            Entity &ent = gst.entities[view.selected_entity];
            view.moves = gst.ground.move_area(gst, ent);
            return move;
        }
    ); 
    fsm.arcs.emplace_back(
        move, sel_ground, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            std::cout << "moved to " << p << "\n";
            Entity &ent = gst.entities[view.selected_entity];
            view.moves.clear();
            ent.x = p % gst.ground.sizex;
            ent.y = p / gst.ground.sizex;
            ent.moved = 1;
            open_unit_menu(gst, view, fsm, p);
            return menu_unit;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_unit, opt, Menu_unit::Opts::attack,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_unit.close();
            std::cout << "attack " << p << "\n";
            Entity &ent = gst.entities[view.selected_entity];
            view.attacks = gst.ground.attack_targets(gst, ent);
            return attack;
        }
    ); 
    fsm.arcs.emplace_back(
        attack, sel_ground, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            std::cout << "attacked " << p << "\n";
            Entity &atk = gst.entities[view.selected_entity];
            int x = view.cursor_ground % gst.ground.sizex;
            int y = view.cursor_ground / gst.ground.sizex;
            std::cout << "selg " << x << " " << y << "\n";
            Entity &def = gst.get_at(x, y);
            atk.done = true;
            gst.battle(atk, def);
            view.attacks.clear();
            view.selected_entity = -1;
            return select;
        }
    ); 
    fsm.arcs.emplace_back(
        menu_unit, opt, Menu_unit::Opts::done,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.menu_unit.close();
            gst.entities[view.selected_entity].done = true;
            view.selected_entity = -1;
            std::cout << "done " << p << "\n";
            return select;
        }
    ); 
}

void Player_control::process (Gst &gst, View &view) {
    if (view.cursor_entity != -1) {
        fsm.transition(gst, view, fsm, sel_unit, view.cursor_entity);
    }
    if (view.cursor_ground != -1) {
        fsm.transition(gst, view, fsm, sel_ground, view.cursor_ground);
    }
    if (view.back != -1) {
        fsm.transition(gst, view, fsm, back, 0);
    }
    if (view.opt != -1) {
        fsm.transition(gst, view, fsm, opt, view.opt);
    }
}
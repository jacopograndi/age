#include <iostream>
#include <string>

#include "playercontrol.h"
#include "entity.h"

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
            // end turn calcs
            for (Entity &e : gst.entities) {
                e.done = false;
            }
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
        select, sel_unit, -1,
        [](Gst &gst, View &view, Fsm &fsm, int p) { 
            view.selected_entity = p;
            view.menu_unit.options.clear();
            view.menu_unit.options.emplace_back("Move", Menu_unit::Opts::move);
            view.menu_unit.options.emplace_back("Attack", Menu_unit::Opts::attack);
            view.menu_unit.options.emplace_back("Done", Menu_unit::Opts::done);
            view.menu_unit.open(view.res);
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
            // remove move points
            view.menu_unit.options.clear();
            view.menu_unit.options.emplace_back("Attack", Menu_unit::Opts::attack);
            view.menu_unit.options.emplace_back("Done", Menu_unit::Opts::done);
            view.menu_unit.open(view.res);
            return menu_unit;
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
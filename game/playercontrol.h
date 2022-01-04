#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <iostream>

#include "gst.h"
#include "view.h"
#include <vector>
#include <functional>

enum pc_state {
    sel,
    move,
    attack,
    train, 
    build,
    merge,
    trade,
    age_up,
    heal,
    power,
    move_target,
    attack_target,
    menu_train,
    menu_build,
    target_build,
    merge_target,
    target_heal,
    target_convert,
    menu_power,
    target_power,
    menu_unit,
    menu_day,
    menu_tech,
    menu_trade,
    menu_age_up,
    end
};

enum pc_action {
    sel_unit,
    sel_ground,
    opt,
    back
};



class Fsm;
using lambda = std::function<pc_state(Gst&, View&, Fsm&, int p)>;


class Arc {
    public:
    Arc (pc_state from, pc_action act, int p, lambda f) 
        : from(from), act(act), p(p), f(f) {};
    pc_state from;
    pc_action act;
    int p;
    
	lambda f;
};

class Fsm {
    public:
    Fsm() { state = sel; }
    
    void transition (Gst &gst, View &view, Fsm &fsm, pc_action act, int p) {
        std::cout << "> transitioning from " << state << " with " << act << std::endl;
        for (Arc a : arcs) {
            if (a.from == state && a.act == act && (a.p == p || a.p == -1)) {
                state = a.f(gst, view, fsm, p);
                break;
            }
        }
    }
    std::vector<Arc> arcs;
    
    private:
    pc_state state;
};


class Player_control {
    public:
    Player_control ();
    void process (Gst &gst, View &view);
    
    Fsm fsm;
};

#endif

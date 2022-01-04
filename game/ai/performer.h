#ifndef PERFORMER_H
#define PERFORMER_H

#include <iostream>

#include <string>
#include <vector>

#include "../ground.h"
#include "../gst.h"
#include "action.h"
#include "tactic.h"

namespace ai {

class performer {
    public:
    performer (Gst &gst) : init(gst) {}
    Gst &init;
    
    Gst apply (tactic t) {
        Gst next { init };
        for (action a : t.acts) {
            next = act(next, a);
        }
        next.end_day();
        return next;
    }
    
    Gst& act (Gst &gst, action a) {
        if (a.type == actype::move) {
            Entity &ent = gst.get_at(a.x, a.y);
            ent.x = a.mx; ent.y = a.my;
            ent.moved = 1; ent.done = true;
        }
        if (a.type == actype::attack) {
            Entity &ent = gst.get_at(a.x, a.y);
            ent.x = a.mx; ent.y = a.my;
            ent.moved = 1; 
            Entity &def = gst.get_at(a.tx, a.ty);
            gst.battle(ent, def);
            ent.done = true;
        }
        return gst;
    }
};

}
#endif
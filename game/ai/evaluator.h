#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <iostream>

#include <string>
#include <vector>

#include "../ground.h"
#include "../gst.h"

namespace ai {

class evaluator {
    public:
    evaluator (Gst gst) : gst(gst) {}
    Gst gst;
    
    float eval (int player) {
        float val = 0;
        for (Entity &ent : gst.entities) {
            int own = (int)(ent.owner != player)*2-1;
            val += own*(ent.info->cost[0] + ent.info->cost[1])*(ent.hp/100.0f);
        }
        return val;
    }
};

}
#endif
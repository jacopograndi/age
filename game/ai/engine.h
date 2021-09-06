#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>

#include <string>
#include <vector>
#include <limits>
#include <cmath>

#include "../ground.h"
#include "../gst.h"

#include "action.h"
#include "tactic.h"
#include "generator.h"
#include "evaluator.h"
#include "performer.h"

namespace ai {

class engine {
    public:
    engine (Gst &gst) : init(gst) {}
    Gst &init;
    
    tactic get_best () {
        tactic t { search(init, 4) };
        std::cout << t.to_string();
        return t;
    }
    
    tactic search (Gst &gst, int depth) {
        generator gen { gst };
        std::vector<tactic> tactics = gen.tactics();
        tactic best; best.eval = std::numeric_limits<float>::lowest();
        for (tactic t : tactics) {
            performer perf { gst };
            Gst next { perf.apply(t) };
            t.eval = negamax(next, depth, gst.turn);
            std::cout << "depth " << depth << " eval " << t.eval << "\n";
            if (t.eval > best.eval) best = t;
        }
        return best;
    }
    
    float negamax (Gst gst, int depth, int player) {
        //for (int i=0; i<3-depth; i++) std::cout << " "; std::cout << depth << "\n";
        if (depth == 0) {
            evaluator eval { gst };
            return eval.eval(player);
        }
        float value = std::numeric_limits<float>::lowest();
        generator gen { gst };
        auto tactics = gen.tactics();
        for (tactic t : tactics) {
            performer perf { gst };
            Gst next { perf.apply(t) };
            value = fmax(value, negamax(next, depth-1, -player));
        }
        return -value;
    }
};

}
#endif
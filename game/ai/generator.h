#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>

#include <string>
#include <vector>

#include "../ground.h"
#include "../gst.h"
#include "action.h"
#include "tactic.h"
#include "performer.h"

namespace ai {
    
const int maxiter = 100000;


class generator {
    public:
    generator (Gst &gst) : init(gst) {}
    Gst &init;
    
    std::vector<action> valid_actions (Gst &gst) {
        Ground &gr = gst.inv->ground;
        std::vector<action> acts;
        for (Entity &ent : gst.entities) {
            if (ent.done) continue;
            if (ent.owner != gst.turn) continue;
            
            std::vector<action> ent_acts;
            
            std::vector<int> moves = gr.move_area(gst, ent);
            for (auto move : moves) {
                int mx = move%gr.sizex, my = move/gr.sizex;
                action act { actype::move, ent.x, ent.y, mx, my };
                
                performer perf { gst };
                Gst next { gst };
                next = perf.act(next, act);
                Entity &matk = next.get_at(mx, my);
                std::vector<int> atks = gr.attack_targets(next, matk);
                for (auto atk : atks) {
                    int tx = atk%gr.sizex, ty = atk/gr.sizex;
                    action act_atk { actype::attack, ent.x, ent.y, 
                        mx, my, tx, ty };
                    Entity &mdef = next.get_at(tx, ty);
                    
                    BattleResult res = gst.battle_res(matk, mdef);
                    float heur = (mdef.hp-res.def_hp);
                    heur *= (mdef.info->cost[0] + mdef.info->cost[1]);
                    act_atk.heuristic = heur;
                    ent_acts.push_back(act_atk);
                }
                
                int dist = 999999;
                int pos = gst.get_nearest_enemy(ent, dist);
                float heur = 1.0f/dist;
                act.heuristic = heur;
                ent_acts.push_back(act);
            }
            std::sort(ent_acts.begin(), ent_acts.end(), compare_action);
            for (int i=0; i<3 && i<ent_acts.size(); i++) {
                acts.push_back(ent_acts[i]);
            }
        }
        std::sort(acts.begin(), acts.end(), compare_action);
        return acts;
    }
    
    tactic valid_tactic (int &n) {
        tactic t;
        Gst gst { init };
        for (int i=0; i<ai::maxiter; i++) {
            std::vector<action> acts { valid_actions(gst) };
            if (acts.size() == 0) break;
            performer perf { gst };
            int j = n % (acts.size()); 
            n /= acts.size();
            action &act = acts[j];
            gst = perf.act(gst, act);
            t.acts.push_back(act);
            //std::cout << act.to_string() << std::endl;
        }
        return t;
    }
    
    std::vector<tactic> tactics () {
        std::vector<tactic> ts;
        for (int i=0; i<3; i++) {
            int n = i;
            tactic t = valid_tactic(n);
            if (n > 0) break;
            ts.push_back(t); 
        }
        return ts;
    }
};

}
#endif
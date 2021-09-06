#ifndef TACTIC_H
#define TACTIC_H

#include <string>
#include <vector>

#include "action.h"

namespace ai {
    
class tactic {
    public:
    tactic () { }
    
    std::vector<action> acts;
    float eval = 0;
    
    // copy constructor
    tactic (const tactic& rhs) { acts = rhs.acts; eval = rhs.eval; }
    tactic& operator=(const tactic& rhs) { acts = rhs.acts; eval = rhs.eval; }
    
    std::string to_string () {
        std::string str = "tactic eval= " + std::to_string(eval) + "\n";
        for (action act : acts) {
            str += act.to_string() + "\n";
        }
        return str;
    } 
};

}
#endif
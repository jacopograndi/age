#include "action.h"

bool ai::compare_action(ai::action a, ai::action b) {
    return (a.heuristic < b.heuristic);
}

std::string ai::action::to_string () {
    if (type == actype::attack) {
        if (abs(x-mx)+abs(y-my) != 0) {
            return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"
                + " move " + 
                "[" + std::to_string(mx) + ", " + std::to_string(my) + "]" 
                + " attack " + 
                "[" + std::to_string(tx) + ", " + std::to_string(ty) + "]";
        } else {
            return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"
                + " attack " + 
                "[" + std::to_string(tx) + ", " + std::to_string(ty) + "]";
        }
    }
    if (type == actype::heal) {
        if (abs(x-mx)+abs(y-my) != 0) {
            return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"
                + " move " + 
                "[" + std::to_string(mx) + ", " + std::to_string(my) + "]" 
                + " heal " + 
                "[" + std::to_string(tx) + ", " + std::to_string(ty) + "]";
        } else {
            return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"
                + " heal " + 
                "[" + std::to_string(tx) + ", " + std::to_string(ty) + "]";
        }
    }
    if (type == actype::convert) {
        if (abs(x-mx)+abs(y-my) != 0) {
            return "[" + std::to_string(x) + ", " + std::to_string(y) + "]" 
                + " move " + 
                "[" + std::to_string(mx) + ", " + std::to_string(my) + "]" 
                + " convert " + 
                "[" + std::to_string(tx) + ", " + std::to_string(ty) + "]";
        } else {
            return "[" + std::to_string(x) + ", " + std::to_string(y) + "]" 
                + " convert " + 
                "[" + std::to_string(tx) + ", " + std::to_string(ty) + "]";
        }
    }
    if (type == actype::move) {
        return "[" + std::to_string(x) + ", " + std::to_string(y) + "]" 
            + " move " + 
            "[" + std::to_string(mx) + ", " + std::to_string(my) + "]";
    }
    if (type == actype::build) {
        if (abs(x-mx)+abs(y-my) != 0) {
            return "[" + std::to_string(x) + ", " + std::to_string(y) 
                + "]" + " move " + 
                "[" + std::to_string(mx) + ", " + std::to_string(my) 
                + "]" + " build " + 
                std::to_string(v);
        } else {
            return "[" + std::to_string(mx) + ", " + std::to_string(my) 
                + "]" + " build " + 
                std::to_string(v);
        }
    }
    if (type == actype::train) {
        return "[" + std::to_string(x) + ", " + std::to_string(y) + "]" + 
            " train " + std::to_string(v);
    }
    if (type == actype::trade) {
        return "[" + std::to_string(x) + ", " + std::to_string(y) + "]" + 
            " trade " + std::to_string(v);
    }
    if (type == actype::tech) {
        return "tech " + std::to_string(v);
    }
}
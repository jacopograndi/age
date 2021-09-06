#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <vector>

namespace ai {
    
enum actype { 
    attack, heal, convert, build, train, trade, move, tech
};

class action {
    public:
    action (actype type, int v) : 
        type(type), v(v) {}
    action (actype type, int x, int y, int v) : 
        type(type), x(x), y(y), v(v) {}
    action (actype type, int x, int y, int mx, int my) : 
        type(type), x(x), y(y), mx(mx), my(my) {}
    action (actype type, int x, int y, int mx, int my, int v) : 
        type(type), x(x), y(y), mx(mx), my(my), v(v) {}
    action (actype type, int x, int y, int mx, int my, int tx, int ty) : 
        type(type), x(x), y(y), mx(mx), my(my), tx(tx), ty(ty) {}
    action (actype type, int x, int y, int mx, int my, int tx, int ty, int v) : 
        type(type), x(x), y(y), mx(mx), my(my), tx(tx), ty(ty), v(v) {}
        
    action (const action& rhs) { 
        type = rhs.type; x = rhs.x; y = rhs.y; mx = rhs.mx; my = rhs.my; 
        tx = rhs.tx; ty = rhs.ty; v = rhs.v; 
    }
    action& operator=(const action& rhs) {
        type = rhs.type; x = rhs.x; y = rhs.y; mx = rhs.mx; my = rhs.my; 
        tx = rhs.tx; ty = rhs.ty; v = rhs.v; 
    }
        
    actype type;
    int x, y; // start
    int mx, my; // moved
    int tx, ty; // target
    int v; // value (id of trainee, building or f/g trade, or tech id)
    
    float heuristic { 0 };
    
    std::string to_string ();
};

bool compare_action(action a, action b);

}

#endif
#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <functional>

#include "gst.h"
#include "menu.h"

class View {
    public:
    View (vec2 res) : res(res) {}
    
    vec2 res;
    
    int hover_ground {-1};
    int selected_ground {-1};
    int selected_entity {-1};
    int cursor_ground {-1};
    int cursor_entity {-1};
    int back {-1};
    int opt {-1};
    std::vector<int> moves;
    std::vector<int> attacks;
    std::vector<int> builds;
    std::vector<int> heals;
    std::vector<int> converts;
    
    Menu_unit menu_unit;
    Menu_day menu_day;
    Menu_build menu_build;
    Menu_train menu_train;
    Menu_tech menu_tech;
    Menu_trade menu_trade;
    Menu_age_up menu_age_up;
    
    void process (Gst &gst, vec2 cam, vec2 mouse, int *mheld);
};

#endif
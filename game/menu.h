#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

#include "../umath/vec2.h"

class Option {
    public:
    Option(std::string name, int id) : name(name), id(id) {}
    
    std::string name;
    int id;
};


class Menu {
    public:
    bool active { false };    
    std::vector<Option> options;
    vec2 pos, size;
    
    void open (vec2 res);
    void close ();
    int mouse_option (vec2 mouse);
};

class Menu_unit : public Menu {
    public:
    Menu_unit () {}
    
    enum Opts {
        move, attack, done
    };
};

class Menu_day : public Menu {
    public:
    Menu_day () {}
    
    enum Opts {
        end_day, tech, empire_review, scoring
    };
};

#endif
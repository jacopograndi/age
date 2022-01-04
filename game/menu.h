#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

#include "../umath/vec2.h"
#include "tech.h"

class Option {
    public:
    Option(std::string name, int id) : name(name), id(id) {}
    
    std::string name;
    int id;
    
    std::vector<float> cost;
};


class Menu {
    public:
    bool active { false };    
    std::vector<Option> options;
    vec2 pos, size;
    int over;
    
    void close ();
    virtual void open (vec2 res);
    virtual int mouse_option (vec2 mouse);
};

class Menu_unit : public Menu {
    public:
    Menu_unit () {}
    
    enum Opts {
        move, attack, done, build, train, trade, age_up, heal, convert
    };
};

class Menu_day : public Menu {
    public:
    Menu_day () {}
    
    enum Opts {
        end_day, tech, empire_review, scoring
    };
};

class Menu_build : public Menu {
    public:
    Menu_build () {}
};
class Menu_train : public Menu {
    public:
    Menu_train () {}
};
class Menu_trade : public Menu {
    public:
    Menu_trade () {}
    
    enum Opts {
        food, gold
    };
};
class Menu_age_up : public Menu {
    public:
    Menu_age_up () {}
};

class OptionTech {
    public:
    OptionTech(std::string name, Tech *tech) : name(name), tech(tech) {}
    
    std::string name;
    Tech *tech;
};

class Menu_tech : public Menu {
    public:
    Menu_tech () {}
    std::vector<OptionTech> tech_options;
    std::vector<std::vector<OptionTech>> tech_opt_ordered;
    
    void open (vec2 res) override;
    int mouse_option (vec2 mouse) override;
};

#endif
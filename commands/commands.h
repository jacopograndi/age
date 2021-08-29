#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class Commands_sdl {
    public:
    Commands_sdl ();
    ~Commands_sdl ();
    bool process_events ();
    void process_clicks ();
    void process_keys ();
    bool check_keydown(SDL_Scancode key);
    bool check_key(SDL_Scancode key);
    int mx, my;
    int mclick[3];
    int mheld[3];
    
    int resx, resy;
    
    private:
    std::vector<SDL_Scancode> keys;
    std::vector<SDL_Scancode> keysdown;
};

#endif
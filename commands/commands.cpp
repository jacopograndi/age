#include <algorithm>

#include "commands.h"

Commands_sdl::Commands_sdl () {
    for (int i=0; i<3; i++) {
        mclick[i] = 0;
        mheld[i] = 0;
    }
}

Commands_sdl::~Commands_sdl () {
    
}

bool Commands_sdl::check_keydown (SDL_Scancode key) {
    return std::find(keysdown.begin(), keysdown.end(), key) != keysdown.end();
}
bool Commands_sdl::check_key (SDL_Scancode key) {
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool Commands_sdl::process_events () {
    SDL_GetMouseState(&mx, &my);
    
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            return true;
        }
        if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            keys.push_back(e.key.keysym.scancode);
            keysdown.push_back(e.key.keysym.scancode);
        }
        if(e.type == SDL_KEYUP) {
            keysdown.erase(std::remove(keysdown.begin(), keysdown.end(), 
                e.key.keysym.scancode), keysdown.end()); 
        }
        
        if(e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) { mclick[0] = 1; }
            if (e.button.button == SDL_BUTTON_MIDDLE) { mclick[1] = 1; }
            if (e.button.button == SDL_BUTTON_RIGHT) { mclick[2] = 1; }
        }
        if(e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) { mclick[0] = 0; }
            if (e.button.button == SDL_BUTTON_MIDDLE) { mclick[1] = 0; }
            if (e.button.button == SDL_BUTTON_RIGHT) { mclick[2] = 0; }
        }
    }
    return false;
}

void Commands_sdl::process_clicks () {
    for (int i=0; i<3; i++) {
        if (mclick[i] > 0) { mheld[i] += 1; }
        else if (mheld[i] > 0) { mheld[i] = -1; }
        else { mheld[i] = 0; }
    }
}

void Commands_sdl::process_keys () {
    keys.clear();
}
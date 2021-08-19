#ifndef TIMING_H
#define TIMING_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class timing_sdl {
    public:
    timing_sdl(double fps);
    void process();
    bool check_process();
    bool check_render();
    
    private:
    double frame_time;
    double last_time;
    double frame_counter;
    double tot_time;
    double unprocessed_time;
    int frames;
    bool render;
};

#endif
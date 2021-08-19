#include "timing.h"

timing_sdl::timing_sdl (double fps) { 
    frame_time = 1/fps;
    last_time = SDL_GetTicks();
    frame_counter = 0;
    tot_time = 0;
    unprocessed_time = 0;
    frames = 0;
    render = false;
}

void timing_sdl::process () {
    render = false;
    
    double startTime = SDL_GetTicks();
    double passedTime = (startTime - last_time)/1000;
    last_time = startTime;
    
    unprocessed_time += passedTime;
    frame_counter += passedTime;
    tot_time += passedTime;

    if (frame_counter >= 1.0) {
        //printf("FPS: %i | %f ms\n", frames, 1000.0 / ((double)frames));
        frames = 0;
        frame_counter = 0;
    }
}

bool timing_sdl::check_process () {
    if (unprocessed_time > frame_time) {
        render = true;
        unprocessed_time -= frame_time;
        return true;
    } else {
        return false;
    }
}

bool timing_sdl::check_render () {
    if (render) { frames ++; }
    return render;
}
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "../game/gst.h"
#include "../game/view.h"
#include "cam.h"

#include <string>


class Graphics_sdl_text {
    public:
    Graphics_sdl_text();
    int get_text_width (char str[]);
    void render_text (std::string str, vec2 off);
    SDL_Renderer* gRenderer;
    SDL_Texture *tex;
    int char_width[128];
};

class Graphics_sdl {
    public:
    Graphics_sdl (int resx, int resy);
    ~Graphics_sdl ();
    SDL_Renderer* get_renderer ();
    void load_sheet ();
    void present ();
    void render_sprite (
        int x, int y, int w, int h, 
        int u, int v, int s, int t);
    void render_rect (
        int r, int g, int b, int a,
        int x, int y, int w, int h);
    void render_sprite (
        int x, int y, int w, int h, 
        int u, int v, int s, int t,
        int r, int g, int b);
    
    Graphics_sdl_text txt;
    
    private:
    SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
    SDL_Renderer* rend = NULL;
    SDL_Texture* txsprites = NULL;
};


class Graphics {
    public:
    Graphics (int resx, int resy) 
        : resx(resx), resy(resy), backend(resx, resy) {}
    ~Graphics ();
    
    Graphics_sdl backend;

    int resx, resy;
    Cam cam;
    
    void render (Gst &gst, View &view);
    void present ();
};

#endif
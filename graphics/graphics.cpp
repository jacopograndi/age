#include <algorithm>
#include <iostream>

#include "graphics.h"
#include "../game/menu.h"
#include "../game/tile.h"

Graphics::~Graphics () {
}

void Graphics::present () {
    backend.present();
}

void Graphics::render (Gst &gst, View &view) 
{
    Ground &gr = gst.ground;
    std::vector<Entity> &entities = gst.entities;
    vec2 res { (float)resx, (float)resy };
    
    for (int y=0; y<gr.sizey; y++) {
        for (int x=0; x<gr.sizex; x++) {
            Tile &tile = gst.tiles[gr.tiles[gr.at(x,y)]];
            backend.render_sprite(
                (int)tile.spritebounds.x,
                (int)tile.spritebounds.y, 16, 16, 
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }            
    for (int i=0; i<entities.size(); i++) {
        int done = 0;
        done += 160 * entities[i].done;
        
        Player &player = gst.players[entities[i].owner];
        backend.render_sprite(
            (int)entities[i].info.spritebounds.x,
            (int)entities[i].info.spritebounds.y+done, 16, 16,
            (int)cam.pos.x + (int)entities[i].x*32, 
            (int)cam.pos.y + (int)entities[i].y*32, 32, 32
        );
        if (done == 0) {
            backend.render_sprite(
                (int)entities[i].info.spritebounds.x,
                (int)entities[i].info.spritebounds.y+240, 16, 16,
                (int)cam.pos.x + (int)entities[i].x*32, 
                (int)cam.pos.y + (int)entities[i].y*32, 32, 32,
                player.r, player.g, player.b
            );
        }
    }
    
    for (int y=0; y<gr.sizey; y++) {
        for (int x=0; x<gr.sizex; x++) {
            Tile &tile = gst.tiles[gr.tiles[gr.at(x,y)]];
            backend.render_sprite(
                (int)tile.spritebounds.x,
                (int)tile.spritebounds.y-16, 16, 16, 
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32-32, 32, 32
            );
        }
    }
    
    if (view.selected_entity != -1) {
        int i = view.selected_entity;
        vec2 pos { (float)entities[i].x*32, (float)entities[i].y*32 };
        backend.render_sprite (
            (int)entities[i].info.spritebounds.x,
            (int)entities[i].info.spritebounds.y+16*5, 16, 16, 
            (int)cam.pos.x + (int)pos.x, 
            (int)cam.pos.y + (int)pos.y, 32, 32
        );
    } 
    
    if (view.selected_ground != -1) {
        int x = view.selected_ground % gr.sizex;
        int y = view.selected_ground / gr.sizex;
        vec2 pos { (float)x*32, (float)y*32 };
        Tile &tile = gst.tiles[gr.tiles[gr.at(x,y)]];
        backend.render_sprite(
            (int)tile.spritebounds.x,
            (int)tile.spritebounds.y+4*16, 16, 32, 
            (int)cam.pos.x + (int)pos.x, 
            (int)cam.pos.y + (int)pos.y-32, 32, 64
        );
    } 
    
    if (view.menu_unit.active) {
        vec2 pos { res };
        float height = view.menu_unit.options.size() * 20;
        vec2 size { 120, height+10 };
        pos *= 0.5f;
        pos -= size/2;
        backend.render_rect(
            255,255,255,255,
            view.menu_unit.pos.x,view.menu_unit.pos.y,
            view.menu_unit.size.x,view.menu_unit.size.y
        );
        float acc = 0;
        for (Option opt : view.menu_unit.options) {
            backend.txt.render_text(opt.name, view.menu_unit.pos + vec2 {10, 10 + acc});
            acc += 20;
        }
    }
    
    if (view.menu_day.active) {
        vec2 pos { res };
        float height = view.menu_day.options.size() * 20;
        vec2 size { 120, height+10 };
        pos *= 0.5f;
        pos -= size/2;
        backend.render_rect(
            255,255,255,255,
            view.menu_day.pos.x,view.menu_day.pos.y,
            view.menu_day.size.x,view.menu_day.size.y
        );
        float acc = 0;
        for (Option opt : view.menu_day.options) {
            backend.txt.render_text(opt.name, view.menu_day.pos + vec2 {10, 10 + acc});
            acc += 20;
        }
    }
    
    if (view.moves.size() > 0) {
        for (int m : view.moves) {
            int x = m % gr.sizex;
            int y = m / gr.sizex;
            backend.render_rect(
                0, 120, 255, 100,
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
}


Graphics_sdl::Graphics_sdl (int resx, int resy) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    window = SDL_CreateWindow("age", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, resx, resy, SDL_WINDOW_SHOWN 
        | SDL_WINDOW_RESIZABLE);
    screenSurface = SDL_GetWindowSurface(window);

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    
    load_sheet();
    
    txt.gRenderer = rend;
}

Graphics_sdl::~Graphics_sdl () {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Renderer* Graphics_sdl::get_renderer () { return rend; }

void Graphics_sdl::present () {    
    SDL_RenderPresent(rend);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
}

void Graphics_sdl::load_sheet () {
    SDL_Surface* surf = SDL_LoadBMP("content/sprites.bmp");
    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf ->format,255,0,255));
	if (!surf) {
		std::cout << "Error loading image: " << SDL_GetError() << std::endl;
		system("pause");
		return;
	}
    txsprites = SDL_CreateTextureFromSurface(rend, surf);
    SDL_FreeSurface(surf);
    
    SDL_Surface* surftext = SDL_LoadBMP("content/gf.bmp");
    SDL_SetColorKey(surftext, SDL_TRUE, SDL_MapRGB(surftext ->format,255,0,255));
	if (!surftext) {
		std::cout << "Error loading image: " << SDL_GetError() << std::endl;
		system("pause");
		return;
	}
    txt.tex = SDL_CreateTextureFromSurface(rend, surftext);
    SDL_FreeSurface(surftext);
}

void Graphics_sdl::render_sprite (
    int x, int y, int w, int h, 
    int u, int v, int s, int t)
{
    SDL_Rect srcRect = { x, y, w, h };
    SDL_Rect dstRect = { u, v, s, t };
    SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
}

void Graphics_sdl::render_sprite (
    int x, int y, int w, int h, 
    int u, int v, int s, int t,
    int r, int g, int b)
{
    SDL_SetTextureColorMod(txsprites, r, g, b);
    render_sprite(x, y, w, h, u, v, s, t);
    SDL_SetTextureColorMod(txsprites, 255, 255, 255);
}

void Graphics_sdl::render_rect (
    int r, int g, int b, int a,
    int x, int y, int w, int h)
{
    SDL_SetRenderDrawColor(rend, r, g, b, a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(rend, &rect);
}



Graphics_sdl_text::Graphics_sdl_text () {
    for (int i=0; i<128; char_width[i++]=5);
    char_width['('] = 2; char_width[')'] = 2;
    char_width['['] = 2; char_width[']'] = 2;
    char_width['{'] = 3; char_width['}'] = 3;
    char_width[','] = 1; char_width['-'] = 3; char_width['.'] = 1; 
    char_width['/'] = 4; char_width['!'] = 1;
    char_width[':'] = 1; char_width[';'] = 1;
    char_width['%'] = 4;
    char_width['<'] = 3; char_width['>'] = 3; char_width['='] = 4;
    char_width['I'] = 1;
    char_width['f'] = 4; char_width['i'] = 1; char_width['j'] = 3;
    char_width['l'] = 1; char_width['k'] = 4; char_width['t'] = 4;
}

int Graphics_sdl_text::get_text_width (char str[]) {
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) { 
        width += char_width[str[i]]; 
        if (str[i+1]!='\0') width++;
    }
    return width;
}

void Graphics_sdl_text::render_text (std::string str, vec2 off) {
    SDL_SetTextureColorMod(tex, 0, 0, 0);
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*6+1, (char_i/32)*12+1, 5, 11 };
        SDL_Rect dstRect = { (int)off.x+width, (int)off.y, 5, 11 };
        SDL_RenderCopy(gRenderer, tex, &srcRect, &dstRect);
        width += char_width[char_i]+1;
    }
}

/*
void render_text_scaled (SDL_Renderer* rend, char str[], 
    float off[], txtd *t, float scale) 
{
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*6+1, (char_i/32)*12+1, 5, 11 };
        SDL_Rect dstRect = { off[0]+width, off[1], 5*scale, 11*scale };
        SDL_RenderCopy(rend, t->tex, &srcRect, &dstRect);
        width += t->cw[char_i]*scale+1*scale;
    }
}*/
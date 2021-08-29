#include <algorithm>
#include <iostream>
#include <string>

#include "graphics.h"
#include "../game/menu.h"
#include "../game/tile.h"

Graphics::~Graphics () {
}

void Graphics::present () {
    backend.present();
}

void render_ents (Graphics *graphics, Gst &gst, int unit) {
    std::vector<Entity> &entities = gst.entities;
    for (int i=0; i<entities.size(); i++) {
        if (entities[i].info->unit != unit) continue;
        int done = 0;
        if (unit == 1) done += 512 * entities[i].done;
        
        Player &player = gst.players[entities[i].owner];
        graphics->backend.render_sprite(
            (int)entities[i].info->spritebounds.x,
            (int)entities[i].info->spritebounds.y+done, 16, 16,
            (int)graphics->cam.pos.x + (int)entities[i].x*32, 
            (int)graphics->cam.pos.y + (int)entities[i].y*32, 32, 32
        );
        if (done == 0) {
            graphics->backend.render_sprite(
                (int)entities[i].info->spritebounds.x,
                (int)entities[i].info->spritebounds.y+768, 16, 16,
                (int)graphics->cam.pos.x + (int)entities[i].x*32, 
                (int)graphics->cam.pos.y + (int)entities[i].y*32, 32, 32,
                player.r, player.g, player.b
            );
        }
    }
}

void render_menu (Graphics *graphics, Gst &gst, Menu &menu) {
    vec2 res { (float)graphics->resx, (float)graphics->resy };
    if (menu.active) {
        vec2 pos { res };
        float height = menu.options.size() * 20;
        vec2 size { 120, height+10 };
        pos *= 0.5f;
        pos -= size/2;
        graphics->backend.render_rect(
            255,255,255,255,
            menu.pos.x,menu.pos.y,menu.size.x,menu.size.y
        );
        float acc = 0;
        for (Option opt : menu.options) {
            graphics->backend.txt.render_text(opt.name, menu.pos + vec2 {10, 10 + acc});
            float width = graphics->backend.txt.get_width(opt.name);
            if (opt.cost.size() > 0) {
                graphics->backend.txt.render_text(
                    std::to_string (opt.cost[0]) + "f", 
                    menu.pos + vec2 { width + 20, 10 + acc });
                graphics->backend.txt.render_text(
                    std::to_string (opt.cost[1]) + "g", 
                    menu.pos + vec2 { width + 50, 10 + acc });
            }
            acc += 20;
        }
    }
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
    
    for (Resource res : gst.ground.resources) {
        int x = res.pos % gr.sizex;
        int y = res.pos / gr.sizex;
        backend.render_sprite(
            16*res.kind, 16*2, 16, 16, 
            (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
        );
    }
    
    render_ents(this, gst, 0);
    render_ents(this, gst, 1);
    
    /* overlay */
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
            (int)entities[i].info->spritebounds.x,
            (int)entities[i].info->spritebounds.y+256, 16, 16, 
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
            (int)tile.spritebounds.y+256-16, 16, 32, 
            (int)cam.pos.x + (int)pos.x, 
            (int)cam.pos.y + (int)pos.y-32, 32, 64
        );
    } 
    
    for (int i=0; i<entities.size(); i++) {
        if (entities[i].info->unit == 1) {
            backend.render_rect(
                0, 0, 0, 255,
                (int)cam.pos.x + (int)entities[i].x*32+2, 
                (int)cam.pos.y + (int)entities[i].y*32+30, 28, 2
            );
            int amt = 28 * (entities[i].hp / 100);
            Player &player = gst.players[entities[i].owner];
            backend.render_rect(
                player.r, player.g, player.b, 255,
                (int)cam.pos.x + (int)entities[i].x*32+2, 
                (int)cam.pos.y + (int)entities[i].y*32+30, amt, 2
            );
        }
    }
    
    render_menu(this, gst, view.menu_unit);
    render_menu(this, gst, view.menu_day);
    render_menu(this, gst, view.menu_build);
    render_menu(this, gst, view.menu_train);
    
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
    
    if (view.attacks.size() > 0) {
        for (int m : view.attacks) {
            int x = m % gr.sizex;
            int y = m / gr.sizex;
            backend.render_rect(
                255, 120, 0, 100,
                (int)cam.pos.x + x*32, (int)cam.pos.y + y*32, 32, 32
            );
        }
    }
    
    // top bar
    vec2 pos { 0,0 };
    backend.render_rect (
        255,255,255,255,
        (int)pos.x,(int)pos.y,(int)res.x, 30
    );
    Player &player = gst.players[gst.turn];
    backend.render_rect (
        player.r, player.g, player.b, 255,
        (int)pos.x+5,(int)pos.y+5, 20, 20
    );
    
    std::string txtfood = std::to_string (player.res[0]) + "f";
    std::string txtgold = std::to_string (player.res[1]) + "g";
    backend.txt.render_text(
        txtfood, pos + vec2 { -backend.txt.get_width(txtfood) + res.x/2-10, 10 }
    );
    backend.txt.render_text(
        txtgold, pos + vec2 { res.x/2+10, 10 }
    );
    // low bar
    backend.render_rect (
        255,255,255,255,
        0,(int)res.y-30,(int)res.x, 30
    );
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

int Graphics_sdl_text::get_width (std::string str) {
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
#ifndef GFX_H
#define GFX_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "types.h"
#include "input.h"

#define FB_WIDTH 256
#define FB_HEIGHT 240
#define TILESIZE 16

#define min(x, y) ((x) < (y) ? (x) : (y))

typedef struct framebuffer
{
    u16 width, height, x, y;
    f32 scale;
    ALLEGRO_BITMAP* bitmap;
} framebuffer_t;

extern framebuffer_t fb;
extern ALLEGRO_BITMAP* tileset_bitmap;

void init_gfx();
void init_framebuffer();

void scale_framebuffer(ALLEGRO_DISPLAY* display);
void draw_framebuffer();

void destroy_gfx();
void destroy_framebuffer();

#endif
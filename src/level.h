#ifndef LEVEL_H
#define LEVEL_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "types.h"
#include "gfx.h"

#define MAX_TILES 128

typedef struct tile
{
    s16 x, y;
    s16 x_positions[4];
    s16 y_positions[4];
    u8 sides;
    ALLEGRO_BITMAP* bitmap;
} tile_t;

typedef struct level
{
    tile_t* tiles[MAX_TILES];
} level_t;

extern level_t level;

void init_level(level_t* level);
void draw_level(level_t level);
void destroy_level(level_t* level);
b1 get_line_collision(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4);

#endif
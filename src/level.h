#ifndef LEVEL_H
#define LEVEL_H

#include <allegro5/allegro.h>
#include "types.h"
#include "gfx.h"

typedef struct tile
{
    s16 x, y;
    u8 width, height;
    ALLEGRO_BITMAP* bitmap;
} tile_t;

#endif
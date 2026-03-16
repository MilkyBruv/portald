#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "gfx.h"
#include "input.h"

#define PLAYER_SPEED ((u8) 5);
#define PLAYER_WIDTH ((u8) 8);
#define PLAYER_HEIGHT ((u8) 16);

typedef enum portal_color { BLUE, ORANGE } portal_color_t;
typedef enum portal_direction { UP, DOWN, LEFT, RIGHT } portal_direction_t;
typedef struct portal
{
    s16 x, y;
    portal_color_t color;
    portal_direction_t direction;
    ALLEGRO_BITMAP* bitmap;
} portal_t;
typedef struct portal_gun
{
    s16 x, y;
    portal_color_t color;
    ALLEGRO_BITMAP* bitmap;
    f32 angle;
} portal_gun_t;

typedef struct player
{
    s16 x;
    s16 y;
    b1 split;
    u8 split_x;
    u8 split_y;
    ALLEGRO_BITMAP* bitmap;
} player_t;

extern player_t* player;
extern portal_gun_t* portal_gun;

void init_player();
void update_player();
void draw_player();
void destroy_player();

void init_portal_gun();
void update_portal_gun();
void draw_portal_gun();
void destroy_portal_gun();

#endif
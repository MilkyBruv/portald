#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "gfx.h"
#include "input.h"
#include "sfx.h"

#define PLAYER_SPEED 1
#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 16
#define MAX_BULLETS 4
#define BULLET_SPEED 16

typedef enum portal_color { BLUE, ORANGE } portal_color_t;
typedef enum portal_direction { UP, DOWN, LEFT, RIGHT } portal_direction_t;
typedef struct portal
{
    s16 x, y;
    portal_color_t color;
    portal_direction_t direction;
    ALLEGRO_BITMAP* bitmap;
} portal_t;
typedef struct portal_gun_bullet
{
    f32 x, y;
    f32 dx, dy, angle;
    portal_color_t color;
    ALLEGRO_BITMAP* bitmap;
} portal_gun_bullet_t;
typedef struct portal_gun
{
    s16 x, y;
    portal_color_t color;
    ALLEGRO_BITMAP* bitmap;
    f32 angle;
    b1 hasShot;
    portal_gun_bullet_t* bullets[MAX_BULLETS];
    u8 current_bullet_index;
} portal_gun_t;

typedef struct player
{
    s16 x, y;
    s32 speed;
    b1 split;
    u8 split_x, split_y;
    ALLEGRO_BITMAP* bitmap;
} player_t;

extern player_t* player;
extern portal_gun_t* portal_gun;
extern ALLEGRO_BITMAP* blue_portal_gun_bitmap;
extern ALLEGRO_BITMAP* orange_portal_gun_bitmap;
extern ALLEGRO_BITMAP* blue_portal_bullet_bitmap;
extern ALLEGRO_BITMAP* orange_portal_bullet_bitmap;

void init_player();
void update_player();
void draw_player();
void destroy_player();

void init_portal_gun();
void update_portal_gun();
void draw_portal_gun();
void destroy_portal_gun();

#endif
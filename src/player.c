#include "player.h"

player_t* player;
portal_gun_t* portal_gun;

void init_player()
{
    player = malloc(sizeof(player_t));
    player->x = 0;
    player->y = 0;
    player->split = false;
    player->split_x = 0;
    player->split_y = 0;
    player->bitmap = al_create_sub_bitmap(tileset_bitmap, 64, 48, 8, 16);
}

void update_player()
{
    player->x = inputs.fb_mouse_x;
    player->y = inputs.fb_mouse_y;
}

void draw_player()
{
    al_draw_bitmap(player->bitmap, player->x, player->y, 0);
}

void destroy_player()
{
    al_destroy_bitmap(player->bitmap);
    free(player);
}

void init_portal_gun()
{
    portal_gun = malloc(sizeof(portal_gun_t));
    portal_gun->x = (FB_WIDTH / 2);
    portal_gun->y = (FB_HEIGHT / 2);
    portal_gun->bitmap = al_create_sub_bitmap(tileset_bitmap, 80, 48, 24, 16);
    portal_gun->color = BLUE;
    portal_gun->angle = 0.0f;
}

void update_portal_gun()
{
    f32 dist_x = inputs.fb_mouse_x - (FB_WIDTH / 2);
    f32 dist_y = inputs.fb_mouse_y - (FB_WIDTH / 2);

    portal_gun->angle = atan2f(dist_y, dist_x) - atan2f(0.0f, 1.0f);
}

void draw_portal_gun()
{
    al_draw_rotated_bitmap(portal_gun->bitmap, 12, 8, portal_gun->x, portal_gun->y, portal_gun->angle, 0);
}

void destroy_portal_gun()
{
    al_destroy_bitmap(portal_gun->bitmap);
    free(portal_gun);
}

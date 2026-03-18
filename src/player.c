#include "player.h"

player_t* player;
portal_gun_t* portal_gun;
ALLEGRO_BITMAP* blue_portal_gun_bitmap;
ALLEGRO_BITMAP* orange_portal_gun_bitmap;
ALLEGRO_BITMAP* blue_portal_bullet_bitmap;
ALLEGRO_BITMAP* orange_portal_bullet_bitmap;

void init_player()
{
    player = malloc(sizeof(player_t));
    player->x = 0;
    player->y = 0;
    player->speed = 3 * FB_SCALE;
    player->split = false;
    player->split_x = 0;
    player->split_y = 0;
    player->bitmap = al_create_sub_bitmap(tileset_bitmap, 64, 48, 8, 16);
}

void update_player()
{
    if (al_key_down(&inputs.keyboard_state, ALLEGRO_KEY_A))
    {
        player->x -= player->speed;
    }
    if (al_key_down(&inputs.keyboard_state, ALLEGRO_KEY_D))
    {
        player->x += player->speed;
    }
    if (al_key_down(&inputs.keyboard_state, ALLEGRO_KEY_W))
    {
        player->y -= player->speed;
    }
    if (al_key_down(&inputs.keyboard_state, ALLEGRO_KEY_S))
    {
        player->y += player->speed;
    }
}

void draw_player()
{
    al_draw_scaled_bitmap(player->bitmap, player->x, player->y, 8, 16, player->x, player->y, 16, 32, 0);
    al_draw_scaled_rotated_bitmap(player->bitmap, 4, 8, player->x, player->y, FB_SCALE, FB_SCALE, 0.0f, 0);
}

void destroy_player()
{
    al_destroy_bitmap(player->bitmap);
    free(player);
}

void init_portal_gun()
{
    blue_portal_gun_bitmap = al_create_sub_bitmap(tileset_bitmap, 80, 48, 24, 16);
    orange_portal_gun_bitmap = al_create_sub_bitmap(tileset_bitmap, 80, 64, 24, 16);

    portal_gun = malloc(sizeof(portal_gun_t));
    portal_gun->x = (FB_WIDTH / 2);
    portal_gun->y = (FB_HEIGHT / 2);
    portal_gun->bitmap = blue_portal_gun_bitmap;
    portal_gun->color = BLUE;
    portal_gun->angle = 0.0f;
    portal_gun->hasShot = false;
    portal_gun->current_bullet_index = 0;

    blue_portal_bullet_bitmap = al_create_sub_bitmap(tileset_bitmap, 104, 48, 8, 8);
    orange_portal_bullet_bitmap = al_create_sub_bitmap(tileset_bitmap, 104, 64, 8, 8);

    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        portal_gun->bullets[i] = malloc(sizeof(portal_gun_bullet_t));
        portal_gun_bullet_t* bullet = portal_gun->bullets[i];
        
        
        bullet->x = -10;bullet->y = -10, bullet->dx = 0.0f, bullet->dy = 0.0f, 
            bullet->bitmap = blue_portal_bullet_bitmap, bullet->angle = 0.0f, bullet->color = BLUE;
    }
    
}

void update_portal_gun()
{
    f32 dist_x = inputs.fb_mouse_x - (portal_gun->x + 12);
    f32 dist_y = inputs.fb_mouse_y - (portal_gun->y + 8);

    portal_gun->angle = atan2f(dist_y, dist_x) - atan2f(0.0f, 1.0f);

    if (inputs.mouse_state.buttons & 1 && !portal_gun->hasShot)
    {
        portal_gun->color = BLUE;
        portal_gun->bitmap = blue_portal_gun_bitmap;
        if (al_get_sample_instance_playing(samples->shoot_instances[0])) al_stop_sample_instance(samples->shoot_instances[0]);
        al_play_sample_instance(samples->shoot_instances[0]);

        portal_gun_bullet_t* bullet = portal_gun->bullets[portal_gun->current_bullet_index];
        bullet->color = BLUE;
        bullet->angle = portal_gun->angle;
        bullet->bitmap = blue_portal_bullet_bitmap;
        bullet->dx = cos(portal_gun->angle);
        bullet->dy = sin(portal_gun->angle);
        bullet->x = portal_gun->x + (cos(bullet->angle) * (7 * FB_SCALE));
        bullet->y = portal_gun->y + (sin(bullet->angle) * (7 * FB_SCALE));
        portal_gun->current_bullet_index = portal_gun->current_bullet_index == 3 ? 0 : portal_gun->current_bullet_index + 1;

        portal_gun->hasShot = true;
    }
    if (inputs.mouse_state.buttons & 2 && !portal_gun->hasShot)
    {
        portal_gun->color = ORANGE;
        portal_gun->bitmap = orange_portal_gun_bitmap;
        if (al_get_sample_instance_playing(samples->shoot_instances[1])) al_stop_sample_instance(samples->shoot_instances[1]);
        al_play_sample_instance(samples->shoot_instances[1]);

        portal_gun_bullet_t* bullet = portal_gun->bullets[portal_gun->current_bullet_index];
        bullet->color = ORANGE;
        bullet->angle = portal_gun->angle;
        bullet->bitmap = orange_portal_bullet_bitmap;
        bullet->dx = cos(portal_gun->angle);
        bullet->dy = sin(portal_gun->angle);
        bullet->x = portal_gun->x + (cos(bullet->angle) * (7 * FB_SCALE));
        bullet->y = portal_gun->y + (sin(bullet->angle) * (7 * FB_SCALE));
        printf("%f, %f\n", bullet->dx, bullet->dy);
        portal_gun->current_bullet_index = portal_gun->current_bullet_index == 3 ? 0 : portal_gun->current_bullet_index + 1;

        portal_gun->hasShot = true;
    }
    
    if (!(inputs.mouse_state.buttons & 1) && !(inputs.mouse_state.buttons & 2))
    {
        portal_gun->hasShot = false;
    }

    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        portal_gun->bullets[i]->x += portal_gun->bullets[i]->dx * BULLET_SPEED;
        portal_gun->bullets[i]->y += portal_gun->bullets[i]->dy * BULLET_SPEED;
    }

    portal_gun->x = player->x;
    portal_gun->y = player->y;
}

void draw_portal_gun()
{
    int flipFlags = inputs.fb_mouse_x < portal_gun->x ? ALLEGRO_FLIP_VERTICAL : 0;
    al_draw_scaled_rotated_bitmap(portal_gun->bitmap, 12, 8, portal_gun->x, portal_gun->y, FB_SCALE, FB_SCALE, 
        portal_gun->angle, flipFlags);

    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        portal_gun_bullet_t* bullet = portal_gun->bullets[i];
        if (!bullet) return;
        if (!bullet->bitmap) return;
        al_draw_scaled_rotated_bitmap(bullet->bitmap, 4, 4, bullet->x, bullet->y, FB_SCALE, FB_SCALE, bullet->angle, 0);
    }
}

void destroy_portal_gun()
{
    al_destroy_bitmap(portal_gun->bitmap);

    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        al_destroy_bitmap(portal_gun->bullets[i]->bitmap);
        free(portal_gun->bullets[i]);
    }
    

    free(portal_gun);
}

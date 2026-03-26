#include "player.h"

player_t* player;
portal_gun_t* portal_gun;
ALLEGRO_BITMAP* blue_portal_gun_bitmap;
ALLEGRO_BITMAP* orange_portal_gun_bitmap;
ALLEGRO_BITMAP* blue_portal_bullet_bitmap;
ALLEGRO_BITMAP* orange_portal_bullet_bitmap;

void init_player()
{
    // Create player and set properties
    player = malloc(sizeof(player_t));
    player->x = 0;
    player->y = 0;
    player->speed = 3;
    player->split = false;
    player->split_x = 0;
    player->split_y = 0;
    player->bitmap = al_create_sub_bitmap(tileset_bitmap, 64, 48, 8, 16);
}

void update_player()
{
    // Get input
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
    // Draw player (using rotated draw cuz scaling is easier)
    al_draw_rotated_bitmap(player->bitmap, 4, 8, player->x, player->y, 0.0f, 0);
}

void destroy_player()
{
    // Cleanup player
    al_destroy_bitmap(player->bitmap);
    free(player);
}

void init_portal_gun()
{
    // Create bitmaps
    blue_portal_gun_bitmap = al_create_sub_bitmap(tileset_bitmap, 80, 48, 24, 16);
    orange_portal_gun_bitmap = al_create_sub_bitmap(tileset_bitmap, 80, 64, 24, 16);
    blue_portal_bullet_bitmap = al_create_sub_bitmap(tileset_bitmap, 104, 48, 8, 8);
    orange_portal_bullet_bitmap = al_create_sub_bitmap(tileset_bitmap, 104, 64, 8, 8);

    // Create portal gun and set properties
    portal_gun = malloc(sizeof(portal_gun_t));
    portal_gun->x = (FB_WIDTH / 2);
    portal_gun->y = (FB_HEIGHT / 2);
    portal_gun->bitmap = blue_portal_gun_bitmap;
    portal_gun->color = BLUE;
    portal_gun->angle = 0.0f;
    portal_gun->hasShot = false;
    portal_gun->current_bullet_index = 0;

    // Create bullets and set properties
    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        portal_gun->bullets[i] = malloc(sizeof(portal_gun_bullet_t));
        portal_gun_bullet_t* bullet = portal_gun->bullets[i];
        bullet->x = -10;
        bullet->y = -10;
        bullet->dx = 0.0f;
        bullet->dy = 0.0f;
        bullet->angle = 0.0f;
        bullet->color = BLUE;
        bullet->bitmap = blue_portal_bullet_bitmap;
    }
}

void update_portal_gun()
{
    // Set angle to point at mouse
    f32 dist_x = inputs.fb_mouse_x - portal_gun->x;
    f32 dist_y = inputs.fb_mouse_y - portal_gun->y;
    portal_gun->angle = atan2f(dist_y, dist_x) - atan2f(0.0f, 1.0f);

    if (inputs.mouse_state.buttons & 1 && !portal_gun->hasShot)
    {
        portal_gun->color = BLUE;
        portal_gun->bitmap = blue_portal_gun_bitmap;

        // Play shoot sound
        if (al_get_sample_instance_playing(samples->shoot_instances[0])) al_stop_sample_instance(samples->shoot_instances[0]);
        al_play_sample_instance(samples->shoot_instances[0]);

        shoot_bullet();
        portal_gun->hasShot = true;
    }
    if (inputs.mouse_state.buttons & 2 && !portal_gun->hasShot)
    {
        portal_gun->color = ORANGE;
        portal_gun->bitmap = orange_portal_gun_bitmap;

        // Play shoot sound
        if (al_get_sample_instance_playing(samples->shoot_instances[1])) al_stop_sample_instance(samples->shoot_instances[1]);
        al_play_sample_instance(samples->shoot_instances[1]);

        shoot_bullet();
        portal_gun->hasShot = true;
    }
    
    if (!(inputs.mouse_state.buttons & 1) && !(inputs.mouse_state.buttons & 2))
    {
        portal_gun->hasShot = false;
    }

    check_bullet_collisions();

    portal_gun->x = player->x;
    portal_gun->y = player->y;
}

void shoot_bullet()
{
    // Get first available bullet and cycle index
    portal_gun_bullet_t* bullet = portal_gun->bullets[portal_gun->current_bullet_index];
    portal_gun->current_bullet_index = portal_gun->current_bullet_index == MAX_BULLETS - 1 ? 0 : portal_gun->current_bullet_index + 1;

    // Set properties
    bullet->color = portal_gun->color;
    bullet->angle = portal_gun->angle;
    bullet->bitmap = bullet->color == BLUE ? blue_portal_bullet_bitmap : orange_portal_bullet_bitmap;
    bullet->dx = cos(portal_gun->angle);
    bullet->dy = sin(portal_gun->angle);
    
    // Set position to end of portal gun
    bullet->x = portal_gun->x + (cos(bullet->angle) * 7);
    bullet->y = portal_gun->y + (sin(bullet->angle) * 7);
}

void check_bullet_collisions()
{
    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        portal_gun_bullet_t* bullet = portal_gun->bullets[i];

        if (bullet->x <= -8 || bullet->y >= fb.width + 8 || 
            bullet->y <= -8 || bullet->y >= fb.width + 8)
        {
            bullet->dx = 0;
            bullet->dy = 0;
            bullet->bitmap = NULL;
            continue;
        }

        // Get next pos, set ray to next pos, reset pos
        f32 old_x = bullet->x;
        f32 old_y = bullet->y;
        f32 next_x = bullet->x += bullet->dx * BULLET_SPEED;
        f32 next_y = bullet->y += bullet->dy * BULLET_SPEED;
        bullet->x = next_x;
        bullet->y = next_y;
        set_bullet_ray(bullet);
        bullet->x = old_x;
        bullet->y = old_y;

        for (size_t j = 0; j < MAX_TILES; j++)
        {
            tile_t* tile = level.tiles[i];
            b1 hits = false;

            if (tile == NULL) { continue; }

            for (u8 s = 0; s < tile->sides; s++)
            {
                // Next position index
                u8 next = s == tile->sides - 1 ? 0 : s + 1;

                if (get_line_collision(tile->x_positions[s], tile->y_positions[s],
                    tile->x_positions[next], tile->y_positions[next],
                    bullet->ray_start_x, bullet->ray_start_y,
                    bullet->ray_end_x, bullet->ray_end_y))
                {
                    hits = true;
                }
            }
        }
    }
}

void set_bullet_ray(portal_gun_bullet_t* bullet)
{
    bullet->ray_start_x = bullet->x - (cos(bullet->angle) * 4);
    bullet->ray_start_y = bullet->y - (sin(bullet->angle) * 4);

    bullet->ray_end_x = bullet->x + (cos(bullet->angle) * 4);
    bullet->ray_end_y = bullet->y + (sin(bullet->angle) * 4);
}

void draw_portal_gun()
{
    // Draw portal gun and flip if facing left
    int flipFlags = inputs.fb_mouse_x < portal_gun->x ? ALLEGRO_FLIP_VERTICAL : 0;
    al_draw_rotated_bitmap(portal_gun->bitmap, 12, 8, portal_gun->x, portal_gun->y, 
        portal_gun->angle, flipFlags);

    // Draw bullets
    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        portal_gun_bullet_t* bullet = portal_gun->bullets[i];
        if (bullet->bitmap == NULL) { continue; }
        al_draw_rotated_bitmap(bullet->bitmap, 4, 4, bullet->x, bullet->y, bullet->angle, 0);
        al_draw_line(bullet->ray_start_x, bullet->ray_start_y, bullet->ray_end_x, bullet->ray_end_y, al_map_rgb(0, 255, 0), 1.0f);
    }
}

void destroy_portal_gun()
{
    // Cleanup portal gun and bullets

    al_destroy_bitmap(blue_portal_gun_bitmap);
    al_destroy_bitmap(orange_portal_gun_bitmap);
    al_destroy_bitmap(blue_portal_bullet_bitmap);
    al_destroy_bitmap(orange_portal_bullet_bitmap);

    for (u8 i = 0; i < MAX_BULLETS; i++)
    {
        free(portal_gun->bullets[i]);
    }

    free(portal_gun);
}

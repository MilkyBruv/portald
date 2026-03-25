#include "level.h"

level_t level;

void init_level(level_t* level)
{
    for (size_t i = 0; i < MAX_TILES; i++)
    {
        level->tiles[i] = malloc(sizeof(tile_t));
        level->tiles[i]->bitmap = NULL;
    }
    
    level->tiles[0]->x = 0;
    level->tiles[0]->y = 0;
    level->tiles[0]->sides = 4;
    level->tiles[0]->x_positions[0] = 0;
    level->tiles[0]->x_positions[1] = 16;
    level->tiles[0]->x_positions[2] = 16;
    level->tiles[0]->x_positions[3] = 0;
    level->tiles[0]->y_positions[0] = 0;
    level->tiles[0]->y_positions[1] = 0;
    level->tiles[0]->y_positions[2] = 16;
    level->tiles[0]->y_positions[3] = 16;
    level->tiles[0]->bitmap = al_create_sub_bitmap(tileset_bitmap, 0, 0, 16, 16);
}

void draw_level(level_t level)
{
    for (size_t i = 0; i < MAX_TILES; i++)
    {
        tile_t* tile = level.tiles[i];

        if (tile == NULL) { continue; }
        if (tile->bitmap == NULL) { continue; }

        al_draw_scaled_bitmap(tile->bitmap, 0, 0, 16, 16, tile->x, tile->y, 
            16, 16, 0);

        for (u8 s = 0; s < tile->sides; s++)
        {
            // Next position index
            u8 next = s == tile->sides - 1 ? 0 : s + 1;

            al_draw_line(tile->x_positions[s], tile->y_positions[s],
                tile->x_positions[next], tile->y_positions[next], al_map_rgb(255, 0, 0), 2.0f);
        }
    }
}

void destroy_level(level_t* level)
{
    for (size_t i = 0; i < MAX_TILES; i++)
    {
        if (level->tiles[i] == NULL) { continue; }
        if (level->tiles[i]->bitmap == NULL) { continue; }

        al_destroy_bitmap(level->tiles[i]->bitmap);
        free(level->tiles[i]);
    }
}

b1 get_line_collision(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4)
{
    // https://www.jeffreythompson.org/collision-detection/line-line.php
    f32 u1 = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / 
        ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    f32 u2 = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / 
        ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    if (u1 >= 0 && u1 <= 1 && u2 >= 0 && u2 <= 1)
    {
        return true;
    }

    return false;
}

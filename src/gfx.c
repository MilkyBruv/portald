#include "gfx.h"

framebuffer_t fb;
ALLEGRO_BITMAP* tileset_bitmap;

void init_gfx()
{
    tileset_bitmap = al_load_bitmap("./res/tileset.png");
    init_framebuffer();
}

void init_framebuffer()
{
    fb.x = 0;
    fb.y = 0;
    fb.width = FB_WIDTH;
    fb.height = FB_HEIGHT;
    fb.scale = 1.0f;
    fb.bitmap = al_create_bitmap(FB_WIDTH, FB_HEIGHT);
}

void scale_framebuffer(ALLEGRO_DISPLAY* display)
{
    fb.scale = (f32) min(
        (f32) ((f32) al_get_display_width(display) / (f32) FB_WIDTH),
        (f32) ((f32) al_get_display_height(display) / (f32) FB_HEIGHT)
    );
    fb.width = FB_WIDTH * fb.scale;
    fb.height = FB_HEIGHT * fb.scale;
    fb.x = (al_get_display_width(display) / 2) - (fb.width / 2);
    fb.y = (al_get_display_height(display) / 2) - (fb.height / 2);

    inputs.fb_mouse_x = (inputs.mouse_state.x - fb.x) / fb.scale;
    inputs.fb_mouse_y = (inputs.mouse_state.y - fb.y) / fb.scale;
}

void draw_framebuffer()
{
    al_draw_scaled_bitmap(fb.bitmap, 0, 0, FB_WIDTH, FB_HEIGHT, fb.x, fb.y,
        fb.width, fb.height, 0);
}

void destroy_gfx()
{
    al_destroy_bitmap(tileset_bitmap);
}

void destroy_framebuffer()
{
    al_destroy_bitmap(fb.bitmap);
}

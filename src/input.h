#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>
#include "types.h"

typedef struct input
{
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_MOUSE_STATE mouse_state;
    u16 fb_mouse_x, fb_mouse_y;
} input_t;

extern input_t inputs;

#endif
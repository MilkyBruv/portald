#ifndef SFX_H
#define SFX_H

#include <stdio.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "types.h"

typedef struct samples
{
    ALLEGRO_SAMPLE* shoot;
    ALLEGRO_SAMPLE_INSTANCE* shoot_instances[2];
} samples_t;

extern samples_t* samples;
extern ALLEGRO_VOICE* voice;
extern ALLEGRO_MIXER* mixer;

void init_sfx();
void destroy_sfx();

#endif
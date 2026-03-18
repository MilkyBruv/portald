#include "sfx.h"

samples_t* samples;
ALLEGRO_MIXER* mixer;
ALLEGRO_VOICE* voice;

void init_sfx()
{
    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);

    samples = malloc(sizeof(samples_t));
    
    samples->shoot = al_load_sample("./res/shoot1.ogg");
    samples->shoot_instances[0] = al_create_sample_instance(samples->shoot);
    samples->shoot_instances[1] = al_create_sample_instance(samples->shoot);

    if (!samples->shoot) printf("fumble\n");
    
    for (u8 i = 0; i < 2; i++)
    {
        al_attach_sample_instance_to_mixer(samples->shoot_instances[i], mixer);
        al_set_sample_instance_playmode(samples->shoot_instances[i], ALLEGRO_PLAYMODE_ONCE);
    }
}

void destroy_sfx()
{
    al_destroy_sample(samples->shoot);
    al_destroy_sample_instance(samples->shoot_instances[0]);
    al_destroy_sample_instance(samples->shoot_instances[1]);
    free(samples);
    al_detach_mixer(mixer);
    al_destroy_mixer(mixer);
    al_destroy_voice(voice);
}

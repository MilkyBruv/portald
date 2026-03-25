#define SDL_MAIN_HANDLED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include "types.h"
#include "player.h"
#include "input.h"
#include "level.h"

int main(int argc, const char* argv[])
{
    printf("Allegro v%i\n", al_get_allegro_version());

    al_init();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_install_keyboard();
    al_install_mouse();

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
    al_set_window_title(display, "PortalD");

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    init_gfx();
    init_sfx();
    init_player();
    init_portal_gun();
    init_level(&level);

    b1 running = true;
    b1 redraw = false;
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    while (running)
    {
        al_wait_for_event(event_queue, &event);
        al_get_keyboard_state(&inputs.keyboard_state);
        al_get_mouse_state(&inputs.mouse_state);

        switch (event.type)
        {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;

        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(display);
            break;

        case ALLEGRO_EVENT_TIMER:
            // update
            update_player();
            update_portal_gun();

            // enable drawing
            redraw = true;
            break;
        
        default:
            break;
        }

        if (al_is_event_queue_empty(event_queue) && redraw)
        {
            // draw
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_set_target_bitmap(fb.bitmap);

            al_clear_to_color(al_map_rgb(192, 192, 192));
            draw_player();
            draw_portal_gun();
            draw_level(level);

            al_set_target_bitmap(al_get_backbuffer(display));
            scale_framebuffer(display);
            draw_framebuffer();

            al_flip_display();

            redraw = false;
        }
    }

    al_stop_timer(timer);

    destroy_player();
    destroy_portal_gun();

    destroy_level(&level);

    destroy_gfx();
    destroy_sfx();

    al_unregister_event_source(event_queue, al_get_display_event_source(display));
    al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
    al_unregister_event_source(event_queue, al_get_keyboard_event_source());
    al_unregister_event_source(event_queue, al_get_mouse_event_source());

    al_shutdown_image_addon();
    al_shutdown_primitives_addon();
    al_uninstall_audio();
    al_uninstall_keyboard();
    al_uninstall_mouse();
    
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
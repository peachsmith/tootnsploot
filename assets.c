#include "assets.h"

#include <stdio.h>
#include <stdlib.h>

int tns_load_all_assets(cr_app *app)
{
    // images
    if (cr_load_texture(app, "assets/images/ui.png") == NULL)
    {
        fprintf(stderr, "failed to load ui image\n");
        return 0;
    }
    if (cr_load_texture(app, "assets/images/tootnsploot.png") == NULL)
    {
        fprintf(stderr, "failed to load Toot n Sploot sprite sheet\n");
        return 0;
    }
    if (cr_load_texture(app, "assets/images/title.png") == NULL)
    {
        fprintf(stderr, "failed to load Toot n Sploot sprite sheet\n");
        return 0;
    }

    // fonts
    if (cr_load_font(app, "assets/fonts/Kenney Pixel.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load Kenney Pixel font\n");
        return 0;
    }
    if (cr_load_font(app, "assets/fonts/pokemon_fire_red.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load pokemon_fire_red font\n");
        return 0;
    }
    if (cr_load_font(app, "assets/fonts/press-start.regular.ttf", 8) == NULL)
    {
        fprintf(stderr, "failed to load press-start.regular font\n");
        return 0;
    }

    // sounds
    if (cr_load_sound(app, "assets/audio/confirmation_002.ogg", AUDIO_TYPE_SOUND_EFFECT) == NULL)
    {
        fprintf(stderr, "failed to load confirmation_002.ogg\n");
        return 0;
    }

    return 1;
}

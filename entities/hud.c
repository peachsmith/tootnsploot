#include "tootnsploot.h"
#include "assets.h"
#include "entities/entity_types.h"
#include "entities/hud.h"
#include "util/sprite.h"

#define HUD_BUFSIZE 256

static void render_hud(cr_app *app, cr_entity *hud)
{
    char buffer[HUD_BUFSIZE];
    int n = HUD_BUFSIZE - 1;

    cr_entity *corgi = app->extension->entity_handles[TNS_HANDLE_CORGI];
    if (corgi == NULL)
    {
        return;
    }

    // render the HUD panel
    cr_rect r = {
        .x = hud->x_pos,
        .y = hud->y_pos,
        .w = app->entity_types[hud->type].width,
        .h = app->entity_types[hud->type].height};
    cr_set_color(app, CR_COLOR_VINIK_BLACK);
    cr_draw_rect(app, &r, 1);

    // render the score
    int res = snprintf(
        buffer,
        n,
        "SCORE %d",
        app->extension->counters[TNS_COUNTER_SCORE]);
    if (res < 0 || res >= n)
    {
        return;
    }
    cr_draw_text(app, app->fonts[TNS_FONT_PRESS_START], buffer, 2, 2);

    // render the available music notes
    cr_draw_text(app, app->fonts[TNS_FONT_PRESS_START], "BREATH", 2, 12);
    for (int i = 0; i < app->extension->counters[TNS_COUNTER_BREATH]; i++)
    {
        sprite_draw_note(app, 55 + i * 13, 10);
    }
}

void tns_register_hud(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_HUD;
    t->width = 240;
    t->height = 22;
    t->render = render_hud;
    t->collide = NULL;
}

cr_entity *tns_create_hud(cr_app *app, int x, int y)
{
    cr_entity *hud = NULL;

    hud = cr_create_entity(app);
    if (hud == NULL)
    {
        return NULL;
    }

    hud->type = ENTITY_TYPE_HUD;
    hud->x_pos = x;
    hud->y_pos = y;

    return hud;
}

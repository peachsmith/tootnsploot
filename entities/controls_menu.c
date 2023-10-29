#include "tootnsploot.h"
#include "assets.h"
#include "entities/entity_types.h"
#include "entities/main_menu.h"
#include "util/ui.h"

#include "common/util.h"

static char *items[4] = {
    "Press the left and right\narrow keys to move Skippy.",
    "Tap space bar to jump",
    "Hold space bar to sploot.\nSplooting allows Skippy\nto catch his breath.",
    "Press x to toot. The power\nof music restores the\ncritters' will to live."};

#define ITEM_HEIGHT 30

static void render_controls_menu(cr_app *app, cr_entity *menu)
{
    cr_font *font = app->fonts[TNS_FONT_PRESS_START];
    int x_origin = 20;
    int y_origin = 45;

    int w = app->entity_types[menu->type].width;
    int h = app->entity_types[menu->type].height;

    // Render the menu panel.
    // For the main menu, this takes up the entire screen.
    cr_rect r = {
        .x = 0,
        .y = 0,
        .w = w,
        .h = h};
    cr_set_color(app, CR_COLOR_VINIK_BLACK);
    cr_draw_rect(app, &r, 1);

    // Render the menu title.
    cr_draw_text(
        app,
        font,
        "CONTROLS",
        88,
        10);

    // Render menu items.
    int i;
    int y_offset = 0;
    for (i = 0; i < 4; i++)
    {
        int dy = 0;
        cr_rect bounds = {
            .x = x_origin,
            .y = y_origin + y_offset,
            .w = 210,
            .h = 0};
        cr_draw_text_bounded(
            app,
            font,
            items[i],
            &bounds,
            &dy);
        y_offset += dy + 10;
    }
}

static void update_controls_menu(cr_app *app, cr_entity *menu)
{
}

void tns_register_controls_menu(cr_entity_type *t)
{
    t->width = 240;
    t->height = 160;

    t->render = render_controls_menu;
    t->update = update_controls_menu;
}

cr_entity *tns_create_controls_menu(cr_app *app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->type = ENTITY_TYPE_CONTROLS_MENU;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}

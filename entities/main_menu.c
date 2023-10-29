#include "tootnsploot.h"
#include "assets.h"
#include "entities/entity_types.h"
#include "entities/main_menu.h"
#include "util/ui.h"

#include "common/util.h"

static char *items[3] = {
    "START",
    "CONTROLS",
    "MEET THE CHARACTERS"};

#define ITEM_HEIGHT 20

static void render_main_menu(cr_app *app, cr_entity *menu)
{
    cr_font *font = app->fonts[TNS_FONT_PRESS_START];
    int x_origin = 60;
    int y_origin = 90;

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

    // Render the title.
    cr_rect src = {
        .x = 0,
        .y = 0,
        .w = 140,
        .h = 70};
    cr_rect dest = {
        .x = 45,
        .y = 5,
        .w = 140,
        .h = 70};
    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_TITLE],
        &src,
        &dest,
        0);

    // Render menu items.
    int i;
    for (i = 0; i < 3; i++)
    {
        cr_draw_text(
            app,
            font,
            items[i],
            x_origin,
            y_origin + i * ITEM_HEIGHT);
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        x_origin - 20,
        y_origin - 3 + menu->cursor_y * ITEM_HEIGHT);
}

static void update_main_menu(cr_app *app, cr_entity *menu)
{
    menu->ticks++;

    // We don't have any reason to exceed 60 ticks, so we just reset the tick
    // count and start over whenever we reach that threshold. The number 60
    // was chosen because that's the desired framerate, but it doesn't really
    // matter.
    if (menu->ticks >= 60)
    {
        menu->ticks = 0;
    }
}

void tns_register_main_menu(cr_entity_type *t)
{
    t->width = 240;
    t->height = 160;

    t->render = render_main_menu;
    t->update = update_main_menu;
}

cr_entity *tns_create_main_menu(cr_app *app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->type = ENTITY_TYPE_MAIN_MENU;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}

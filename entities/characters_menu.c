#include "tootnsploot.h"
#include "assets.h"
#include "entities/entity_types.h"
#include "entities/main_menu.h"
#include "util/ui.h"
#include "util/sprite.h"

#include "common/util.h"

#define ITEM_COUNT 6
#define ITEM_HEIGHT 20

// a list of all the characters in the game
static char *character_names[ITEM_COUNT] = {
    "Skippy",
    "Squirrelton",
    "Squirrelina",
    "Thomas H. Foolery",
    "Byarf Squirrelson",
    "Throbgar the Sumptuous"};

// character names meant to be rendered as the title of a biography
static char *character_titles[ITEM_COUNT] = {
    "        Skippy",
    "      Squirrelton",
    "      Squirrelina",
    "   Thomas H. Foolery",
    "   Byarf Squirrelson",
    "Throbgar the Sumptuous"};

// biographies of the various characters
static char *character_bios[ITEM_COUNT] = {
    "Skippy is a corgi who made\nhis living as a street\nperformer playing the\ntrumpet. "
    "He was abandoned\nas a puppy and had to grow\nup fast."
    " If there's one\nthing Skippy can't stand,\nit's a frown. So he's made\nit his mission"
    " to cheer\neveryone up with the power\nof music.",

    "Squirrelton is an average\njoe just trying to live\nhis best life."
    " He enjoys\nburying acorns and\nforgetting where he put\nthem."
    " It's like a game of\nhide and seek that he\nplays every winter.",

    "Squirrelina is active in\nher community. She is the\ndirector of"
    " the parks and\nrecreation department.\nIn her spare time, she\nenjoys making"
    " pottery and\nvolunteering at her local\nsoup kitchen.",

    "Thomas H. Foolery is the\nson of a wealthy business\nsquirrel. Due to his\n"
    "connections, he was able\nto weasel his way into\nsquirrel harvard, where"
    " he\nmajored in deez nuts.",

    "Byarf, son of Squirrel, is\na mighty warrior who cut\nhis teeth at the battle\nof"
    " Beaver Bend. A\ndecorated hero, he now\nresides at Oak Hill, where\nhe runs a coffee"
    " shop\nwith his son, Hyorf. They\nhave a pet alligator named\nChompers.",

    "Throbgar the Sumptuous,\nAKA: The Nutcracker\nAKA: Chestnut\nAKA: Hot Cheeks\n"
    "AKA: Squirrel Marlon Brando\nAKA: Throbby Tables\nis a total prick."};

static void draw_skippy(cr_app *app, int x, int y)
{
    sprite_draw_corgi(app, x, y, 0, 0);
}

static void draw_squirrel(cr_app *app, int x, int y)
{
    sprite_draw_critter(app, x, y, 0, 0);
}

// an array of functions
// each function signature is effectively this:
// void draw_something(cr_app*, int, int)
static void (*draw_functions[ITEM_COUNT])(cr_app *, int, int) = {
    draw_skippy,
    draw_squirrel,
    draw_squirrel,
    draw_squirrel,
    draw_squirrel,
    draw_squirrel};

static void render_characters_menu(cr_app *app, cr_entity *menu)
{
    cr_font *font = app->fonts[TNS_FONT_PRESS_START];
    int x_origin = 60;
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
    if (menu->data)
    {
        cr_draw_text(
            app,
            font,
            character_titles[menu->data - 1],
            30,
            10);
    }
    else
    {
        cr_draw_text(
            app,
            font,
            "MEET THE CHARACTERS",
            50,
            10);
    }

    // Render menu items.
    if (menu->data)
    {
        int x_offset = menu->data > 1 ? 45 : 40;
        draw_functions[menu->data - 1](
            app,
            x_origin + x_offset,
            y_origin - 20);

        cr_rect bounds = {
            .x = 15,
            .y = y_origin + 10,
            .w = 210,
            .h = 0};
        cr_draw_text_bounded(
            app,
            font,
            character_bios[menu->data - 1],
            &bounds,
            NULL);
    }
    else
    {
        int i;
        for (i = 0; i < ITEM_COUNT; i++)
        {
            cr_draw_text(
                app,
                font,
                character_names[i],
                x_origin,
                y_origin + i * ITEM_HEIGHT);

            int y_offset = i > 0 ? 5 : -5;
            draw_functions[i](
                app,
                x_origin - 30,
                (y_origin - 10) + y_offset + i * ITEM_HEIGHT);

            // For debugging text boundaries
            // bounds.h = dy;
            // cr_set_color(app, CR_COLOR_VINIK_SEA_GREEN);
            // cr_draw_rect(app, &bounds, 0);
        }

        // Render the cursor.
        ui_draw_cursor(
            app,
            x_origin - 55,
            y_origin - 3 + menu->cursor_y * ITEM_HEIGHT);
    }
}

static void update_characters_menu(cr_app *app, cr_entity *menu)
{
}

void tns_register_characters_menu(cr_entity_type *t)
{
    t->width = 240;
    t->height = 160;

    t->render = render_characters_menu;
    t->update = update_characters_menu;
}

cr_entity *tns_create_characters_menu(cr_app *app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->type = ENTITY_TYPE_CHARACTERS_MENU;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}

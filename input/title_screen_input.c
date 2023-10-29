#include "tootnsploot.h"
#include "input/input.h"
#include "entities/entity_types.h"
#include "scenes/scenes.h"

#include "common/util.h"

/**
 * This function starts the scene transition process.
 * First, it prepares the scene loader and input handler for the next scene.
 * Then it locates any transition entities in the current scene and sets their
 * data field to 1. This tells the transition entity to begin doing whatever
 * it needs to do. The transition entity will call the scene loader to load
 * the next scene.
 */
static void begin_transition(cr_app *app, cr_func loader, cr_func input)
{
    app->transition_loader = loader;
    app->transition_input = input;

    // Start the screen transition by marking the data field as 1 for any
    // entity that has an entity type of ENTITY_TYPE_TRANSITION.
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_TRANSITION && app->entities[i].present)
        {
            app->entities[i].data = 1 | TX_RESUME;
            app->overlays[app->overlay_count++] = &(app->entities[i]);
        }
    }
}

void tns_title_screen_input(cr_app *app)
{
    cr_entity *menu = app->menus[app->menu_count - 1];
    cr_entity **handles = app->extension->entity_handles;

    if (menu == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->done = 1;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP) && menu->cursor_y > 0)
    {
        menu->cursor_y--;
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN) && menu->cursor_y < 2)
    {
        menu->cursor_y++;
    }

    // menu item selection
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu->cursor_y)
        {
        case 0:
            // Leave the title screen and load the forest scene.
            cr_pop_input_handler(app);
            begin_transition(app, tns_load_forest_scene, tns_forest_input);
            break;

        case 1:
        {
            // Switch to the controls menu.
            app->menus[app->menu_count++] = handles[TNS_HANDLE_CONTROLS];
            cr_push_input_handler(app, tns_controls_menu_input);
        }
        break;

        case 2:
        {
            // Switch to the characters menu.
            app->menus[app->menu_count++] = handles[TNS_HANDLE_CHARACTERS];
            cr_push_input_handler(app, tns_characters_menu_input);
        }
        break;

        default:
            break;
        }
    }
}

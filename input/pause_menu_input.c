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
            app->entities[i].data = 1;
            app->overlays[app->overlay_count++] = &(app->entities[i]);
        }
    }
}

void tns_pause_menu_input(cr_app *app)
{
    // Locate the pause menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    // Close the pause menu if the escape key or x key is pressed.
    if (cr_consume_input(app, CR_KEYCODE_X) || cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        app->pause = 0;
        cr_pop_input_handler(app);
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP) && menu_entity->cursor_y > 0)
    {
        menu_entity->cursor_y--;
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN) && menu_entity->cursor_y < 2)
    {
        menu_entity->cursor_y++;
    }

    // Resume
    // Main Menu
    // Quit
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
            app->menu_count--;
            app->pause = 0;
            cr_pop_input_handler(app);
            break;
        case 1:
            // Remove the input handlers of the pause menu and the forest
            // scene, then transition to the main menu.
            cr_pop_input_handler(app);
            cr_pop_input_handler(app);
            begin_transition(app, tns_load_title_screen, tns_title_screen_input);
            break;
        case 2:
            app->done = 1;
            break;
        default:
            break;
        }
    }
}

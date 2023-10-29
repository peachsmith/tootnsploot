#include "tootnsploot.h"
#include "input/input.h"
#include "entities/entity_types.h"
#include "scenes/scenes.h"

void tns_characters_menu_input(cr_app *app)
{
    // Locate the characters menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    // If either the escape key or the x key is pressed, return to the main menu.
    if (cr_consume_input(app, CR_KEYCODE_ESCAPE) || cr_consume_input(app, CR_KEYCODE_X))
    {
        if (menu_entity->data)
        {
            menu_entity->data = 0;
            return;
        }

        app->menu_count--;
        cr_pop_input_handler(app);
        return;
    }

    // If a character is selected, stop processing input here.
    if (menu_entity->data)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 5)
        {
            menu_entity->cursor_y++;
        }
    }

    // menu item selection
    // Skippy
    // Squirrelton
    // Squirrelina
    // Thomas H. Foolery
    // Byarf Squirrelson
    // Throbgar the Sumptuous
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
            menu_entity->data = 1;
            break;
        case 1:
            menu_entity->data = 2;
            break;
        case 2:
            menu_entity->data = 3;
            break;
        case 3:
            menu_entity->data = 4;
            break;
        case 4:
            menu_entity->data = 5;
            break;
        case 5:
            menu_entity->data = 6;
            break;
        default:
            break;
        }
    }
}

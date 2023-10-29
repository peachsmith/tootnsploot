#include "tootnsploot.h"
#include "input/input.h"
#include "entities/entity_types.h"
#include "scenes/scenes.h"

void tns_controls_menu_input(cr_app *app)
{
    // If either the escape key or the x key is pressed, return to the main menu.
    if (cr_consume_input(app, CR_KEYCODE_ESCAPE) || cr_consume_input(app, CR_KEYCODE_X))
    {
        app->menu_count--;
        cr_pop_input_handler(app);
        return;
    }
}

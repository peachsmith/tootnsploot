#include "tootnsploot.h"
#include "input/input.h"
#include "scenes/scenes.h"
#include "entities/transition.h"
#include "entities/forest.h"
#include "entities/main_menu.h"
#include "entities/controls_menu.h"
#include "entities/characters_menu.h"

#include "common/util.h"

void tns_load_title_screen(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;
    
    app->scene = TNS_SCENE_TITLE_SCREEN;

    util_set_camera(app, CR_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // Create the main menu and set it as the current active menu.
    cr_entity *main_menu = tns_create_main_menu(app);
    app->menus[app->menu_count++] = main_menu;
    app->pause = 1;

    handles[TNS_HANDLE_CONTROLS] = tns_create_controls_menu(app);
    handles[TNS_HANDLE_CHARACTERS] = tns_create_characters_menu(app);

    // scene transition
    handles[TNS_HANDLE_TRANSITION] = tns_create_transition(app);
}

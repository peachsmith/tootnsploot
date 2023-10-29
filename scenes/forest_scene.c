#include "tootnsploot.h"
#include "input/input.h"
#include "scenes/scenes.h"
#include "entities/transition.h"
#include "entities/pause_menu.h"
#include "entities/hud.h"
#include "entities/forest.h"
#include "entities/corgi.h"
#include "entities/floor.h"
#include "entities/critter.h"
#include "entities/wall.h"

#include "common/util.h"

void tns_load_forest_scene(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    app->scene = TNS_SCENE_FOREST;

    // Reset the counters.
    app->extension->counters[TNS_COUNTER_BREATH] = 3;
    app->extension->counters[TNS_COUNTER_CRITTERS] = 0;
    app->extension->counters[TNS_COUNTER_SCORE] = 0;
    for (int i = 0; i < 8; i++)
    {
        app->extension->counters[i + TNS_COUNTER_CRITTER_SLOT_OFFSET] = 0;
    }

    util_set_camera(app, CR_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    tns_create_pause_menu(app);

    // scene transition
    handles[TNS_HANDLE_TRANSITION] = tns_create_transition(app);

    // background
    tns_create_forest(app, 0, 22);

    // floor and walls
    tns_create_floor(app, 0, 142);
    tns_create_wall(app, -24, 2);
    tns_create_wall(app, 240, 2);

    // player
    app->extension->entity_handles[TNS_HANDLE_CORGI] = tns_create_corgi(app, 80, 118);

    // heads up display
    tns_create_hud(app, 0, 0);
}

void tns_perform_forest(cr_app *app)
{
    if (app->pause)
    {
        return;
    }

    int critter_count = app->extension->counters[TNS_COUNTER_CRITTERS];
    if (!(app->ticks % 120) && critter_count < 4)
    {
        // Generate random number from 0 to 7. This is the critter slot.
        // If the slot is unoccupied, then create a critter and mark the
        // slot as occupied.
        int slot = rand() % 8;
        if (!app->extension->counters[slot + TNS_COUNTER_CRITTER_SLOT_OFFSET])
        {
            cr_entity *critter = tns_create_critter(app, 4 + slot * 20 + slot * 10, 25);
            app->extension->counters[TNS_COUNTER_CRITTERS]++;
            app->extension->counters[slot + TNS_COUNTER_CRITTER_SLOT_OFFSET] = 1;
            critter->data = slot + TNS_COUNTER_CRITTER_SLOT_OFFSET;
            if (slot > 3)
            {
                cr_set_flag(critter, ENTITY_FLAG_MIRROR);
            }
        }
    }
}

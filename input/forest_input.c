#include "tootnsploot.h"
#include "input/input.h"
#include "entities/entity_types.h"
#include "entities/corgi.h"
#include "entities/note.h"

#include "common/util.h"
#include "common/collision.h"

void tns_forest_input(cr_app *app)
{
    // Pause the application.
    if (cr_consume_input(app, CR_KEYCODE_ESCAPE) || cr_consume_input(app, CR_KEYCODE_Q))
    {
        // Locate the pause menu.
        cr_entity *pause_menu = NULL;
        for (int i = 0; i < app->entity_cap; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_PAUSE_MENU && app->entities[i].present)
            {
                pause_menu = &(app->entities[i]);
            }
        }

        // Set the pause menu as the active menu.
        app->menus[app->menu_count++] = pause_menu;

        cr_push_input_handler(app, tns_pause_menu_input);

        app->pause = 1;
        return;
    }

    // Locate the player entity.
    cr_entity *target = app->extension->entity_handles[TNS_HANDLE_CORGI];
    if (target == NULL)
    {
        return;
    }

    int splooting = app->actuation_counters[CR_KEYCODE_SPACE] >= 20;

    int max_walk = 6;
    int min_walk = 1;
    int walk_acc = 1;

    // left movement
    if (cr_peek_input(app, CR_KEYCODE_LEFT) && !splooting)
    {
        if (app->actuation_counters[CR_KEYCODE_LEFT] < 10)
        {
            app->actuation_counters[CR_KEYCODE_LEFT]++;
        }

        cr_clear_flag(target, ENTITY_FLAG_MIRROR);

        if (target->x_acc > -min_walk)
        {
            if (target->x_acc < min_walk)
            {
                target->x_acc = -min_walk;
            }
        }
        else
        {
            target->x_acc -= walk_acc;
        }

        if (target->x_acc < -max_walk)
        {
            target->x_acc = -max_walk;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_LEFT] = 0;
    }

    // right movement
    if (cr_peek_input(app, CR_KEYCODE_RIGHT) && !splooting)
    {
        if (app->actuation_counters[CR_KEYCODE_RIGHT] < 10)
        {
            app->actuation_counters[CR_KEYCODE_RIGHT]++;
        }

        cr_set_flag(target, ENTITY_FLAG_MIRROR);

        if (target->x_acc < min_walk)
        {
            if (target->x_acc > -min_walk)
            {
                target->x_acc = min_walk;
            }
        }
        else
        {
            target->x_acc += walk_acc;
        }

        if (target->x_acc > max_walk)
        {
            target->x_acc = max_walk;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_RIGHT] = 0;
    }

    // down arrow key
    if (cr_peek_input(app, CR_KEYCODE_DOWN))
    {
        if (!cr_check_flag(target, ENTITY_FLAG_DOWN))
        {
            cr_set_flag(target, ENTITY_FLAG_DOWN);
        }
    }
    else if (cr_check_flag(target, ENTITY_FLAG_DOWN))
    {
        cr_clear_flag(target, ENTITY_FLAG_DOWN);
    }

    // jumping
    if (cr_peek_input(app, CR_KEYCODE_SPACE))
    {
        if (cr_check_flag(target, ENTITY_FLAG_GROUND) &&
            !cr_check_flag(target, ENTITY_FLAG_JUMP) &&
            app->actuation_counters[CR_KEYCODE_SPACE] == 0)
        {
            cr_clear_flag(target, ENTITY_FLAG_GROUND);
            cr_set_flag(target, ENTITY_FLAG_JUMP);
            cr_clear_flag(target, ENTITY_FLAG_MOVE);

            // Clear the carrier and the correction factor.
            target->carrier = NULL;
            target->y_t = 0;

            // Set the vertical acceleration to some significant negative
            // value to launch the entity upward.
            target->y_acc = -13;

            app->actuation_counters[CR_KEYCODE_SPACE]++;
        }

        // Increase the actuation counter to allow jump height control in the
        // target entity's update function.
        else if (app->actuation_counters[CR_KEYCODE_SPACE] < 20)
        {
            app->actuation_counters[CR_KEYCODE_SPACE]++;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_SPACE] = 0;
    }

    // play a musical note
    if (cr_consume_input(app, CR_KEYCODE_X) && !splooting)
    {
        // Limit the number of notes to 3 at any given time.
        if (app->extension->counters[TNS_COUNTER_BREATH] > 0)
        {
            cr_entity *f = tns_create_note(
                app,
                target->x_pos - app->cam.x,
                target->y_pos - app->cam.y + 4);
            if (f != NULL)
            {
                if (cr_check_flag(target, ENTITY_FLAG_MIRROR))
                {
                    f->x_pos += app->entity_types[target->type].width / 2;
                    f->x_acc = 16;
                }
                else
                {
                    f->x_acc = -16;
                    cr_set_flag(f, ENTITY_FLAG_MIRROR);
                }
            }

            app->extension->counters[TNS_COUNTER_BREATH]--;
        }
    }
}

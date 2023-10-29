#include "tootnsploot.h"
#include "entities/entity_types.h"
#include "entities/transition.h"
#include "scenes/scenes.h"

#include "common/util.h"
#include "common/dialog.h"

#define HANDOFF_THRESHOLD 30

static void handoff(cr_app *app, cr_func next_scene)
{
    cr_entity payload;
    cr_entity *handle;

    // Get the transition entity from the current scene.
    handle = app->extension->entity_handles[TNS_HANDLE_TRANSITION];
    if (handle == NULL)
    {
        return;
    }

    // If the TX_RESUME flag is set on the transition's data field, set
    // the update flag on the transition's flags field so the next scene
    // doesn't start in a paused state.
    if (handle->data & TX_RESUME)
    {
        cr_set_flag(handle, ENTITY_FLAG_UPDATE);
    }

    // Preserve the necessary information from the transition entity.
    payload.ticks = handle->ticks;
    payload.data = handle->data;
    payload.flags = handle->flags;

    // Clear the current scene and load the next scene.
    tns_clear_scene(app);
    next_scene(app);

    // Get the transition entity from the next scene.
    handle = app->extension->entity_handles[TNS_HANDLE_TRANSITION];
    if (handle == NULL)
    {
        return;
    }

    // Populate the transition entity of the new scene with the information
    // from the previous scene.
    // Increment the tick count of the new transition entity to
    // prevent the update function from being called again.
    handle->ticks = payload.ticks + 1;
    handle->data = payload.data;
    handle->flags = payload.flags;

    // Add the new transition entity to the overlay slice.
    app->overlays[app->overlay_count++] = handle;

    // Let the new transition entity know that the handoff is complete.
    handle->data |= TX_HANDOFF;
}

static void render_transition(cr_app *app, cr_entity *transition)
{
    if (!transition->data)
    {
        return;
    }

    cr_rect transition_rect;
    transition_rect.x = 0;
    transition_rect.y = 0;
    transition_rect.w = app->screen_width;
    transition_rect.h = app->screen_height;

    uint32_t c = 0x00000000;
    uint32_t a = 0xFF;

    if (transition->ticks < 30)
    {
        a = transition->ticks * 8;
        if (transition->ticks >= 29)
        {
            a = 0xFF;
        }
    }
    else if (transition->ticks < 60)
    {
        a = 0xFF;
    }
    else
    {
        a = 0xFF - ((transition->ticks - 60) * 8);
        if (transition->ticks >= 89)
        {
            a = 0x00;
        }
    }

    cr_set_color(app, c | (a << 24));
    cr_draw_rect(app, &transition_rect, 1);
}

static void update_transition(cr_app *app, cr_entity *transition)
{
    if (!transition->data)
    {
        return;
    }

    // The screen fades out for 30 frames, followed by 30 frames of blank
    // screen, followed by 30 frames of the next frame fading in.
    // During the second 30 frame period, we load the next scene.
    if (transition->ticks < transition->tick_limit)
    {
        // Load the next scene.
        if (transition->ticks == HANDOFF_THRESHOLD && !(transition->data & TX_HANDOFF))
        {
            handoff(app, app->transition_loader);

            // Once the next scene has been loaded, the memory for the
            // previous scene has been freed. This means the reference
            // to the transition entity we have been using should be
            // considered no longer valid. So we return to the update
            // loop.
            return;
        }

        transition->ticks++;

        return;
    }

    // Once this point has been reached, the transition is complete.

    // Reset these fields so they can be used to transition to a new scene.
    transition->ticks = 0;
    transition->data = 0;

    cr_push_input_handler(app, app->transition_input);

    // If the update flag is set, unpause the application so the next scene
    // doesn't start in a paused state.
    if (cr_check_flag(transition, ENTITY_FLAG_UPDATE))
    {
        app->pause = 0;
    }
}

void tns_register_transition(cr_entity_type *t)
{
    t->width = 50;
    t->height = 50;
    t->render = render_transition;
    t->update = update_transition;
}

cr_entity *tns_create_transition(cr_app *app)
{
    cr_entity *transition = NULL;

    transition = cr_create_entity(app);
    if (transition == NULL)
    {
        return NULL;
    }

    transition->type = ENTITY_TYPE_TRANSITION;
    transition->tick_limit = 90;
    cr_set_flag(transition, ENTITY_FLAG_PAUSE);
    cr_set_flag(transition, ENTITY_FLAG_MENU);

    return transition;
}
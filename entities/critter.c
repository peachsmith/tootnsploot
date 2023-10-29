#include "tootnsploot.h"
#include "assets.h"
#include "entities/entity_types.h"
#include "entities/critter.h"
#include "util/sprite.h"

#include "common/util.h"
#include "common/dialog.h"

static int get_critter_x_vel(cr_entity *critter)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4};

    int x_vel = 0;
    // int adjusted = 0;

    // Convert acceleration to velocity
    if (critter->x_acc > 0)
    {
        x_vel = a_to_v[critter->x_acc];
    }
    else if (critter->x_acc < 0)
    {
        x_vel = -(a_to_v[-(critter->x_acc)]);
    }

    // The critter has no reason to collide with a solid block from
    // the side.
    // if (critter->x_t)
    // {
    //     x_vel += critter->x_t;
    // }

    return x_vel; // adjusted;
}

static int get_critter_y_vel(cr_entity *critter)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1};

    int y_vel = 0;
    if (critter->y_acc > 0)
    {
        y_vel = a_to_v[critter->y_acc];
    }
    else if (critter->y_acc < 0)
    {
        y_vel = -(a_to_v[-(critter->y_acc)]);
    }

    if (critter->y_t)
    {
        y_vel += critter->y_t;
    }

    if (critter->ticks < 120)
    {
        return 0;
    }

    return y_vel;
}

static void render_critter(cr_app *app, cr_entity *critter)
{
    int tile = 0;
    int grounded = cr_check_flag(critter, ENTITY_FLAG_GROUND);

    // If the critter has fallen from the tree, is still in the air, and has
    // not yet been healed with the power of music, render it upside down.
    if (!grounded && !critter->result)
    {
        tile = 2;
    }
    else if (grounded && critter->result)
    {
        if (critter->animation_ticks < 5)
        {
            tile = 1;
        }
    }
    else if (!critter->result)
    {
        tile = 2;
    }

    // Render the critter sprite.
    if (critter->ticks >= 120)
    {
        if (critter->result || critter->ticks < 400)
        {
            sprite_draw_critter(
                app,
                critter->x_pos,
                critter->y_pos,
                cr_check_flag(critter, ENTITY_FLAG_MIRROR),
                tile);
        }
    }
    else
    {
        tile = 0;
        if (critter->animation_ticks < 8)
        {
            tile = 1;
        }
        sprite_draw_leaves(
            app,
            critter->x_pos,
            critter->y_pos,
            tile);
    }

    // Render the sparkle animation.
    if (critter->result && critter->cursor_x && critter->cursor_x < 60)
    {
        // 5 animation frames rendered over the course of 60 game loop frames
        int sparkle_tile = critter->cursor_x < 12 ? 0 : critter->cursor_x < 24 ? 1
                                                    : critter->cursor_x < 36   ? 2
                                                    : critter->cursor_x < 48   ? 3
                                                                               : 4;
        sprite_draw_sparkle(
            app,
            critter->x_pos,
            critter->y_pos,
            sparkle_tile);
    }

    // Render the expiration animation.
    if (!critter->result && critter->cursor_x && critter->cursor_x < 45)
    {
        // 3 animation frames rendered over the course of 45 game loop frames
        int puff_tile = critter->cursor_x < 15 ? 0 : critter->cursor_x < 30 ? 1
                                                                            : 2;
        sprite_draw_puff(
            app,
            critter->x_pos + 2,
            critter->y_pos + 3,
            puff_tile);
    }

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = critter->x_pos + app->cam.x;
        hit_box.y = critter->y_pos + app->cam.y;
        hit_box.w = app->entity_types[critter->type].width;
        hit_box.h = app->entity_types[critter->type].height;

        // Render the critter hit box.
        cr_set_color(app, CR_COLOR_VINIK_ORANGE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

static void update_critter(cr_app *app, cr_entity *critter)
{
    int avx = get_critter_x_vel(critter);
    int avy = get_critter_y_vel(critter);
    int grounded = cr_check_flag(critter, ENTITY_FLAG_GROUND);
    int mirror = cr_check_flag(critter, ENTITY_FLAG_MIRROR);

    //--------------------------------------------------------------------
    // Horizontal Movement

    if (critter->result && grounded)
    {
        critter->x_acc = mirror ? 1 : -1;
    }

    // Update horizontal position.
    critter->x_pos += avx;

    //--------------------------------------------------------------------
    // Vertical Movement

    // vertical correction factor applied by collision with a solid object
    if (critter->y_t)
    {
        critter->y_t = 0;
    }

    // Update vertical position.
    if (app->ticks % 2)
    {
        critter->y_pos += avy;
    }

    //--------------------------------------------------------------------
    // Behavior Logic

    critter->ticks++;

    // rustling leaves animation
    if (critter->ticks < 120)
    {
        critter->animation_ticks++;
        if (critter->animation_ticks >= 16)
        {
            critter->animation_ticks = 0;
        }
    }

    // running animation
    if (critter->result && grounded)
    {
        critter->animation_ticks++;
        if (critter->animation_ticks >= 10)
        {
            critter->animation_ticks = 0;
        }
    }

    // healing and expiration animations
    if (critter->cursor_x && critter->cursor_x < 60)
    {
        critter->cursor_x++;
    }

    // If 400 ticks have passed and the critter has not yet been healed by
    // the power of music, then send it to live on a farm.
    if (critter->ticks == 400 && !critter->result)
    {
        // start the expiration animation
        critter->cursor_x = 1;
    }

    if (critter->ticks >= 445 && !critter->result && critter->present)
    {
        critter->present = 0;
        app->extension->counters[TNS_COUNTER_CRITTERS]--;
        app->extension->counters[critter->data]--;
    }

    // If the critter runs off the screen, remove it from the scene.
    if ((critter->x_pos < -20 || critter->x_pos > 240) && critter->present)
    {
        critter->present = 0;
        app->extension->counters[TNS_COUNTER_CRITTERS]--;
        app->extension->counters[critter->data]--;
    }
}

static void collide_critter(
    cr_app *app,
    cr_entity *critter,
    cr_entity *other,
    cr_collision *t_res)
{
    if (critter->ticks < 120 || critter->result || other->type != ENTITY_TYPE_NOTE)
    {
        return;
    }

    // If the critter has reached its expiration time and has still not been
    // healed, then no more collision should occur.
    if (critter->ticks >= 400 && !critter->result)
    {
        return;
    }

    if (other->present)
    {
        other->present = 0;
    }

    if (!critter->result)
    {
        critter->result = 1;
        // Start the sparkle animation.
        // For this, we appropriate the cursor_x field, since it's not used
        // for anything else in this entity. In the future, we may want to
        // find a smarter way of doing this.
        critter->cursor_x = 1;

        // play a whimsical sound effect
        cr_play_sound(app, app->sounds[TNS_SOUND_EFFECT_CONFIRMATION]);
    }

    // If a certain score is reached, the game ends.
    if (app->extension->counters[TNS_COUNTER_SCORE] < 100)
    {
        app->extension->counters[TNS_COUNTER_SCORE]++;
    }
    else
    {
        app->done = 1;
    }
}

void tns_register_critter(cr_entity_type *t)
{
    t->width = 20;
    t->height = 12;
    t->render = render_critter;
    t->update = update_critter;
    t->get_x_vel = get_critter_x_vel;
    t->get_y_vel = get_critter_y_vel;
    t->collide = collide_critter;
    t->spur = 1;
}

cr_entity *tns_create_critter(cr_app *app, int x, int y)
{
    cr_entity *critter = NULL;

    critter = cr_create_entity(app);
    if (critter == NULL)
    {
        return NULL;
    }

    critter->type = ENTITY_TYPE_CRITTER;
    critter->x_pos = x;
    critter->y_pos = y;
    critter->y_acc = 1;

    return critter;
}

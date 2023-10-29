#include "tootnsploot.h"
#include "entities/entity_types.h"
#include "entities/note.h"
#include "util/sprite.h"

#include "common/util.h"
#include "common/dialog.h"

static int get_note_x_vel(cr_entity *note)
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
    if (note->x_acc > 0)
    {
        x_vel = a_to_v[note->x_acc];
    }
    else if (note->x_acc < 0)
    {
        x_vel = -(a_to_v[-(note->x_acc)]);
    }

    if (note->x_t)
    {
        x_vel += note->x_t;
    }

    return x_vel;
}

static int get_note_y_vel(cr_entity *note)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1};

    int y_vel = 0;
    if (note->y_acc > 0)
    {
        y_vel = a_to_v[note->y_acc];
    }
    else if (note->y_acc < 0)
    {
        y_vel = -(a_to_v[-(note->y_acc)]);
    }

    if (note->y_t)
    {
        y_vel += note->y_t;
    }

    return y_vel;
}

static void render_note(cr_app *app, cr_entity *note)
{
    sprite_draw_note(
        app,
        note->x_pos,
        note->y_pos);

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = note->x_pos + app->cam.x;
        hit_box.y = note->y_pos + app->cam.y;
        hit_box.w = app->entity_types[note->type].width;
        hit_box.h = app->entity_types[note->type].height;

        // Render the note hit box.
        cr_set_color(app, CR_COLOR_VINIK_ORANGE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

static void update_note(cr_app *app, cr_entity *note)
{
    int avx = get_note_x_vel(note);
    int avy = get_note_y_vel(note);
    int mirror = cr_check_flag(note, ENTITY_FLAG_MIRROR);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // Update horizontal position.
    if (note->ticks > 35)
    {
        if (!(note->ticks % 4))
        {
            note->x_pos += avx;
        }
    }
    else if (note->ticks > 20)
    {
        if (!(note->ticks % 2))
        {
            note->x_pos += avx;
        }
    }
    else
    {
        note->x_pos += avx;
    }

    // Update horizontal acceleration.
    if (note->ticks < 35)
    {
        if (!mirror && note->x_acc > 0 && !(note->ticks % 2))
        {
            note->x_acc--;
        }
        if (mirror && note->x_acc < 0 && !(note->ticks % 2))
        {
            note->x_acc++;
        }
    }
    else
    {
        if (note->ticks < 85 || note->ticks >= 135)
        {
            note->x_acc = mirror ? 1 : -1;
        }
        else
        {
            note->x_acc = mirror ? -1 : 1;
        }
    }

    //--------------------------------------------------------------------
    // Vertical Movement

    // Update vertical position.
    if (app->ticks % 2)
    {
        note->y_pos += avy;
    }

    // Update vertical acceleration.
    if (note->ticks == 35)
    {
        note->y_acc = -1;
    }

    //--------------------------------------------------------------------
    // Behavior Logic

    note->animation_ticks++;
    if (note->animation_ticks >= 15)
    {
        note->animation_ticks = 0;
    }

    note->ticks++;

    if (note->ticks >= 185)
    {
        note->present = 0;
    }
}

static void collide_note(
    cr_app *app,
    cr_entity *note,
    cr_entity *other,
    cr_collision *t_res)
{
}

void tns_register_note(cr_entity_type *t)
{
    t->width = 11;
    t->height = 11;
    t->render = render_note;
    t->update = update_note;
    t->get_x_vel = get_note_x_vel;
    t->get_y_vel = get_note_y_vel;
    t->collide = collide_note;
    t->spur = 1;
}

cr_entity *tns_create_note(cr_app *app, int x, int y)
{
    cr_entity *note = NULL;

    note = cr_create_entity(app);
    if (note == NULL)
    {
        return NULL;
    }

    note->type = ENTITY_TYPE_NOTE;
    note->x_pos = x;
    note->y_pos = y;

    return note;
}

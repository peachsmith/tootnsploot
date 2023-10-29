#include "tootnsploot.h"
#include "entities/entity_types.h"
#include "entities/floor.h"
#include "util/sprite.h"

#include "common/util.h"
#include "common/collision.h"

static void collide_block(
    cr_app *app,
    cr_entity *block,
    cr_entity *other,
    cr_collision *t_res)
{
    // The collision resolution correction factor formula is pulled from
    // the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.

    // Don't collide with musical notes.
    if (other->type == ENTITY_TYPE_NOTE)
    {
        return;
    }

    // Clear the jump flag and set the ground flag.
    // Only do this if the entity collided with the block from above.
    if (t_res->cn.y < 0 && !t_res->cn.x)
    {
        cr_clear_flag(other, ENTITY_FLAG_JUMP);
        cr_set_flag(other, ENTITY_FLAG_GROUND);
    }

    // Correction factor.
    float t1 = 1 - t_res->t;

    if (t_res->cn.x)
    {
        int avx = app->entity_types[other->type].get_x_vel(other);
        int absx = avx > 0 ? avx : -(avx);
        float correction = t_res->cn.x * absx * t1;

        // Account for rounding.
        if (correction < 0)
        {
            correction -= 0.5f;
        }
        else if (correction > 0)
        {
            correction += 0.5f;
        }

        other->x_t += (int)correction;
    }

    if (t_res->cn.y)
    {
        // Correction factor for landing on top of the block.
        int avy = app->entity_types[other->type].get_y_vel(other);
        int absy = avy > 0 ? avy : -(avy);
        float correction = t_res->cn.y * absy * t1;

        // Account for rounding.
        if (correction < 0)
        {
            correction -= 0.5f;
        }
        else if (correction > 0)
        {
            correction += 0.5f;
        }

        other->y_t += (int)correction;
    }
}

static void render_wall(cr_app *app, cr_entity *wall)
{
    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = wall->x_pos + app->cam.x;
        hit_box.y = wall->y_pos + app->cam.y;
        hit_box.w = app->entity_types[wall->type].width;
        hit_box.h = app->entity_types[wall->type].height;

        cr_set_color(app, CR_COLOR_VINIK_YELLOW);
        cr_draw_rect(app, &hit_box, 0);
    }
}

void tns_register_wall(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_WALL;
    t->width = 24;
    t->height = 140;
    t->render = render_wall;
    t->collide = collide_block;
}

cr_entity *tns_create_wall(cr_app *app, int x, int y)
{
    cr_entity *wall = NULL;

    wall = cr_create_entity(app);
    if (wall == NULL)
    {
        return NULL;
    }

    wall->type = ENTITY_TYPE_WALL;
    wall->x_pos = x;
    wall->y_pos = y;

    return wall;
}

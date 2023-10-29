#include "common/collision.h"
#include "common/util.h"

#include <stdlib.h>

// Directions.
// These determine which way we traverse a linked list using previous and
// next pointers.
#define DIR_BACKWARD 0
#define DIR_FORWARD 1

// maximum allowable collisions for one entity
#define COL_LIMIT 64

#define COL_VERTICAL 1
#define COL_HORIZONTAL 2

typedef struct col_res
{
    cr_collision col;
    cr_entity *target;
} col_res;

static void detect_collisions(
    cr_app *app,
    cr_entity *source,
    int source_i,
    col_res *cols,
    int *count,
    int direction)
{
    // Don't bother checking for collisions with entities that have been removed.
    if (!source->present)
    {
        return;
    }

    // If direction has a non zero value, then we check every entity after
    // the source. Otherwise we check every entity before the source.
    int target_i = direction ? source_i + 1 : source_i - 1;
    if (target_i < 0 || target_i >= app->entity_cap - 1)
    {
        return;
    }

    cr_entity *target = NULL;

    for (int i = target_i; direction ? i < app->entity_cap : i >= 0; direction ? i++ : i--)
    {
        target = &(app->entities[i]);

        cr_collision res;
        res.t = 0.0f;
        res.tx = 0.0f;
        res.ty = 0.0f;

        if (target->present && app->entity_types[target->type].slope)
        {
            if (common_line(app, source, target, &res))
            {
                // Add the collision result to the array.
                if (*count < COL_LIMIT)
                {
                    cols[*count].col = res;
                    cols[*count].target = target;
                    *count = *count + 1;
                }
            }
        }
        else if (target->present)
        {
            // Use swept AABB to determine if the two entities will collide.
            if (common_swept_aabb(app, source, target, &res))
            {
                // Add the collision result to the array.
                if (*count < COL_LIMIT)
                {
                    cols[*count].col = res;
                    cols[*count].target = target;
                    *count = *count + 1;
                }
            }
        }
    }
}

/**
 * A comparator function for sorting a list of collision results.
 *
 * Params:
 *   col_res* - a reference to a collision result
 *   col_res* - a reference to another collision result
 */
static int greater(col_res *a, col_res *b)
{
    float t0; // t value of entity a
    float t1; // t value of entity b
    float tx0;
    float ty0;
    float tx1;
    float ty1;
    cr_point cn0; // contact normal of entity a
    cr_point cn1; // contact normal of entity b
    int corner;   // corner collision flag

    tx0 = a->col.tx;
    ty0 = a->col.ty;
    tx1 = b->col.tx;
    ty1 = b->col.ty;
    t0 = a->col.t;
    t1 = b->col.t;
    cn0 = a->col.cn;
    cn1 = b->col.cn;
    corner = 0;

    // Check the contact normals.
    // If both the x and y components of a contact normal
    // are non zero, then it is considered "greater" than
    // a true contact normal.
    if (cn0.x && cn0.y && (!cn1.x || !cn1.y))
    {
        corner = 1;
    }

    if ((tx0 || ty0) && (!tx1 && !ty1))
    {
        return 0;
    }

    if ((tx1 || ty1) && (!tx0 && !ty0))
    {
        return 1;
    }

    // The t value takes priority.
    // We only check the corner collision into account if
    // both t values are equal.
    if (t0 > t1 || (t0 == t1 && corner))
    {
        return 1;
    }

    return 0;
}

void common_handle_collisions(cr_app *app)
{
    // The collision detection has three stages:
    // Stage 1: Collision Detection
    // Stage 2: Collision Sorting
    // Stage 3: Collision Resolution

    col_res cols[COL_LIMIT]; // collision result list

    for (int i = 0; i < app->entity_cap; i++)
    {
        int count = 0;
        cr_entity *source = &(app->entities[i]);

        // Clear the ground flag of the source entity.
        cr_clear_flag(source, ENTITY_FLAG_GROUND);

        //--------------------------------------------------------
        // Stage 1: Collision Detection
        // We traverse the entity list forwards and backwards from the source
        // entity. This prevents checking an entity for collision with itself.
        detect_collisions(app, source, i, cols, &count, DIR_FORWARD);
        detect_collisions(app, source, i, cols, &count, DIR_BACKWARD);

        //--------------------------------------------------------
        // Stage 2: Collision Sorting
        int sorted = 0;
        while (!sorted)
        {
            sorted = 1;
            for (int i = 0; i < count; i++)
            {
                col_res tmp;
                if (i < count - 1)
                {
                    if (greater(&(cols[i]), &(cols[i + 1])))
                    {
                        sorted = 0;
                        tmp = cols[i];
                        cols[i] = cols[i + 1];
                        cols[i + 1] = tmp;
                    }
                }
            }
        }

        int corner_resolution = 0;
        if (count >= 1)
        {
            int walls = 0;
            int floors = 0;

            for (int i = 0; i < count; i++)
            {
                if (cols[i].col.cn.x && !cols[i].col.cn.y)
                {
                    walls++;
                }

                if (!cols[i].col.cn.x && cols[i].col.cn.y)
                {
                    floors++;
                }

                if (cols[i].col.cn.x && cols[i].col.cn.y)
                {
                    if (walls > floors)
                    {
                        corner_resolution = COL_HORIZONTAL;
                        cols[i].col.cn.y = 0;
                    }
                    else
                    {
                        corner_resolution = COL_VERTICAL;
                        cols[i].col.cn.x = 0;
                    }
                }
            }
        }

        //--------------------------------------------------------
        // Stage 3: Collision Resolution
        for (int i = 0; i < count; i++)
        {
            cr_entity *a;     // source entity A
            cr_entity *b;     // target entity B
            cr_collision col; // swept AABB result
            cr_collider cola; // source collision callback
            cr_collider colb; // target collision callback

            a = source;
            b = cols[i].target;

            if (app->entity_types[b->type].slope)
            {
                if (common_line(app, a, b, &col))
                {
                    // Call the source entity's collision function.
                    cola = app->entity_types[a->type].collide;
                    if (cola != NULL)
                    {
                        cola(app, a, b, &col);
                    }

                    // Call the target entity's collision function.
                    colb = app->entity_types[b->type].collide;
                    if (colb != NULL)
                    {
                        colb(app, b, a, &col);
                    }
                }
            }
            else
            {
                if (common_swept_aabb(app, a, b, &col))
                {
                    // Convert corner collisions to either horizontal or
                    // vertical resolution.
                    if (col.cn.x && col.cn.y)
                    {
                        switch (corner_resolution)
                        {
                        case COL_HORIZONTAL:
                            col.cn.y = 0;
                            break;
                        case COL_VERTICAL:
                            col.cn.x = 0;
                            break;
                        default:
                            break;
                        }
                    }

                    // Call the source entity's collision function.
                    cola = app->entity_types[a->type].collide;
                    if (cola != NULL)
                    {
                        cola(app, a, b, &col);
                    }

                    // Call the target entity's collision function.
                    colb = app->entity_types[b->type].collide;
                    if (colb != NULL)
                    {
                        colb(app, b, a, &col);
                    }
                }
            }
        }

        // Clear the update flag of the current source entity.
        cr_clear_flag(source, ENTITY_FLAG_UPDATE);

    } // END main collision detection loop
}

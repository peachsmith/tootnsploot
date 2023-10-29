#include "common/collision.h"
#include "common/util.h"

#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Determines if a ray intersects a rectangle.
 * It is based on the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.
 *
 * This is an implementation of Swept AABB (Axis-Aligned Bounding Box).
 * It assumes that all entities are rectangles that are axis-aligned, meaning
 * their sides are parallel with the x and y axes.
 *
 * Given a origin point P and an ending point Q, the direction vector D can
 * be calculated as Q - P.
 * The position of the target rectangle, RP, is the point of the top left
 * vertex of the rectangle. The size of the rectangle, RS, is a vector whose
 * components are the width and height of the rectangle. The point of the
 * bottom right vertex can therefore be calculated as RP + RS.
 *
 * Any point along the line segment formed by direction vector D can be
 * calculated as follows:
 * P(t) = D * t + P
 * where P(0) = P and P(1) = Q.
 *
 * Intersection Points N and F
 * ---------------------------
 * The intersection points are the points along the line D where the line
 * intersects the sides of the target rectangle. These points are the near
 * point N and the far point F.
 * Near point N is the intersection point closest to origin point P and far
 * point F is the intersection point closes to point Q.
 * For the purposes of this implementation, near point N is considered to be
 * the contact point CP.
 *
 * Given a contact point CP, the contact normal CN is the unit vector
 * orthogonal to the side of the target rectangle
 *
 *
 *     P
 *      \
 *       \
 *        \ N
 *  RP +---*--------------+
 *     |    \             |
 *     |     \            |
 *     |      \           |
 *     |       \ F        |
 *     +--------*---------+ RP + RS
 *               \
 *                \
 *                 Q
 *
 *
 * Perfect Corner Collision
 * ------------------------
 * The source video does not take into account perfect corner collisions where
 * the x and y components of t_near are equal. The caller of this function
 * must, therefore, make a context-dependent decision on how to resolve the
 * collision.
 * For example, if the application is using a top-down view, then a
 * perfect corner collision may be treated as a head on collision with a
 * wall, and both the x and y velocities could be set to 0.
 * Alternatively, if the application is a side-scrolling platformer, the
 * x velocity could be set to 0, and the y velocity would be controlled by
 * gravity.
 *
 * Params:
 *   cr_point* - the origin point of the ray
 *   cr_point* - the direction vector of the ray
 *   cr_rect* - the target rectangle
 *   cr_collision* - results of intersection detection
 */
static int ray_v_rect(
    cr_point *p,
    cr_point *d,
    cr_rect *r,
    cr_collision *res)
{
    if (p == NULL || d == NULL || r == NULL)
    {
        return 0;
    }

    // Verify that a collision is possible before performing

    // source to the left of the target
    // moving left
    if (d->x < 0 && r->x >= p->x)
    {
        return 0;
    }

    // source to the right of the target
    // moving right
    if (d->x > 0 && r->x + r->w <= p->x)
    {
        return 0;
    }

    // source above of the target
    // moving up
    if (d->y < 0 && r->y >= p->y)
    {
        return 0;
    }

    // source below the target
    // moving down
    if (d->y > 0 && r->y + r->h <= p->y)
    {
        return 0;
    }

    // Direction vector components.
    float dx = (float)d->x;
    float dy = (float)d->y;

    // Flags to detect division by 0.
    // These will be referred to as dz flags.
    int near_x_dz = 0;
    int near_y_dz = 0;
    int far_x_dz = 0;
    int far_y_dz = 0;

    //--------------------------------------------------------------------
    // Detect division by zero.
    // The values stored in the dz flags indicate the sign of the dividend.
    // A dz value of 1 indicates division of a positive dividen by 0, and a
    // dz value of -1 indicates division of a negative dividend by 0.
    // When attempting to divide by 0, if the dividend of the t_far component
    // is 0, then we do not consider a collision to have occurred.
    if (dx == 0)
    {
        near_x_dz = 1;
        far_x_dz = 1;

        int near_div = r->x - p->x;
        int far_div = r->x + r->w - p->x;

        // spurious collision
        if (far_div == 0)
        {
            return 0;
        }

        if (near_div < 0)
        {
            near_x_dz = -1;
        }

        if (far_div < 0)
        {
            far_x_dz = -1;
        }
    }

    if (dy == 0)
    {
        near_y_dz = 1;
        far_y_dz = 1;

        int near_div = r->y - p->y;
        int far_div = r->y + r->h - p->y;

        // spurious collision
        if (far_div == 0)
        {
            return 0;
        }

        if (near_div < 0)
        {
            near_y_dz = -1;
        }

        if (far_div < 0)
        {
            far_y_dz = -1;
        }
    }

    //--------------------------------------------------------------------
    // Calculate the intersection points N and F.
    // The components of near point N are near_x and near_y.
    // The components of far point F are far_x and far_y.
    //
    // In the event of a division by 0, we assign the intersection point
    // component a value of FLT_MAX or -FLT_MAX depending on the sign of the
    // dividend. If the dividend is positive, the component will be FLT_MAX.
    // If the dividend is negative, the component will be -FLT_MAX.
    //
    // When not dividing by 0, the intersection point components can be
    // calculated like so:
    // near_x = (RPx - Px) / Dx
    // far_x = (RPx + RSx - Px) / Dx
    // near_y = (RPy - Py) / Dy
    // far_y = (RPy + RSy - Py) / Dy

    float near_x;
    float near_y;
    float far_x;
    float far_y;

    // near_x
    if (near_x_dz)
    {
        near_x = near_x_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        near_x = ((float)(r->x) - p->x) / dx;
    }

    // near_y
    if (near_y_dz)
    {
        near_y = near_y_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        near_y = ((float)(r->y) - p->y) / dy;
    }

    // far_x
    if (far_x_dz)
    {
        far_x = far_x_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        far_x = ((float)(r->x) + r->w - p->x) / dx;
    }

    // far_y
    if (far_y_dz)
    {
        far_y = far_y_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        far_y = ((float)(r->y) + r->h - p->y) / dy;
    }

    //--------------------------------------------------------------------
    // Verify the intersection points N and F.
    // In order for the first collision check to work, we need to ensure that
    // N < F. We can do this by swapping the values of the near and far
    // components if the near components are greater than the far components.
    // This accounts for negative values, allowing us to detect collisions in
    // all directions.

    // Ensure that near_x < far_x
    if (near_x > far_x)
    {
        float tmp = near_x;
        near_x = far_x;
        far_x = tmp;
    }

    // Ensure that near_y < far_y
    if (near_y > far_y)
    {
        float tmp = near_y;
        near_y = far_y;
        far_y = tmp;
    }

    // Perform the first collision check.
    // For a collision to occur, the following conditions must be met:
    // near_x < far_y and near_y < far_x.
    // So if near_x > far_y or near_y > far_x, then a collision has not
    // occurred.
    if (near_x > far_y || near_y > far_x)
    {
        return 0;
    }

    //--------------------------------------------------------------------
    // Calculate the t values used to calculate the intersection points N and
    // F. Using the function P(t) = D * t + P we want to find the values
    // t_near and t_far such that P(t_near) = N and P(t_far) = F.

    // t_near is be the maximum of the set { near_x, near_y }.
    float t_near = near_x >= near_y ? near_x : near_y;

    // t_far is be the minimum of the set { far_x, far_y }.
    float t_far = far_x < far_y ? far_x : far_y;

    //--------------------------------------------------------------------
    // Check the t values to verify that the intersection points N and F
    // exist in appropriate locations relative to P and Q.

    // Perform the second collision check.
    // A negative t_far value indicates that the contact point is behind the
    // point P. We do not consider this to be a valid collision.
    //
    //    Q     P  +---------+
    //    *<----*  | target  |
    //             +---------+
    if (t_far < 0)
    {
        return 0;
    }

    // Perform the third collision check.
    // A t_near value of >= 1 indicates that the contact point is at or after
    // point Q. We also do not consider this to be a valid collision.
    //
    //    P     Q  +---------+
    //    *---->*  | target  |
    //             +---------+
    if (t_near >= 1)
    {
        return 0;
    }

    // Prevent spurious collisions due to the source rectangle sitting flush
    // with the target rectangle behind it.
    // We only perform this check if the spur flag is 0.
    //
    //  +---------++---------+
    //  | target  || source  |
    //  +---------++---------+
    if (t_near <= -1)
    {
        return 0;
    }

    //--------------------------------------------------------------------
    // Calculate and set the output values.
    // We want to determine three things:
    // 1. A t value for which P(t) = CP
    // 2. The contact point CP
    // 3. The contact normal CN

    // The t value has already been calculated, so we just set it here.
    res->t = t_near;

    // Calculate the contact point CP.
    // P(t) = D * t + P
    res->cp.x = (int)(dx * t_near + p->x);
    res->cp.y = (int)(dy * t_near + p->y);

    // Determine the contact normal CN.
    // Note that if near_x == near_y, then we have a perfect corner collision
    // and the contact normal will not be set.
    if (near_x > near_y)
    {
        // If near_x > near_y and Dx < 0, then CN is (-1, 0)
        // If near_x > near_y and Dx > 0, then CN is (1, 0)
        if (dx < 0)
        {
            res->cn.x = 1;
            res->cn.y = 0;
        }
        else
        {
            res->cn.x = -1;
            res->cn.y = 0;
        }
    }
    else if (near_x < near_y)
    {
        // If near_x < near_y and Dy < 0, then CN is (0, 1)
        // If near_x < near_y and Dy > 0, then CN is (0, -1)
        if (dy < 0)
        {
            res->cn.x = 0;
            res->cn.y = 1;
        }
        else
        {
            res->cn.x = 0;
            res->cn.y = -1;
        }
    }
    else
    {
        // If we reached this point, we have a perfect corner collision.
        // In this case, the contact normal is not a true normal.
        // We set both x and y to 1 or -1 so that the caller of this
        // function can detect corner collisions and handle them.
        res->cn.x = dx < 0 ? 1 : -1;
        res->cn.y = dy < 0 ? 1 : -1;
    }

    return 1;
}

int common_swept_aabb(
    cr_app *app,
    cr_entity *a,
    cr_entity *b,
    cr_collision *res)
{
    cr_point p; // origin point P
    cr_point d; // direction vector D
    cr_rect r;  // target rectangle R

    if (app == NULL || a == NULL || b == NULL || res == NULL)
    {
        return 0;
    }

    // Get the effective velocity of entity A.
    int avx = app->entity_types[a->type].get_x_vel(a);
    int avy = app->entity_types[a->type].get_y_vel(a);

    // Check for velocity.
    // This function is only intended for a collision scenario in which at
    // least one of the entities has a velocity.
    // Two entities may be overlapping, and that may have some effect
    // elsewhere, but for the purposes of ray casting, it not considered a
    // collision.
    if (avx == 0 && avy == 0 && b->x_vel == 0 && b->y_vel == 0)
    {
        return 0;
    }

    // A moving platform cannot be the source entity.
    if (app->entity_types[a->type].move)
    {
        return 0;
    }

    int spur = 0;
    if (app->entity_types[a->type].spur && app->entity_types[b->type].spur)
    {
        spur = 1;
    }

    // Get the width and height of source entity A.
    int aw = app->entity_types[a->type].width;
    int ah = app->entity_types[a->type].height;

    // Create a rectangle representing the boundaries of target entity B.
    // To construct the target boundary, we add half of the source width to
    // the target width, and half of the source height to the target height.
    // We also add the camera position.
    r.x = b->x_pos + app->cam.x - (aw / 2);
    r.y = b->y_pos + app->cam.y - (ah / 2);
    r.w = app->entity_types[b->type].width + aw;
    r.h = app->entity_types[b->type].height + ah;

    if (aw & 1)
    {
        r.x--;
    }

    if (ah & 1)
    {
        r.y--;
    }

    // The origin point P is the center point of source entity A.
    p.x = a->x_pos + aw / 2;
    p.y = a->y_pos + ah / 2;

    // If the source entity does not have camera focus, add the camera
    // position to the source entity's position.
    if (!app->entity_types[a->type].control)
    {
        p.x += app->cam.x;
        p.y += app->cam.y;
    }

    // If the target entity has camera focus, subtract the camera
    // position from the target entity's position.
    if (app->entity_types[b->type].control)
    {
        r.x -= app->cam.x;
        r.y -= app->cam.y;
    }

    // The direction vector D is the velocity of source entity A.
    d.x = avx;
    d.y = avy;

    if (ray_v_rect(&p, &d, &r, res))
    {
        if (app->debug.collisions)
        {
            util_draw_collision(app, &r, res, &p, &d);
        }

        return 1;
    }
    else if (spur)
    {
        // If both entities are moving, then their collision may be
        // missed by the normal swept AABB method. So we check for
        // overlap between the two entities.
        cr_rect arect = {
            .x = a->x_pos,
            .y = a->y_pos,
            .w = app->entity_types[a->type].width,
            .h = app->entity_types[a->type].height,
        };
        cr_rect brect = {
            .x = b->x_pos,
            .y = b->y_pos,
            .w = app->entity_types[b->type].width,
            .h = app->entity_types[b->type].height,
        };
        cr_overlap o;
        if (common_is_overlapped(&arect, &brect, &o))
        {
            return 1;
        }
    }

    // If the source entity is being carried by a moving entity,
    // we verify that the source entity is still in contact with
    // the carrier.
    if (a->carrier != NULL && app->entity_types[b->type].move)
    {
        int ax = a->x_pos;
        int ay = a->y_pos;
        int bx = b->x_pos + app->cam.x;
        int by = b->y_pos + app->cam.y;
        int bw = app->entity_types[b->type].width;

        if (!app->entity_types[a->type].control)
        {
            ax += app->cam.x;
            ay += app->cam.y;
        }

        // Verify that the source entity's x position is within the
        // horizontal bounds of the platform.
        // We also check that the sum of the sources's y position and height
        // is equal to the y position of the target.
        if (((ax >= bx && ax <= bx + bw) ||
             (ax + aw >= bx && ax + aw <= bx + bw)) &&
            ay + ah == by)
        {
            res->cn.x = 0;
            res->cn.y = -1;
            res->cp.x = p.x;
            res->cp.y = p.y;
            res->t = 0;
            return 1;
        }
    }

    return 0;
}

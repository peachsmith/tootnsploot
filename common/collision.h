#ifndef COMMON_COLLISION_H
#define COMMON_COLLISION_H

#include "crumbs.h"

// defintion of cr_collision struct
struct cr_collision
{
    cr_point cp; // contact point
    cr_point cn; // contact normal
    float t;     // t such that P(t) = CP
    float tx;    // horizontal correction factor for diagonal lines
    float ty;    // veritcal correction factor for diagonal lines
};

/**
 * Result of rectangle overlap calculation.
 */
typedef struct cr_overlap
{
    // The fields dx0, dx1, dy0, and dy1 are referred to as the deltas.
    // They represent the differences between the positions of the sides
    // of two rectangles.
    // Given two rectangles A and B:
    // dx0 is difference between the right side of A and the left side of B.
    // dx1 is difference between the right side of B and the left side of A.
    // dy0 is difference between the bottom side of A and the top side of B.
    // dy1 is difference between the bottom side of B and the top side of A.
    int dx0;
    int dx1;
    int dy0;
    int dy1;
} cr_overlap;

/**
 * Determines if a collision will occur between a moving source entity A and
 * a static target entity B. This uses an implementation of Swept AABB, which
 * assumes that all rectangles are axis-aligned.
 *
 * If A is static and B is moving, then this function treats A as if it were
 * the target and B as if it were the source.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - the source entity
 *   cr_entity* - the target entity
 *   cr_collision* - a pointer to the collision result struct
 */
int common_swept_aabb(
    cr_app *app,
    cr_entity *a,
    cr_entity *b,
    cr_collision *res);

/**
 * Used for slopes.
 * UNDER CONSTRUCTION
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - the source entity
 *   cr_entity* - the target entity
 *   cr_collision* - a pointer to the collision result struct
 */
int common_line(
    cr_app *app,
    cr_entity *a,
    cr_entity *b,
    cr_collision *res);

/**
 * Determines if two rectangles overlap.
 * The third argument is a pointer to an cr_overlap struct.
 * This will receive the results of the overlap calculations.
 *
 * Params:
 *   cr_rect* - a reference to a rect
 *   cr_rect* - a reference to another rect
 *   cr_overlap* - a reference to the overlap calculation result struct
 */
int common_is_overlapped(
    cr_rect *a,
    cr_rect *b,
    cr_overlap *res);

/**
 * Detects and handles collisions between all entities in an application.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
void common_handle_collisions(cr_app *app);

#endif
#include "common/collision.h"

int common_is_overlapped(
    cr_rect *a,
    cr_rect *b,
    cr_overlap *res)
{
    // Get the width and height of both entities.
    int aw = a->w;
    int ah = a->h;
    int bw = b->w;
    int bh = b->h;

    // Get the x and y position of both entities.
    // These will be the position of the left and right edges.
    int a_left = a->x;
    int a_top = a->y;
    int b_left = b->x;
    int b_top = b->y;

    // Get the positions of the right and bottom edges.
    int a_right = a_left + aw;
    int a_bottom = a_top + ah;
    int b_right = b_left + bw;
    int b_bottom = b_top + bh;

    // Clear the result data.
    // dx0 is difference between the right side of A and the left side of B.
    // dx1 is difference between the right side of B and the left side of A.
    // dy0 is difference between the bottom side of A and the top side of B.
    // dy1 is difference between the bottom side of B and the top side of A.
    res->dx0 = 0;
    res->dx1 = 0;
    res->dy0 = 0;
    res->dy1 = 0;

    int overlap_x = 0;
    int overlap_y = 0;

    // Check if the left edge of A is in between the left and right edges
    // of b.
    if (a_left > b_left && a_left < b_right)
    {
        res->dx0 = a_right - b_left;
        res->dx1 = b_right - a_left;
        overlap_x = 1;
    }

    // Check if the right edge of a is in between the left and right edges
    // of B.
    if (a_right > b_left && a_right < b_right)
    {
        res->dx0 = a_right - b_left;
        res->dx1 = b_right - a_left;
        overlap_x = 1;
    }

    // Check if B is inside of A.
    if (b_left >= a_left && b_left <= a_right &&
        b_right >= a_left && b_right <= a_right)
    {
        overlap_x = 1;
    }

    // Check if the top edge of A is in between the top and bottom edges
    // of B.
    if (a_top > b_top && a_top < b_bottom)
    {
        res->dy0 = a_bottom - b_top;
        res->dy1 = b_bottom - a_top;
        overlap_y = 1;
    }

    // Check if the bottom edge of A is in between the top and bottom
    // edges of B.
    if (a_bottom > b_top && a_bottom < b_bottom)
    {
        res->dy0 = a_bottom - b_top;
        res->dy1 = b_bottom - a_top;
        overlap_y = 1;
    }

    // Check if B is inside of A.
    if (b_top >= a_top && b_top <= a_bottom &&
        b_bottom >= a_top && b_bottom <= a_bottom)
    {
        overlap_y = 1;
    }

    // If the boundaries of the two entities overlap in both directions, then
    // a there is a collision.
    if (overlap_x && overlap_y)
    {
        return 1;
    }

    return 0;
}

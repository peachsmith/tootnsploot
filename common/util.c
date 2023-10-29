#include "common/util.h"
#include "common/collision.h"
#include "entities/entity_types.h"

void util_draw_collision(
    cr_app *app,
    cr_rect *r,
    cr_collision *res,
    cr_point *p,
    cr_point *d)
{
    cr_rect target_bounds;
    cr_rect cp_rect;
    cr_point p0;
    cr_point p1;

    // Draw the target rectangle in red.
    // The dimensions should be the width and height of the target entity
    // plus the width and height of the source entity.
    target_bounds.x = r->x;
    target_bounds.y = r->y;
    target_bounds.w = r->w;
    target_bounds.h = r->h;
    cr_set_color(app, CR_COLOR_VINIK_RED);
    cr_draw_rect(app, &target_bounds, 0);

    // Draw the contact point CP in cyan.
    cr_set_color(app, CR_COLOR_VINIK_CYAN);
    cp_rect.x = res->cp.x - 5;
    cp_rect.y = res->cp.y - 5;
    cp_rect.w = 10;
    cp_rect.h = 10;
    cr_draw_rect(app, &cp_rect, 1);

    // Draw the contact normal CN in a pink.
    cr_set_color(app, CR_COLOR_VINIK_PINK);
    p0.x = res->cp.x;
    p0.y = res->cp.y;
    p1.x = res->cp.x + res->cn.x * 20;
    p1.y = res->cp.y + res->cn.y * 20;
    cr_draw_line(app, &p0, &p1);

    // Draw the direction vector D in green.
    cr_set_color(app, CR_COLOR_VINIK_SEA_GREEN);
    p0.x = p->x;
    p0.y = p->y;
    p1.x = p->x + d->x * 10;
    p1.y = p->y + d->y * 10;
    cr_draw_line(app, &p0, &p1);
}

void util_draw_sat(
    cr_app *app,
    cr_point *pa,
    cr_point *pb,
    cr_point *n0,
    cr_point *n1,
    cr_point *q)
{

    cr_rect rects[7];

    // All of the vertex rectangles will have a width of 4.
    for (int i = 0; i < 7; i++)
    {
        rects[i].w = 4;
        rects[i].h = 4;
    }

    // Create rectangles at each vertex of entity A.
    rects[0].x = pa[0].x - 2;
    rects[0].y = pa[0].y - 2;
    rects[1].x = pa[1].x - 2;
    rects[1].y = pa[1].y - 2;
    rects[2].x = pa[2].x - 2;
    rects[2].y = pa[2].y - 2;
    rects[3].x = pa[3].x - 2;
    rects[3].y = pa[3].y - 2;

    // Create rectangles at each vertex of entity B.
    rects[4].x = pb[0].x - 2;
    rects[4].y = pb[0].y - 2;
    rects[5].x = pb[1].x - 2;
    rects[5].y = pb[1].y - 2;
    rects[6].x = pb[2].x - 2;
    rects[6].y = pb[2].y - 2;

    // Draw the vertices of entity A.
    cr_set_color(app, CR_COLOR_VINIK_LAVENDAR);
    for (int i = 0; i < 4; i++)
    {
        cr_draw_rect(app, &(rects[i]), 1);
    }

    // Draw the vertices of entity B.
    cr_set_color(app, CR_COLOR_VINIK_YELLOW);
    for (int i = 4; i < 7; i++)
    {
        cr_draw_rect(app, &(rects[i]), 1);
    }

    // Draw the normal axis.
    cr_set_color(app, CR_COLOR_VINIK_MILITARY_GREEN);
    cr_draw_line(app, n0, n1);

    // Draw the normal origin Q.
    cr_set_color(app, CR_COLOR_VINIK_LIGHT_GRAY);
    cr_rect rq = {.x = q->x - 2, .y = q->y - 2, .w = 4, .h = 4};
    cr_draw_rect(app, &rq, 1);
}

void util_draw_camera(cr_app *app)
{
    cr_point l0, l1, r0, r1, t0, t1, b0, b1;

    cr_set_color(app, CR_COLOR_VINIK_MAUVE);

    // left boundary
    l0.x = app->cam.cl;
    l0.y = 0;
    l1.x = app->cam.cl;
    l1.y = app->screen_height;
    cr_draw_line(app, &l0, &l1);

    // right boundary
    r0.x = app->cam.cr;
    r0.y = 0;
    r1.x = app->cam.cr;
    r1.y = app->screen_height;
    cr_draw_line(app, &r0, &r1);

    cr_set_color(app, CR_COLOR_VINIK_TEAL);

    // top boundary
    t0.x = 0;
    t0.y = app->cam.ct;
    t1.x = app->screen_width;
    t1.y = app->cam.ct;
    cr_draw_line(app, &t0, &t1);

    // bottom boundary
    b0.x = 0;
    b0.y = app->cam.cb;
    b1.x = app->screen_width;
    b1.y = app->cam.cb;
    cr_draw_line(app, &b0, &b1);
}

void util_set_camera(cr_app *app, int config)
{
    app->cam.config = config;

    if (config == CR_CAMERA_ALL)
    {
        app->cam.cl = 50;
        app->cam.cr = 180;
        app->cam.ct = 20;
        app->cam.cb = 140;
        return;
    }

    if (config == CR_CAMERA_ALL)
    {
        app->cam.cl = 0;
        app->cam.cr = 0;
        app->cam.ct = 0;
        app->cam.cb = 0;
        return;
    }
}

int util_is_on_screen(cr_app *app, cr_entity *entity)
{
    cr_overlap o;
    int cam_x = 0, cam_y = 0;
    if (!app->entity_types[entity->type].control)
    {
        cam_x = app->cam.x;
        cam_y = app->cam.y;
    }

    cr_rect screen_rect = {
        .x = 0,
        .y = 0,
        .w = app->screen_width,
        .h = app->screen_height};

    cr_rect entity_rect = {
        .x = entity->x_pos + cam_x,
        .y = entity->y_pos + cam_y,
        .w = app->entity_types[entity->type].width,
        .h = app->entity_types[entity->type].height};

    return common_is_overlapped(&screen_rect, &entity_rect, &o);
}

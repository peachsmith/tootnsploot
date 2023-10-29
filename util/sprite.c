#include "tootnsploot.h"
#include "assets.h"
#include "util/sprite.h"

void sprite_draw_corgi(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 0;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_note(cr_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = 4;
    int tile_y = 0;

    // The note sprite is 11 x 11 pixels, so we subtract 13
    // from the tile width and height.
    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w - 13,
        .h = tile_h - 13};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w - 13,
        .h = tile_h - 13};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_critter(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 20;
    int tile_h = 12;

    cr_rect src = {
        .x = 96 + tile * tile_w,
        .y = 11,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_sparkle(cr_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 19;
    int tile_h = 9;

    cr_rect src = {
        .x = 116 + tile * tile_w,
        .y = 0,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_puff(cr_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 16;
    int tile_h = 8;

    cr_rect src = {
        .x = 158 + tile * tile_w,
        .y = 10,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_leaves(cr_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 20;
    int tile_h = 12;

    cr_rect src = {
        .x = 96 + tile * tile_w,
        .y = 24,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_trumpet(cr_app *app, int x, int y, int mirror)
{
    cr_rect src = {
        .x = 137,
        .y = 25,
        .w = 14,
        .h = 8};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = 14,
        .h = 8};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_ground(cr_app *app, int x, int y)
{
    cr_rect src = {
        .x = 0,
        .y = 24,
        .w = 24,
        .h = 18};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = 24,
        .h = 18};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_forest(cr_app * app, int x, int y)
{
    cr_rect src = {
        .x = 0,
        .y = 44,
        .w = 240,
        .h = 120};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = 240,
        .h = 120};

    cr_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

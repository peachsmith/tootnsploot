#ifndef TNS_SPRITE_H
#define TNS_SPRITE_H

#include "crumbs.h"

/**
 * Renders the corgi character.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_corgi(cr_app *app, int x, int y, int mirror, int tile);

/**
 * Renders a musical note.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 */
void sprite_draw_note(cr_app *app, int x, int y);

/**
 * Renders a critter.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_critter(cr_app *app, int x, int y, int mirror, int tile);

/**
 * Renders a sparkling effect.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_sparkle(cr_app *app, int x, int y, int tile);

/**
 * Renders a puff of smoke.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_puff(cr_app *app, int x, int y, int tile);

/**
 * Renders some leaves rustling in the trees.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_leaves(cr_app *app, int x, int y, int tile);

/**
 * Renders a tiny trumpet.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 */
void sprite_draw_trumpet(cr_app *app, int x, int y, int mirror);

/**
 * Renders a piece of the forest floor.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 */
void sprite_draw_ground(cr_app *app, int x, int y);

/**
 * Renders the forest background.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 */
void sprite_draw_forest(cr_app *app, int x, int y);

#endif
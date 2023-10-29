#ifndef TNS_UI_H
#define TNS_UI_H

#include "crumbs.h"

#define UI_INDICATOR_ADVANCE 1
#define UI_INDICATOR_SCROLL_UP 2
#define UI_INDICATOR_SCROLL_DOWN 3
#define UI_INDICATOR_SCROLL_LEFT 4
#define UI_INDICATOR_SCROLL_RIGHT 5

/**
 * Draws a panel.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
void ui_draw_panel(cr_app *, int, int, int, int);

/**
 * Draws text.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
void ui_draw_text(cr_app *);

/**
 * Draws a cursor. This indicates the position of control, such as which
 * menu item will be selected.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
void ui_draw_cursor(cr_app *, int, int);

/**
 * Draws an indicator. An indicator signifies that more content will be
 * accessible through user input. For example, a list of items may have
 * an arrow at the bottom indicating that the user can scroll down to see
 * more items.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - the type of indicator to render
 */
void ui_draw_indicator(cr_app *, int, int, int);

#endif
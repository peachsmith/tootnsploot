#ifndef COMMON_DIALOG_H
#define COMMON_DIALOG_H

#include "crumbs.h"

// #define COMMON_MAX_DIALOGS 10
#define COMMON_DIALOG_BUFSIZE 256

// common dialog text rendering speed
#define COMMON_DIALOG_SPEED_SCALE 4

// common dimensions of a dialog panel
#define COMMON_DIALOG_WIDTH 224
#define COMMON_DIALOG_HEIGHT 48

void common_dialog_renderer(
    cr_app *,
    cr_entity *,
    cr_font *,
    cr_entity_func,
    cr_entity_func);

#endif
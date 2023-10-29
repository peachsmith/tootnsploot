#ifndef TNS_H
#define TNS_H

#include "crumbs.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// image assets
#define TNS_TEXTURE_UI 0
#define TNS_TEXTURE_SPRITES 1
#define TNS_TEXTURE_TITLE 2

// font assets
#define TNS_FONT_KENNY_PIXEL 0
#define TNS_FONT_POKEMON_FIRE_RED 1
#define TNS_FONT_PRESS_START 2

// audio assets
#define TNS_SOUND_EFFECT_CONFIRMATION 0

// counters
// counters [3:10] represent available critter slots
#define TNS_COUNTER_BREATH 0
#define TNS_COUNTER_CRITTERS 1
#define TNS_COUNTER_SCORE 2
#define TNS_COUNTER_CRITTER_SLOT_OFFSET 3
#define TNS_COUNTER_MAX 20

// entity handles
#define TNS_HANDLE_CORGI 0
#define TNS_HANDLE_TRANSITION 1
#define TNS_HANDLE_CONTROLS 2
#define TNS_HANDLE_CHARACTERS 3
#define TNS_MAX_ENTITY_HANDLES 10

// limits
#define TNS_MAX_ENTITIES 256
#define TNS_MAX_INPUT_HANDLERS 20
#define TNS_MAX_MENUS 10
#define TNS_MAX_DIALOGS 10
#define TNS_MAX_OVERLAYS 10
#define TNS_MAX_TEXTURES 10
#define TNS_MAX_FONTS 10
#define TNS_MAX_SOUNDS 10

// extension structure definition
struct cr_extension
{
    cr_entity **entity_handles;
    int *counters;
};

/**
 * Populates am app struct with the data needed for runtime.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int tns_init_app(cr_app *);

#endif
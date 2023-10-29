#ifndef TNS_HUD_H
#define TNS_HUD_H

// A heads-up display (HUD)

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the hud entity.
 *
 * Params:
 *   cr_entity_type* - the hud's entity type definition
 */
void tns_register_hud(cr_entity_type *);

/**
 * Creates a hud entity.
 *
 * Params:
 *   int - the hud entity's initial x position
 *   int - the hud entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new hud entity.
 */
cr_entity *tns_create_hud(cr_app *, int, int);

#endif
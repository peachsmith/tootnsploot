#ifndef TNS_FLOOR_H
#define TNS_FLOOR_H

// The floor is the ground on which everything stands.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the floor entity.
 *
 * Params:
 *   cr_entity_type* - the floor's entity type definition
 */
void tns_register_floor(cr_entity_type *);

/**
 * Creates a floor entity.
 *
 * Params:
 *   int - the floor entity's initial x position
 *   int - the floor entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new floor entity.
 */
cr_entity *tns_create_floor(cr_app *, int, int);

#endif
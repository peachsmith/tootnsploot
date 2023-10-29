#ifndef TNS_WALL_H
#define TNS_WALL_H

// A wall functions as a barrier restricting certain entities movement.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the wall entity.
 *
 * Params:
 *   cr_entity_type* - the wall's entity type definition
 */
void tns_register_wall(cr_entity_type *);

/**
 * Creates a wall entity.
 *
 * Params:
 *   int - the wall entity's initial x position
 *   int - the wall entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new wall entity.
 */
cr_entity *tns_create_wall(cr_app *, int, int);

#endif
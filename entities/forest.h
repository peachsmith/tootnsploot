#ifndef TNS_FOREST_H
#define TNS_FOREST_H

// The background scenery for the forest scene.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the forest entity.
 *
 * Params:
 *   cr_entity_type* - the forest's entity type definition
 */
void tns_register_forest(cr_entity_type *);

/**
 * Creates a forest entity.
 *
 * Params:
 *   int - the forest entity's initial x position
 *   int - the forest entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new forest entity.
 */
cr_entity *tns_create_forest(cr_app *, int, int);

#endif
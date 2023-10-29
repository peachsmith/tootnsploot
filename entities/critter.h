#ifndef TNS_CRITTER_H
#define TNS_CRITTER_H

// A critter, in this context, is an animal that may fall from the trees
// due to various forms of emotional distress.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the critter entity.
 *
 * Params:
 *   cr_entity_type* - the critter's entity type definition
 */
void tns_register_critter(cr_entity_type *);

/**
 * Creates a critter entity.
 *
 * Params:
 *   int - the critter entity's initial x position
 *   int - the critter entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new critter entity.
 */
cr_entity *tns_create_critter(cr_app *, int, int);

#endif
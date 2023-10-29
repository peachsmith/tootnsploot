#ifndef TNS_CORGI_H
#define TNS_CORGI_H

// The corgi represents the player avatar. His name is Skippy.
// His job is to restore the woodland critters' will to live with the power
// of music.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the corgi entity.
 *
 * Params:
 *   cr_entity_type* - the corgi's entity type definition
 */
void tns_register_corgi(cr_entity_type *);

/**
 * Creates a corgi entity.
 *
 * Params:
 *   int - the corgi entity's initial x position
 *   int - the corgi entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new corgi entity.
 */
cr_entity *tns_create_corgi(cr_app *, int, int);

#endif
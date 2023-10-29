#ifndef TNS_NOTE_H
#define TNS_NOTE_H

// A note represents the sound produced by a musical instrument.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the note entity.
 *
 * Params:
 *   cr_entity_type* - the note's entity type definition
 */
void tns_register_note(cr_entity_type *);

/**
 * Creates a note entity.
 *
 * Params:
 *   int - the note entity's initial x position
 *   int - the note entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new note entity.
 */
cr_entity *tns_create_note(cr_app *, int, int);

#endif
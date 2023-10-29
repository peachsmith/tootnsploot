#ifndef TNS_CHARACTERS_MENU_H
#define TNS_CHARACTERS_MENU_H

// The characters menu for Toot n Sploot.
// This menu contains short biographies for the characters.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the characters menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the characters menu entity type definition
 */
void tns_register_characters_menu(cr_entity_type *);

/**
 * Creates a characters menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new characters menu entity.
 */
cr_entity *tns_create_characters_menu(cr_app *);

#endif
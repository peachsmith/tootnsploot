#ifndef TNS_MAIN_MENU_H
#define TNS_MAIN_MENU_H

// The main menu for Toot n Sploot.
// This represents the content of the title screen.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the main menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the main menu entity type definition
 */
void tns_register_main_menu(cr_entity_type *);

/**
 * Creates a main menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new main menu entity.
 */
cr_entity *tns_create_main_menu(cr_app *);

#endif
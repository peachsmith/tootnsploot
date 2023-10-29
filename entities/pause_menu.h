#ifndef TNS_PAUSE_MENU_H
#define TNS_PAUSE_MENU_H

// The pause menu halts normal gameplay and allows the user to exit the
// application or return to the main menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the pause menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the pause menu entity type definition
 */
void tns_register_pause_menu(cr_entity_type *);

/**
 * Creates a pause menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new pause menu entity.
 */
cr_entity *tns_create_pause_menu(cr_app *);

#endif
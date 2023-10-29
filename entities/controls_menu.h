#ifndef TNS_CONTROLS_MENU_H
#define TNS_CONTROLS_MENU_H

// The controls menu for Toot n Sploot.
// This menu explains how to play the game.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the controls menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the controls menu entity type definition
 */
void tns_register_controls_menu(cr_entity_type *);

/**
 * Creates a controls menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new controls menu entity.
 */
cr_entity *tns_create_controls_menu(cr_app *);

#endif
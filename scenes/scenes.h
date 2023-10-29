#ifndef TNS_SCENES_H
#define TNS_SCENES_H

#include "crumbs.h"

// scenes
#define TNS_SCENE_TITLE_SCREEN 0
#define TNS_SCENE_FOREST 1

void tns_clear_scene(cr_app *);

// scene loaders
void tns_load_title_screen(cr_app *);
void tns_load_forest_scene(cr_app *);

// scene behavior
void tns_perform_forest(cr_app*);

#endif
#include "tootnsploot.h"
#include "entities/entity_types.h"
#include "entities/forest.h"
#include "util/sprite.h"

static void render_forest(cr_app *app, cr_entity *forest)
{
    sprite_draw_forest(app, forest->x_pos, forest->y_pos);
}

void tns_register_forest(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_FOREST;
    t->width = 240;
    t->height = 120;
    t->render = render_forest;
    t->collide = NULL;
}

cr_entity *tns_create_forest(cr_app *app, int x, int y)
{
    cr_entity *forest = NULL;

    forest = cr_create_entity(app);
    if (forest == NULL)
    {
        return NULL;
    }

    forest->type = ENTITY_TYPE_FOREST;
    forest->x_pos = x;
    forest->y_pos = y;

    return forest;
}

#include "crumbs.h"
#include "tootnsploot.h"

int main(int argc, char **argv)
{
    if (!cr_initialize())
    {
        return 1;
    }

    cr_app *app = cr_create_app();
    if (app == NULL)
    {
        cr_terminate();
        return 1;
    }

    if (!tns_init_app(app))
    {
        cr_destroy_app(app);
        cr_terminate();
        return 1;
    }

    while (!app->done)
    {
        cr_begin_frame(app);
        app->update(app);
        app->draw(app);
        cr_end_frame(app);
    }

    cr_destroy_app(app);
    cr_terminate();

    return 0;
}

#include "common/util.h"
#include "common/dialog.h"

#include <stdlib.h>

void common_dialog_renderer(
    cr_app *app,
    cr_entity *dialog,
    cr_font *font,
    cr_entity_func draw_panel,
    cr_entity_func draw_indicator)
{
    char buffer[COMMON_DIALOG_BUFSIZE];
    int buf_len;
    int i;

    // Render the dialog panel.
    if (draw_panel != NULL)
    {
        draw_panel(app, dialog);
    }

    // Copy the dialog text into the buffer.
    buf_len = dialog->ticks / COMMON_DIALOG_SPEED_SCALE;
    for (i = 0; i < buf_len; i++)
    {
        if (i < dialog->text_len && i < COMMON_DIALOG_BUFSIZE - 1)
        {
            buffer[i] = dialog->text[i];
        }
    }

    // Terminate the buffer with a NUL character.
    buffer[i] = '\0';

    // Render the dialog text.
    cr_rect bounds = {
        .x = dialog->x_pos + 5,
        .y = dialog->y_pos + 5,
        .w = 210,
        .h = 0};
    cr_draw_text_bounded(
        app,
        font,
        buffer,
        &bounds,
        NULL);

    // Render the panel advance indicator.
    if (buf_len == dialog->text_len && draw_indicator != NULL)
    {
        draw_indicator(app, dialog);
    }
}

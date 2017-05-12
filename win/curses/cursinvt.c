/* vim:set cin ft=c sw=4 sts=4 ts=8 et ai cino=Ls\:0t0(0 : -*- mode:c;fill-column:80;tab-width:8;c-basic-offset:4;indent-tabs-mode:nil;c-file-style:"k&r" -*-*/

#include "curses.h"
#include "hack.h"
#include "wincurs.h"
#include "cursinvt.h"

/* Permanent inventory for curses interface */

/* Runs when the game indicates that the inventory has been updated */
void
curses_update_inv(void)
{
    WINDOW *win = curses_get_nhwin(INV_WIN);

    /* Check if the inventory window is enabled in first place */
    if (!win)
        return;

    boolean border = curses_window_has_border(INV_WIN);

    /* Figure out drawing area */
    int x = 0;
    int y = 0;
    if (border) {
        x++;
        y++;
    }

    /* Clear the window as it is at the moment. */
    werase(win);

    display_inventory(NULL, FALSE);

    if (border)
        box(win, 0, 0);

    wrefresh(win);
}

/* Adds an inventory item. */
void
curses_add_inv(int y, int glyph, CHAR_P accelerator, attr_t attr,
               const char *str, const ANY_P *identifier)
{
    WINDOW *win = curses_get_nhwin(INV_WIN);

    /* Figure out where to draw the line */
    int x = 0;
    y--;
    if (curses_window_has_border(INV_WIN)) {
        x++;
        y++;
    }

    wmove(win, y, x);
    if (accelerator) {
        attr_t bold = A_BOLD;
        wattron(win, bold);
        waddch(win, accelerator);
        wattroff(win, bold);
        wprintw(win, ") ");
    }

    if (accelerator && glyph != NO_GLYPH && iflags.use_menu_glyphs) {
        unsigned dummy = 0; /* Not used */
        int color = 0;
        int curletter = 0;
        mapglyph_obj(glyph, &curletter, &color, &dummy,
                     u.ux, u.uy, identifier->a_obj);
        attr = curses_color_attr(color, 0);
        wattron(win, attr);
        wprintw(win, "%c ", curletter);
        wattroff(win, attr);
    }

#ifdef MENU_COLOR
    if (accelerator && /* Don't colorize categories */
        iflags.use_menu_color) {
        int color = NO_COLOR;
        boolean menu_color = FALSE;
        char str_mutable[BUFSZ];
        Strcpy(str_mutable, str);
        attr = 0;
        get_menu_coloring(str_mutable, &color, &attr);
        if (color != NO_COLOR)
            attr |= curses_color_attr(color, 0);
    }
#endif
    
    wattron(win, attr);
    wprintw(win, "%s", str);
    wattroff(win, attr);
    wclrtoeol(win);
}

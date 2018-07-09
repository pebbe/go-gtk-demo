#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include "_cgo_export.h"
#include "cairo.h"

typedef struct Klok_
{
    int
        hour,
        minute,
        second;
} Klok;

GtkImage
    *image = NULL;
cairo_surface_t
    *surface = NULL;
cairo_t
    *cr = NULL;

void run()
{
    static char
        buf[1000];
    GtkBuilder
        *builder;
    GError
        *error = NULL;
    GtkWidget
        *window,
        *canvas,
        *container;

    gtk_init(NULL, NULL);

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 440, 440);
    cr = cairo_create(surface);
    cairo_translate(cr, 220, 220);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "cairo.ui", &error))
    {
        g_snprintf(buf, 999, "%s", error->message);
        go_message(idERROR, buf);
        return;
    }
    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    image = GTK_IMAGE(gtk_builder_get_object(builder, "my-image"));

    go_message(idREADY, "Let's begin!");
    gtk_widget_show_all(window);
    gtk_main();
}

G_MODULE_EXPORT gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    go_message(idDELETE, "Delete!");

    return FALSE;
}

G_MODULE_EXPORT void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();

    go_message(idDESTROY, "Destroy!");
}

gboolean update_image_do(gpointer data)
{
    Klok
        *klok;
    GdkPixbuf
        *pixbuf;
    float
        r,
        x, y, l, hr, mn, sc;
    int
        i;

    if (!image)
    {
        free(data);
        return FALSE; /* don't repeat */
    }

    cairo_set_line_width(cr, 1);
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_arc(cr, 0, 0, 204, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_set_source_rgba(cr, .5, .5, .5, 1);
    cairo_arc(cr, 0, 0, 200, 0, 2 * M_PI);
    for (i = 0; i < 60; i++)
    {
        r = (float)i / 30.0 * M_PI;
        x = sin(r);
        y = cos(r);
        l = i % 5 ? 196.0 : 185.0;
        cairo_move_to(cr, x * 200, y * 200);
        cairo_line_to(cr, x * l, y * l);
    }
    cairo_stroke(cr);

    klok = (Klok *)data;

    sc = (float)klok->second / 60.0;
    mn = ((float)klok->minute + sc) / 60.0;
    hr = ((float)klok->hour + mn) / 12.0;

    sc *= 2 * M_PI;
    mn *= 2 * M_PI;
    hr *= 2 * M_PI;

    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_set_line_width(cr, 10);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 140 * sin(hr), -140 * cos(hr));
    cairo_stroke(cr);

    cairo_set_line_width(cr, 4);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 180 * sin(mn), -180 * cos(mn));
    cairo_stroke(cr);

    cairo_set_line_width(cr, 2);
    cairo_set_source_rgba(cr, .7, 0, 0, 1);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 200 * sin(sc), -200 * cos(sc));
    cairo_stroke(cr);

    cairo_arc(cr, 0, 0, 14, 0, 2 * M_PI);
    cairo_fill(cr);

    pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 440, 440);
    gtk_image_set_from_pixbuf(image, pixbuf);
    g_object_unref(pixbuf);

    free(data);
    return FALSE; /* don't repeat */
}

void update_image(int hour, int minute, int second)
{
    Klok
        *klok = (Klok *)malloc(sizeof(Klok));

    klok->hour = hour;
    klok->minute = minute;
    klok->second = second;
    gdk_threads_add_idle(update_image_do, (gpointer)klok);
}

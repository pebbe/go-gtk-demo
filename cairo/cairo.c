#include "_cgo_export.h"
#include "cairo_my.h"
#include <gtk/gtk.h>
#include <math.h>

typedef struct Klok_ {
    int hour, minute, second;
} Klok;

GtkWindow *main_window;
cairo_t *cr = NULL;
static cairo_surface_t *surface = NULL;

#if GTK_MAJOR_VERSION == 3
#define UI "cairo.ui3"
GtkImage *image = NULL;
#endif
#if GTK_MAJOR_VERSION == 4
#define UI "cairo.ui4"
GtkDrawingArea *image = NULL;

void draw_image(GtkDrawingArea *da, cairo_t *cr, int x, int y, void *data) {
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
}
#endif

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 440, 440);
    cr = cairo_create(surface);
    cairo_translate(cr, 220, 220);

    builder = gtk_builder_new_from_file(UI);
#if GTK_MAJOR_VERSION == 3
    gtk_builder_connect_signals(builder, NULL);
    image = GTK_IMAGE(gtk_builder_get_object(builder, "my-image"));
#endif
#if GTK_MAJOR_VERSION == 4
    image = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "my-image"));
    gtk_drawing_area_set_draw_func(image, draw_image, NULL, NULL);
#endif

    /* MORE */

    main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    gtk_window_present(GTK_WINDOW(main_window));
    gtk_application_add_window(app, main_window);
    go_message(idREADY, "Let's begin!");
}

int run() {
    GtkApplication *app;
    int status;

    app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return status;
}

G_MODULE_EXPORT gboolean close_request(GtkWidget *widget, GdkEvent *event,
                                       gpointer data) {
    go_message(idCLOSE, "Close!");

    // Return value
    // Type: gboolean
    // True to stop other handlers from being invoked for the signal.
    return FALSE;
}

#if GTK_MAJOR_VERSION == 3
G_MODULE_EXPORT void destroy(GtkWidget *widget, gpointer data) {
    go_message(idDESTROY, "Destroy!");
}
#endif

gboolean update_image_do(gpointer data) {
    Klok *klok;
    float r, x, y, l, hr, mn, sc;
    int i;

    if (!data)
        return FALSE; /* don't repeat */

    if (!image) {
        free(data);
        return FALSE; /* don't repeat */
    }

    klok = (Klok *)data;
    sc = (float)klok->second / 60.0;
    mn = ((float)klok->minute + sc) / 60.0;
    hr = ((float)klok->hour + mn) / 12.0;
    free(data);

    cairo_set_line_width(cr, 1);
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_arc(cr, 0, 0, 204, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_set_source_rgba(cr, .5, .5, .5, 1);
    cairo_arc(cr, 0, 0, 200, 0, 2 * M_PI);
    for (i = 0; i < 60; i++) {
        r = (float)i / 30.0 * M_PI;
        x = sin(r);
        y = cos(r);
        l = i % 5 ? 196.0 : 185.0;
        cairo_move_to(cr, x * 200, y * 200);
        cairo_line_to(cr, x * l, y * l);
    }
    cairo_stroke(cr);

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

#if GTK_MAJOR_VERSION == 3
    GdkPixbuf *pixbuf;
    pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 440, 440);
    gtk_image_set_from_pixbuf(image, pixbuf);
    g_object_unref(pixbuf);
#endif
#if GTK_MAJOR_VERSION == 4
    gtk_widget_queue_draw(GTK_WIDGET(image));
#endif

    return FALSE; /* don't repeat */
}

void update_image(int hour, int minute, int second) {
    Klok *klok = (Klok *)malloc(sizeof(Klok));

    klok->hour = hour;
    klok->minute = minute;
    klok->second = second;
    g_idle_add(update_image_do, (gpointer)klok);
}

gboolean quit_do(gpointer nul) {
    gtk_window_close(main_window);

    return FALSE;
}

void quit() {
    // request to quit
    g_idle_add(quit_do, NULL);
}

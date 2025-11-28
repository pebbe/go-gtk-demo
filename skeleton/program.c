#include "_cgo_export.h"
#include "program_my.h"
#include <gtk/gtk.h>

GtkWidget *main_window;

void run() {
    static char buf[1000];
    GtkBuilder *builder;
    GError *error = NULL;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "program.ui", &error)) {
        g_snprintf(buf, 999, "%s", error->message);
        go_message(idERROR, buf);
        return;
    }
    gtk_builder_connect_signals(builder, NULL);

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));

    /* MORE */

    go_message(idREADY, "Let's begin!");
    gtk_widget_show_all(main_window);
    gtk_main();
}

G_MODULE_EXPORT gboolean delete_event(GtkWidget *widget, GdkEvent *event,
                                      gpointer data) {
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    go_message(idDELETE, "Delete!");

    return FALSE;
}

G_MODULE_EXPORT void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();

    go_message(idDESTROY, "Destroy!");
}

gboolean quit_do(gpointer nul) {
    gtk_window_close(GTK_WINDOW(main_window));
    return FALSE;
}

void quit() {
    // request to quit
    g_idle_add(quit_do, NULL);
}

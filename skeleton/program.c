#include "_cgo_export.h"
#include "program_my.h"
#include <gtk/gtk.h>
#include <stdio.h>

#if GTK_MAJOR_VERSION == 3
#define UI "program.ui3"
#endif
#if GTK_MAJOR_VERSION == 4
#define UI "program.ui4"
#endif

GtkWindow *main_window;

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file(UI);
#if GTK_MAJOR_VERSION == 3
    gtk_builder_connect_signals(builder, NULL);
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

#if GTK_MAJOR_VERSION == 3
G_MODULE_EXPORT void destroy(GtkWidget *widget, gpointer data) {
    go_message(idDESTROY, "Destroy!");
}
#endif

G_MODULE_EXPORT gboolean close_request(GtkWidget *widget, GdkEvent *event,
                                       gpointer data) {
    go_message(idCLOSE, "Close!");

    // Return value
    // Type: gboolean
    // True to stop other handlers from being invoked for the signal.
    return FALSE;
}

gboolean quit_do(gpointer nul) {
    gtk_window_close(main_window);

    return FALSE;
}

void quit() {
    // request to quit
    g_idle_add(quit_do, NULL);
}

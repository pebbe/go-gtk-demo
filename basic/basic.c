#include "_cgo_export.h"
#include "basic_my.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#if GTK_MAJOR_VERSION == 3
#define UI "basic.ui3"
#endif
#if GTK_MAJOR_VERSION == 4
#define UI "basic.ui4"
#endif

GtkWindow *main_window;
GtkBuilder *builder;
GtkLabel *label = NULL;
GtkTextBuffer *textbuffer = NULL;

static void activate(GtkApplication *app, gpointer user_data) {
    builder = gtk_builder_new_from_file(UI);
#if GTK_MAJOR_VERSION == 3
    gtk_builder_connect_signals(builder, NULL);
#endif

    label = GTK_LABEL(gtk_builder_get_object(builder, "my-label"));
    textbuffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(gtk_builder_get_object(builder, "my-text")));
    gtk_text_buffer_set_text(textbuffer, "Hello, this is some text", -1);

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

G_MODULE_EXPORT void hello(GtkWidget *widget, gpointer data) {
    go_message(idBUTTON, "Hello World");
}

gboolean update_label_do(gpointer text) {
    if (label)
        gtk_label_set_label(label, (char const *)text);

    free(text);
    return FALSE; /* don't repeat */
}

void update_label(void const *data) {
    char *text = strdup((char const *)data);

    g_idle_add(update_label_do, (gpointer)text);
}

gboolean get_text_do(gpointer nul) {
    static char buf[10000];
    GtkTextIter start, end;

    if (!textbuffer)
        return FALSE; /* don't repeat */

    gtk_text_buffer_get_iter_at_offset(textbuffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset(textbuffer, &end, -1);
    g_snprintf(buf, 9999, "Current text: %s",
               gtk_text_buffer_get_text(textbuffer, &start, &end, TRUE));
    go_message(idTEXT, buf);

    return FALSE; /* don't repeat */
}

void get_text() { g_idle_add(get_text_do, NULL); }

gboolean quit_do(gpointer nul) {
    gtk_window_close(main_window);

    return FALSE;
}

void quit() {
    // request to quit
    g_idle_add(quit_do, NULL);
}

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "_cgo_export.h"
#include "my_basic.h"

GtkLabel *label = NULL;
GtkTextBuffer *textbuffer = NULL;

void run()
{
    static char buf[1000];
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *window;

    gtk_init (NULL, NULL);

    builder = gtk_builder_new ();
    if (!gtk_builder_add_from_file (builder, "basic.ui", &error)) {
	g_snprintf (buf, 999, "%s", error->message);
	go_message (idERROR, buf);
	return;
    }
    gtk_builder_connect_signals (builder, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "main-window"));
    label = GTK_LABEL (gtk_builder_get_object (builder, "my-label"));
    textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (gtk_builder_get_object (builder, "my-text")));
    gtk_text_buffer_set_text (textbuffer, "Hello, this is some text", -1);

    gtk_widget_show_all (window);
    gtk_main ();
}

G_MODULE_EXPORT gboolean delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    go_message (idDELETE, "Delete!");

    return FALSE;
}

G_MODULE_EXPORT void destroy (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();

    go_message (idDESTROY, "Destroy!");
}

G_MODULE_EXPORT void hello (GtkWidget *widget, gpointer data)
{
    go_message (idBUTTON, "Hello World");
}

gboolean update_label_do (gpointer text)
{
    gchar lbl[65];

    if (!label)
	return FALSE; /* don't repeat */

    g_snprintf (lbl, 64, "%s", (char const *)text);
    gtk_label_set_label (label, lbl);
    free (text);

    return FALSE; /* don't repeat */
}

void update_label(char *text)
{
    gdk_threads_add_idle(update_label_do, (gpointer)text);
}

gboolean get_text_do (gpointer nul)
{
    static char buf[10000];
    GtkTextIter start, end;

    if (!textbuffer)
	return FALSE; /* don't repeat */

    gtk_text_buffer_get_iter_at_offset (textbuffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, -1);
    g_snprintf (buf, 9999, "Current text: %s",
		gtk_text_buffer_get_text (textbuffer, &start, &end, TRUE));
    go_message (idTEXT, buf);

    return FALSE; /* don't repeat */
}

void get_text()
{
    gdk_threads_add_idle(get_text_do, NULL);
}

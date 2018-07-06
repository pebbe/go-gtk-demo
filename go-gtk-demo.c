#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <stdlib.h>
#include <string.h>
#include "_cgo_export.h"
#include "go-gtk-demo.h"

GtkLabel *label = NULL;
GtkTextBuffer *textbuffer = NULL;

char **return_argv;

char *return_arg(int i)
{
    return return_argv[i];
}

int init(int argc, void *argv)
{
    int ac, i;
    GoString *args;

    args = (GoString *) argv;

    ac = argc;
    return_argv = (char **) malloc (argc * sizeof(char *));
    for (i = 0; i < argc; i++)
	return_argv[i] = strndup(args[i].p, args[i].n);

    /* This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application. */
    gtk_init (&ac, &return_argv);

    return ac;
}

void run()
{
    static char buf[1000];
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *window, *box;
    WebKitWebView *webview = NULL;

    builder = gtk_builder_new ();
    if (!gtk_builder_add_from_file (builder, "go-gtk-demo.ui", &error)) {
	g_snprintf (buf, 999, "%s", error->message);
	go_message (idERROR, buf);
	return;
    }
    gtk_builder_connect_signals (builder, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "main-window"));
    label = GTK_LABEL (gtk_builder_get_object (builder, "my-label"));
    textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (gtk_builder_get_object (builder, "my-text")));
    gtk_text_buffer_set_text (textbuffer, "Hello, this is some text", -1);

    box = GTK_WIDGET (gtk_builder_get_object (builder, "my-box"));
    webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webview), TRUE, TRUE, 0);
    webkit_web_view_load_uri(webview, "http://pkleiweg.home.xs4all.nl/");

    gtk_widget_show_all (window);
    gtk_main ();
}

G_MODULE_EXPORT void hello (GtkWidget *widget, gpointer data)
{
    go_message (idBUTTON, "Hello World");
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

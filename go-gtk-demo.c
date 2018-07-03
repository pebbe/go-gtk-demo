#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "_cgo_export.h"

static const gchar *interface =
    "<interface>"
    "  <object class=\"GtkWindow\" id=\"main-window\">"
    "    <property name=\"title\">Go/GTK+ Demo</property>"
    "    <property name=\"border-width\">5</property>"
    "    <signal name=\"destroy\" handler=\"destroy\"/>"
    "    <signal name=\"delete-event\" handler=\"delete_event\"/>"
    "    <child>"
    "      <object class=\"GtkButton\" id=\"my-button\">"
    "        <property name=\"label\">Hello World</property>"
    "        <signal name=\"clicked\" handler=\"hello\"/>"
    "      </object>"
    "    </child>"
    "  </object>"
    "</interface>";

GtkButton *button = NULL;

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
    char buffer[1000];
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *window;

    builder = gtk_builder_new ();
    if (!gtk_builder_add_from_string (builder, interface, -1, &error)) {
	g_snprintf (buffer, 999, "%s", error->message);
	go_message (0, buffer);
	return;
    }
    gtk_builder_connect_signals (builder, NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "main-window"));
    button = GTK_BUTTON (gtk_builder_get_object (builder, "my-button"));

    gtk_widget_show_all (window);
    gtk_main ();
}

G_MODULE_EXPORT void hello (GtkWidget *widget, gpointer data)
{
    go_message (1, "Hello World");
}

G_MODULE_EXPORT gboolean delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    go_message (1, "Delete!");

    return FALSE;
}

G_MODULE_EXPORT void destroy (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();

    go_message (0, "Destroy!");
}

gboolean update_button_do (gpointer text)
{
    gchar lbl[64];
    g_snprintf (lbl, 64, "%s", (char const *)text);
    if (button)
	gtk_button_set_label (button, lbl);
    free (text);

    return FALSE; /* don't repeat */
}

void update_button(char *text)
{
    gdk_threads_add_idle(update_button_do, (gpointer)text);
}

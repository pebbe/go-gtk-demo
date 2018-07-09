#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "_cgo_export.h"
#include "my_webkit.h"

void run()
{
    static char buf[1000];
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *window, *box;
    WebKitWebView *webview = NULL;
    WebKitSettings *settings = NULL;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "webkit.ui", &error))
    {
        g_snprintf(buf, 999, "%s", error->message);
        go_message(idERROR, buf);
        return;
    }
    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));

    box = GTK_WIDGET(gtk_builder_get_object(builder, "my-box"));
    settings = webkit_settings_new();
    webkit_settings_set_enable_webgl(settings, TRUE);
    webview = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(settings));
    gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webview), TRUE, TRUE, 0);
    webkit_web_view_load_uri(webview, "https://get.webgl.org/");

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

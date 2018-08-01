#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "_cgo_export.h"
#include "webkit_my.h"

static void web_view_load_changed(WebKitWebView *web_view,
                                  WebKitLoadEvent load_event,
                                  gpointer user_data)
{
    static char
        buf[1000];
    const gchar
        *provisional_uri,
        *redirected_uri,
        *uri;

    switch (load_event)
    {
    case WEBKIT_LOAD_STARTED:
        /* New load, we have now a provisional URI */
        provisional_uri = webkit_web_view_get_uri(web_view);
        printf("WEBKIT_LOAD_STARTED %s\n", provisional_uri);
        break;
    case WEBKIT_LOAD_REDIRECTED:
        redirected_uri = webkit_web_view_get_uri(web_view);
        printf("WEBKIT_LOAD_REDIRECTED %s\n", redirected_uri);
        break;
    case WEBKIT_LOAD_COMMITTED:
        /* The load is being performed. Current URI is
         * the final one and it won't change unless a new
         * load is requested or a navigation within the
         * same page is performed */
        uri = webkit_web_view_get_uri(web_view);
        printf("WEBKIT_LOAD_COMMITTED %s\n", uri);
        break;
    case WEBKIT_LOAD_FINISHED:
        /* Load finished */
        uri = webkit_web_view_get_uri(web_view);
        printf("WEBKIT_LOAD_FINISHED %s\n", uri);
        g_snprintf(buf, 999, "%s", uri);
        go_message(idLOADED, buf);
        break;
    }
}

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
    g_signal_connect(webview, "load-changed", G_CALLBACK(web_view_load_changed), NULL);
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

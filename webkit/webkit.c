#include "_cgo_export.h"
#include "webkit_my.h"
#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION == 3
#include <webkit2/webkit2.h>
#define UI "webkit.ui3"
#endif
#if GTK_MAJOR_VERSION == 4
#include <webkit/webkit.h>
#define UI "webkit.ui4"
#endif

static void web_view_load_changed(WebKitWebView *web_view,
                                  WebKitLoadEvent load_event,
                                  gpointer user_data) {
    static char buf[1000];
    const gchar *provisional_uri, *redirected_uri, *uri;

    switch (load_event) {
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

GtkWindow *main_window;

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    WebKitWebView *webview = NULL;
    WebKitSettings *settings = NULL;
    GtkWidget *box;

    builder = gtk_builder_new_from_file(UI);
#if GTK_MAJOR_VERSION == 3
    gtk_builder_connect_signals(builder, NULL);
#endif

    /* MORE */
    box = GTK_WIDGET(gtk_builder_get_object(builder, "my-box"));
    settings = webkit_settings_new();
    webkit_settings_set_enable_webgl(settings, TRUE);
#if GTK_MAJOR_VERSION == 3
    webview = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(settings));
    gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webview), TRUE, TRUE, 0);
    gtk_widget_show(GTK_WIDGET(webview));
#endif
#if GTK_MAJOR_VERSION == 4
    webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_widget_set_hexpand(GTK_WIDGET(webview), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(webview), TRUE);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(webview));
    webkit_web_view_set_settings(webview, settings);
#endif
    g_signal_connect(webview, "load-changed", G_CALLBACK(web_view_load_changed),
                     NULL);
    webkit_web_view_load_uri(webview, "https://get.webgl.org/");

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

gboolean quit_do(gpointer nul) {
    gtk_window_close(main_window);

    return FALSE;
}

void quit() {
    // request to quit
    g_idle_add(quit_do, NULL);
}

#include "_cgo_export.h"
#include "image_my.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

typedef struct Img_ {
    void *data;
    int size;
} Img;

#if GTK_MAJOR_VERSION == 3
#define UI "image.ui3"
GtkImage *image = NULL;
#endif
#if GTK_MAJOR_VERSION == 4
#define UI "image.ui4"
GtkPicture *image = NULL;
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
#if GTK_MAJOR_VERSION == 3
    image = GTK_IMAGE(gtk_builder_get_object(builder, "my-image"));
#endif
#if GTK_MAJOR_VERSION == 4
    image = GTK_PICTURE(gtk_builder_get_object(builder, "my-image"));
#endif
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
G_MODULE_EXPORT void destroy(GtkWidget *widget, gpointer data)

{
    go_message(idDESTROY, "Destroy!");
}
#endif

gboolean update_image_do(gpointer data) {
    GError *error = NULL;
    static char buf[1000];
    Img *img;

    if (!data)
        return FALSE; /* don't repeat */

    img = (Img *)data;

    if (image) {
#if GTK_MAJOR_VERSION == 3
        GInputStream *stream = NULL;
        GdkPixbuf *pixbuf = NULL;

        stream =
            g_memory_input_stream_new_from_data(img->data, img->size, NULL);
        pixbuf = gdk_pixbuf_new_from_stream(stream, NULL, &error);
        if (!pixbuf) {
            g_snprintf(buf, 999, "%s", error->message);
            go_message(idERROR, buf);
            g_error_free(error);
        } else {
            gtk_image_set_from_pixbuf(image, pixbuf);
            g_object_unref(pixbuf);
        }
#endif
#if GTK_MAJOR_VERSION == 4
        GBytes *gbytes;
        GdkTexture *texture;

        gbytes = g_bytes_new(img->data, img->size);
        texture = gdk_texture_new_from_bytes(gbytes, &error);
        if (!texture) {
            g_snprintf(buf, 999, "%s", error->message);
            go_message(idERROR, buf);
            g_error_free(error);
        } else {
            gtk_picture_set_paintable(image, GDK_PAINTABLE(texture));
            g_object_unref(texture);
        }
        g_bytes_unref(gbytes);
#endif
    }
    free(img->data);
    free(data);
    return FALSE; /* don't repeat */
}

void update_image(void *data, int size) {
    Img *img = (Img *)malloc(sizeof(Img));

    img->data = malloc(size);
    memcpy(img->data, data, size);
    img->size = size;
    g_idle_add(update_image_do, (gpointer)img);
}

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

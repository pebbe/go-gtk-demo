#include <gtk/gtk.h>
#include <stdlib.h>
#include "_cgo_export.h"
#include "my_image.h"

typedef struct Img_
{
    void *data;
    int size;
} Img;

GtkImage
    *image = NULL;

void run()
{
    static char buf[1000];
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *window;

    gtk_init (NULL, NULL);

    builder = gtk_builder_new ();
    if (!gtk_builder_add_from_file (builder, "image.ui", &error)) {
	g_snprintf (buf, 999, "%s", error->message);
	go_message (idERROR, buf);
	return;
    }
    gtk_builder_connect_signals (builder, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "main-window"));
    image = GTK_IMAGE(gtk_builder_get_object(builder, "my-image"));
 
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

gboolean update_image_do(gpointer data)
{
    GInputStream
        *stream = NULL;
    GError
        *error = NULL;
    GdkPixbuf
        *pixbuf = NULL;
    Img
        *img;

    if (!image)
    {
        free(data);
        return FALSE; /* don't repeat */
    }

    img = (Img *)data;

    stream = g_memory_input_stream_new_from_data(img->data, img->size, NULL);
    pixbuf = gdk_pixbuf_new_from_stream(stream, NULL, &error);
    gtk_image_set_from_pixbuf(image, pixbuf);
    g_object_unref(pixbuf);

    free(data);
    return FALSE; /* don't repeat */
}

void update_image(void *data, int size)
{
    Img
        *img = (Img *)malloc(sizeof(Img));

    img->data = data;
    img->size = size;
    gdk_threads_add_idle(update_image_do, (gpointer)img);
}

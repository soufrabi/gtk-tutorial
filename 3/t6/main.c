#include <gtk/gtk.h>


static void app_activate(GApplication *app, gpointer *user_data) {
    g_print("GtkApplication is activated.\n");

    GtkWidget *win;
    GtkWidget *tv;
    GtkWidget *btn1;
    GtkTextBuffer *tb;
    gchar *text;

    text = 
        "Once Upon a time."
        "There was a farmer."
        ;

    win = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(win), "Taketori");
    gtk_window_set_default_size(GTK_WINDOW(win), 200, 200);

    tv = gtk_text_view_new ();
    tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
    gtk_text_buffer_set_text(tb,text,-1);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);

    gtk_window_set_child(GTK_WINDOW(win),tv);

    gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("com.github.To", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

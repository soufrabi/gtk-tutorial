#include <gtk/gtk.h>

static void clicked1_cb(GtkButton *btn) {
    const char *s; 
    
    s = gtk_button_get_label(btn);
    if(g_strcmp0 (s,"Hello.") == 0)
        gtk_button_set_label(btn, "Good-Bye.");
    else 
        gtk_button_set_label(btn, "Hello.");

}
static void clicked2_cb(GtkButton *btn, GtkWindow *win) {
    g_print("Clicked.\n");
    gtk_window_destroy(win);
}

static void app_activate(GApplication *app, gpointer *user_data) {
    g_print("GtkApplication is activated.\n");

    GtkWidget *win;
    GtkWidget *box;
    GtkWidget *btn1;
    GtkWidget *btn2;

    win = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(win), "Application Window Title");
    gtk_window_set_default_size(GTK_WINDOW(win), 200, 200);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
    gtk_window_set_child(GTK_WINDOW(win), box);

    btn1 = gtk_button_new_with_label("Hello.");
    g_signal_connect(btn1, "clicked", G_CALLBACK(clicked1_cb), NULL);

    btn2 = gtk_button_new_with_label("Close.");
    g_signal_connect(btn2, "clicked", G_CALLBACK(clicked2_cb), win);

    gtk_box_append(GTK_BOX(box), btn1);
    gtk_box_append(GTK_BOX(box), btn2);

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

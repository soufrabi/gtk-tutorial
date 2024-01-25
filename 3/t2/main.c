#include <gtk/gtk.h>

static void app_activate(GApplication *app, gpointer *user_data){
    g_print ("GtkApplication is activated "); 

    GtkWidget *win;
    GtkWidget *lab;

    win = gtk_application_window_new (GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(win),"Application Window Title");
    gtk_window_set_default_size (GTK_WINDOW (win), 200,200);

    lab = gtk_label_new ("Hello");
    gtk_window_set_child (GTK_WINDOW(win),lab);
    gtk_window_present(GTK_WINDOW(win));
}


int main(int argc, char **argv){
    GtkApplication *app;
    int status ;
    app = gtk_application_new("com.github.To", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (app_activate),NULL);
    status = g_application_run( G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;

}

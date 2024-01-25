#include <gtk/gtk.h>


int main (int argc, char **argv){

gtk_init (&argc,&argv);

GtkWidget *window;
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window),"This App has buttons");
gtk_window_set_default_size(GTK_WINDOW(window),500,400);
gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

GtkWidget *button1, *button2;
button1 = gtk_button_new_with_label("button1");
button2 = gtk_button_new_with_label("button2");

gtk_container_add(GTK_CONTAINER(window),button1);
gtk_container_remove(GTK_CONTAINER(window),button1);

gtk_container_add(GTK_CONTAINER(window),button2);

g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit),NULL);

gtk_widget_show_all(window);

gtk_main ();

return 0;
}

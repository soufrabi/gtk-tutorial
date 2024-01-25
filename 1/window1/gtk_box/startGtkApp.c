#include <gtk/gtk.h>


int main (int argc, char **argv){

gtk_init (&argc,&argv);

GtkWidget *window;
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window),"This App has buttons");
gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
// gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
gtk_window_set_resizable(GTK_WINDOW(window),TRUE);

GtkWidget *box;
box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
// box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

GtkWidget *button1, *button2;
button1 = gtk_button_new_with_label("button1");
gtk_widget_set_size_request(GTK_WIDGET(button1),500,500);
button2 = gtk_button_new_with_label("button2");


gtk_container_add(GTK_CONTAINER(window),box);


gtk_box_pack_start(GTK_BOX(box),button1, TRUE,TRUE,50);
gtk_box_pack_start(GTK_BOX(box),button2, TRUE,FALSE,50);

int isHomo;

isHomo = gtk_box_get_homogeneous(GTK_BOX(box));
printf("Is Homogenous? %d \n",isHomo);

gtk_box_set_homogeneous(GTK_BOX(box),TRUE);


g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit),NULL);

gtk_widget_show_all(window);

gtk_main ();

return 0;
}

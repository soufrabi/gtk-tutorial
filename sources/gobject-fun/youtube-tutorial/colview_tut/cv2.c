#include <gtk/gtk.h>

static void
setup_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new ("");

	gtk_list_item_set_child (list_item, label);
}

static void
bind_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	GtkStringObject *strobj = gtk_list_item_get_item (list_item);
	GtkWidget *label = gtk_list_item_get_child (list_item);

	gtk_label_set_text (GTK_LABEL(label), gtk_string_object_get_string (strobj));
}

static void
add_stuff_to_store (GListStore *store)
{
	g_autoptr(GtkStringObject) strobj1 = gtk_string_object_new ("foo");
	g_autoptr(GtkStringObject) strobj2 = gtk_string_object_new ("bar");

	g_list_store_append (store, strobj1);
	g_list_store_append (store, strobj2);
}

static GtkListItemFactory *
create_factory (void)
{
	GtkListItemFactory *factory = gtk_signal_list_item_factory_new ();

	g_signal_connect (factory, "setup", G_CALLBACK(setup_cb), NULL);
	g_signal_connect (factory, "bind", G_CALLBACK(bind_cb), NULL);

	return factory;
}

static GtkWidget *
create_column_view (void)
{
	GListStore *store = g_list_store_new (GTK_TYPE_STRING_OBJECT);
	GtkSelectionModel *selection = GTK_SELECTION_MODEL(gtk_single_selection_new (G_LIST_MODEL(store)));
	GtkWidget *column_view = gtk_column_view_new (selection);
	GtkListItemFactory *factory = create_factory ();
	GtkColumnViewColumn *column1 = gtk_column_view_column_new ("My First Column", factory);
	GtkColumnViewColumn *column2 = gtk_column_view_column_new ("My Second Column", factory);

	add_stuff_to_store (store);
	gtk_column_view_append_column (GTK_COLUMN_VIEW(column_view), column1);
	gtk_column_view_append_column (GTK_COLUMN_VIEW(column_view), column2);

	return column_view;
}

static void
activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *scrolled_window;
	GtkWidget *column_view;

	window = gtk_window_new ();
	scrolled_window = gtk_scrolled_window_new ();
	column_view = create_column_view ();

	gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(scrolled_window), column_view);
	gtk_window_set_child (GTK_WINDOW(window), scrolled_window);
	gtk_window_set_default_size (GTK_WINDOW(window), 400, 400);
	gtk_application_add_window (app, GTK_WINDOW(window));

	gtk_window_present (GTK_WINDOW(window));
}

int
main (int argc, char *argv[])
{
	g_autoptr(GtkApplication) app = NULL;
	int status;

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	G_GNUC_END_IGNORE_DEPRECATIONS
	g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run (G_APPLICATION(app), argc, argv);

	return status;
}

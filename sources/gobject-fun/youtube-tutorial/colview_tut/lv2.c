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
	GtkWidget *child = gtk_list_item_get_child (list_item);

	gtk_label_set_text (GTK_LABEL(child), gtk_string_object_get_string (strobj));
}

static void
add_stuff_to_store (GListStore *store)
{
	/* g_list_store_append is (transfer none) on the receiving end,
	 * so we should free our originating objects at the end of the scope b/c we
	 * no longer need them.
	 */
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
create_list_view (void)
{
	/* Here is a version using g_steal_pointer(). Note that it is a bit more
	 * cumbersome to do it this way. Given that we're just creating a widget
	 * out of a chain of objects, all of which take ownership of the last, it's
	 * easier NOT to do it this way. Note that we now nullify all autoptrs at
	 * the top of the block, for consistency.
	 */
	g_autoptr(GListStore) store = NULL;
	g_autoptr(GtkSelectionModel) selection = NULL;
	g_autoptr(GtkListItemFactory) factory = NULL;
	g_autoptr(GtkWidget) list_view = NULL;

	store = g_list_store_new (GTK_TYPE_STRING_OBJECT);
	add_stuff_to_store (store);
	selection = GTK_SELECTION_MODEL(gtk_single_selection_new (G_LIST_MODEL(g_steal_pointer (&store))));
	factory = create_factory ();
	list_view = gtk_list_view_new (g_steal_pointer (&selection), g_steal_pointer (&factory));

	/* If we didn't use g_steal_pointer() in the return statement, list_view
	 * would get freed at the end of the block, and the function getting it on
	 * the receiving end would be operating on invalid memory.
	 */
	return g_steal_pointer (&list_view);
}

static void
activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window = gtk_window_new ();
	GtkWidget *scrolled_window = gtk_scrolled_window_new ();
	GtkWidget *list_view = create_list_view ();

	gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(scrolled_window), list_view);
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

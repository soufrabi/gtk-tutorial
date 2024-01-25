#include <gtk/gtk.h>

/* <DemoStringTuple> */

#define DEMO_TYPE_STRING_TUPLE (demo_string_tuple_get_type ())
G_DECLARE_FINAL_TYPE (DemoStringTuple, demo_string_tuple, DEMO, STRING_TUPLE, GObject)

enum
{
	PROP_STR1 = 1,
	PROP_STR2,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

struct _DemoStringTuple
{
	GObject parent_instance;

	char *str1;
	char *str2;
};

G_DEFINE_TYPE (DemoStringTuple, demo_string_tuple, G_TYPE_OBJECT)

static void
demo_string_tuple_set_property (GObject *object,
		guint property_id,
		const GValue *value,
		GParamSpec *pspec)

{
	DemoStringTuple *self = DEMO_STRING_TUPLE(object);

	switch (property_id)
	{
		case PROP_STR1:
			self->str1 = g_value_dup_string (value);
			break;

		case PROP_STR2:
			self->str2 = g_value_dup_string (value);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void
demo_string_tuple_get_property (GObject *object,
		guint property_id,
		GValue *value,
		GParamSpec *pspec)
{
	DemoStringTuple *self = DEMO_STRING_TUPLE(object);

	switch (property_id)
	{
		case PROP_STR1:
			g_value_set_string (value, self->str1);
			break;

		case PROP_STR2:
			g_value_set_string (value, self->str2);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void
demo_string_tuple_finalize (GObject *object)
{
	DemoStringTuple *self = DEMO_STRING_TUPLE(object);

	g_free (self->str1);
	g_free (self->str2);
}

static void
demo_string_tuple_init (DemoStringTuple *self)
{
}

static void
demo_string_tuple_class_init (DemoStringTupleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GParamFlags flags = G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT;

	object_class->finalize = demo_string_tuple_finalize;
	object_class->set_property = demo_string_tuple_set_property;
	object_class->get_property = demo_string_tuple_get_property;

	properties[PROP_STR1] = g_param_spec_string ("str1", NULL, NULL, NULL, flags);
	properties[PROP_STR2] = g_param_spec_string ("str2", NULL, NULL, NULL, flags);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/* </DemoStringTuple> */

static void
setup1_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new ("");

	gtk_list_item_set_child (list_item, label);
}

static void
bind1_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	DemoStringTuple *demo = gtk_list_item_get_item (list_item);
	GtkWidget *label = gtk_list_item_get_child (list_item);
	g_autofree char *str = NULL;

	g_object_get (demo, "str1", &str, NULL);
	gtk_label_set_text (GTK_LABEL(label), str);
}

static void
setup2_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new ("");

	gtk_list_item_set_child (list_item, label);
}

static void
bind2_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	DemoStringTuple *demo = gtk_list_item_get_item (list_item);
	GtkWidget *label = gtk_list_item_get_child (list_item);
	g_autofree char *str = NULL;

	g_object_get (demo, "str2", &str, NULL);
	gtk_label_set_text (GTK_LABEL(label), str);
}

static void
add_stuff_to_store (GListStore *store)
{
	/* G_DECLARE_FINAL_TYPE 'automagically' adds g_autoptr functionality to our object.
	 */
	g_autoptr(DemoStringTuple) demo1 = g_object_new (DEMO_TYPE_STRING_TUPLE,
			"str1", "foo",
			"str2", "bar",
			NULL);
	g_autoptr(DemoStringTuple) demo2 = g_object_new (DEMO_TYPE_STRING_TUPLE,
			"str1", "baz",
			"str2", "boo",
			NULL);

	g_list_store_append (store, demo1);
	g_list_store_append (store, demo2);
}

static GtkListItemFactory *
create_factory1 (void)
{
	GtkListItemFactory *factory = gtk_signal_list_item_factory_new ();

	g_signal_connect (factory, "setup", G_CALLBACK(setup1_cb), NULL);
	g_signal_connect (factory, "bind", G_CALLBACK(bind1_cb), NULL);

	return factory;
}

static GtkListItemFactory *
create_factory2 (void)
{
	GtkListItemFactory *factory = gtk_signal_list_item_factory_new ();

	g_signal_connect (factory, "setup", G_CALLBACK(setup2_cb), NULL);
	g_signal_connect (factory, "bind", G_CALLBACK(bind2_cb), NULL);

	return factory;
}

static GtkWidget *
create_column_view (void)
{
	GListStore *store = g_list_store_new (DEMO_TYPE_STRING_TUPLE);
	GtkSelectionModel *selection = GTK_SELECTION_MODEL(gtk_single_selection_new (G_LIST_MODEL(store)));
	GtkWidget *column_view = gtk_column_view_new (selection);
	GtkListItemFactory *factory1 = create_factory1 ();
	GtkListItemFactory *factory2 = create_factory2 ();
	GtkColumnViewColumn *column1 = gtk_column_view_column_new ("My First Column", factory1);
	GtkColumnViewColumn *column2 = gtk_column_view_column_new ("My Second Column", factory2);

	add_stuff_to_store (store);

	gtk_column_view_append_column (GTK_COLUMN_VIEW(column_view), column1);
	gtk_column_view_append_column (GTK_COLUMN_VIEW(column_view), column2);

	return column_view;
}

static void
activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window = gtk_window_new ();
	GtkWidget *scrolled_window = gtk_scrolled_window_new ();
	GtkWidget *column_view = create_column_view ();

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

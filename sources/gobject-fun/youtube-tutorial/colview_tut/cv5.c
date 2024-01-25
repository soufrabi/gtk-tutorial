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

static DemoStringTuple *
demo_string_tuple_new (const char *str1, const char *str2)
{
	return g_object_new (DEMO_TYPE_STRING_TUPLE,
			"str1", str1,
			"str2", str2,
			NULL);
}

/* </DemoStringTuple> */

static void
setup_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new ("");

	gtk_list_item_set_child (list_item, label);
}

/* Here is a new version of bind_cb using `GtkExpression`.
 *
 * Note that we never actually explicitly grab the `item` as a GObject and
 * extract its string and manually set the label as we did before.
 *
 * Instead, we basically create the following binding in pseudocode:
 *
 *   DEMO_STRING_TUPLE(GtkListItem.item)->{"str1"} ==> GTK_LABEL(label)->{"label"}
 *
 * 		... in other words, we bind the "str1" (or "str2", depending on the
 * 		`prop` argument) property of our DemoStringTuple -- which we know to be
 * 		the `item` of the GtkListItem -- to the "label" property of our
 * 		GtkLabel, which we know to be the `child` of the same GtkListItem.
 *
 * This may seem pointless in code, but the usefulness of this technique will
 * (hopefully) become clearer when we look at how GtkExpressions can be used in
 * GtkBuilder XML files.
 */
static void
bind_cb (GtkSignalListItemFactory *self, GtkListItem *list_item, const char *prop)
{
	GtkWidget *label = gtk_list_item_get_child (list_item);
	GtkExpression *expr1, *expr2;

	expr1 = gtk_property_expression_new (GTK_TYPE_LIST_ITEM, NULL, "item");
	expr2 = gtk_property_expression_new (DEMO_TYPE_STRING_TUPLE, expr1, prop);
	gtk_expression_bind (expr2, label, "label", /*this*/ list_item);
}

static void
add_stuff_to_store (GListStore *store)
{
	g_autoptr(DemoStringTuple) demo1 = demo_string_tuple_new ("foo", "bar");
	g_autoptr(DemoStringTuple) demo2 = demo_string_tuple_new ("baz", "boo");

	g_list_store_append (store, demo1);
	g_list_store_append (store, demo2);
}

static GtkListItemFactory *
create_factory (const char *prop)
{
	GtkListItemFactory *factory = gtk_signal_list_item_factory_new ();

	g_signal_connect (factory, "setup", G_CALLBACK(setup_cb), NULL);
	g_signal_connect (factory, "bind", G_CALLBACK(bind_cb), (gpointer)prop);

	return factory;
}

static GtkWidget *
create_column_view (void)
{
	GListStore *store = g_list_store_new (DEMO_TYPE_STRING_TUPLE);
	GtkSelectionModel *selection = GTK_SELECTION_MODEL(gtk_single_selection_new (G_LIST_MODEL(store)));
	GtkWidget *column_view = gtk_column_view_new (selection);
	GtkListItemFactory *factory1 = create_factory ("str1");
	GtkListItemFactory *factory2 = create_factory ("str2");
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

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
add_stuff_to_store (GListStore *store)
{
	g_autoptr(DemoStringTuple) demo1 = demo_string_tuple_new ("foo", "bar");
	g_autoptr(DemoStringTuple) demo2 = demo_string_tuple_new ("baz", "boo");

	g_list_store_append (store, demo1);
	g_list_store_append (store, demo2);
}

static void
setup_column_view (GtkWidget *column_view)
{
	GListStore *store = g_list_store_new (DEMO_TYPE_STRING_TUPLE);
	GtkSelectionModel *selection = GTK_SELECTION_MODEL(gtk_single_selection_new (G_LIST_MODEL(store)));

	add_stuff_to_store (store);
	gtk_column_view_set_model (GTK_COLUMN_VIEW(column_view), GTK_SELECTION_MODEL(selection));
}

static void
activate (GtkApplication *app, gpointer user_data)
{
	g_autoptr(GtkBuilder) builder = gtk_builder_new_from_file ("cv.ui");
	GtkWidget *window = gtk_window_new ();
	GtkWidget *scrolled_window = gtk_scrolled_window_new ();
	GtkWidget *column_view = GTK_WIDGET(gtk_builder_get_object (builder, "column_view"));

	setup_column_view (column_view);

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

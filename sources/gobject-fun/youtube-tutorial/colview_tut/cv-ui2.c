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

/* Note that in this final version, after we handle the creation of the custom
 * DemoStringTuple object, pretty much all of the remaining GTK GUI code is
 * boilerplate, just to get the window up on the screen. The meat & potatoes of
 * creating the GUI is almost all contained within the .ui file now.
 */
static void
activate (GtkApplication *app, gpointer user_data)
{
	g_autoptr(GtkBuilder) builder = NULL;
	GtkWindow *window;

	/* Ensure our custom object type is registered with the GObject system
	 * before attempting to utilize it through GtkBuilder.
	 */
	g_type_ensure (DEMO_TYPE_STRING_TUPLE);

	builder = gtk_builder_new_from_file ("cv-ui2.ui");
	window = GTK_WINDOW(gtk_builder_get_object (builder, "window"));

	gtk_application_add_window (app, window);
	gtk_window_present (window);
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

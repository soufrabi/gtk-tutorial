#include "demo-list-model.h"

/* Needed for GtkStringObject */
#include <gtk/gtk.h>

static void
items_changed_cb (GListModel *list,
               guint       position,
               guint       removed,
               guint       added,
               gpointer    user_data)
{
	g_print ("Item added at position: %u\n", position);
}

int main (void)
{
	DemoListModel *demo = demo_list_model_new (GTK_TYPE_STRING_OBJECT);

	/* Recall: g_autoptr will unref these objects when they go out of scope,
	 * unless the pointers are set to NULL, in which case it will do nothing at
	 * that time.
	 */
	g_autoptr(GtkStringObject) strobj1 = gtk_string_object_new ("foo");
	g_autoptr(GtkStringObject) strobj2 = gtk_string_object_new ("bar");

	/* Temporary object which we WANT to be destroyed when it goes out of
	 * scope, because we will get our own ref as a result of
	 * g_list_model_get_item being (transfer full).
	 */
	g_autoptr(GtkStringObject) tmp = NULL;

	g_signal_connect (demo, "items-changed", G_CALLBACK(items_changed_cb), NULL);

	/* Since we know our list model takes ownership of objects provided to it,
	 * let's hammer that point home by using g_steal_pointer.
	 */
	demo_list_model_append (demo, g_steal_pointer (&strobj1));
	demo_list_model_append (demo, g_steal_pointer (&strobj2));

	g_print ("Num items in model: %u\n", g_list_model_get_n_items (G_LIST_MODEL(demo)));

	/* Grab the 2nd (1th) item: */
	tmp = g_list_model_get_item (G_LIST_MODEL(demo), 1);
	g_print ("Contents of string in 2nd (1th) item: %s\n", gtk_string_object_get_string (tmp));

	/* Side note - if we now try to access the strings directly, we no longer
	 * can, since the list model has stolen ownership of it. This will throw a
	 * runtime error:
	 */
	g_print ("Contents of strobj2: %s\n", gtk_string_object_get_string (strobj2));

	return 0;
}

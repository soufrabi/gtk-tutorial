#include "demo-list-model.h"

enum
{
	PROP_TYPE = 1,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

struct _DemoListModel
{
	GObject parent_instance;

	GType type;
	GPtrArray *arr;
};

/* Instead of G_DEFINE_TYPE, we use G_DEFINE_TYPE_WITH_CODE to set up our interface.
 */

/* Forward declaration required. */
static void demo_list_model_interface_init (GListModelInterface *iface);

G_DEFINE_TYPE_WITH_CODE (DemoListModel, demo_list_model, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL, demo_list_model_interface_init))

static void
demo_list_model_set_property (GObject *object,
		guint property_id,
		const GValue *value,
		GParamSpec *pspec)

{
	DemoListModel *self = DEMO_LIST_MODEL(object);

	switch (property_id)
	{
		case PROP_TYPE:
			self->type = g_value_get_gtype (value);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void
demo_list_model_get_property (GObject *object,
		guint property_id,
		GValue *value,
		GParamSpec *pspec)
{
	DemoListModel *self = DEMO_LIST_MODEL(object);

	switch (property_id)
	{
		case PROP_TYPE:
			g_value_set_gtype (value, self->type);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}
static void
demo_list_model_dispose (GObject *object)
{
	DemoListModel *self = DEMO_LIST_MODEL(object);

	g_clear_pointer (&self->arr, g_ptr_array_unref);
}

static void
demo_list_model_class_init (DemoListModelClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	object_class->dispose = demo_list_model_dispose;
	object_class->set_property = demo_list_model_set_property;
	object_class->get_property = demo_list_model_get_property;

	properties[PROP_TYPE] = g_param_spec_gtype ("type", NULL, NULL,
			/* This means that the base type of any GType specified must be a
			 * GObject or a derivative thereof. */
			G_TYPE_OBJECT,
			G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
demo_list_model_init (DemoListModel *self)
{
	self->arr = g_ptr_array_new_with_free_func (g_object_unref);
}

static GType
demo_list_model_get_item_type (GListModel *list)
{
	DemoListModel *self = DEMO_LIST_MODEL(list);

	return self->type;
}

static guint
demo_list_model_get_n_items (GListModel *list)
{
	DemoListModel *self = DEMO_LIST_MODEL(list);

	/* We have direct access to the structure with a GPtrArray, and its 'len'
	 * member is already a guint.
	 */
	 return self->arr->len;
}

/* Acc. to TFM, the get_item implementation must be (transfer full). Thus we
 * must ref our returned object, which means that whoever is on the receiving
 * end is responsible for freeing their copy.
 */
static gpointer
demo_list_model_get_item (GListModel *list, guint position)
{
	DemoListModel *self = DEMO_LIST_MODEL(list);
	/* Function-style macro which returns a pointer to the element requested.
	 */
	GObject *obj = g_ptr_array_index (self->arr, position);

	return g_object_ref (obj);
}

static void
demo_list_model_interface_init (GListModelInterface *iface)
{
	/* To find out what methods your interface needs to implement, cut & paste
	 * from the documentation. Then, write functions that fit the prototypes of
	 * the relevant function pointers, as we have done above.
	 */
	iface->get_item_type = demo_list_model_get_item_type;
	iface->get_n_items = demo_list_model_get_n_items;
	iface->get_item = demo_list_model_get_item;
}

/* To keep it simple, we're just going to allow users to add objects to our
 * list model, and that's it. We take ownership of the object by NOT taking a
 * ref. Note that other list models take the opposite approach which we'll see
 * later.
 */
void
demo_list_model_append (DemoListModel *self, gpointer item)
{
	guint position;

	g_return_if_fail (G_IS_OBJECT (item));

	position = self->arr->len;
	g_ptr_array_add (self->arr, item);

	/* This is a GListModel function that we DON'T have to implement ourselves.
	 * It's not on the vtable for the iface. It emits the "items-changed"
	 * signal and it is up to us to fill in the correct information.
	 */
	g_list_model_items_changed (G_LIST_MODEL (self), position, /*removed*/ 0, /*added*/ 1);
}

DemoListModel *
demo_list_model_new (GType type)
{
	/* Ensure our library users have passed a valid GObject to this function.
	 */
	g_return_val_if_fail (g_type_is_a (type, G_TYPE_OBJECT), NULL);

	return g_object_new (DEMO_TYPE_LIST_MODEL,
			"type", type,
			NULL);
}

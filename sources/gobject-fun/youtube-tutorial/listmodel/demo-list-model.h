#pragma once

#include <glib-object.h>

/* Needed for GListModel */
#include <gio/gio.h>

G_BEGIN_DECLS

#define DEMO_TYPE_LIST_MODEL (demo_list_model_get_type ())
G_DECLARE_FINAL_TYPE (DemoListModel, demo_list_model, DEMO, LIST_MODEL, GObject)

DemoListModel *demo_list_model_new (GType type);
void demo_list_model_append (DemoListModel *self, gpointer item);

G_END_DECLS

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

/* Type declaration */
#define DEMO_TYPE_WIDGET demo_widget_get_type ()
G_DECLARE_FINAL_TYPE (DemoWidget,demo_widget,DEMO,WIDGET,GtkWidget)

/* Method declaration */

GtkWidget * demo_widget_new (void);

G_END_DECLS

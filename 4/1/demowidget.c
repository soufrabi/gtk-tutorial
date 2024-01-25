#include "demowidget.h"

struct _DemoWidget {
    /*
     * subclassing don't use a pointter * start here
     */
    GtkWidget parent_instance;
};

G_DEFINE_TYPE(DemoWidget, demo_widget, GTK_TYPE_WIDGET)

static void demo_widget_init(DemoWidget *self) {
}

static void demo_widget_class_init (DemoWidgetClass *kclass){
}

GtkWidget *
demo_widget_new(void){
    return g_object_new(DEMO_TYPE_WIDGET,NULL);
}

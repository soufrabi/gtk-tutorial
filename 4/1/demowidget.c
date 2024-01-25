#include "demowidget.h"

/*
 * Internal declarations private methods
 */

static void demo_widget_dispose(GObject *object) ;
static void demo_widget_finalize(GObject *object);


struct _DemoWidget {
    /*
     * subclassing don't use a pointter * start here
     */
    GtkWidget parent_instance;

    /*
     * Custom meat potatoes
     * you do using pointers
     */
    GtkWidget *button;
};

G_DEFINE_TYPE(DemoWidget, demo_widget, GTK_TYPE_WIDGET)

static void demo_widget_init(DemoWidget *self) {
    /* not necessary but it is  useful shorthand */
    GtkWidget *widget = GTK_WIDGET(self);

    self->button = gtk_button_new_with_label("Hello, World");
    gtk_widget_set_hexpand(self->button, TRUE);
    gtk_widget_set_parent(self->button,widget);
}


static void demo_widget_dispose(GObject *object){
    /*
     * dispose is called multiple types but finalize is called only once
     */
    DemoWidget *self = DEMO_WIDGET(object);
    /*
     * g_clear_pointer takes a pointer to a g_pointer
     * so that it can nullify the pointer
     */
    g_clear_pointer(&self->button, gtk_widget_unparent);
    G_OBJECT_CLASS(demo_widget_parent_class)->dispose(object);

}

static void demo_widget_finalize(GObject *object){
    /* Always chain up to parent class */
    G_OBJECT_CLASS(demo_widget_parent_class)->finalize(object);

}

static void demo_widget_class_init (DemoWidgetClass *kclass){
    // Establish shorthand to avoid some casts
    GObjectClass *object_class = G_OBJECT_CLASS(kclass);
    object_class->dispose = demo_widget_dispose;
    object_class->finalize = demo_widget_finalize;

    /*
     * WHich type of layout manager to use 
     * Box type is a good default
     */

    gtk_widget_class_set_layout_manager_type(GTK_WIDGET_CLASS(kclass), GTK_TYPE_BOX_LAYOUT);
}

GtkWidget *
demo_widget_new(void){
    return g_object_new(DEMO_TYPE_WIDGET,NULL);
}

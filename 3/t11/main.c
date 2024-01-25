#include <gtk/gtk.h>
#include <vte/vte.h>


static void
child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) {
        g_print("Quit App : child-ready did not start properly");
    }
}

static void
quit_app(VteTerminal *terminal) {
    g_print("Quit App");

}


static void app_activate(GApplication *app, gpointer *user_data) {
    g_print("GtkApplication is activated.\n");

    GtkWidget *win;
    GtkWidget *terminal;

    win = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(win), "Application Window Title");
    gtk_window_set_default_size(GTK_WINDOW(win), 200, 200);

    terminal = vte_terminal_new();
    gtk_window_set_child(GTK_WINDOW(win), terminal);
    
    /* Configure terminal */
    /* vte_terminal_set_word_chars(VTE_TERMINAL(terminal),"-A-Za-z0-9:./?%&#_=+@~"); */
	vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal),
	    1000);
	vte_terminal_set_scroll_on_output(VTE_TERMINAL(terminal),
	    FALSE);
	vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(terminal),
	    TRUE);
	/* vte_terminal_set_rewrap_on_resize(VTE_TERMINAL(terminal), TRUE); */

  /* Start a new shell */
    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    g_strfreev(envp);
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        NULL,         /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        0,            /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */
        NULL);        /* user_data */


     /* Connect some signals */
    g_signal_connect(win, "delete-event", G_CALLBACK(quit_app), NULL);
    g_signal_connect(terminal, "child-exited",G_CALLBACK(quit_app), NULL);

    gtk_window_present(GTK_WINDOW(win));
    
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("com.github.To", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

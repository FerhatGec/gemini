/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <stdio.h>
#include <vte/vte.h> /* LibVTE */

/*
	TODO: Add color scheme, transparent option and version.
*/

GtkWidget *window, *terminal;
GdkPixbuf *icon;

GdkPixbuf *create_pixbuf(const gchar * filename) {
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}


void gemini_start() {
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GeminiTerm");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);
    gtk_window_set_resizable (GTK_WINDOW(window), TRUE);

    icon = create_pixbuf("resource/gemini.png");
    gtk_window_set_icon(GTK_WINDOW(window), icon);	
    /* Start a new shell */
    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL }; /* Get SHELL environment. */
    g_strfreev(envp);

    /* Spawn asynchronous terminal */
    vte_terminal_spawn_async(VTE_TERMINAL(terminal), 
        VTE_PTY_DEFAULT, 
        NULL,
        command, 
        NULL, 
        G_SPAWN_DO_NOT_REAP_CHILD,
        NULL,
        NULL,
        NULL,
        -1,
        NULL,
        NULL,
        NULL);

    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);

    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    g_object_unref(icon);
    gtk_main();
}

int main(int argc, char *argv[]) {
    /* Initialize GTK, the window and the terminal */  
    gtk_init(&argc, &argv);
    gemini_start();
}

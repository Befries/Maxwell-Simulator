#include "canvas_manager.h"


static void activate_app(GtkApplication *app, gpointer user_data) {
  
  GtkBuilder *builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "./src/ui/builder.ui", NULL);
  
  GObject *window = gtk_builder_get_object(builder, "window");
  gtk_window_set_application(GTK_WINDOW(window), app);
  
  
  GObject *canvas = gtk_builder_get_object(builder, "canvas");  
  g_timeout_add(33, G_SOURCE_FUNC(update_canvas), canvas);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);
  g_object_unref(builder);   
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("befries.simulation.MaxwellSim", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK(activate_app), NULL);
  status = g_application_run (G_APPLICATION(app), argc, argv);
  g_object_unref (app);

  return status;
}


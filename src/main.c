#include "canvas_manager.h"
#include "config_manager.h"

static void activate_app(GtkApplication* app, gpointer user_data);
static void free_carrier(gpointer data);

static void activate_app(GtkApplication* app, gpointer user_data) {
  
  read_config("./config");

  // initialize the field manager at the start

  GtkBuilder* builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "./src/ui/MaxwellUI.ui", NULL);
  
  GObject* window = gtk_builder_get_object(builder, "Window");
  gtk_window_set_application(GTK_WINDOW(window), app);
  
  // instantiate all modifiables;
  GObject* canvas = gtk_builder_get_object(builder, "Canvas");
  GObject* run_time_label = gtk_builder_get_object(builder, "RunTimeLabel");

  struct modifiables* modifiables_carrier = malloc(sizeof(struct modifiables*));
  modifiables_carrier->canvas = GTK_WIDGET(canvas);
  modifiables_carrier->run_time_label = GTK_LABEL(run_time_label);

  gtk_label_set_max_width_chars(GTK_LABEL(run_time_label), 12);
  gtk_label_set_text(GTK_LABEL(run_time_label), "0000.0000\0");
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(canvas), draw_canvas, NULL, NULL);
  g_signal_connect(GTK_DRAWING_AREA(canvas), "resize", G_CALLBACK(on_canvas_resize), NULL);
  g_timeout_add(33, G_SOURCE_FUNC(update_canvas), modifiables_carrier);

  
  GObject* run_button = gtk_builder_get_object(builder, "RunButton");
  g_signal_connect(run_button, "clicked", G_CALLBACK(toggle_run), NULL);

  GObject* reset_button = gtk_builder_get_object(builder, "ResetButton");
  g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_run), NULL);

  
  gtk_widget_set_visible(GTK_WIDGET(window), true);
  g_object_unref(builder);   
}


int main (int argc, char** argv) {
  GtkApplication* app;
  int status;

  app = gtk_application_new("befries.simulation.MaxwellSim", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK(activate_app), NULL);
  status = g_application_run (G_APPLICATION(app), argc, argv);
  g_object_unref (app);

  return status;
}

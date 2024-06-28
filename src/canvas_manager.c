#include "canvas_manager.h"
#include "cairo.h"
#include <stdio.h>

// surface on which we draw
static cairo_surface_t* surface = NULL;

static int running = 0;
static double run_time = 0;

// on every resize (as well at the start)
void on_canvas_resize(GtkWidget* canvas) {

  // if the surface already exist, destroy it
  if (surface)
    {
      cairo_surface_destroy (surface);
      surface = NULL;
    }
  
  // create the surface via the DrawingArea widget
  if (gtk_native_get_surface(gtk_widget_get_native(canvas))) {
    surface = gdk_surface_create_similar_surface (gtk_native_get_surface (gtk_widget_get_native (canvas)),
                                                    CAIRO_CONTENT_COLOR,
                                                    gtk_widget_get_width (canvas),
                                                    gtk_widget_get_height (canvas));
    }
}


// make the canvas blank
void clear_canvas(cairo_t* cr) {
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);
}


void toggle_run() {
  running = !running;
}


void reset_run() {
  running = 0;
  run_time = 0;
  clear_fields();
}


// update to the canvas each tick (main loop)
int update_canvas(struct modifiables* ui_elements) {
  cairo_t* cr;
  GtkWidget* canvas = ui_elements->canvas;

  cr = cairo_create(surface);
  clear_canvas(cr);
  
  int canvas_width = gtk_widget_get_width(canvas);
  int canvas_height = gtk_widget_get_height(canvas);

  if (running) {
    update_fields();
    run_time += get_dt();
  }

  draw_grid(cr, get_points_amount(), get_cols_amount(), get_view_port(), canvas_width, canvas_height);

  cairo_destroy(cr);
  char time_str[12];
  snprintf(time_str, 12, "%10.5f", run_time);
  gtk_label_set_text(ui_elements->run_time_label, time_str);
  gtk_widget_queue_draw(canvas);
  return 1;
}


// takes a grid of double between 0 and 1 and draw it on the canvas
void draw_grid(cairo_t* cr, int amount, int cols_amount, double* grid, double canvas_width, double canvas_height) {
  int rows_amount = amount/cols_amount;
  double atomic_width = canvas_width / cols_amount;
  double atomic_height = canvas_height / rows_amount;

  cairo_set_line_width(cr, 0);
  
  for (int i = 0; i < amount; i++) {
    cairo_set_source_rgba(cr, 0, 0, 0, 1 - grid[i]);
    cairo_rectangle(cr,
                    (i % cols_amount) * atomic_width,
                    (i / cols_amount) * atomic_height,
                    atomic_width,
                    atomic_height);
    cairo_fill(cr);
  }
    
}


// apply the drawing when the drawing_area requires it
void draw_canvas(GtkDrawingArea* drawing_area, cairo_t* cr, int width, int height, gpointer data) {
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
}


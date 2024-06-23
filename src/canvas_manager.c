#include "canvas_manager.h"
#include "cairo.h"

// surface on which we draw
static cairo_surface_t* surface = NULL;

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


// update to the canvas each tick (main loop)
int update_canvas(GtkWidget* widget) {
  
  cairo_t* cr;

  cr = cairo_create(surface);
  clear_canvas(cr);
  
  int canvas_width = gtk_widget_get_width(widget);
  int canvas_height = gtk_widget_get_height(widget);

  // temporary test
  
  draw_grid(cr, AMOUNT, get_cols_amount(), get_view_port(), canvas_width, canvas_height);
  
  cairo_destroy(cr);
  gtk_widget_queue_draw(widget);
  return 1;
}


// takes a grid of double between 0 and 1 and draw it on the canvas
void draw_grid(cairo_t* cr, int amount, int cols_amount, double* grid, double canvas_width, double canvas_height) {
  int rows_amount = amount/cols_amount;
  double atomic_width = canvas_width / cols_amount;
  double atomic_height = canvas_height / rows_amount;
  
  int i = 0;
  for (i; i < amount; i++) {
    cairo_set_source_rgba(cr, grid[i], grid[i], grid[i], 0.8);
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


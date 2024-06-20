#include "canvas_manager.h"
#include "cairo.h"

static cairo_surface_t *surface = NULL;


void init_canvas(GtkWidget *canvas) {
  // if the surface already exist, destroy it
  if (surface)
    {
      cairo_surface_destroy (surface);
      surface = NULL;
    }
  
  // create the surface via the DrawingArea widget
  if (gtk_native_get_surface(gtk_widget_get_native(canvas)))
    {
      surface = gdk_surface_create_similar_surface (gtk_native_get_surface (gtk_widget_get_native (canvas)),
                                                    CAIRO_CONTENT_COLOR,
                                                    gtk_widget_get_width (canvas),
                                                    gtk_widget_get_height (canvas));
    }
}


int clear_canvas(cairo_t *cr) {
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  return 1;
}

int update_canvas() {
  
  cairo_t *cr;
  cr = cairo_create(surface);
  clear_canvas(cr);

  return 1;
}




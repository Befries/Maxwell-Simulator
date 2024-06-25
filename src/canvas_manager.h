#include<gtk/gtk.h>
#include "field_manager.h"
#define AMOUNT 20

struct modifiables {
  GtkWidget* canvas;
  GtkLabel* run_time_label;
};

void on_canvas_resize(GtkWidget* canvas);
int update_canvas(struct modifiables* ui_elements); 
void clear_canvas(cairo_t* cr);
void draw_grid(cairo_t* cr, int amount, int cols_amount, double* grid, double canvas_width, double canvas_height);
void draw_canvas(GtkDrawingArea* drawing_area, cairo_t* cr, int width, int height, gpointer data);
void toggle_run();
void reset_run();

#include "config_expander.h"
#include <stddef.h>

static void init_properties_grid();
static void set_global_margin(GtkWidget* widget, int start, int end, int top, int bottom);
static GtkEntry* add_property_entry(GtkGrid* grid, char* label, int nchar, int col, int row, int col_span, int row_span);

static GtkBox* box;

void init_expander(GtkBox* expander_box) {
  box = expander_box;
  init_properties_grid();
}


void display_config() {
  
}


static void free_entries(GtkWidget* widget, gpointer data) {
  GtkEntry** entries = data;
  g_print("freeing entries \n");
  free(entries);
}


static void init_properties_grid() {
 
 GtkWidget* properties_grid = gtk_grid_new();
 gtk_box_prepend(box, properties_grid); 
 gtk_widget_set_hexpand(properties_grid, TRUE);
 gtk_widget_set_halign(properties_grid, GTK_ALIGN_FILL);
 set_global_margin(properties_grid, 4, 4, 4, 8);
 gtk_grid_set_row_spacing(GTK_GRID(properties_grid), 4);

 GtkEntry* width_entry = add_property_entry(GTK_GRID(properties_grid), "width:", 4, 0, 0, 1, 1);
 GtkEntry* height_entry = add_property_entry(GTK_GRID(properties_grid), "height:", 4, 1, 0, 1, 1);

 GtkWidget* apply_grid_size_button = gtk_button_new(); 
 gtk_button_set_label(GTK_BUTTON(apply_grid_size_button), "apply new properties");
 gtk_grid_attach(GTK_GRID(properties_grid), apply_grid_size_button, 0, 2, 4, 1);
 
 // array of the entries in the properies grid (change number depending on amount of)
 GtkEntry** entries = malloc(2 * sizeof(GtkEntry*));
 entries[0] = width_entry;
 entries[1] = height_entry;

 g_signal_connect(properties_grid, "destroy", G_CALLBACK(free_entries), entries);
}



static GtkEntry* add_property_entry(GtkGrid* grid, char* label, int nchar, int col, int row, int col_span, int row_span) {
 GtkWidget* entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
 gtk_grid_attach(grid, entry_box, col, row, col_span, row_span);

 GtkWidget* entry_label = gtk_label_new(label);
 gtk_box_prepend(GTK_BOX(entry_box), entry_label);
 gtk_widget_set_hexpand(entry_label, TRUE);

 GtkWidget* entry = gtk_entry_new();
 gtk_box_append(GTK_BOX(entry_box), entry);
 gtk_entry_set_max_length(GTK_ENTRY(entry), nchar);
 gtk_editable_set_max_width_chars(GTK_EDITABLE(entry), nchar);

 return GTK_ENTRY(entry);
}

static void set_global_margin(GtkWidget* widget, int start, int end, int top, int bottom) {
 gtk_widget_set_margin_top(widget, top);
 gtk_widget_set_margin_end(widget, end);
 gtk_widget_set_margin_start(widget, start);
 gtk_widget_set_margin_bottom(widget, bottom);
}

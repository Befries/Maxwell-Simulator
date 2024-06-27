#include "config_expander.h"
#include "materials.h"
#include "canvas_manager.h"
#include <stddef.h>
#include <stdio.h>

static GtkEntry** init_properties_grid();
static void set_global_margin(GtkWidget* widget, int start, int end, int top, int bottom);
static GtkEntry* add_property_entry(GtkGrid* grid, char* label, int nchar, int col, int row, int col_span, int row_span);

static GtkBox* box;

void init_expander(GtkBox* expander_box) {
  box = expander_box;
  GtkEntry** entries = init_properties_grid();
  display_config(entries);

}


void display_config(GtkEntry** entries) {
  // display the basic properties
  int width = get_cols_amount();
  int height = get_points_amount()/width;

  double e0 = get_void_material()->permittivity;
  double mu0 = get_void_material()->permeability;
  
  char width_str[4]; sprintf(width_str, "%d", width);
  gtk_editable_set_text(GTK_EDITABLE(entries[0]), width_str);

  char height_str[4];
  sprintf(height_str, "%d", height);
  gtk_editable_set_text(GTK_EDITABLE(entries[1]), height_str);

  char e0_str[6];
  sprintf(e0_str, "%2.3f", e0);
  gtk_editable_set_text(GTK_EDITABLE(entries[2]), e0_str);
  
  char mu0_str[6];
  sprintf(mu0_str, "%2.3f", mu0);
  gtk_editable_set_text(GTK_EDITABLE(entries[3]), mu0_str);
}


static void free_entries(GtkWidget* widget, gpointer data) {
  GtkEntry** entries = data;
  free(entries);
}


static GtkEntry** init_properties_grid() {
  
  GtkWidget* properties_grid = gtk_grid_new();
  gtk_box_prepend(box, properties_grid); 
  gtk_widget_set_hexpand(properties_grid, TRUE);
  gtk_widget_set_halign(properties_grid, GTK_ALIGN_FILL);
  set_global_margin(properties_grid, 4, 4, 4, 8);
  gtk_grid_set_row_spacing(GTK_GRID(properties_grid), 4);

  GtkEntry* width_entry = add_property_entry(GTK_GRID(properties_grid), "width:", 4, 0, 0, 1, 1);
  GtkEntry* height_entry = add_property_entry(GTK_GRID(properties_grid), "height:", 4, 1, 0, 1, 1);
  GtkEntry* permittivity_entry = add_property_entry(GTK_GRID(properties_grid), "e0", 6, 0, 1, 1, 1);
  GtkEntry* permeability_entry = add_property_entry(GTK_GRID(properties_grid), "mu0", 6, 1, 1, 1, 1);
 
  GtkWidget* apply_grid_size_button = gtk_button_new(); 
  gtk_button_set_label(GTK_BUTTON(apply_grid_size_button), "apply new properties");
  gtk_grid_attach(GTK_GRID(properties_grid), apply_grid_size_button, 0, 2, 4, 1);
 
  // array of the entries in the properies grid (change number depending on amount of)
  GtkEntry** entries = malloc(4 * sizeof(GtkEntry*));
  entries[0] = width_entry;
  entries[1] = height_entry;
  entries[2] = permittivity_entry;
  entries[3] = permeability_entry;

  g_signal_connect(properties_grid, "destroy", G_CALLBACK(free_entries), entries);
  g_signal_connect(apply_grid_size_button, "clicked", G_CALLBACK(update_properties), entries);

  return entries;
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


void update_properties(GtkWidget* widget, gpointer data) {
  GtkEntry** entries = data;
  
  // read new width and height;
  int new_width = atoi(gtk_editable_get_text(GTK_EDITABLE(entries[0])));
  if (new_width == 0) {
    new_width = 50;
    g_print("invalid width\n");
  }

  int new_height = atoi(gtk_editable_get_text(GTK_EDITABLE(entries[1])));
  if (new_height == 0) {
    new_height = 50;
    g_print("invalid height\n");
  }
  
  // new e0 & mu0
  double new_e0 = atof(gtk_editable_get_text(GTK_EDITABLE(entries[2])));
  if (new_e0 <= 0.0) {
    new_e0 = 1.0;
    g_print("invalid permittivity\n");
  }

  double new_mu0 = atof(gtk_editable_get_text(GTK_EDITABLE(entries[3])));
  if (new_mu0 <= 0.0) {
    new_e0 = 1.0;
    g_print("invalid permeability\n");
  }
  
  reset_run();
  free_fields();
  set_void_material(new_e0, new_mu0);
  init_field_manager(new_width, new_height); 

}

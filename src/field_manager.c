#include <stdlib.h>
#include <stdio.h>
#include "field_manager.h"
#include "materials.h"
#include "utils.h"
void normalize_intensity(double (*norm)(double*, int), double* intensity);

static material** properties;
static double* fields[3]; 
static double* fields_buffer[3];
static int amount;
static int cols_amount;

static double dt = 0.01, dx = 0.01, dy = 0.01;


static double* view_port;
static int mode = 0;

void update_view_port() {
  if (mode) {
    for (int i = 0; i < amount; i++) {
      view_port[i] = SQUARE((fields[1])[i]) + SQUARE((fields[2])[i]);
    }
  } else {
    for (int i = 0; i < amount; i++) {
      view_port[i] = SQUARE((fields[0])[i]);
    }
  }

  normalize_intensity(infinity_norm, view_port);
}

void normalize_intensity(double (*norm)(double*, int), double* intensity) {
  double normalizer = norm(intensity, amount);
  for (int i = 0; i < amount; i++) intensity[i] = intensity[i]/normalizer;
}


int get_points_amount() {
  return amount;
}

int get_cols_amount() {
  return cols_amount;
}

double* get_view_port() {
  return view_port;
}


void init_field_manager(int _rows_amount, int _cols_amount) {
  
  amount = _rows_amount * cols_amount;
  cols_amount = _cols_amount;

  // properties is an array of pointers to struct of the concerned materials
  properties = malloc(amount * sizeof(material*));
  // initially, the whole space is void.
  material* void_material = getVoidMaterial();
  for (int i = 0; i < amount; i++) properties[i] = void_material;
  // W.I.P. rajouter demande config;

  // intensity chosen to be viewed
  int data_size = amount * sizeof(double);
  view_port = malloc(data_size);
  // 0 = Ez, 1 = Bx, 2 = By

  for (int i = 0; i < 3; i++) {
    fields[i] = malloc(data_size);
    fields_buffer[i] = malloc(data_size);
  }
  
  clear_fields();

}


void clear_fields() {
 for (int i = 0; i < 3; i++) {
    for (int j = 0; j < amount; j++) (fields[i])[j] = 0;
  }
}


void update_fields() {
  
  for (int i = 0; i < 3; i++) {
    copy_array(fields[i], fields_buffer[i], amount);
  }

  for (int i = 0; i < amount; i++) {

    // difference calculation
    double under_E = 0, under_Bx = 0;
    int under_index = i - cols_amount;
    if (under_index > 0) {
       under_E = fields_buffer[0][under_index];
       under_Bx = fields_buffer[1][under_index];
    }

    double upper_E = 0, upper_Bx = 0;
    int upper_index = i + cols_amount;
    if (upper_index < cols_amount) {
       upper_E = fields_buffer[0][upper_index];
       upper_Bx = fields_buffer[1][upper_index];
    }

    double left_E = 0, left_By = 0;
    if (i % cols_amount > 0) {
      int left_index = i - 1;
      left_E = fields_buffer[0][left_index];
      left_By = fields_buffer[2][left_index];
    }

    double right_E = 0, right_By = 0;
    if (i % cols_amount + 1 < cols_amount) {
      int right_index = i + 1;
      right_E = fields_buffer[0][right_index];
      right_By = fields_buffer[2][right_index];
    }
    
    // todo: add user input to current
    double current_term = (properties[i]->conductivity * fields[0][i]) / properties[i]->permittivity;
    double dxBy = (right_By - left_By) / (2 * dx);
    double dyBx = (upper_Bx - under_Bx) / (2 * dy);
    double difference_term = properties[i]->speed_squared * (dxBy - dyBx);
    fields[0][i] = fields_buffer[0][i] + dt * (difference_term - current_term);

    double dyE = (upper_E - under_E) / (2 * dy);
    fields[1][i] = fields_buffer[1][i] - dt * dyE;

    double dxE = (right_E - left_E) / (2 * dx);
    fields[2][i] = fields_buffer[2][i] + dt * dxE;
  }
 
  update_view_port();

}

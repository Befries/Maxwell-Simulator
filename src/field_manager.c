#include <stdlib.h>
#include <stdio.h>
#include "field_manager.h"
#include "materials.h"
#include "utils.h"

static material** properties;
static double** fields; 
static double** fields_buffer;
static int amount;
static int cols_amount;
static double* arr;


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
}

void normalize_intensity(double (*norm)(double*, int), double* intensity, int _amount) {
  double normalizer = norm(intensity, _amount);
  for (int i = 0; i < _amount; i++) intensity[i] = intensity[i]/normalizer;
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
    fields_buffer = malloc(data_size);
  }

}


void update_fields(double dt) {
  
  

}


#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "field_manager.h"
#include "materials.h"
#include "utils.h"
void normalize_intensity(double (*norm)(double*, int), double* intensity);
void bound_view_port(double boundary);

static material** properties;

static double* fields[3]; 
static double* fields_buffer[3];
static int amount;
static int cols_amount;

static double dt = 0.0001, dx = 0.001, dy = 0.001;


static double* view_port;
static int mode = 0;

double electric_boundary = 0.002;
double magnetic_boundary = 0.0002;

void update_view_port() {
  if (mode) {
    for (int i = 0; i < amount; i++) {
      view_port[i] = SQUARE((fields[1])[i]) + SQUARE((fields[2])[i]);
    }
  } else {
    for (int i = 0; i < amount; i++) {
      view_port[i] = fabs((fields[0])[i]);
    }
  }
 
  //double test_max = max(view_port, amount);
  //if (test_max > current_max) {
    //current_max = test_max;
    //printf("new max: %2.6f\n", current_max);
  //}

  bound_view_port(electric_boundary);
  //bound_view_port(magnetic_boundary);
}


void bound_view_port(double boundary) {
  for (int i = 0; i < amount; i++) view_port[i] = view_port[i]/boundary;
}


void normalize_intensity(double (*norm)(double*, int), double* intensity) {
  double normalizer = norm(intensity, amount);
  if (normalizer == 0) return;
  for (int i = 0; i < amount; i++) intensity[i] = intensity[i]/normalizer;
}


int get_points_amount() {
  return amount;
}


int get_cols_amount() {
  return cols_amount;
}


double* get_view_port() {
  update_view_port();
  return view_port;
}


void init_field_manager(int _rows_amount, int _cols_amount) {
  
  amount = _rows_amount * _cols_amount;
  printf("total amount of points: %d \n", amount);
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


double forward_difference(double* current_field, int index);
double backward_difference(double* current_field, int index);
double upward_difference(double* current_field, int index);
double downward_difference(double* current_field, int index);

double simple_centered_vertical_difference(double* current_field, int index);
double simple_centered_horizontal_difference(double* current_field, int index);
double advanced_centered_vertical_difference(double* current_field, int index);
double advanced_centered_horizontal_difference(double* current_field, int index);


void update_fields() {
  
  for (int i = 0; i < 3; i++) {
    copy_array(fields[i], fields_buffer[i], amount);
  }

  for (int i = 0; i < amount; i++) {
    
    // difference to calculate
    double dxE, dyE, dxBy, dyBx;

    // difference calculation
    if (i < cols_amount) {
      dyE = upward_difference(fields_buffer[0], i);
      dyBx = upward_difference(fields_buffer[1], i); 
    } else if (i + cols_amount >= amount) {
      dyE = downward_difference(fields_buffer[0], i);
      dyBx = downward_difference(fields_buffer[1], i);
    } else if (i < 2*cols_amount || i + 2*cols_amount >= amount) {
      dyE = simple_centered_vertical_difference(fields_buffer[0], i);
      dyBx = simple_centered_vertical_difference(fields_buffer[1], i);
    } else {
      dyE = advanced_centered_vertical_difference(fields_buffer[0], i);
      dyBx = advanced_centered_vertical_difference(fields_buffer[1], i);
    }

    if (i % cols_amount <= 0) {
      dxE = forward_difference(fields_buffer[0], i);
      dxBy = forward_difference(fields_buffer[2], i);
    } else if (i % cols_amount >= cols_amount - 1) {
      dxE = backward_difference(fields_buffer[0], i);
      dxBy = backward_difference(fields_buffer[2], i);
    } else if (i % cols_amount <= 1 || i % cols_amount >= cols_amount - 2) {
      dxE = simple_centered_horizontal_difference(fields_buffer[0], i);
      dxBy = simple_centered_horizontal_difference(fields_buffer[2], i);
    } else {
      dxE = advanced_centered_horizontal_difference(fields_buffer[0], i);
      dxBy = advanced_centered_horizontal_difference(fields_buffer[2], i);      
    }

    // manually added current
    double J = 0;
    if (i%cols_amount == (int)(0.4312 * cols_amount)) J = 1;

    // todo: add user input to current
    double current_term = (properties[i]->conductivity * fields[0][i] + J) / properties[i]->permittivity;
    
    double difference_term = properties[i]->speed_squared * (dxBy - dyBx);
    fields[0][i] = fields_buffer[0][i] + dt * (difference_term - current_term);
    fields[1][i] = fields_buffer[1][i] - dt * dyE;
    fields[2][i] = fields_buffer[2][i] + dt * dxE;
  }

}


// does O(dx^2) for the borders
double forward_difference(double* current_field, int index) {
  return (-current_field[index+2]+4*current_field[index+1]-3*current_field[index])/(2*dx);
} 

double backward_difference(double* current_field, int index) {
  return (current_field[index-2]-4*current_field[index-1]+3*current_field[index])/(2*dx);
}

double upward_difference(double* current_field, int index) {
  return (-current_field[index+2*cols_amount]+4*current_field[index+cols_amount]-3*current_field[index])/(2*dy);
} 

double downward_difference(double* current_field, int index) {
  return (current_field[index-2*cols_amount]-4*current_field[index-cols_amount]+3*current_field[index])/(2*dy);
}


// does centered O(dx^2) for almost bordered
double simple_centered_vertical_difference(double* current_field, int index) {
  return (current_field[index+cols_amount]-current_field[index-cols_amount])/(2*dy);
}

double simple_centered_horizontal_difference(double* current_field, int index) {
  return (current_field[index+1]-current_field[index-1])/(2*dx);
}

// does center O(dx^4) for every other points 
double advanced_centered_vertical_difference(double* current_field, int index) {
  return (-current_field[index+2*cols_amount]+8*current_field[index+cols_amount]-8*current_field[index-cols_amount]+current_field[index-2*cols_amount])/(12*dy);
}

double advanced_centered_horizontal_difference(double* current_field, int index) {
  return (-current_field[index+2]+8*current_field[index+1]-8*current_field[index-1]+current_field[index-2])/(12*dx);
}

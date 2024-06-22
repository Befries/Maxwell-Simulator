#include <stdlib.h>
#include <stdio.h>
#include "field_manager.h"

//static double** fields; 
//static int amount;
//static int cols_amount;
static double* arr;

double* convert_to_intensity(double (*transform)(double*), double** _fields, int _amount) {
  
  double* result = malloc(_amount*sizeof(double));
  for (int i = 0; i < _amount; i++) {
    result[i] = transform(_fields[i]);
  }

  return result;
}


void init_field_manager(int amount) {
  arr = malloc(amount*sizeof(double));
  srand(0);
}


double* return_random_array(int amount) {
  for(int i = 0; i < amount; i++) {
    arr[i] = (double)((rand() % 100) / 100.0);
  }
  return arr;
}

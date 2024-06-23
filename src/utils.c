#include "utils.h"


double max(double* array, int amount) {
  double res = *array;
  for (int i = 1; i < amount; i++) {
    if (array[i] > res) res = array[i];
  }
  return res;
}


double infinity_norm(double* vector, int amount) {
  return max(vector, amount);
}


void copy_array(double* reference, double* receiver, int amount) {
  for (int i = 0; i < amount; i++) receiver[i] = reference[i];
}

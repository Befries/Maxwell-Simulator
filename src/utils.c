#include "utils.h"


double max(double* array, int _amount) {
  double res = *array;
  for (int i = 1; i < _amount; i++) {
    if (array[i] > res) res = array[i];
  }
  return res;
}


double infinity_norm(double* vector, int _amount) {
  return max(vector, _amount);
}


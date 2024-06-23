#include "materials.h"

static material void_material = {1.0, 1.0, 0.0, 1.0};

material* getVoidMaterial() {
  return &void_material;
} 


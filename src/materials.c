#include "materials.h"

static material void_material = {1.0, 1.0, 0.0, 1.0};


void set_void_material(double permittivity, double permeability) {
  void_material.permittivity = permittivity;
  void_material.permeability = permeability;
  void_material.speed_squared = 1/(permittivity * permeability);
}


material* get_void_material() {
  return &void_material;
} 


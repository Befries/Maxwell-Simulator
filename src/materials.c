#include "materials.h"

#define BLOCK_AMOUNT 10

static material void_material = {1.0, 1.0, 0.0, 1.0};
static material custom_materials[BLOCK_AMOUNT];

void set_void_material(double permittivity, double permeability) {
  void_material.permittivity = permittivity;
  void_material.permeability = permeability;
  void_material.speed_squared = 1/(permittivity * permeability);
}


material* get_void_material() {
  return &void_material;
} 


void add_custom_material(double permittivity, double permeability, double conductivity, int index) {
  custom_materials[index] = (material) {
    permittivity,
    permeability,
    conductivity,
    1/(permittivity*permeability)
  };
}

material* get_custom_material(int key) {
  return custom_materials + key;
}

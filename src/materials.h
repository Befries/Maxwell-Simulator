
typedef struct {
  double permittivity;
  double permeability;
  double conductivity;
  double speed_squared;
} material;

material* get_custom_material(int key);
material* get_void_material();
void set_void_material(double permittivity, double permeability);
void add_custom_material(double permittivity, double permeability, double conductivity);

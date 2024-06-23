
typedef struct {
  double permittivity;
  double permeability;
  double conductivity;
  double speed_squared;
} material;

material* getCustomMaterial(int key);
material* getVoidMaterial();
void addCustomMaterial(double permittivity, double permeability, double conductivity);

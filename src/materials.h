
typedef struct {
  double permittivity;
  double permeability;
  double conductivity;
} material;

material* getCustomMaterial(int key);
material* getVoidMaterial();
void addCustomMaterial(double permittivity, double permeability, double conductivity);

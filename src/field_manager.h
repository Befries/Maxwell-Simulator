typedef struct {
  char* ID;
  int x0;
  int y0;
  int width;
  int height;
  double permittivity;
  double permeability;
  double conductivity;
} block;


void init_field_manager(int _rows_amount, int _cols_amount);
void init_fields(int _rows_amount, int _cols_amount);
void init_properties_array();
void refresh_properties();
void add_block(char* ID, int x0, int y0, int width, int height, double permittivity, double permeability, double conductivity); 
void update_fields();

int get_points_amount();
int get_cols_amount();
double* get_view_port();
double get_dt();
int get_blocks_amount();
block get_block(int index);

void clear_fields();
void mark_dirty();
void free_fields(); 
void free_blocks();
int find_block_index(char* ID);
void free_block(int index);

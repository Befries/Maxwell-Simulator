
void init_field_manager(int rows_amount, int cols_amount);
void update_fields();

int get_points_amount();
int get_cols_amount();
double* get_view_port();
double get_dt();

void clear_fields();
void mark_dirty();
void free_fields(); 

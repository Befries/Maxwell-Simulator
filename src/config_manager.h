#define LINE_LEN 40
#define MAX_KEY_LEN 20
#define MAX_DATA_LEN 20

typedef struct {
  char* key;
  char* data;
} config_info;


void read_config(char* config_path);
config_info parse_line(char* line);
void print_config(config_info* info_set);
void free_config_reader(config_info* info_set);

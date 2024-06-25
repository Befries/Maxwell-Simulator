#define LINE_LEN 40
#define MAX_KEY_LEN 20
#define MAX_DATA_LEN 20

typedef struct {
  char* key;
  char* data;
  int isValidKey;
} config_info;


void read_config(char* config_path);
config_info parse_line(char* line);
void print_config(config_info* info_set);
void init_config();
void free_config_reader(config_info* info_set);
void set_entries(config_info* info_set);

int scan_key(config_info info);

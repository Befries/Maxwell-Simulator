#include "config_manager.h"
#include "field_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// default config_info
int width = 50, height = 50;

// special config_info
config_info faulty_line = {"faulty_line", "\0"};
config_info end_of_config = {"end_of_config", "\0"};


void read_config(char* config_path) {

  FILE* config;
  config_info info_set[20];

  config = fopen(config_path, "r");
  char line[LINE_LEN];
  int info_amount = 0;

  while (fgets(line, LINE_LEN, config) != NULL) {
   
    config_info info = parse_line(line);
    if (strcmp(info.key, "faulty_line") == 0) continue;

    info_set[info_amount] = info; 
    info_amount++;
  }

  fclose(config);
  info_set[info_amount] = end_of_config;

  set_entries(info_set);
  print_config(info_set);
  free_config_reader(info_set);

  init_config(); 
}



void print_config(config_info* info_set) {
  int i = 0;
  while (strcmp(info_set[i].key, "end_of_config") != 0) {
    printf("key: \"%s\", data:\"%s\"", info_set[i].key, info_set[i].data);
    switch(info_set[i].isValidKey) {
      case 1: printf(", valid entry\n");
      break;
      case -1: printf(", key is not valid\n");
      break;
      default: printf(", error occured for its treatment");
    }
    i++;
  } 
}



config_info parse_line(char* line) {

  char key_buffer[MAX_KEY_LEN];
  char data_buffer[MAX_DATA_LEN];
  int key_length = 0;

  for (; key_length <= MAX_KEY_LEN; key_length++) {
    if (line[key_length] == '\n') return faulty_line;
    if (line[key_length] != '=') key_buffer[key_length] = line[key_length];
    else break;
  }
  key_buffer[key_length] = '\0';

  int data_length = 0;
  for (; data_length < MAX_DATA_LEN; data_length++) {
    if (line[key_length + data_length + 1] == '\n') {
      if (data_length == 0) return faulty_line;
      break; 
    }
    data_buffer[data_length] = line[key_length + data_length + 1];
  }
  data_buffer[data_length] = '\0';
  
  char* key = malloc(key_length);
  char* data = malloc(data_length);
  strcpy(key, key_buffer); strcpy(data, data_buffer);
  return (config_info){key, data};
}


void set_entries(config_info *info_set) {
  int i = 0;
  while (strcmp(info_set[i].key, "end_of_config") != 0) {
    info_set[i].isValidKey = scan_key(info_set[i]);
    i++;
  }
}


void init_config() {
  init_field_manager(height, width);
}


void free_config_reader(config_info* info_set) {
  int i = 0;
  while (strcmp(info_set[i].key, "end_of_config") != 0) {
    free(info_set[i].key);
    free(info_set[i].data);
    i++;
  }
}


int scan_key(config_info info) {
  if (strcmp(info.key, "width") == 0) {
    width = atoi(info.data);
    return 1;
  } else if (strcmp(info.key, "height") == 0) {
    height = atoi(info.data);
    return 1;
  }
  // no match found
  return -1;
}

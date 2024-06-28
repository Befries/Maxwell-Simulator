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
config_info end_object = {"end_object", "\0"};


void read_config(char* config_path) {

  FILE* config;
  config_info* info_set = malloc(sizeof(config_info)*50);

  config = fopen(config_path, "r");
  char line[LINE_LEN];
  int info_amount = 0;

  while (fgets(line, LINE_LEN, config) != NULL) {
   
    config_info info = parse_line(line);
    if (strcmp(info.key, "faulty_line") == 0) continue;
    
    // info_set allocates 50 per 50 blocks of memory;
     
    info_set[info_amount] = info; 
    info_amount++;
    if (info_amount % 50 == 0) info_set = realloc(info_set, sizeof(config_info)* (info_amount + 50));
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

  // if spaces at the beginning, they are ignored
  int pseudo_key_length = 0;
  int spaces_amount = 0;
  
  if (line[0] == '}') return end_object; 

  for (; pseudo_key_length <= MAX_KEY_LEN; pseudo_key_length++) {
    if (line[pseudo_key_length] == '\n') return faulty_line;
    if (line[pseudo_key_length] == ' ') spaces_amount++;
    if (line[pseudo_key_length] != '=') key_buffer[pseudo_key_length - spaces_amount] = line[pseudo_key_length];
    else break;
  }
  key_buffer[pseudo_key_length - spaces_amount] = '\0';

  if (line[pseudo_key_length + 1] == '{') {
    char* key = malloc(pseudo_key_length - spaces_amount);
    strcpy(key, key_buffer);
    return (config_info) {"begin_object", key};
  }

  int data_length = 0;
  for (; data_length < MAX_DATA_LEN; data_length++) {
    if (line[pseudo_key_length + data_length + 1] == '\n' || line[pseudo_key_length + data_length + 1] == ',') {
      if (data_length == 0) return faulty_line;
      break; 
    } 
    data_buffer[data_length] = line[pseudo_key_length + data_length + 1];
  }
  data_buffer[data_length] = '\0';
  
  char* key = malloc(pseudo_key_length - spaces_amount);
  char* data = malloc(data_length);
  strcpy(key, key_buffer); strcpy(data, data_buffer);
  return (config_info){key, data};
}


void set_entries(config_info *info_set) {
  int i = 0;
  while (strcmp(info_set[i].key, "end_of_config") != 0) {
    if (strcmp(info_set[i].key, "begin_object") == 0) {
      info_set[i].isValidKey = 1;
      i = handle_object(info_set, i);
      info_set[i].isValidKey = 1;
    } else {
      info_set[i].isValidKey = scan_key(info_set[i]);
    }
    i++;
  }
}



void init_config() {
  init_field_manager(height, width);
}


void free_config_info(config_info info) {
  free(info.key);
  free(info.data);
}


void free_config_reader(config_info* info_set) {
  int i = 0;
  while (strcmp(info_set[i].key, "end_of_config") != 0) {
    if (strcmp(info_set[i].key, "begin_object") == 0) {
      free(info_set[i].data); i++;

      while (strcmp(info_set[i].key, "end_object") != 0) {
        free_config_info(info_set[i]);
        i++;
      }
    } else {
      free_config_info(info_set[i]);
    }
    i++;
  }
  free(info_set);
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

struct object_fetcher {
  int x0; int y0; int width; int height;
  double permittivity;   
  double permeability; 
  double conductivity;
};


int scan_object_property(struct object_fetcher* fetcher, config_info info) {
  if (strcmp(info.key, "x0") == 0) {
    fetcher->x0 = atoi(info.data);
    return 1;
  } else if (strcmp(info.key, "y0") == 0) {
    fetcher->y0 = atoi(info.data);
    return 1;
  } else if (strcmp(info.key, "width") == 0) {
    fetcher->width = atoi(info.data);
    return 1;
  } else if (strcmp(info.key, "height") == 0) {
    fetcher->height = atoi(info.data);
    return 1;
  } else if (strcmp(info.key, "permittivity") == 0) {
    fetcher->permittivity = atof(info.data);
    return 1;
  } else if (strcmp(info.key, "permeability") == 0) {
    fetcher->permeability = atof(info.data);
    return 1;
  } else if (strcmp(info.key, "conductivity") == 0) {
    fetcher->conductivity = atof(info.data);
    return 1;
  }
  return 0;
}


int handle_object(config_info* info_set, int i) {
  char* ID = info_set[i++].data;

  struct object_fetcher fetcher = {0, 0, 0, 0, 0, 0, 0};
  while(strcmp(info_set[i].key, end_object.key) != 0) {
    info_set[i].isValidKey = scan_object_property(&fetcher, info_set[i]);
    i++;
  } 

  add_block(
    ID,
    fetcher.x0,
    fetcher.y0,
    fetcher.width,
    fetcher.height,
    fetcher.permittivity,
    fetcher.permeability,
    fetcher.conductivity
  );

  return i;
}

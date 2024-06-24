
LIB_FLAGS=`pkg-config --cflags gtk4`
LIB_REF=`pkg-config --libs gtk4`

OBJ_DIR=build/obj
SRC_DIR=src
OBJECT_FILES=canvas_manager.o field_manager.o materials.o utils.o config_manager.o
HEADER_FILES=$(SRC_DIR)/*.h

default: main

main: ./src/main.c $(OBJECT_FILES) $(HEADER_FILES)
	cc -Wall $(LIB_FLAGS) src/main.c $(OBJ_DIR)/*.o $(HEADER_FILES) -o $@ $(LIB_REF) 


%.o: src/%.c
	cc -Wall $(LIB_FLAGS) -c $< -o $(OBJ_DIR)/$@ $(LIB_REF)  

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm main

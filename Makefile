TARGET_EXEC = Game 
CC = clang++

DEBUG = Release

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin
ENGINE_DIR = lib/engine/
RENDERER_DIR = $(ENGINE_DIR)/lib/renderer/
MAIN_LIBS = $(RENDERER_DIR)/lib/

LIBS = $(ENGINE_DIR)/bin/libEngine.a $(RENDERER_DIR)/bin/libRenderer.a $(MAIN_LIBS)/bgfx/.build/linux64_gcc/bin/libbgfx$(DEBUG).a $(MAIN_LIBS)/bgfx/.build/linux64_gcc/bin/libbx$(DEBUG).a $(MAIN_LIBS)/bgfx/.build/linux64_gcc/bin/libbimg$(DEBUG).a $(MAIN_LIBS)/bgfx/.build/linux64_gcc/bin/libbimg_decode$(DEBUG).a $(MAIN_LIBS)/glfw/build/src/libglfw3.a -lGL -lX11 -lpthread -ldl -lrt
SHADERS_DIR = resources/shaders

INC_DIR_SRC = -Isrc 
INC_DIR_LIB = -I$(ENGINE_DIR)include/engine -I$(ENGINE_DIR)include -I$(RENDERER_DIR)include -I$(RENDERER_DIR)include/renderer -I$(MAIN_LIBS)/bgfx/include -I$(MAIN_LIBS)/bimg/include -I$(MAIN_LIBS)/bx/include -I$(MAIN_LIBS)/glfw/include -I$(MAIN_LIBS)/glm -I$(MAIN_LIBS)

DEBUGFLAGS = $(INC_DIR_SRC) $(INC_DIR_LIB) -Wall -g -DDEBUG=1 -DBX_CONFIG_DEBUG=1
RELEASEFLAGS = $(INC_DIR_SRC) $(INC_DIR_LIB) -O2 -DDEBUG=1 -DBX_CONFIG_DEBUG=0
ASMFLAGS = $(INC_DIR_SRC) $(INC_DIR_LIBS) -Wall
LDFLAGS = $(LIBS) -lm -fuse-ld=mold 

.PHONY: all libs clean test

all: clean dirs shaders 
	$(MAKE) -j16 bld
	$(MAKE) link

dirs:
	mkdir -p ./$(BIN)

link: $(OBJ)
	$(CC) -o $(BIN)/$(TARGET_EXEC) $^ $(LDFLAGS)

bld: 
	$(MAKE) obj

obj: $(OBJ)

%.o: %.cpp
	$(CC) -std=c++20 -o $@ -c $^ $(DEBUGFLAGS)

build: dirs link

run:
	./$(BIN)/$(TARGET_EXEC) 

engine:
	make -C $(ENGINE_DIR)

renderer:
	make -C $(RENDERER_DIR)

headers:
	make -C $(ENGINE_DIR) headers
	make -C $(RENDERER_DIR) headers

shaders:
	make -C $(SHADERS_DIR) 

clean:
	make -C $(SHADERS_DIR) clean
	rm -rf $(BIN) $(OBJ)
	clear


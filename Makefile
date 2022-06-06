CXX = g++
LANG_STD = -std=c++20
CFLAGS = -g -O2 -Os
DEFINE = -D DEBUG
LFLAGS = -lSDL2 -lSDL2_image
SRC_FILES = src/*.cpp \
			src/Game/*.cpp \
			src/Logger/*.cpp \
			src/ECS/*.cpp \
			src/AssetsStore/*.cpp \
			src/Utils/*.cpp \
			src/Map/*.cpp \
			thirdparty/imgui/*.cpp

INCLUDE = -I"./thirdparty/"
OBJ_NAME = engine

build:
	$(CXX) $(LANG_STD) $(DEFINE) $(CFLAGS) $(INCLUDE) $(SRC_FILES) $(LFLAGS) -o $(OBJ_NAME)
 
run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
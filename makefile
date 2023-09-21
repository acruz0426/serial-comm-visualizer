
CC = g++
CFLAGS = -Wall -g3

SRC_DIR = src
BUILD_DIR = build
SDL = `sdl2-config --cflags --libs` -lSDL2_ttf

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
EXECUTABLE = serial-comm-visualizer
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/obj/%.o,$(SOURCES))

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL)

$(BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# g++ -o main ${SRC}/main.c `sdl2-config --cflags --libs`

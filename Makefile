CXX = g++
CXXFLAGS = -Wall -Iinclude -std=c++17
LIBS = -lX11 -lXext -lXrender

SRC = $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))

TARGET = build/test

# Ensure build directory exists
$(shell mkdir -p build)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Ensure object files are compiled correctly
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $(LIBS)

# Create the build directory if missing
build:
	mkdir -p build

clean:
	rm -rf build
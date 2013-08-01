FLAGS = -O2 -lSDL -lSDL -lSDL_ttf -lgfx

all: radar

radar: $(wildcard src/*.cpp) $(wildcard src/*.hpp)
	g++ $(wildcard src/*.cpp) $(FLAGS) -o bin/radar

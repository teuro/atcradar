FLAGS = -s -O3 -lSDL -lSDL_ttf -lSDL_gfx -Wall -pedantic

all: radar

radar: $(wildcard src/*.cpp) $(wildcard src/*.hpp)
	g++ $(wildcard src/*.cpp) $(FLAGS) -o ohjelma/radar

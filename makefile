FLAGS = -s -O2 -lSDL -lSDL_ttf -lSDL_gfx -lSDL2_image -Wall -pedantic

all: radar

radar: $(wildcard src/*.cpp) $(wildcard src/*.hpp)
	g++ $(wildcard src/*.cpp) $(FLAGS) -o ohjelma/radar

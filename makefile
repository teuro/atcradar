FLAGS = -O2 -lSDL -lSDL_ttf -lSDL_gfx

all: radar

radar: $(wildcard src/*.cpp) $(wildcard src/*.hpp)
	g++ $(wildcard src/*.cpp) $(FLAGS) -o ohjelma/radar

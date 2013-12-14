FLAGS = -s -O2 -lmingw32 -lSDLmain -lSDL -lSDL_ttf -lSDL_gfx -lSDL_image -Wall -pedantic

all: radar

radar: $(wildcard src/*.cpp) $(wildcard src/*.hpp)
	g++ $(wildcard src/*.cpp) $(FLAGS) -o ohjelma/radar.exe

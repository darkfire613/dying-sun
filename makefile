//makefile code sourced from:
//http://www.willusher.io/sdl2%20tutorials/2013/08/15/lesson-0-mac-command-line/

CXX = clang++
SDL = -framework SDL2
CXXFLAGS = -Wall -c -std=c++11
LDFLAGS = $(SDL)
EXE = SDL_Lesson0

all: $(EXE)

$(EXE): main.o
	$(CXX) $(LDFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)

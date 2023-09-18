CXX = g++

CXXFLAGS = -std=c++17 -F /Library/Frameworks -I /opt/homebrew/Cellar/sdl2/2.28.3/include -I /opt/homebrew/Cellar/sdl2_mixer/2.6.3_1/include -I /opt/homebrew/Cellar/sdl2/2.28.3/include/SDL2 -I /opt/homebrew/Cellar/sdl2_mixer/2.6.3_1/include/SDL2

LDFLAGS = -v -F /Library/Frameworks -framework SDL2 -L /opt/homebrew/Cellar/sdl2_mixer/2.6.3_1/lib -lSDL2_mixer

APP = loss_mon

all: $(APP)

$(APP): $(APP).o
	$(CXX) $(LDFLAGS) $(APP).o -o $(APP)

obj/$(APP).o : $(APP).cpp
	$(CXX) $(CXXFLAGS) -c $(APP).cpp -o $(APP).o -arch arm64

clean:
	rm $(APP).o $(APP)


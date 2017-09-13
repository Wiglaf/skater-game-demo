CXX = clang++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_ttf -lSDL2_image

OBJS = \
	subjectsprite.o \
	smartSprite.o \
	chunk.o \
	explodingSprite.o \
	collisionStrategy.o \
	bullet.o \
	bulletPool.o \
	shootingSprite.o \
 	renderContext.o \
	ioMod.o \
	parseXML.o \
	gamedata.o \
	viewport.o \
	world.o \
	unpack.o \
	frame.o \
	frameFactory.o \
	frameGenerator.o \
   	player.o \
	sprite.o \
	multisprite.o \
	twoWaySprite.o \
	scaledSprite.o \
	vector2f.o \
	clock.o \
	HUD.o \
	engine.o \
	main.o
EXEC = run

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
	rm -rf frames/*.

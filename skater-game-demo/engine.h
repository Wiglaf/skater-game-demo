#include <vector>
#include <SDL.h>
#include "ioMod.h"
//#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "HUD.h"
#include "ShootingSprite.h"
#include "smartSprite.h"

class CollisionStrategy;


class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  void checkForCollisions();
  void resetPosition(Drawable* item);
  void explodePlayer();
  void checkBulletCollisions();
  void eliminateAI();
  bool getGodModeStatus() {return godMode;}

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  SDL_Renderer * const renderer;
  HUD hud;

  World gradientSky;   //background
  World clouds;        //background parallax
  World cityscape;     //foreground parallax
  World brickRoofPath; //foreground path
  Viewport& viewport;
  CollisionStrategy* strategy;
  int collisions;
  bool godMode;
  bool exploded;
  bool shotsFired;
  unsigned int respawnDelay;


  ShootingSprite playerOne; //keyboard controlled sprite
  Sprite aiSkater;          //Ai Skater to be interacted with 

  std::vector<Drawable*> sprites;
  std::vector<Drawable*> playerSprites;    //player staged animations
  std::vector<Drawable*> paintedSprites;   //to be sorted & painted at depths
  std::vector<Drawable*> aiSprites;      //sprites to be interacted with as AI

  BulletPool bullets;
  int currentSprite;
  bool makeVideo;

  //////
  void draw() const;
  void update(Uint32);

  //explicitly disallow w/ '=delete'
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <random>
#include <algorithm>
#include "sprite.h"
#include "multisprite.h"
#include "twoWaySprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "scaledSprite.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"
#include "sound.h"



class SpriteLess 
{
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const 
  {
    return lhs->getScale() < rhs->getScale();
  }
};

/*DESTRUCTOR*/
Engine::~Engine() 
{ 
  std::cout << "Terminating program" << std::endl;
  for (auto&& items : sprites) delete items;
  // for (auto&& items : playerSprites) delete items;
  // for (auto&& items : paintedSprites) delete items;

  delete strategy;
}

/*CONSTRUCTOR*/
Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  hud(),
  //ADD TEXTURES 
  gradientSky("gradientSky", Gamedata::getInstance().getXmlInt("gradientSky/factor") ),
  clouds("clouds", Gamedata::getInstance().getXmlInt("clouds/factor") ),
  cityscape("cityscape", Gamedata::getInstance().getXmlInt("cityscape/factor") ),
  brickRoofPath("brickRoofPath", Gamedata::getInstance().getXmlInt("brickRoofPath/factor") ),
  viewport( Viewport::getInstance() ),
  strategy(new RectangularCollisionStrategy),
  collisions(),
  godMode(false),
  exploded(false),
  shotsFired(false),
  respawnDelay(0),

  //ADD PLAYER & SPRITE VECTORS
  playerOne("pushSkater"),
  aiSkater("aiSkater"),
  sprites(),
  playerSprites(),
  paintedSprites(),
  aiSprites(),
  bullets(playerOne.getBullets()),
  currentSprite(-1),


  makeVideo( false )
{

  constexpr float u = 1.0f; //Mean Size
  constexpr float d = 0.5f; //STD Deviation

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);

  //ADD AI SPRITES
  aiSprites.push_back(new twoWaySprite("aiSkater") );

  //ADD NPC SPRITES   
  sprites.push_back( new Sprite("kingKong") );
  sprites.push_back( new Sprite("movingCloud") );

  //ADD PAINTERS ALGORITHM SPRITES TO VECTOR
   unsigned int n = Gamedata::getInstance().getXmlInt("numberOfPaintedSprites");
   for ( unsigned int i = 0; i < n; ++i ) 
   {
    auto* s = new ScaledSprite("flyingPig");
    //auto* ss = new ScaledSprite("flyingPig");

    float scale = dist(mt);

    while(scale < 0.001f) { scale = dist(mt); }

    s->setScale(scale);
    //ss->setScale(scale);

    paintedSprites.push_back(s);
    //paintedSprites.push_back(ss);
   }

  std::vector<Drawable*>::iterator ptr =  paintedSprites.begin();
  ++ptr;
  sort(ptr, paintedSprites.end(), SpriteLess());

  //ADD PLAYER SPRITES
  playerSprites.push_back(&playerOne);

  //set initial sprite to track
  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const 
{
  //draw textures
  gradientSky.draw();
  clouds.draw();

  //draw sprites in front of sky
  for (unsigned int i = 0; i < paintedSprites.size()/2; i++)
  {
    paintedSprites[i]->draw();
  }
  
  cityscape.draw();

  //draw NPC king kong
  sprites[0]->draw();

  //draw sprites in front of cityscape
  for (unsigned int i = paintedSprites.size()/2; i < paintedSprites.size(); i++)
  {
    paintedSprites[i]->draw();
  }

  brickRoofPath.draw();

  //draw moving cloud sprite 
  sprites[1]->draw();

  //draw player 
  //playerOne.draw();
  playerSprites[0]->draw();
  aiSprites[0]->draw();
  

  //draw hud if status = true 
  if (hud.getStatus())
  {
    hud.draw();
    viewport.draw();
  }

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) 
{
  //update NPC sprites
  for(auto* s : sprites) s->update(ticks);
  //update painted sprites
  for(auto* ps : paintedSprites) ps->update(ticks);
  for(auto* as : aiSprites) as->update(ticks);

  //update textures 
  gradientSky.update();
  clouds.update();
  cityscape.update();
  brickRoofPath.update();
  // always update viewport last
  viewport.update(); 
}

void Engine::switchSprite()
{
  //currently only tracks playerOne
  currentSprite = ++currentSprite % sprites.size();
  //Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
  Viewport::getInstance().setObjectToTrack(playerSprites[0]);
}

//sets player explosion animation into motion
void Engine::explodePlayer()
{
  Sprite s("pushSkater");
  s.setFrame(playerSprites[0]->getFrame());
  s.setPosition(playerSprites[0]->getPosition());
  Drawable* boom = new ExplodingSprite(s);
  //delete playerSprites[0]; //causes segfault?
  playerSprites[0] = boom;
  exploded = true;
  respawnDelay = SDL_GetTicks() + 2000;
}


//explode AI player 
void Engine::eliminateAI()
{

  //const Sprite s(**ait,(*ait)->getFrame());
  //Drawable* boom = new ExplodingSprite(s);
  //temp sprite
  Sprite s("aiSkater");
  s.setFrame(aiSprites[0]->getFrame());
  s.setPosition(aiSprites[0]->getPosition());

  Drawable* boom = new ExplodingSprite(s);
  delete aiSprites[0]; 
  aiSprites[0] = boom;

  std::cout << "here";
}

//if collision with specific sprites
//explode player and reset player position
void Engine::checkForCollisions() 
{

  //player to check for collisions with
  Drawable* player = playerSprites[0]; 
  
  //sprites to check for collision with 
  std::vector<Drawable*>::const_iterator it = paintedSprites.begin();

  //maintain collisions w/ front-most sprites only
  //sets iterator to start with front-most sprites in vector 
  unsigned int i = paintedSprites.size()/2;
  for (unsigned int j= 0; j != i; j++)
    it++;
  
  //check for player collision with flying pigs
  while ( it != paintedSprites.end() ) 
  {
    if ( strategy->execute(*player, **it) ) 
    {
      explodePlayer();
      //std::cout << "collision: " << ++collisions << std::endl;
      ++collisions;
    }
    it++;
  } 
}

void Engine::checkBulletCollisions()
{
 
  //Doesn't work...
  //std::list<Bullet>::const_iterator it = bullets.getBulletList().begin();
  //check for bullet collisions with AI
  // while (it != bullets.getBulletList().end())
  // {
  //   if ( strategy->execute(*ai, *it))
  //   {
  //     resetPosition(aiSprites[0]);
  //     std::cout<< "got here";
  //   }
  //   it++;
  // }

  //check for bullet collisions with AI
  // Drawable* ai = aiSprites[0];
  // if (static_cast<ShootingSprite*>(playerSprites[0])->collidedWith(ai))
  // {
  //   resetPosition(aiSprites[0]);
  //   std::cout << "here";
  // }
  // //////////////////////////////////////////
  // //check for bullet collisions with flying pigs
  // std::vector<Drawable*>::iterator it = paintedSprites.begin();
  // //maintain collisions w/ front-most sprites only
  // //sets iterator to start with front-most sprites in vector 
  // unsigned int i = paintedSprites.size()/2;
  // for (unsigned int j= 0; j != i; j++) {it++;}

  // while ( it != paintedSprites.end() ) 
  // {
  //   if (static_cast<ShootingSprite*>(playerSprites[0])->collidedWith(*it))
  //     resetPosition(*it);
  // }

  //take 3...still doesn't work right...
  //std::vector<Drawable*>::iterator ait = aiSprites.begin();

  //while (ait != aiSprites.end())
  //{
  // if (playerOne.collidedWith(*ait))
    if (playerOne.collidedWith(aiSprites[0]))
    {
      std::cout << "here";
      eliminateAI();
    }
    //ait++;
  //}

  shotsFired = false;
  //////////////////////////////////////////
}


//reset object position
void Engine::resetPosition(Drawable* item)
{
  item->setPosition(Vector2f(Gamedata::getInstance().getXmlInt(item->getName()+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(item->getName()+"/startLoc/y")));
}

//EVENT LOOP
void Engine::play() 
{
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;

  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) 
  {
    while ( SDL_PollEvent(&event) ) 
    {
      keystate = SDL_GetKeyboardState(NULL);

      if (event.type ==  SDL_QUIT) { done = true; break; }

      if(event.type == SDL_KEYDOWN) 
      {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) 
        {
          done = true;
          break;
        }

        //toggle Pause
        if ( keystate[SDL_SCANCODE_P] ) 
        {

          //std::cout << "P pressed";
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }

        //toggle HUD
        if (keystate[SDL_SCANCODE_F1])
        {
          if (hud.getStatus())
          {
            hud.draw();
          }

          hud.setStatus();
        }

        //toggle Slo-Mo
        if ( keystate[SDL_SCANCODE_F2] ) 
        {
          clock.toggleSloMo();
        }

        //Switch Sprite to Track
        if ( keystate[SDL_SCANCODE_T] ) 
        {
          switchSprite();
        }

        //Toggle Player GodMode
        if ( keystate[SDL_SCANCODE_G] ) 
        {
          if (godMode == true) {godMode = false; hud.setGodMode(false);}

          else {godMode = true; hud.setGodMode(true);}
        }

        //Reset Player Location
        if ( keystate[SDL_SCANCODE_R] ) 
        {
          resetPosition(playerSprites[0]);
        }

        //Initiate Frame Capture
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) 
        {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }

        //Terminate Frame Capture
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) 
        {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    //----within the "While (!Done) Loop"----
    //left & right cancel each other out, no movement
    if (keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D])
    {
      playerOne.stop();
    }

    //if 'A' move left
    else if (keystate[SDL_SCANCODE_A])
    {
      playerOne.left();
      playerOne.update(ticks);
    }

    //if "D" move right 
    else if (keystate[SDL_SCANCODE_D])
    {
      playerOne.right();
      playerOne.update(ticks);
    }

    //up & down cancel each other out, no movement
    if (keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S])
    {
      playerOne.stop();
    }

    //if "W" update player twice as fast for sprint
    else if (keystate[SDL_SCANCODE_W])
    {
      playerOne.update(ticks);
    }

    //if "S" move down
    else if (keystate[SDL_SCANCODE_S])
    {
      //don't do anything for this key press
      //playerOne.down();
      //playerOne.update(ticks);
    }

    //if "spacebar" implement jump
    else if (keystate[SDL_SCANCODE_SPACE])
    {
      if (playerOne.getJumpCoolDown() == true)
      {
        playerOne.fall();
      }

      else 
      {
        playerOne.jump();
      }

      if (!keystate[SDL_SCANCODE_SPACE])  
        playerOne.fall();
        //playerOne.update(ticks);
    } 

    //if "B" shoot projectile 
    else if (keystate[SDL_SCANCODE_B])
    {
      if (!exploded)
      {
        static_cast<ShootingSprite*>(playerSprites[0])->shoot();
        shotsFired= true; //allows bullet collision check
        //playerSprites[0]->update(ticks);
        playerSprites[0]->draw();
      }
    }

    //manually explode player
    else if (keystate[SDL_SCANCODE_E])
    {
      explodePlayer();
    }

    /////////

    ticks = clock.getElapsedTicks();

    //begin explosion animation
    if (exploded == true)
    {
      playerSprites[0]->update(ticks);
      playerSprites[0]->draw();
    }

    //complete explosion animation
    if ((exploded == true) && SDL_GetTicks() > respawnDelay)
    {
      exploded = false;

      //recreate & reset player position
      //delete playerSprites[0];
      //do the same for AI 
      playerSprites[0] = &playerOne;
      resetPosition(playerSprites[0]);
    }

    if ( ticks > 0 ) 
    {
      clock.incrFrame();
      draw();
      update(ticks);

      //Collison Check
      if (shotsFired == true) {checkBulletCollisions();}
      if (godMode == false) {checkForCollisions();}
     

      //create frames if true
      if ( makeVideo ) 
      {
        frameGen.makeFrame();
      }
    }
  }
}

#include <random>
#include <cmath>
#include "scaledSprite.h"
#include "gamedata.h"
#include "renderContext.h"

const float PI = 4.0f*std::atan(1.0f);

using RADRAND_t = decltype(std::bind(std::declval<std::uniform_real_distribution
                  <float> >(),std::declval<std::mt19937>()));
using NORRAND_t = decltype(std::bind(std::declval<std::normal_distribution
                  <float> >(),std::declval<std::mt19937>()));

std::mt19937 getRand()
{
  static std::random_device rd;
  return std::mt19937(rd());
}

RADRAND_t getRadianDist()
{
  return std::bind(std::uniform_real_distribution<float>(0.0f,2.0f*PI),getRand());
}

NORRAND_t getNormalDist(float u, float dev)
{
  return std::bind(std::normal_distribution<float>(u,dev),getRand());
}

/////////////////////////////////////////////////////////////////////
Vector2f ScaledSprite::makeVelocity(int vx, int vy) const 
{
  static auto rad = getRadianDist();
  auto nor = getNormalDist(vx,vy);

  float v_rad = rad();
  float v_mag = nor();

  return v_mag*Vector2f(std::cos(v_rad), std::sin(v_rad));
}

void ScaledSprite::advanceFrame(Uint32 ticks) 
{
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

/*CONSTRUCTOR*/
ScaledSprite::ScaledSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(spreadSprites(), spreadSprites()),
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                        Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  scale(1)
{ }

/*COPY CONSTRUCTOR*/
ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s), 
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  scale(s.scale)
  {}

//creates random start location to spread out sprites across world 
int ScaledSprite::spreadSprites() const
{
  int startLoc = rand()%(500 - 20 + 1) + 20;
  return startLoc;
}

//used in draw()
inline namespace {constexpr float SCALE_EPSILON = 2e-7;}

//draw with scale factor
void ScaledSprite::draw() const 
{
  if (getScale() < SCALE_EPSILON) {return;}

  frames[currentFrame]->draw(getX(), getY(), scale);
}

void ScaledSprite::update(Uint32 ticks) 
{ 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  // float xPos = Gamedata::getInstance().getXmlFloat(this->getName()+"/startLoc/x");
  // float yPos = Gamedata::getInstance().getXmlFloat(this->getName()+"/startLoc/y");

  // //start location
  // Vector2f startPos;
  // startPos[0] = xPos, 
  // startPos[1] = yPos;

   //loops sprites from top->bottom
  // if ( getY() == 450)
  // {
  //   setPosition(startPos);
  // }

  //down motion
  if ( getY() < 0) 
  {
    setVelocityY( fabs( getVelocityY() ) );
  }

  //upward motion
  if ( getY() > worldHeight-frameHeight) 
  {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  //horizontal right motion
  if ( getX() < 0) 
  {
    setVelocityX( fabs( getVelocityX() ) );
  }

  //horizontal left motion
  if ( getX() > worldWidth-frameWidth) 
  {
    setVelocityX( -fabs( getVelocityX() ) );
  }  
}

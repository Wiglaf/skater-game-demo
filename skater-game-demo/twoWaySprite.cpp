#include "twoWaySprite.h"
#include "gamedata.h"
#include "renderContext.h"

void twoWaySprite::advanceFrame(Uint32 ticks) 
{
	timeSinceLastFrame += ticks;

	if (timeSinceLastFrame > frameInterval) 
  {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

/*CONSTRUCTOR*/
twoWaySprite::twoWaySprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  //fill left & right vectors with relevant frames
  framesR( RenderContext::getInstance()->getFrames(name) ),
  framesL( RenderContext::getInstance()->getFrames(name + "L") ),
  //Initalized with R 
  frames(RenderContext::getInstance()->getFrames(name) ), 


  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  //gets frame size values from R sprite- assumes both are equivalent
  frameWidth(framesR[0]->getWidth()),
  frameHeight(framesR[0]->getHeight())
{ }

//overloaded constructor for subject sprite
twoWaySprite::twoWaySprite(const std::string& name,
                          const Vector2f& pos, const Vector2f& vel,
                          const std::vector<Frame*>& fms) :
  Drawable(name, pos, vel), 
  frames(fms),


  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/FrameInterval") ),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(fms[0]->getWidth()),
  frameHeight(fms[0]->getHeight())
{}

/*COPY CONSTRUCTOR*/
twoWaySprite::twoWaySprite(const twoWaySprite& s) :
  Drawable(s), 
  framesR(s.framesR),
  framesL(s.framesL),
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void twoWaySprite::draw() const 
{
  frames[currentFrame]->draw(getX(), getY());
}

//add condition for direction frame switch here?
//Draws L or R frame set depending on value of velocity 
void twoWaySprite::update(Uint32 ticks) 
{ 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) 
  {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) 
  {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) 
  {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) 
  {
    setVelocityX( -fabs( getVelocityX() ) );
  }

  //SWAP SPRITES DEPENDING ON DIRECTION  
  if (getVelocityX() > 0) 
  {
    frames.assign(framesR.begin(), framesR.end()); 
  }

  else if (getVelocityX() < 0)
  {
    frames.assign(framesL.begin(), framesL.end()); 
  }
}


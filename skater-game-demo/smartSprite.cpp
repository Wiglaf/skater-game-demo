#include <cmath>
#include "viewport.h"
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name, 
                         const Vector2f& pos, int w, int h) :
 Sprite(name, pos, 
   Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
            Gamedata::getInstance().getXmlInt(name+"/speedY"))
 ), 
 io(IOmod::getInstance()), 
 playerPos(),
 playerWidth(w),
 playerHeight(h),
 currentMode(NORMAL) 
{ }

void SmartSprite::goLeft()  { 
  if (getX() > 0) setVelocityX( -std::abs(getVelocityX()) ); 
}
void SmartSprite::goRight() { setVelocityX( fabs(getVelocityX()) ); }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) ); }

void SmartSprite::draw() const {
  Sprite::draw();
  // int x = 10+ getX() - Viewport::getInstance().getX();
  // int y = 10+ getY() - Viewport::getInstance().getY();
  // std::stringstream strm;
  // strm << currentMode;
  // io.printMessageBlackAt( strm.str(), x, y); 
}

void SmartSprite::update(Uint32 ticks) {
  Sprite::update(ticks);
  float x= getX()+getFrame()->getWidth()/2;
  float y= getY()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }
}


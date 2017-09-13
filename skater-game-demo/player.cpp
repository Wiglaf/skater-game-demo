#include <cmath>
#include "player.h"

/*CONSTRUCTOR*/
Player::Player(const std::string& name) :
	twoWaySprite(name),
	frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
	minFrameDelay(Gamedata::getInstance().getXmlFloat(name+"/minFrameDelay")),
	frameDelay(0),
	keyPressedX(false),
	keyPressedY(false),
	initialVelocity(getVelocity()),
	slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
	initialHeight(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
	jumpSpeed(Gamedata::getInstance().getXmlInt(name+"/jumpSpeed")),
	fallSpeed(Gamedata::getInstance().getXmlInt(name+"/fallSpeed")),
	jumpCoolDown(false)
	{}

/*COPY CTOR*/
Player::Player(const Player& p) :
	twoWaySprite(p),
	frameInterval(p.frameInterval),
	minFrameDelay(p.minFrameDelay),
	frameDelay(p.frameDelay),
	keyPressedX(p.keyPressedX),
	keyPressedY(p.keyPressedY),
	initialVelocity(p.initialVelocity),
	slowDown(p.slowDown),
	initialHeight(p.initialHeight),
	jumpSpeed(p.jumpSpeed),
	fallSpeed(p.fallSpeed),
	jumpCoolDown(p.jumpCoolDown)
	{}


/*DESTRUCTOR*/ //not created yet


void Player::stop()
{
	//player slowed gradually via slowDown factor (slides...)
	if (!keyPressedX) setVelocityX(slowDown*getVelocityX());
	if (!keyPressedY) setVelocityY(0);
}

void Player::right()
{
	keyPressedX = true;

	if (getX() < worldWidth-frameWidth)
	{
		setVelocityX(-initialVelocity[0]);
	}
}

void Player::left()
{
	keyPressedX = true;

	if (getX() > 0)
	{
		setVelocityX(initialVelocity[0]);
	}
}

void Player::up()
{
	keyPressedY = true;

	if (getY() > 0)
	{
		setVelocityY(-initialVelocity[1]);
	}
}

void Player::down()
{
	keyPressedY = true;

	 if (getY() < worldHeight-frameHeight)
	 {
	 	setVelocityY(initialVelocity[1] + 1);
	 }
}

void Player::jump()
{
	if (getY() <= frameHeight/2) 
	{
    	jumpCoolDown = true;
 	}

  	else if (getY() > frameHeight/2) 
  	{
    	setVelocityY(jumpSpeed);
  	}
}

void Player::fall()
{
	jumpCoolDown = true;

	if (getY() < initialHeight) 
	{
		setVelocityY(fallSpeed);
	}

	if (getY() >= initialHeight) 
	{
		jumpCoolDown = false;
	}
}

void Player::update(Uint32 ticks)
{
	advanceFrame(ticks);

	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;

	setPosition(getPosition() + incr);

	// //Updates Velocity Values to be Checked Below
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
	//if no keys pressed, stop
	//if (!keyPressedX) stop();
	//if (!keyPressedY) stop();


	keyPressedX = false;
	keyPressedY = false;

	stop();
}

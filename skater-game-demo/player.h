#ifndef player__H
#define player__H
#include <string>
#include "twoWaySprite.h"
#include "gamedata.h"

class Player : public twoWaySprite {
	
public:
	Player(const std::string& name);
	Player(const Player& p);
	//~Player();
	void stop();
	void right();
	void left();
	void up();
	void down();
	void jump();
	void fall();
	void update(Uint32 ticks);
	bool getJumpCoolDown() {return jumpCoolDown;}

private:
	//twoWaySprite thePlayer;
	float frameInterval; 
	float minFrameDelay;
	float frameDelay;

	bool keyPressedX;
	bool keyPressedY;

	Vector2f initialVelocity;
	float slowDown;

	int initialHeight;	
	int jumpSpeed;
	int fallSpeed;
	bool jumpCoolDown;

	Player& operator=(const Player&);	
};
#endif
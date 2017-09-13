#include <iostream>
#include "sprite.h"
#include "ioMod.h"

// SmartSprite is an observer of subject SubjectSprite, which
// in this example is a spinning star.

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  void incrSafeDistance() {++safeDistance; }
  void decrSafeDistance() { --safeDistance; }
  float getSafeDistance() { return safeDistance; }
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:	
  enum MODE {NORMAL, EVADE};
  IOmod& io;	
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  float safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};

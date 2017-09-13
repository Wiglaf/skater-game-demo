#ifndef shootingSprite__H
#define shootingSprite__H
#include <string>
#include <iostream>
#include "sprite.h"
#include "player.h"
#include "bulletPool.h"

class ShootingSprite : public Player {
public:
  ShootingSprite(const std::string& n);
  ShootingSprite(const ShootingSprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  virtual bool collidedWith(const Drawable*) const;
  BulletPool getBullets() {return bullets;};

private:
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;
  ShootingSprite& operator=(const ShootingSprite&);
};
#endif
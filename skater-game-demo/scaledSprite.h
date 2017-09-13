#ifndef SCALEDSPRITE__H
#define SCALEDSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ScaledSprite : public Drawable {
public:
  ScaledSprite(const std::string&);
  ScaledSprite(const ScaledSprite&);

  int spreadSprites() const;
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

  inline float getScale()const {return scale;}
  void setScale(float s){scale = s;}

protected:
  const std::vector<Frame *> frames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  double scale;

  Vector2f makeVelocity(int,int) const;
  void advanceFrame(Uint32 ticks);
};
#endif

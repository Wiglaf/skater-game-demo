#ifndef TWOWAYSPRITE__H
#define TWOWAYSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class twoWaySprite : public Drawable 
{
public:
  twoWaySprite(const std::string&);
  twoWaySprite(const twoWaySprite&);
  twoWaySprite(const std::string& name,
               const Vector2f& pos, const Vector2f& vel,
               const std::vector<Frame*>& fms);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Frame* getFrame() const
  {
    return frames[currentFrame];
  }

 //  //return right direction frames
 //  virtual const Frame* getFrameR() const  
 //  { 
 //    return framesR[currentFrame]; 
 //  }
 //  //return left direction frames
 // virtual const Frame* getFrameL() const 
 //  { 
 //    return framesL[currentFrame]; 
 //  }

protected:
  //vector of frames held for each direction
  const std::vector<Frame *> framesR;
  const std::vector<Frame *> framesL;
  std::vector<Frame *> frames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
#endif

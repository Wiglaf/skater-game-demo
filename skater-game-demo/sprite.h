#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const Sprite&);
  Sprite(const std::string& name, 
         const Vector2f& pos, const Vector2f& vel);
  virtual ~Sprite() { } 
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  //added for chunk functionality 
  void setFrame( const Frame* f) {frame = f;}
  virtual void draw() const;
  virtual void update(Uint32 ticks);
private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Sprite*) const;
};
#endif

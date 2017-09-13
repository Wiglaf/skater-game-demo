#include <string>
#include <vector>
#include <map>
#include "frame.h"
#include "gamedata.h"
#include "unpack.h"

class FrameFactory {
private:
friend class RenderContext;

  static FrameFactory* getInstance();
  ~FrameFactory();

  Frame* getFrame(const std::string&);
  std::vector<Frame*> getFrames(const std::string&);
  //added for collision strategy
  SDL_Surface* getSurface(const std::string&);

  static FrameFactory* instance;
  const Gamedata& gdata;
  
  //added for collision strategy
  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, Frame*> frames;

  std::map<std::string, std::vector<SDL_Texture*> > multiTextures;
  std::map<std::string, std::vector<Frame*> > multiFrames;

  // FrameFactory() : 
  //   gdata( Gamedata::getInstance() ), 
  //   textures(),
  //   frames()
  // {}

  FrameFactory() : 
    gdata( Gamedata::getInstance() ), 
    surfaces(),
    textures(),
    frames(),
    multiTextures(),
    multiFrames()
  {}


  /////
  //explicitly disallow w/ '=delete'  
  FrameFactory(const FrameFactory&) = delete;
  FrameFactory& operator=(const FrameFactory&) = delete;
};

#include <SDL_image.h>
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

IOmod& IOmod::getInstance() {
  static IOmod instance;
  return instance;
}

/*DESTRUCTOR*/
IOmod::~IOmod() 
{ 
  TTF_CloseFont(font);
  TTF_Quit(); 
}

/*CONSTRUCTOR*/
IOmod::IOmod() : 
  init(TTF_Init()),
  renderer( RenderContext::getInstance()->getRenderer() ),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size"))),
  textColor({0xff, 0, 0, 0})
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
     throw std::string("error: font not found");
  }
  textColor.r = Gamedata::getInstance().getXmlInt("font/red");
  textColor.g = Gamedata::getInstance().getXmlInt("font/green");
  textColor.b = Gamedata::getInstance().getXmlInt("font/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("font/alpha");
}

SDL_Texture* IOmod::readTexture(const std::string& filename) 
{
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
  if ( texture == NULL ) {
    throw std::string("Couldn't load ") + filename;
  }
  return texture;
}

SDL_Surface* IOmod::readSurface(const std::string& filename) 
{
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if ( !surface ) {
    throw std::string("Couldn't load ") + filename;
  }
  return surface;
}

void IOmod::writeText(const std::string& msg, int x, int y) const 
{
  int textWidth;
  int textHeight;

  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  textWidth = surface->w;
  textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

/*OVERLOADED VERSION- Allows Text Color Change*/
void IOmod::writeText(const std::string& msg, SDL_Color textColor, int x, int y) const 
{
  int textWidth;
  int textHeight;

  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  textWidth = surface->w;
  textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

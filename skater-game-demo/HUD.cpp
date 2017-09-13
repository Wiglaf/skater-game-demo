#include "HUD.h"


/*CONSTRUCTOR*/
HUD::HUD() :
  width(Gamedata::getInstance().getXmlInt("hud/width")),
  height(Gamedata::getInstance().getXmlInt("hud/height")),
  pos(Gamedata::getInstance().getXmlInt("hud/x"), 
      Gamedata::getInstance().getXmlInt("hud/y")),
  io(IOmod::getInstance()),
  viewport(Viewport::getInstance()),
  status(true), //HUD shown at start of game until toggled
  godModeStatus(false), //false until toggled in-game 
  clock(Clock::getInstance())
{ } 

void HUD::draw() const
{
  //create SDL rectangle 
  SDL_Rect r;

  //dimensions of rectangle
  r.x = pos[0];
  r.y = pos[1];
  r.w = width;
  r.h = height;

  // Setup renderer
  SDL_Renderer* renderer = IOmod::getInstance().getRenderer();

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

  // Render rect
  SDL_RenderFillRect( renderer, &r );

  // Set render color to red ( background will be rendered in this color )
  SDL_SetRenderDrawColor( renderer, 208, 209, 210, 255 );
  SDL_RenderDrawRect( renderer, &r );

  // Clear window
  //SDL_RenderClear( renderer );

  //COLOR VALUES
  SDL_Color red = {255,0,0,0}; 
  SDL_Color green = {0,255,0,0}; 
  SDL_Color blue = {0,0,255,0}; 
  SDL_Color black = {0,0,0,0}; 

  std::stringstream strm1, strm2, strm3, strm4, strm5, strm6, strm7, strm8;

  //int TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h)


  strm1 << "fps: " << clock.getFps();
  strm2 << "AVG FPS: " << clock.getAvgFps();
  strm3 << "AD move, hold W sprint";
  strm4 << "B to shoot, hold SPACE jump";
  strm5 << "[R]eset";
  strm6 << "[G]odMode";
  
  io.writeText(strm1.str(), blue, pos[0]+20, pos[1]+50);
  io.writeText(strm2.str(), blue, pos[0]+20, pos[1]+80);
  io.writeText(strm3.str(), black, pos[0]+20, pos[1]+110);
  io.writeText(strm4.str(), black, pos[0]+20, pos[1]+140);
  io.writeText(strm5.str(), black, pos[0]+20, pos[1]+170);

  if (getGodMode() == true)
    io.writeText(strm6.str(), green, pos[0]+150, pos[1]+170);
  if (getGodMode() == false)
     io.writeText(strm6.str(), red, pos[0]+150, pos[1]+170);

  //print game developer name bottom left 
  strm7 << "Clay Moeller";
  io.writeText(strm7.str(), black, pos[0]+20, pos[1]+400);

  //print distance counter and display win when max distance reached
  strm8 << (10000 - viewport.getX()) << " distance left";
  io.writeText(strm8.str(), black, pos[0]+300, pos[1]+400);

  if (viewport.getX() == 10000)
  {
    strm8 << "  YOU WIN!";
    io.writeText(strm8.str(), black, pos[0]+300, pos[1]+400);
  }



  return;
}

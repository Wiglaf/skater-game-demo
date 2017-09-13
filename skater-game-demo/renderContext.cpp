#include "renderContext.h"

/*CONSTRUCTOR*/
RenderContext::RenderContext() : window(nullptr), renderer(nullptr) 
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
	  throw (std::string("Could not init SDL: ") + SDL_GetError());
 	}
  window = initWindow();
  renderer = initRenderer();
}

/*DESTRUCTOR*/
RenderContext::~RenderContext() 
{
  delete FrameFactory::getInstance();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow( window );
  SDL_Quit();
}

/*RETURNS INSTANCE OF RC*/
RenderContext* RenderContext::getInstance() 
{
  if ( instance ) return instance;
  instance = new RenderContext;
  return instance;
}

/*CREATES & INITIALIZES WINDOW*/
SDL_Window* RenderContext::initWindow( ) 
{
	window = SDL_CreateWindow((Gamedata::getInstance().getXmlStr("screenTitle")).c_str(), SDL_WINDOWPOS_CENTERED, 
             SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );

  if( window == NULL ) 
  {
    throw (std::string("Couldn't make a window: ")+SDL_GetError());
  }

  return window;
}

/*INTIALIZES A RENDERER*/
SDL_Renderer* RenderContext::initRenderer() 
{
  // To test the Clock class's ability to cap the frame rate, use:
  // SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
  SDL_Renderer* renderer = 
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  
  if ( renderer == NULL ) throw std::string("No Renderer");
  return renderer;
}

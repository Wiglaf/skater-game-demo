#ifndef HUD__H
#define HUD__H
#include <SDL2/SDL.h>
#include <sstream>
#include "vector2f.h"
#include "ioMod.h"
#include "clock.h"
#include "viewport.h"
#include "rendercontext.h"

class HUD {
public:
	HUD();
	void draw() const;
	bool getStatus() const {return status;} 
	void setStatus() {status ? status = false : status = true;}
	void setGodMode(bool s) {godModeStatus = s;}
	bool getGodMode() const {return godModeStatus;}

private:  
  	unsigned width;
  	unsigned height;
  	Vector2f pos;

  	IOmod& io;
	Viewport& viewport;	

	bool status; //status of HUD
	bool godModeStatus;
	Clock& clock;

};
#endif

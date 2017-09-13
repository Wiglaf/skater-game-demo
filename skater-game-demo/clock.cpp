#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <deque>
#include "clock.h"
#include "gamedata.h"
#include "ioMod.h"


std::deque<int> frameInterval;

Clock& Clock::getInstance() 
{
  static Clock clock; 
  return clock;
}

/*CONSTRUCTOR*/
Clock::Clock() :
  started(false), 
  paused(false), 
  FRAME_CAP_ON(Gamedata::getInstance().getXmlBool("frameCapOn")), 
  PERIOD(Gamedata::getInstance().getXmlInt("period")), 
  frames(0), 
  timeAtStart(0), 
  timeAtPause(0),
  currTicks(0), prevTicks(0), ticks(0) 
  {
  startClock();
}

/*COPY CONSTRUCTOR*/
Clock::Clock(const Clock& c) :
  started(c.started), 
  paused(c.paused), 
  FRAME_CAP_ON(c.FRAME_CAP_ON), 
  PERIOD(c.PERIOD), 
  timeAtStart(c.timeAtStart), timeAtPause(c.timeAtPause),
  currTicks(c.currTicks), prevTicks(c.prevTicks), ticks(c.ticks) 
  {
  startClock();
}

void Clock::toggleSloMo() 
{
  throw( std::string("Slow motion is not implemented yet") );
}

unsigned int Clock::getTicks() const
{ 
  if (paused) return timeAtPause;
  else return SDL_GetTicks() - timeAtStart; 
}

unsigned int Clock::getElapsedTicks() 
{ 
  if (paused) return 0;

  currTicks = getTicks();
  ticks = currTicks-prevTicks;

  if ( FRAME_CAP_ON ) {
    if ( ticks < PERIOD ) return 0;
    prevTicks = currTicks;
    return ticks;
  }
  else {
    prevTicks = currTicks;
    return ticks;
  }
}

int Clock::getFps() const
{ 
  if ( getSeconds() > 0 ) return frames/getSeconds();  

  else if ( getTicks() > 5000 and getFrames() == 0 ) 
  {
    throw std::string("Can't get FPS if you don't increment the frames");
  }
  else return 0;
}

/*Avgs FPS based off of avgFPSInterval*/
int Clock::getAvgFps() const 
{
  unsigned int sum = 0;
  unsigned int max = Gamedata::getInstance().getXmlInt("avgFPSInterval");

  if ( getSeconds() > 0 )
  {
      frameInterval.push_front(getFps());

      if (frameInterval.size() > max)
      {
        frameInterval.pop_back();
      }
        
      for (auto& item : frameInterval)
      {
        sum+= item;
      } 
  }

  else if ( getTicks() > 5000 and getFrames() == 0 ) 
  {
    throw std::string("Can't get FPS if you don't increment the frames");
    return 0;
  }

  return (sum/max);
}

void Clock::incrFrame() 
{ 
  if ( !paused ) {
    ++frames; 
  }
}

void Clock::startClock() 
{ 
  started = true; 
  paused = false; 
  frames = 0;
  timeAtPause = timeAtStart = SDL_GetTicks(); 
  prevTicks = 0;
}

void Clock::pause() 
{
  if( started && !paused ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}

void Clock::unpause() 
{
  if( started && paused ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}


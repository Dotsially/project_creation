#include "timer.h"
#include <iostream>


//replace with custom tick system.
void Timer::Start(f64 seconds){
    if(!started){
        started = true;
        startTime = SDL_GetTicks();  
        timeout = seconds*1000;
    }
}

void Timer::Count(){
    if(started){
        double tempTime = SDL_GetTicks();
        timePassed = tempTime - startTime;
        if(timePassed >= timeout){
            Stop(); 
        }
    }
}

void Timer::Stop(){
    started = false;
    startTime = 0;
    timePassed = 0;
    timeout = 0;
}


u8 Timer::IsStarted(){
    return started;
}
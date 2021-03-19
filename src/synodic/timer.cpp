#include "timer.h"

Timer::Timer() {
	
	Reset();
	
}

void Timer::Reset() {

	tickHold = Clock::now();

}

double Timer::Elapsed() const {

	std::chrono::duration<double, std::milli> ms = Clock::now() - tickHold;
	return ms.count();

}

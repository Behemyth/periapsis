#pragma once

#include <chrono>

class Timer {
	
public:
	
	Timer();
	~Timer() = default;

	void Reset();

	/*
	 *    returns the elapsed time since the last reset in milliseconds.
	 *    @return	The elapsed time in milliseconds.
	 */

	[[nodiscard]] double Elapsed() const;

private:

	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::time_point<Clock> Timestamp;

	Timestamp tickHold;
	
};
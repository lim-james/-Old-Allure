#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer {

	LARGE_INTEGER frequency;
	LARGE_INTEGER begin, previous, current;

	double et, dt;

public:

	Timer();

	void start();
	void update();

	const double& getDeltaTime() const;
	const double& getElapsedTime() const;

	void waitFor(const unsigned& delay);

private:

	double LargeIntToSecs(const LARGE_INTEGER& L) const;

};

#endif
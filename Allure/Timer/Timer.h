#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer {

	LARGE_INTEGER frequency;
	LARGE_INTEGER begin, previous, current;

	double et, dt;

public:

	Timer();

	void Start();
	void Update();

	const double& GetDeltaTime() const;
	const double& GetElapsedTime() const;

	void WaitFor(const unsigned& delay);

private:

	double LargeIntToSecs(const LARGE_INTEGER& L) const;

};

#endif
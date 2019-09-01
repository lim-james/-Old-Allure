#include "Timer.h"

Timer::Timer() {
	QueryPerformanceFrequency(&frequency);
}

void Timer::start() {
	QueryPerformanceCounter(&begin);
	previous = begin;
}

void Timer::update() {
	LARGE_INTEGER elapsed, delta;
	QueryPerformanceCounter(&current);

	elapsed.QuadPart = current.QuadPart - begin.QuadPart;
	delta.QuadPart = current.QuadPart - previous.QuadPart;

	previous = current;

	et = LargeIntToSecs(elapsed);
	dt = LargeIntToSecs(delta);
}

const double& Timer::getDeltaTime() const {
	return dt;
}

const double& Timer::getElapsedTime() const {
	return et;
}

double Timer::LargeIntToSecs(const LARGE_INTEGER& L) const {
	return static_cast<double>(L.QuadPart) / static_cast<double>(frequency.QuadPart);
}

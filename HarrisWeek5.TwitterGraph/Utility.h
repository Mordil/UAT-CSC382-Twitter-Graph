#pragma once

#include <vector>
#include <chrono>

void removeUserFromVector(class TwitterUser*& user, std::vector<class TwitterUser*>& list);

using namespace std;

class Timer
{
	typedef chrono::high_resolution_clock Clock;

public:
	Timer() { }
	~Timer() { }

	void Start() { _start = Clock::now(); }
	chrono::duration<double, milli> Stop() { return Clock::now() - _start; }

private:
	chrono::time_point<chrono::steady_clock> _start;
};

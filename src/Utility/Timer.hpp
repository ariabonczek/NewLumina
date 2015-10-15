//
// A basic static timer class made with chrono
// To use for frame timing, simply call StartFrame and EndFrame at the beginning and end of each frame
// Then, a simple call to GetFrameTime(void) will return your value!
// You can also time custom local points using Start(), End() and GetElapsedTime()
// Aria Bonczek
//

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

typedef std::chrono::system_clock::time_point time_point;

class Timer
{
public:
	// Initializes the static Timer
	static void Initialize(void);

	// Sets the frameStart variable to the time of call
	static void Start(void);
	static void StartFrame(void);

	// Sets the frameEnd variable to the time of call and calculates the elapsed time
	static void Stop(void);
	static void StopFrame(void);

	// Returns the time between frames
	static float GetFrameTime(void);

	// Returns the most recently calculated elapsed time (calculated in Stop(void) function)
	static float GetElapsedTime(void);

	// Returns the total time since the program began
	static float GetTotalTime(void);

	static long long GetTimeSinceEpoch(void);
private:
	static time_point programStart;

	static time_point frameStart;
	static time_point frameEnd;

	static std::chrono::duration<float> frameTime;

	static time_point  localStart;
	static time_point  localEnd;

	static std::chrono::duration<float> elapsedTime;

	static int frameRate;
};

#endif
//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <Core\Common.hpp>

NS_BEGIN

class Renderer;

/// <summary>
/// Schedules tasks and oversees multithread execution and synchronization
/// </summary>
class Scheduler
{
public:
	Scheduler();
	~Scheduler();

	void Initialize(Renderer* renderer);
	void Shutdown();

	void StartThreads();

	void WaitForSync();

private:
	DWORD threadIDArray[1];
	HANDLE threadArray[1];

	Renderer* p_Renderer;
};

NS_END

#endif
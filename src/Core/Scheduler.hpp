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

	/// <summary>
	///
	/// </summary>
	void Initialize(Renderer* renderer);

	/// <summary>
	///
	/// </summary>
	void Shutdown();

	/// <summary>
	///
	/// </summary>
	void StartThreads();

	/// <summary>
	///
	/// </summary>
	void WaitForSync();

private:
#if DX11 || DX12
	DWORD threadIDArray[1];
	HANDLE threadArray[1];
#elif GL43
	pthread_t threadIDs[1];
#endif

	Renderer* p_Renderer;
};

NS_END

#endif
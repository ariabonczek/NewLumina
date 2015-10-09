//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <Core\Common.hpp>

NS_BEGIN

class Renderer;
class WorldManager;

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
	void Initialize(Renderer* renderer, WorldManager* world);

	/// <summary>
	///
	/// </summary>
	void Shutdown();

	/// <summary>
	///
	/// </summary>
	void StartRenderThread();

	/// <summary>
	///
	/// </summary>
	void RunWorkerThreads();

	/// <summary>
	///
	/// </summary>
	void WaitForSync();

private:
#if DX11 || DX12
	SYSTEM_INFO sysInfo;

	DWORD renderThreadID;
	HANDLE renderThread;

	DWORD* workerThreadIDs;
	HANDLE* workerThreads;
#elif GL43
	pthread_t renderThreadID;
	pthread_t threadIDs[3];
#endif

	uint32 numCores;
	uint32 numWorkerThreads;
	Renderer* p_Renderer;
	WorldManager* p_WorldManager;
};

NS_END

#endif
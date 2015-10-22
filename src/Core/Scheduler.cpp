#include <Core\Scheduler.hpp>

#include <Core\Renderer.hpp>
#include <Core\WorldManager.hpp>

NS_BEGIN

Scheduler::Scheduler()
{}

Scheduler::~Scheduler()
{}

void Scheduler::Initialize(Renderer* renderer, WorldManager* world)
{
#if DX11 || DX12
	GetSystemInfo(&sysInfo);
	numCores = sysInfo.dwNumberOfProcessors;
#endif

#if _DEBUG
	Debug::Log("Scheduler Initialized");
	Debug::Log("Number of Cores: " + std::to_string(numCores));
#endif

	p_Renderer = renderer;
	p_WorldManager = world;
	
	//numWorkerThreads = numCores - 2;

	numWorkerThreads = 1;

	workerThreadIDs = new DWORD[numWorkerThreads];
	workerThreads = new HANDLE[numWorkerThreads];
}

void Scheduler::Shutdown()
{
	delete[] workerThreads;
	delete[] workerThreadIDs;
}

void Scheduler::StartRenderThread()
{
#if DX11 || DX12

	renderThread = CreateThread(
		NULL, 0, p_Renderer->FireThread, (void*)p_Renderer, 0, &renderThreadID);
#elif GL43
	pthread_create(renderThreadID, NULL, p_Renderer->FireThread, p_Renderer);
#endif
}

void Scheduler::RunWorkerThreads()
{
	for (uint32 i = 0; i < numWorkerThreads; ++i)
	{
		// TODO: LOCKING NOT DONE YET, HARDCODE WORKER THREADS TO ONE
		workerThreads[i] = CreateThread(NULL, 0, p_WorldManager->GetInstance()->Update, p_WorldManager, 0, &workerThreadIDs[i]);
	}

	WaitForMultipleObjects(numWorkerThreads, workerThreads, TRUE, INFINITE);

	for (uint32 i = 0; i < numWorkerThreads; ++i)
	{
		// TODO: LOCKING NOT DONE YET, HARDCODE WORKER THREADS TO ONE
		workerThreads[i] = CreateThread(NULL, 0, p_WorldManager->GetInstance()->Physics, p_WorldManager, 0, &workerThreadIDs[i]);
	}

	WaitForMultipleObjects(numWorkerThreads, workerThreads, TRUE, INFINITE);

	//p_Renderer->SetupFrame();
	// TODO: THis separation should smart-separate workerThreads into Opaque, Transparent, Particle, and Deferred Geometry
	for (uint32 i = 0; i < numWorkerThreads; ++i)
	{
		// TODO: LOCKING NOT DONE YET, HARDCODE WORKER THREADS TO ONE
		workerThreads[i] = CreateThread(NULL, 0, p_Renderer->GetInstance()->Render, p_Renderer, 0, &workerThreadIDs[i]);
	}

	WaitForMultipleObjects(numWorkerThreads, workerThreads, TRUE, INFINITE);
	p_Renderer->CloseCommandLists();
}

void Scheduler::WaitForSync()
{
#if DX11 || DX12
	WaitForSingleObject(renderThread, INFINITE);
#elif GL43
	pthread_join(threadIDs[0], NULL);
#endif
}

NS_END
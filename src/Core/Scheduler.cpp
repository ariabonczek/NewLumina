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
#if _DEBUG
	Debug::Log("Scheduler Initialized");
#endif
	p_Renderer = renderer;
	p_WorldManager = world;
}

void Scheduler::Shutdown()
{

}

void Scheduler::StartThreads()
{
#if DX11 || DX12
	threadArray[0] = CreateThread(
		NULL, 0, p_Renderer->FireThread, (void*)p_Renderer, 0, &threadIDArray[0]);
	threadArray[1] = CreateThread(
		NULL, 0, p_WorldManager->FireUpdateThread, (void*)p_WorldManager, 0, &threadIDArray[1]);
	threadArray[2] = CreateThread(
		NULL, 0, p_WorldManager->FirePhysicsThread, (void*)p_WorldManager, 0, &threadIDArray[2]);
#elif GL43
	pthread_create(&threadIDs[0], NULL, p_Renderer->FireThread, p_Renderer);
	pthread_create(&threadIDs[1], NULL,p_WorldManager->FireUpdateThread, p_Renderer);
	pthread_create(&threadIDs[2], NULL,p_WorldManager->FirePhysicsThread, p_Renderer);
#endif
}

void Scheduler::WaitForSync()
{
#if DX11 || DX12
	WaitForMultipleObjects(3, threadArray, TRUE, INFINITE);
	CloseHandle(threadArray[0]);
	CloseHandle(threadArray[1]);
	CloseHandle(threadArray[2]);
#elif GL43
	pthread_join(threadIDs[0], NULL);
	pthread_join(threadIDs[1], NULL);
	pthread_join(threadIDs[2], NULL);
#endif
}

NS_END
#include <Core\Scheduler.hpp>

#include <Core\Renderer.hpp>

NS_BEGIN

Scheduler::Scheduler()
{}

Scheduler::~Scheduler()
{}

void Scheduler::Initialize(Renderer* renderer)
{
	p_Renderer = renderer;
}

void Scheduler::Shutdown()
{

}

void Scheduler::StartThreads()
{
#if DX11 || DX12
	threadArray[0] = CreateThread(
		NULL, 0, p_Renderer->ThreadFunc, (void*)p_Renderer, 0, &threadIDArray[0]);
#elif GL43
	pthread_create(&threadIDs[0], NULL, p_Renderer->ThreadFunc, p_Renderer);
#endif
}

void Scheduler::WaitForSync()
{
#if DX11 || DX12
	WaitForMultipleObjects(1, threadArray, TRUE, INFINITE);
	CloseHandle(threadArray[0]);
#elif GL43
	pthread_join(threadIDs[0], NULL);
#endif
}

NS_END
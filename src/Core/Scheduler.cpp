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
	threadArray[0] = CreateThread(
		NULL, 0, p_Renderer->ThreadFunc, (void*)p_Renderer, 0, &threadIDArray[0]);
}

void Scheduler::WaitForSync()
{
	WaitForMultipleObjects(1, threadArray, TRUE, INFINITE);
	CloseHandle(threadArray[0]);
}

NS_END
#include "Allocator.hpp"

uint64 Allocator::memAddress;
uint32 Allocator::memSize;

void Allocator::Initialize(uint32 bytes)
{
	memSize = bytes;
	//memAddress = _mm_malloc(bytes, 64);
}

void Allocator::Shutdown()
{
	//_mm_free(memAddress);
}

void* Allocator::Allocate(uint32 sizeInBytes)
{
	return 0;
}

void Allocator::Release(void* ptr)
{

}
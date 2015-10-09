//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <Core\MemoryRegion.hpp>
#include <Config.hpp>
#include <malloc.h>
#include <stdint.h>

typedef unsigned int uint32;

/// <summary>
/// Handles dynamic memory allocation
/// </summary>
class Allocator
{
public:
	static void Initialize(uint32 bytes);
	static void Shutdown();
	
	static void* Allocate(uint32 sizeInBytes);
	static void Release(void*);
private:
	Allocator();
	~Allocator();

	static uint64 memAddress;
	static uint32 memSize;

	static MemoryRegion regions[MAX_REGIONS];
};


#endif
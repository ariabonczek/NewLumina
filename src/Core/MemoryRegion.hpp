#ifndef MEMORY_REGION_HPP
#define MEMORY_REGION_HPP

typedef unsigned int uint32;
typedef unsigned long long uint64;

class MemoryRegion
{
public:
	MemoryRegion(uint32 size, uint32 align);
	~MemoryRegion();
private:
	uint64 memAddress;
	uint32 memSize;
};


#endif
#include <Core\Common.hpp>
#include <Config.hpp>
#include <Utility\Murmur3\MurmurHash.h>

NS_BEGIN

LGUID Hash(const char* key)
{
	// LGUID is currently a 64 bit integer, but MurmurHash returns a 128 hash code
	// TODO: Write a bigint implementation
	LGUID hash;
	MurmurHash3_x86_32(key, strlen(key), MURMUR3_SEED, &hash);
	return hash;
}

LGUID Hash(long long key)
{
	// LGUID is currently a 64 bit integer, but MurmurHash returns a 128 hash code
	// TODO: Write a bigint implementation
	LGUID hash;
	MurmurHash3_x86_32(&key, sizeof(key), MURMUR3_SEED, &hash);
	return hash;
}

NS_END
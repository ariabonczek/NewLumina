#include <Utility\Resources\Resource.hpp>

NS_BEGIN

Resource::Resource(GUID guid):
guid(guid)
{}

Resource::~Resource()
{

}

GUID Resource::GetGUID()const
{
	return guid;
}

NS_END
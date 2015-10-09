#include <Utility\Resources\Resource.hpp>

NS_BEGIN

Resource::Resource(LGUID guid):
guid(guid)
{}

Resource::~Resource()
{

}

LGUID Resource::GetLGUID()const
{
	return guid;
}

NS_END
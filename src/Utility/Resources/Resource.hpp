#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>

NS_BEGIN

class Resource
{
public:
	Resource(GUID guid);
	virtual ~Resource();

	GUID GetGUID()const;
protected:
	GUID guid;
};

NS_END

#endif
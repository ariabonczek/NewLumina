#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>

NS_BEGIN

class Resource
{
public:
	Resource(LGUID guid);
	virtual ~Resource();

	LGUID GetLGUID()const;
protected:
	LGUID guid;
};

NS_END

#endif
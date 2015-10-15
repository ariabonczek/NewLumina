#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include <Core\Common.hpp>

NS_BEGIN

class PhysicsObject
{
public:

	LGUID GetLGUID()const;
private:
	LGUID guid;
};

NS_END

#endif
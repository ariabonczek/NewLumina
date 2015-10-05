//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef PHYSICS_MANAGER_HPP
#define PHYSICS_MANAGER_HPP

#include <Core\Common.hpp>

NS_BEGIN

/// <summary>
/// Manages physics object encapsulation and simulation
/// </summary>
class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	/// <summary>
	/// Initializes the PhysicsManager
	/// </summary>
	void Initialize();

	/// <summary>
	/// Shuts down the PhysicsManager
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Runs the physics simulation
	/// </summary>
	void Run();
private:

};

NS_END

#endif
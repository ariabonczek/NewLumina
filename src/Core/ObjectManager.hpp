//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include <Core\Common.hpp>

NS_BEGIN

/// <summary>
/// Manages GameObject encapsulation and simulation
/// </summary>
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	/// <summary>
	/// Initializes the ObjectManager
	/// </summary>
	void Initialize();

	/// <summary>
	/// Shuts down the ObjectManager
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Runs the ObjectManager and updates objects
	/// </summary>
	void Run();
private:

};

NS_END

#endif
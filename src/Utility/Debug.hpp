//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <Windows.h>
#include <string>

class Debug
{
public:

	static void Initialize();
	static void Shutdown();

	static void Log(std::string s);
	static void LogError(std::string s);
	static void LogWarning(std::string s);

private:
	static HANDLE consoleHandle;
};

#endif
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

	//template<typename T>
	//static void Log(T val)
	//{
	//	std::string s = std::to_string(val);
	//	s.append("\n");
	//	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	//	WriteConsole(consoleHandle, s.c_str(), strlen(s.c_str()), NULL, NULL);
	//}

	static void Log(std::string s);
	static void LogError(std::string s);
	static void LogWarning(std::string s);

private:
	static HANDLE consoleHandle;
};

#endif
//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Utility\Debug.hpp>

HANDLE Debug::consoleHandle;

void Debug::Initialize()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

#if _DEBUG
	Debug::Log("Debug Initialized");
#endif
}

void Debug::Shutdown()
{
	FreeConsole();
}

void Debug::Log(std::string s)
{
	s.append("\n");
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WriteConsole(consoleHandle, s.c_str(), strlen(s.c_str()), NULL, NULL);
}


void Debug::LogError(std::string s)
{
	s.append("\n");
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WriteConsole(consoleHandle, s.c_str(), strlen(s.c_str()), NULL, NULL);
}

void Debug::LogWarning(std::string s)
{
	s.append("\n");
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	WriteConsole(consoleHandle, s.c_str(), strlen(s.c_str()), NULL, NULL);
}
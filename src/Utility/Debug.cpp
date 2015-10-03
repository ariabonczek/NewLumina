//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#include <Utility\Debug.hpp>

#if _DEBUG

HANDLE Debug::consoleHandle;

void Debug::Initialize()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Debug::Shutdown()
{
	FreeConsole();
}

void Debug::Log(std::string s)
{
	s.append("\n");
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	DWORD written;
	WriteConsole(consoleHandle, s.c_str(), strlen(s.c_str()), &written, NULL);
}

#endif
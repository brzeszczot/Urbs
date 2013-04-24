#include "Engine.hpp"

extern "C" int MAMain()
{
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new Engine());
	return 0;
}

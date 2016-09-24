#include "stdafx.h"

#include "CygtlsStackGuard.h"

// Declare the prototype of the CRT startup function.
// The Visual Studio warning (function definition not found) is intentional.
extern "C" void __stdcall wWinMainCRTStartup();

extern "C" void __stdcall wCygwinCRTStartup()
{
	cygnet::CygtlsStackGuard guard;
	wWinMainCRTStartup();
}
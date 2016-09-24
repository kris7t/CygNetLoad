#include "stdafx.h"

#include "CygtlsStackGuard.h"
#include "InitializeCygwin.h"

namespace cygnet
{
	void InitializeCygwin()
	{
		CygtlsStackGuard::Check();

		auto hmodule = LoadLibrary(_T("C:\\cygwin64\\bin\\cygwin1.dll"));
		if (hmodule == nullptr)
			throw std::runtime_error("Failed to load the Cygwin library.");

		void(*cygwin_dll_init)();
		cygwin_dll_init = reinterpret_cast<void(*)()>(GetProcAddress(hmodule, "cygwin_dll_init"));
		if (cygwin_dll_init == nullptr)
			throw std::runtime_error("Failed to find the Cygwin entry point.");

		cygwin_dll_init();
	}
}
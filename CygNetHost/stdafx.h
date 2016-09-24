// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <winnt.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <memory>
#include <stdexcept>

// TODO: reference additional headers your program requires here

#include <comdef.h>
#include <comutil.h>
#include <metahost.h>

#pragma comment(lib, "mscoree.lib")

//// Import mscorlib.tlb (Microsoft Common Language Runtime Class Library).
//#import "mscorlib.tlb" no_implementation                \
//    high_property_prefixes("_get","_put","_putref")		\
//    rename("ReportEvent", "InteropServices_ReportEvent")


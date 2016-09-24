# CygNetLoad

This is an attempt to get Cygwin dlls load and run under the .NET CLR.

In order to load cygwin1.dll, the stack is shifted by 4K in a custom entry point.
Then, after the Microsoft CRT is initializedi, cygwin1.dll gets loaded and initialized.
It should take care of the stacks of each thread created afterwards, includind CLR threads.

A CLR instance is created through the ICLRMetaHost COM API.
A custom AppDomainManager is set which executes the assembly specified on the command line.

In the executed assembly, it is possible to P/Invoke POSIX function for cygwin1.dll
or functions from other dlls linked with Cygwin.

However, a huge mess is created due to multiple C runtimes being present in the process.
It is impossible to do any I/O or other POSIX calls safely from Cygwin, 
because it *will* cause unexpected crashed when .NET I/O is invoked later.
Even C standard I/O is broken inside the P/Invoked Cygwin dlls.

This defect makes CygNetLoad pretty much pointless, 
since if a library that does not depend on POSIX (or any other side-effectful function),
it can be recompiled statically with MinGW, or even modified to compile under MSVC.
Libraries recompiled this way are much safer targets of P/Invoke that Cygwin ones.

If communication with a POSIX library is desired under .NET,
inter-process RPC communication or message queueing can be a practical alternative,
where the server-side of the RPC call is run under Cygwin while the client is a .NET assembly.

# Useful Links

Reference a GNU C (POSIX) DLL built in GCC against Cygwin, from C#/NET
http://stackoverflow.com/questions/2710465/reference-a-gnu-c-posix-dll-built-in-gcc-against-cygwin-from-c-net

Re: Possible to use pinvoke to call DLL built with cygwin
https://cygwin.com/ml/cygwin/2006-11/msg00030.html

https://github.com/Alexpux/Cygwin/blob/master/winsup/cygwin/how-cygtls-works.txt
https://github.com/Alexpux/Cygwin/blob/master/winsup/testsuite/winsup.api/cygload.h
https://github.com/Alexpux/Cygwin/blob/master/winsup/testsuite/winsup.api/cygload.cc

Re: Question about having msvcrt*.dll and cygwin1.dll in one process
https://cygwin.com/ml/cygwin/2014-11/msg00411.html

http://www.codeproject.com/Articles/416471/CLR-Hosting-Customizing-the-CLR
CLR Hosting - Customizing the CLR

CLR Hosting - A flexible, managed plugin system
http://www.lenholgate.com/blog/2010/07/clr-hosting---a-flexible-managed-plugin-system-part-1.html

Split string containing command-line parameters into string[] in C#
http://stackoverflow.com/questions/298830/split-string-containing-command-line-parameters-into-string-in-c-sharp

Debugging managed code in custom CLR host
http://www.verycomputer.com/8_eb873cb13d531ac0_1.htm

Kernel32!BaseThreadInitThunk - Any Infos?
http://www.rohitab.com/discuss/topic/41193-kernel32basethreadinitthunk-any-infos/

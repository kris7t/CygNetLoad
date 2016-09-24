using System;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;

namespace CygNet.Manager
{
    [Guid("70A3B0BF-1BDE-4E42-9C00-AEC9688CA2B9"), ComVisible(true)]
    public class CygNetAppDomainManager : AppDomainManager, ICygNetManager
    {
        public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
        {
            InitializationFlags = AppDomainManagerInitializationOptions.RegisterWithHost;
            base.InitializeNewDomain(appDomainInfo);
        }

        public int RunCommandLine(string commandLine, string friendlyName)
        {
            Trace.Assert(commandLine != null);
            Trace.Assert(friendlyName != null);

            var splitCommandLine = CommandLineUtils.SplitCommandLine(commandLine);
            if (splitCommandLine.Length == 0)
                throw new ArgumentException("Assembly to be executed must be passed on the command line.");
            var assemblyFile = splitCommandLine[0];
            var args = splitCommandLine.Skip(1).ToArray();

            var appDomain = AppDomain.CreateDomain(friendlyName);
            int status;
            try
            {
                status = appDomain.ExecuteAssembly(assemblyFile, args);

                if (Debugger.IsAttached)
                {
                    // HACK AppDomain unloading may cause Access Violation Exception when debugging
                    // due to the loaded cygwin1.dll.
                    ExitProcess((uint)status);
                }
            }
            finally
            {
                AppDomain.Unload(appDomain);
            }
            return status;
        }

        [DllImport("kernel32.dll")]
        private static extern void ExitProcess(uint uExitCode);
    }
}

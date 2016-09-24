using System.Runtime.InteropServices;

namespace CygNet.Manager
{
    [Guid("69BFE29A-AF29-455F-AB7A-4210F16791EA"), ComVisible(true)]
    public interface ICygNetManager
    {
        int RunCommandLine(string commandLine, string friendlyName);
    }
}
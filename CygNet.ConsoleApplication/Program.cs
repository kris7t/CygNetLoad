using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading;

namespace CygNet.ConsoleApplication
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            var name = args.Length > 0 ? args[0] : "World";
            SayHello(name);
            /*using (var reader = new StreamReader("hello.txt"))
            {
                Console.WriteLine(reader.ReadLine());
            }*/
            Console.ReadKey();
        }

        [DllImport("cygexample.dll",
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "say_hello",
            ExactSpelling = true)]
        [SuppressUnmanagedCodeSecurity]
        private static extern void SayHello([MarshalAs(UnmanagedType.LPStr)] string name);
    }
}

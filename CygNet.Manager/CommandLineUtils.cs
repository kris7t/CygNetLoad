using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CygNet.Manager
{
    /// <summary>
    /// An utility function to split a Windows command line into arguments
    /// based on the code of Daniel Earwicker,
    /// which is available at <seealso cref="http://stackoverflow.com/a/298990/560450"/>.
    /// </summary>
    internal static class CommandLineUtils
    {
        public static string[] SplitCommandLine(string commandLine)
        {
            bool inQuotes = false;

            return commandLine
                .Split(c =>
                {
                    if (c == '\"')
                        inQuotes = !inQuotes;

                    return !inQuotes && c == ' ';
                })
                .Select(arg => arg.Trim().TrimMatchingQuotes('\"'))
                .Where(arg => !string.IsNullOrEmpty(arg))
                .ToArray();
        }

        private static IEnumerable<string> Split(this string str,
                                        Func<char, bool> controller)
        {
            var nextPiece = 0;

            for (var c = 0; c < str.Length; c++)
            {
                if (!controller(str[c])) continue;
                yield return str.Substring(nextPiece, c - nextPiece);
                nextPiece = c + 1;
            }

            yield return str.Substring(nextPiece);
        }

        private static string TrimMatchingQuotes(this string input, char quote)
        {
            if ((input.Length >= 2) &&
                (input[0] == quote) && (input[input.Length - 1] == quote))
                return input.Substring(1, input.Length - 2);

            return input;
        }
    }
}

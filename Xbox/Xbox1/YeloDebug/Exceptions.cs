/*
    OpenSauceBox: SDK for Xbox User Modding

    Copyright (C)  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;

namespace YeloDebug.Exceptions
{
    /// <summary>Thrown when an YeloDebug API function fails</summary>
    public class ApiException : Exception
    {
        public ApiException() : base("An internal method has failed.") { }
        public ApiException(string message) : base(message) { }
        public ApiException(string message, Exception inner) : base(message, inner) { }
    };

    /// <summary>Represents errors that occur when there is no debug connection detected between the xbox and pc.</summary>
    public class NoConnectionException : ApiException
    {
        public NoConnectionException() : base("Requires debug connection.") { }
        public NoConnectionException(string message) : base(message) { }
        public NoConnectionException(string message, Exception inner) : base(message, inner) { }
    };

    /// <summary>Represents errors that occur when a script becomes unresponsive.</summary>
    public class ScriptTimeoutException : ApiException
    {
        public ScriptTimeoutException() : base("Xbox script has become unresponsive.") { }
        public ScriptTimeoutException(string message) : base(message) { }
        public ScriptTimeoutException(string message, Exception inner) : base(message, inner) { }
    };

    /// <summary>Represents errors that occur within the xbox debugging interface.</summary>
    public class XboxDebugException : ApiException
    {
        public XboxDebugException() : base("Unknown error.") { }
        public XboxDebugException(string message) : base(message) { }
        public XboxDebugException(string message, Exception inner) : base(message, inner) { }
    };

    /// <summary>Represents errors that occur when an unsupported method is called.</summary>
    public class UnsupportedException : ApiException
    {
        public UnsupportedException() : base("This method or operation is not supported.") { }
        public UnsupportedException(string message) : base(message) { }
        public UnsupportedException(string message, Exception inner) : base(message, inner) { }
    };

    /// <summary>Represents errors that occur when an xbox memory allocation fails.</summary>
    public class MemoryAllocationException : ApiException
    {
        public MemoryAllocationException() : base("Failed to allocate xbox memory.") { }
        public MemoryAllocationException(string message) : base(message) { }
        public MemoryAllocationException(string message, Exception inner) : base(message, inner) { }
    };

    /// <summary>Represents errors that occur when a corruption is detected in our history pages.</summary>
    public class HistoryCorruptionException : ApiException
    {
        public HistoryCorruptionException() : base("History corruption detected. A new one has been recreated automatically but certain information could be lost.") { }
        public HistoryCorruptionException(string message) : base(message) { }
        public HistoryCorruptionException(string message, Exception inner) : base(message, inner) { }
    };
}
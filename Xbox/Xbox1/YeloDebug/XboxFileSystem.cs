/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using System.Diagnostics;
using YeloDebug.Exceptions;
using System.Net.Sockets;

namespace YeloDebug
{
    /// <summary>
    /// Creates a standard xbox file stream.
    /// </summary>
    public class XboxFileStream : Stream
    {
        #region Fields
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        Xbox Xbox;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        int bufferSize = 0x20000; // 128kb
        public int BufferSize { get { return bufferSize; } set { bufferSize = value; } }

        protected uint position;
        public override long Position
        {
            get { return position; }
            set { position = (uint)value; }
        }
        protected string FileName;
        #endregion

        #region Constructors
        /// <summary>
        /// Creates a new file stream using a client connection to a debug xbox.
        /// </summary>
        /// <param name="xbox">Connection to use.</param>
        /// <param name="fileName">Name of the file to expose stream to.</param>
        /// <param name="mode">File create disposition.</param>
        public XboxFileStream(Xbox xbox, string fileName, FileMode mode)
        {
            this.Xbox = xbox;
            if (xbox == null || !xbox.Connected)
                throw new NoConnectionException("Must connect first.");

            FileName = fileName;
            position = 0;
            xbox.CreateFile(fileName, mode);
        }
        public XboxFileStream(Xbox client, string fileName)
        {
            this.Xbox = client;
            if (client == null)
                throw new NoConnectionException("Must connect first.");

            this.FileName = fileName;
            position = 0;
            client.CreateFile(fileName, FileMode.Create);   // creates the file by default
        }
        #endregion

        #region Methods
        public override bool CanRead { get { return Xbox.Connected; } }
        public override bool CanSeek { get { return Xbox.Connected; } }
        public override bool CanWrite { get { return Xbox.Connected; } }
        public override void Flush() { throw new UnsupportedException(); }

        // bug: will lose track of length if other instances are modifying the same file...
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        long length = -1;
        public override long Length
        {
            get
            {
                if (length == -1)
                {
                    return length = Xbox.GetFileSize(FileName);
                }
                else return length;
            }
        }

        public override long Seek(long offset, System.IO.SeekOrigin origin)
        {
            switch (origin)
            {
                case SeekOrigin.Begin: return position = (uint)offset;
                case SeekOrigin.Current: return position += (uint)offset;
                case SeekOrigin.End: return position = (uint)Length - (uint)offset;
                default: throw new Exception("Invalid SeekOrigin.");
            }
        }

        public override void SetLength(long value)
        {
            Xbox.SetFileSize(FileName, (int)value);
        }

        public void Read(string name, int offset, byte[] buffer, int length, ref int read)
        {
            int iterations = length / bufferSize;
            int remainder = length % bufferSize;
            int index = 0;

            StatusResponse Response;

            for (int i = 0; i < iterations; i++)
            {
                Response = Xbox.SendCommand("getfile name=\"{0}\" offset={1} size={2}", name, position + offset + index, bufferSize);
                if (Response.Type == ResponseType.BinaryResponse)
                {
                    Xbox.Wait(4);
                    byte[] temp = new byte[4];
                    Xbox.Connection.Client.Receive(temp, 4, SocketFlags.None);
                    int bytesRead = BitConverter.ToInt32(temp, 0);
                    if (bytesRead > 0)
                    {
                        Xbox.Wait((int)bytesRead);
                        Xbox.Connection.Client.Receive(buffer, index, bufferSize, SocketFlags.None);
                        read += bytesRead;
                        index += bufferSize;
                        position += (uint)read;
                    }
                    else throw new IOException("File Read Failed");
                }
                else throw new IOException("File Read Failed");
            }

            if (remainder > 0)
            {
                Response = Xbox.SendCommand("getfile name=\"{0}\" offset={1} size={2}", name, position + offset + index, remainder);
                if (Response.Type == ResponseType.BinaryResponse)
                {
                    Xbox.Wait(4);
                    byte[] temp = new byte[4];
                    Xbox.Connection.Client.Receive(temp, 4, SocketFlags.None);
                    int bytesRead = BitConverter.ToInt32(temp, 0);
                    if (bytesRead > 0)
                    {
                        Xbox.Wait((int)bytesRead);
                        Xbox.Connection.Client.Receive(buffer, index, remainder, SocketFlags.None);
                        read += bytesRead;
                        position += (uint)read;
                    }
                    else throw new IOException("File Read Failed");
                }
                else throw new IOException("File Read Failed");
            }
        }
        public override int Read(byte[] buffer, int offset, int count)
        {
            int bytesRead = 0;
            Read(FileName, offset, buffer, count, ref bytesRead);
            return bytesRead;
        }


        public void Write(string name, int offset, byte[] buffer, int length)
        {
            int iterations = length / bufferSize;
            int remainder = length % bufferSize;
            int index = 0;
            this.position = (uint)offset;

            StatusResponse Response;

            for (int i = 0; i < iterations; i++)
            {
                Response = Xbox.SendCommand("writefile name=\"{0}\" offset={1} length={2}", name, position, bufferSize);
                if (Response.Type == ResponseType.ReadyForBinary)
                {
                    Xbox.Connection.Client.Send(buffer, index, bufferSize, SocketFlags.None);
                    if (Xbox.ReceiveStatusResponse().Success)
                    {
                        index += bufferSize;
                        position += (uint)bufferSize;
                        if (position > this.length)
                            this.length = position;
                    }
                    else throw new IOException("File Write Failed");
                }
                else throw new IOException("File Write Failed");
            }

            if (remainder > 0)
            {
                Response = Xbox.SendCommand("writefile name=\"{0}\" offset={1} length={2}", name, position, remainder);
                if (Response.Type == ResponseType.ReadyForBinary)
                {
                    Xbox.Connection.Client.Send(buffer, index, remainder, SocketFlags.None);
                    if (Xbox.ReceiveStatusResponse().Success)
                    {
                        position += (uint)remainder;
                        if (position > this.length)
                            this.length = position;
                    } 
                    else throw new IOException("File Write Failed");
                }
                else throw new IOException("File Write Failed");
            }
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            Write(FileName, offset, buffer, count);
        }
        #endregion
    };
}
/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;
using YeloDebug.Exceptions;

namespace YeloDebug
{
    /// <summary>
    /// Creates a standard xbox memory stream.
    /// </summary>
    public class XboxMemoryStream : Stream
    {
        #region Fields
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        Xbox Xbox;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        int bufferSize = 0x20000; // 128kb
        public int BufferSize { get { return bufferSize; } set { bufferSize = value; } }

        /// <summary>
        /// Use this as a precautionary method against crashes due to invalid memory addresses.
        /// </summary>
        public bool SafeMode { get { return safeMode; } set { safeMode = value; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private bool safeMode;

        protected uint position;
        public override long Position
        {
            get { return position; }
            set { position = (uint)value; }
        }
        public override bool CanRead { get { return Xbox.Connected; } }
        public override bool CanSeek { get { return Xbox.Connected; } }
        public override bool CanWrite { get { return Xbox.Connected; } }
        #endregion

        #region Constructor
        /// <summary>
        /// Creates a new memory stream using a client connection to a debug xbox.
        /// </summary>
        /// <param name="client">Connection to use.</param>
        public XboxMemoryStream(Xbox client)
        {
            Xbox = client;
            if (client == null || !client.Connected)
                throw new NoConnectionException();
            position = 0x10000; // start at a valid memory address
        }
        #endregion

        #region Methods
        public override void Flush() { throw new UnsupportedException(); }
        public override long Seek(long offset, SeekOrigin origin)
        {
            switch (origin)
            {
                case SeekOrigin.Begin: return position = (uint)offset; // zero-based offset
                case SeekOrigin.Current: return position += (uint)offset;
                default: throw new Exception("Invalid SeekOrigin.");
            }
        }
        public long SeekTo(long offset) { return position = (uint)offset; }
        public override long Length { get { throw new UnsupportedException(); } }
        public override void SetLength(long value) { throw new UnsupportedException(); }
        public override int Read(byte[] buffer, int offset, int count)
        {
            int read = 0;
            Read(position, count, buffer, offset, ref read);
            position += (uint)read;
            return read;
        }

        public void Read(uint address, int length, byte[] buffer, int offset, ref int read)
        {
            // only check base address - would add too much overhead to check range
            // plus, it's much more likely that the entire range will be valid if the base address is
            if (safeMode & !Xbox.IsValidAddress(address))
                throw new Exception("Safe Mode detected invalid base address");

            int iterations = (int)length / bufferSize;
            int remainder = (int)length % bufferSize;
            read = 0;

            StatusResponse response;

            for (int i = 0; i < iterations; i++)
            {
                response = Xbox.SendCommand("getmem2 addr=0x{0} length={1}", Convert.ToString(address + read, 16).PadLeft(8, '0'), bufferSize);
                Xbox.Wait(bufferSize);
                Xbox.Connection.Client.Receive(buffer, (int)(offset + read), bufferSize, SocketFlags.None);
                read += bufferSize;
            }

            if (remainder > 0)
            {
                response = Xbox.SendCommand("getmem2 addr=0x{0} length={1}", Convert.ToString(address + read, 16).PadLeft(8, '0'), remainder);
                Xbox.Wait(remainder);
                Xbox.Connection.Client.Receive(buffer, (int)(offset + read), remainder, SocketFlags.None);
                read += remainder;
            }
        }

        /// <summary>
        /// Writes to xbox memory. Performance of ~10MB/s due to a simple xbdm.dll modification.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="length"></param>
        /// <param name="buffer"></param>
        /// <param name="offset"></param>
        public void Write(uint address, int length, byte[] buffer, int offset)
        {
            // only check base address - would add too much overhead to check range
            // plus, it's much more likely that the entire range will be valid anyways
            if (safeMode & !Xbox.IsValidAddress(address))
                throw new Exception("Safe Mode detected invalid base address");

            int iterations = length / bufferSize;
            int remainder = length % bufferSize;
            int index = 0;

            StatusResponse Response;

            for (int i = 0; i < iterations; i++)
            {
                // hack: hijacked writefile routine in xbdm v7887 so that we can send binary data to memory instead of length-limited ascii
                Response = Xbox.SendCommand("writefile name=| offset=0x" + Convert.ToString(address, 16) + " length=" + bufferSize);
                if (Response.Type == ResponseType.ReadyForBinary)
                {
                    Xbox.Connection.Client.Send(buffer, offset, bufferSize, SocketFlags.None);
                    Response = Xbox.ReceiveStatusResponse(); // garbage number of bytes set...it keeps track of total, dont really care to find how to reset it
                    // check for failure
                    index += bufferSize;
                }
                else throw new Exceptions.ApiException("SendCommand");
            }

            if (remainder > 0)
            {
                Response = Xbox.SendCommand("writefile name=| offset=0x" + Convert.ToString(address, 16) + " length=" + remainder);
                if (Response.Type == ResponseType.ReadyForBinary)
                {
                    Xbox.Connection.Client.Send(buffer, offset, remainder, SocketFlags.None);
                    Response = Xbox.ReceiveStatusResponse();
                    // check for failure - parse message and determine bytes written, then return 
                    index += bufferSize;
                }
                else throw new Exceptions.ApiException("SendCommand");
            }
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            Write(position, count, buffer, offset);
            position += (uint)count;
        }
        #endregion
    };

	#region Old bullshit, idk brah
	//public enum SearchPredicate : byte
    //{
    //    Empty, Exact, GreaterThan, LessThan, Unknown, Increased, Decreased, Changed, Unchanged, // value type can be anything except string
    //    ASCII, Unicode, Hex // value type must be a string
    //}

    //public class XboxMemorySearcher<T>
    //{
    //    #region Fields
    //    [DebuggerBrowsable(DebuggerBrowsableState.Never)]
    //    private Xbox Xbox;
    //    [DebuggerBrowsable(DebuggerBrowsableState.Never)]
    //    private const string TempPath = @"C:\WINDOWS\Temp\Searcher\";

    //    public byte[] XboxMemory;
    //    AddressRange AddressRange;  //physical address ranges

    //    public bool InitialSearch = true;
    //    public int SearchNumber = 0;
    //    public int SearchResults;

    //    TimeSpan DumpTime;
    //    TimeSpan SearchTime;
    //    #endregion

    //    public XboxMemorySearcher(Xbox xbox)
    //    {
    //        if (xbox == null) throw new NoConnectionException("Requires debug connection.");
    //        xbox.ConnectionCheck();
    //        Xbox = xbox;
    //        AddressRange = new AddressRange(0, Xbox.MemoryStatistics.TotalPages * 0x1000);

    //    }

    //    #region Methods

    //    /// <summary>
    //    /// Starts a new search.
    //    /// </summary>
    //    public void New()
    //    {
    //        InitialSearch = true;
    //        SearchNumber = 0;

    //        if (Directory.Exists(TempPath))
    //            Directory.Delete(TempPath, true); //deletes all old files


    //        Directory.CreateDirectory(TempPath);   //creates new one
    //    }

    //    public void UndoLast()
    //    {
    //        try
    //        {
    //            if (SearchNumber > 1)
    //            {
    //                SearchNumber--;
    //                new FileInfo(TempPath + "Addresses_" + SearchNumber).Delete();
    //                new FileInfo(TempPath + "Values_" + SearchNumber).Delete();
    //                SearchResults = (int)(new FileInfo(TempPath + "Addresses_" + (SearchNumber - 1)).Length / 4);
    //            }
    //        }
    //        catch (Exception) { }
    //    }

    //    public void Undo(int searches)
    //    {


    //    }

    //    public void UndoTo(int searchNumber)
    //    {

    //    }

    //    /// <summary>
    //    /// Performs an xbox memory dump and searches for the specified value.
    //    /// </summary>
    //    /// <param name="searchType"></param>
    //    /// <param name="dataType"></param>
    //    /// <param name="searchValue"></param>
    //    public void FindValue(T value, SearchPredicate type)
    //    {
    //        // if value is not a string, make sure search type is a value type...
    //        // if value is a string, make sure search type is a string type...


    //        TypeCode dataType = Type.GetTypeCode(value.GetType());
    //        if (dataType == TypeCode.String)
    //        {
    //            if (type == SearchPredicate.ASCII)
    //            {

    //            }
    //            else if (type == SearchPredicate.Unicode)
    //            {
    //                Util.StringToUnicodeArray(Convert.ToString(value));
    //            }
    //            else if (type == SearchPredicate.Hex)
    //            {

    //            }
    //        }

    //        // dump xbox memory
    //        DateTime BeforeDump = DateTime.Now;
    //        XboxMemory = Xbox.DumpMemory();
    //        DumpTime = DateTime.Now - BeforeDump;


    //        DateTime BeforeSearch = DateTime.Now;










    //        // after we have put the compressed addresses and values into the streams,
    //        // we will compress again before finally saving to the hdd
    //        // GZipStream zipStream = new GZipStream();

    //        // i can also add first address and then add each address after as an index from the last, 
    //        //hoping that gzip would compress more than if i were to add packed addresses myself



    //        // when saving value files, even if its an exact search, store all values and let gzip do its job
    //        // although if exact search, maybe lop off high zero bits before writing to bit stream

    //    }

    //    private void PackAddress()
    //    {


    //        // 128mb = 0x7FFFFFF = 27 bits
    //        // 15.625% compression

    //        // 64mb = 0x3FFFFFFF = 26 bits
    //        // 18.75% compression





    //    }

    //    private void PackIntegerValue()
    //    {



    //    }

    //    #endregion
	//}
	#endregion
}
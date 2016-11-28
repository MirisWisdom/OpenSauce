/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Diagnostics;
using YeloDebug.Exceptions;

namespace YeloDebug
{
    // xbox buffers

    // store temp variables in memory that need to be tied to a specific xbox session
    // we leave some form of history on the xbox so if an app that uses yelodebug crashes or quits, 
    // the next time another one reconnects they can read the history and clean up or reuse whatever old memory 
    // the app had allocated, then be back in business ;)



    // do this when scratch buffer is out of history...
    ///// <summary>
    ///// Allocates a new scratch buffer and frees the old.
    ///// </summary>
    ///// <param name="size"></param>
    //public void ChangeScratchBufferSize(uint size)
    //{
    //    uint memptr = AllocateDebugMemory(size);
    //    FreeMemory(scratchBuffer);
    //    scratchBuffer = memptr;
    //    scratchBufferSize = size;
    //}

    /// <summary>
    /// Keeps track of history addresses
    /// </summary>
    public class XboxHistory
    {
        Xbox Xbox;


        // current script buffer information
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public static uint ScriptBufferAddress = 0xB0037800;  // location in xbdm.dll memory
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public static uint ScriptBufferSize = 2048;

        /// <summary>
        /// Gets the address of the buffer automatically allocated for temporary storage in xbox memory.
        /// </summary>
        public uint ScratchBuffer { get { return scratchBuffer; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint scratchBuffer = TempBuffer.BaseAddress;

        /// <summary>
        /// Gets the size of the temporary buffer allocated on the xbox.
        /// </summary>
        public uint ScratchBufferSize { get { return scratchBufferSize; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint scratchBufferSize = 0x1000;


        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public uint XInputGetStateAddress;    // base address of our xinputgetstate function
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public byte[] OriginalGamepadCode; // store our original code so we can unhook if necessary


        const int kSize = 0x4000;
        const uint kBaseAddress = 0x40000000;

        public struct Header
        {
            public const uint BaseAddress = 0x40000000; // "Yelo"
            public const int Size = 32;
        }

        public struct AllocationTable
        {
            public const uint CountAddress = 0x40000020;
            public const uint BufferAddress = 0x40000024;
            public const int BufferSize = 4064;
            public const int MaxCount = 450;
        }

        public struct RemoteExecution
        {
            public const uint ArgumentBuffer = 0x40001008;  // use with pointers
            public const uint XmmContextBuffer = 0x40001040;
            public const uint BufferAddress = 0x40001060;   // call/return address, xmm registers (TODO), script buffer
            public const int MaxParameterCount = 10;
            public const int BufferSize = 992;
        }

        public struct Gamepad
        {
            public const uint XInputGetState = 0x40001440;
            public const uint EnabledAddress = 0x40001444;      //bool IsEnabled
            public const uint PortStatusAddress = 0x40001448;       //uint[4] isPluggedIn <--counts get incremented by one if its plugged in
            public const uint OriginalCodeBuffer = 0x40001458;  //byte[10]
            public const uint StateBufferAddress = 0x40001500;  //4 32-byte buffers
            public const uint ScriptAddress = 0x40001580;
        }

        public struct TempBuffer
        {
            public const uint BaseAddress = 0x40003000;
            public const int Size = 4096;
        }


        /// <summary>
        /// Initializes the history page.
        /// </summary>
        public XboxHistory(Xbox xbox)
        {
            Xbox = xbox;
            Xbox.SetMemory(0xB00292D0, ScriptBufferAddress); // set up the script buffer

            if (IsPresent())
            {
                // restore our current allocations
                Xbox.ReloadAllocationTable();

                // check other settings like controller hook etc...
                XInputGetStateAddress = Xbox.GetUInt32(Gamepad.XInputGetState);
                OriginalGamepadCode = Xbox.GetMemory(Gamepad.OriginalCodeBuffer, 10);
            }
            else
            {
                // allocate memory for our history pages
                AllocateHistoryPages(kSize);
                Xbox.SetMemory(kBaseAddress, 0x6F6C6559);   // "Yelo"
            }
        }

        bool IsPresent()
        {
            Xbox.SendCommand("getmem addr=0x{0} length=4", Convert.ToString(kBaseAddress, 16));
            string yelo = Xbox.ReceiveSocketLine().Replace("\r\n", "");
            Xbox.ReceiveSocketLine();
            return (yelo == "59656C6F");
        }

        /// <summary>
        /// Use in beginning when setting up our history page, since calladdressex will depend on that memory
        /// </summary>
        /// <param name="size"></param>
        /// <returns>Allocated address.</returns>
        uint AllocateHistoryPages(uint size)
        {
            // calculate actual size of allocation
            size = Util.GetAlignedPageBoundary(size);

            // checks if theres enough memory for allocation to take place
            Xbox.IsEnoughMemory(size);

            #region Reserve the memory
            // store address to call
            Xbox.SetMemory(0x10000, size);
            Xbox.SetMemory(0x10004, 0x40000000);

            // inject script
            //push	4	;protect
            //push	2000h	;type
            //push	10000h	;pSize
            //push	0
            //push	10004h	;pAddress
            //mov	eax, 012345678h	;export address
            //call	eax
            //mov	eax, 02DB0000h	;fake success
            //retn	010h
            Xbox.MemoryStream.Position = ScriptBufferAddress;
            byte[] pt1 = { 0x6A, 0x04, 0x68, 0x00, 0x20, 0x00, 0x00, 0x68, 0x00, 0x00, 0x01, 0x00, 0x6A, 0x00, 0x68, 0x04, 0x00, 0x01, 0x00, 0xB8 };
            Xbox.MemoryWriter.Write(pt1);
            Xbox.MemoryWriter.Write(Xbox.Kernel.NtAllocateVirtualMemory);
            byte[] pt2 = { 0xFF, 0xD0, 0xB8, 0x00, 0x00, 0xDB, 0x02, 0xC2, 0x10, 0x00 };
            Xbox.MemoryWriter.Write(pt2);

            // execute script via hijacked crashdump function
            Xbox.SendCommand("crashdump");

            // return the value of eax after the call
            uint ptr = Xbox.GetUInt32(0x10004);

            #endregion

            #region Commit the memory
            // store address to call
            Xbox.SetMemory(0x10000, size);
            Xbox.SetMemory(0x10004, 0x40000000);

            // inject script
            //push	4	;protect
            //push	1000h	;type
            //push	10000h	;pSize
            //push	0
            //push	10004h	;pAddress
            //mov	eax, 012345678h	;export address
            //call	eax
            //mov	eax, 02DB0000h	;fake success
            //retn	010h
            Xbox.MemoryStream.Position = ScriptBufferAddress;
            byte[] pt3 = { 0x6A, 0x04, 0x68, 0x00, 0x10, 0x00, 0x00, 0x68, 0x00, 0x00, 0x01, 0x00, 0x6A, 0x00, 0x68, 0x04, 0x00, 0x01, 0x00, 0xB8 };
            Xbox.MemoryWriter.Write(pt3);
            Xbox.MemoryWriter.Write(Xbox.Kernel.NtAllocateVirtualMemory);
            byte[] pt4 = { 0xFF, 0xD0, 0xB8, 0x00, 0x00, 0xDB, 0x02, 0xC2, 0x10, 0x00 };
            Xbox.MemoryWriter.Write(pt4);

            // execute script via hijacked crashdump function
            Xbox.SendCommand("crashdump");

            // return the value of eax after the call
            ptr = Xbox.GetUInt32(0x10004);
            #endregion

            // check for success, but DONT add to our allocation table...
            if (ptr == 0)
                throw new Exception("Failed to initialize YeloDebug in xbox memory.");

            return ptr;
        }

        /// <summary>
        /// Changes the script buffer size.
        /// </summary>
        /// <param name="size"></param>
        public void ChangeScriptBufferSize(uint size)
        {
            if (ScriptBufferAddress != 0xB0037800)
                Xbox.FreeMemory(ScriptBufferAddress);

            ScriptBufferAddress = Xbox.AllocateDebugMemory(size);
            ScriptBufferSize = size;
            Xbox.SetMemory(0xB00292D0, ScriptBufferAddress);   // reroutes the ScriptBufferAddress ptr...(assumes were already running v7887 xbdm)
        }
    };
}
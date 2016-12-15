/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Runtime.InteropServices;

namespace YeloDebug
{
	public class SurfaceInformation
	{
		public uint Size;
		public uint Format;
		public uint Address;
		public uint PushBufferPut;

        public override string ToString()
        {
            return
                "{ Size: " + Size +
                " Format: " + Format +
                " Address: " + Address +
                " PushBufferPut: " + PushBufferPut +
                " }";
        }
	};


	public class ATAInputRegisters
	{
		public byte Features;      // Used for specifying SMART "commands".
		public byte SectorCount;   // IDE sector count register
		public byte SectorNumber;  // IDE sector number register
		public byte CylinderLow;   // IDE low order cylinder value
		public byte CylinderHigh;  // IDE high order cylinder value
		public byte DriveHead;     // IDE drive/head register
		public byte Command;       // Actual IDE command.
		public readonly byte Reserved = 0;  // reserved for future use.  Must be zero.
	};
	public struct ATAOutputRegisters
	{
		public byte Error;
		public byte SectorCount;
		public byte SectorNumber;
		public byte CylinderLow;
		public byte CylinderHigh;
		public byte DriveHead;
		public byte Status;
	};
	public class ATACommandObject
	{
		public ATAInputRegisters InputRegisters;
		public ATAOutputRegisters OutputRegisters;
		public byte[] Data = new byte[512];
		public uint DataSize = 512;
	};

	public class IDERegisters
	{
		public byte Features;
		public byte SectorCount;
		public byte SectorNumber;
		public byte CylinderLow;
		public byte CylinderHigh;
		public byte DriveHead;
		public byte CommandRegister;
		public byte HostSendsData;
	};
	public /*unsafe*/ class ATAPassThrough
	{
		public IDERegisters Registers;
		public uint DataBufferSize;
		public /* byte* */ UIntPtr DataBuffer;
	};

	public struct ProcessorInformation
	{
		public string Identification;
		public uint Stepping;
		public uint Model;
		public uint Family;

        public override string ToString()
        {
            return
                "{ Identification: " + Identification +
                " Stepping: " + Stepping +
                " Model: " + Model +
                " Family: " + Family +
                " }";
        }
	};

	public struct ProductionInfo
	{
		public string Country;
		public uint LineNumber;
		public uint Week;
		public uint Year;

        public override string ToString()
        {
            return
                "{ Country: " + Country +
                " LineNumber: " + LineNumber +
                " Week: " + Week +
                " Year: " + Year +
                " }";
        }
	};
	//public struct ThreadStopInfo
	//{
	//    public bool IsStopped;
	//    public uint Address;
	//    public uint Thread;

	//}

	//// maybe do separate notification structs for each type: BreakNotification, ExecutionNotification etc...
	//public struct Notification
	//{
	//    NotificationType Type;
	//    string Message;
	//}

	public struct ThreadInfo
	{
		public uint ID;
		public uint Suspend;
		public uint Priority;
		public uint TlsBase;
		public uint Start;
		public uint Base;
		public uint Limit;
		public DateTime CreationTime;
	};

	/// <summary>
	/// Module information.
	/// </summary>
	public class ModuleInfo
	{
		/// <summary>
		/// Name of the module that was loaded.
		/// </summary>
		public string Name;

		/// <summary>
		/// Address that the module was loaded to.
		/// </summary>
		public uint BaseAddress;

		/// <summary>
		/// Size of the module.
		/// </summary>
		public uint Size;

		/// <summary>
		/// Time stamp of the module.
		/// </summary>
		public DateTime TimeStamp;

		/// <summary>
		/// Checksum of the module.
		/// </summary>
		public uint Checksum;

		/// <summary>
		/// Sections contained within the module.
		/// </summary>
		public System.Collections.Generic.List<ModuleSection> Sections;

        public override string ToString()
        {
            return
                "{ Name: " + Name +
                " BaseAddress: " + BaseAddress +
                " Size: " + Size +
                " TimeStamp: " + TimeStamp.ToString() +
                " Checksum: " + Checksum +
                " }";
        }
	};

	/// <summary>
	/// Module section information.
	/// </summary>
	public class ModuleSection
	{
		public string Name;
		public uint Base;
		public uint Size;
		public uint Index;
		public uint Flags;

        public override string ToString()
        {
            return
                "{ Name: " + Name +
                " Base: " + Base +
                " Size: " + Size +
                " Index: " + Index +
                " Flags: " + Flags +
                " }";
        }
	};

	/// <summary>
	/// Structure that contains information about the XBE.
	/// </summary>
	public class XbeInfo
	{
		public string LaunchPath;
		public DateTime TimeStamp;
		public uint Checksum;
		public uint StackSize;

        public override string ToString()
        {
            return
                "{ LaunchPath: " + LaunchPath +
                " TimeStamp: " + TimeStamp +
                " Checksum: " + Checksum +
                " StackSize: " + StackSize +
                " }";
        }
	};

	/// <summary>
	/// Xbox file information.
	/// </summary>
	public class FileInformation
	{
		public string Name;
		public ulong Size;
		public System.IO.FileAttributes Attributes;
		public DateTime CreationTime;
		public DateTime ChangeTime;
	};

	/// <summary>
	/// Xbox memory statistics.
	/// </summary>
	public class MemoryStatistics
	{
		public uint TotalPages;
		public uint AvailablePages;
		public uint StackPages;
		public uint VirtualPageTablePages;
		public uint SystemPageTablePages;
		public uint PoolPages;
		public uint VirtualMappedPages;
		public uint ImagePages;
		public uint FileCachePages;
		public uint ContiguousPages;
		public uint DebuggerPages;

        public override string ToString()
        {
            return
                "{ TotalPages: " + TotalPages +
                " AvailablePages: " + AvailablePages +
                " StackPages: " + StackPages +
                " VirtualPageTablePages: " + VirtualPageTablePages +
                " SystemPageTablePages: " + SystemPageTablePages +
                " PoolPages: " + PoolPages +
                " VirtualMappedPages: " + VirtualMappedPages +
                " ImagePages: " + ImagePages +
                " FileCachePages: " + FileCachePages +
                " ContiguousPages: " + ContiguousPages +
                " DebuggerPages: " + DebuggerPages +
                " }";
        }
	};

	/// <summary>
	/// Xbox memory region.
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public class MemoryRegion
	{
		public UIntPtr BaseAddress;
		public uint Size;
		public MEMORY_FLAGS Protect;
	};

	/// <summary>
	/// Xbox memory address range.
	/// </summary>
	public class AddressRange // really should be a struct, but then you give up the parameterless ctor. sacrifices mike?
	{
		public uint Low;
		public uint High;

		/// <summary>
		/// Specifies a default range of all addresses.
		/// </summary>
		public AddressRange()
		{
			Low = 0;
			High = 0xFFFFFFFF;
		}

		/// <summary>
		/// Specifies a custom address range.
		/// </summary>
		/// <param name="low"></param>
		/// <param name="high"></param>
		public AddressRange(uint low, uint high)
		{
			Low = low;
			High = high;
		}
	};

	/// <summary>
	/// Xbox memory allocation entry.
	/// </summary>
	public struct AllocationEntry
	{
		public uint Address;
		public uint Size;
		public AllocationType Type;

		public AllocationEntry(uint address, uint size, AllocationType type)
		{
			Address = address;
			Size = size;
			Type = type;
		}
	};

	/// <summary>
	/// Basic xbox memory information.
	/// </summary>
	public struct MEMORY_BASIC_INFORMATION
	{
		public uint BaseAddress;
		public uint AllocationBase;
		public MEMORY_FLAGS AllocationProtect;
		public uint RegionSize;
		public MEMORY_FLAGS State;
		public MEMORY_FLAGS Protect;
		public MEMORY_FLAGS Type;
	};

	/// <summary>
	/// Cpu general purpose register context.  Only the registers you change will be set before the call.
	/// </summary>
	public class CPUContext
	{
		// general purpose - assumes integer assignment
		public uint? Eax;
		public uint? Ebx;
		public uint? Ecx;
		public uint? Edx;
		public uint? Esi;
		public uint? Edi;
		public uint? Esp;
		public uint? Ebp;

		// sse - assumes floating point assignment
		// TODO: these really should be doubles
		public float? Xmm0;
		public float? Xmm1;
		public float? Xmm2;
		public float? Xmm3;
		public float? Xmm4;
		public float? Xmm5;
		public float? Xmm6;
		public float? Xmm7;
	};

	/// <summary>
	/// Xbox gamepad input state.
	/// </summary>
	public class InputState
	{
		public Buttons Buttons = 0;
		public byte[] AnalogButtons = new byte[8];
		public short ThumbLX = 0;
		public short ThumbLY = 0;
		public short ThumbRX = 0;
		public short ThumbRY = 0;

		public void AssignState(InputState state)
		{
			this.Buttons = state.Buttons;
			this.AnalogButtons = state.AnalogButtons;
			this.ThumbLX = state.ThumbLX;
			this.ThumbLY = state.ThumbLY;
			this.ThumbRX = state.ThumbRX;
			this.ThumbRY = state.ThumbRY;
		}
	};

	/// <summary>
	/// Xbox command status response.
	/// </summary>
	public class StatusResponse
	{
		public string Full { get; private set; }
		public ResponseType Type { get; private set; }
		public string Message { get; private set; }
		public bool Success { get { return ((int)Type & 200) == 200; } }

		public StatusResponse(string full, ResponseType type, string message)
		{
			this.Full = full;
			this.Type = type;
			this.Message = message;
		}
	};

	/// <summary>
	/// Xbox debug connection information.
	/// </summary>
	public struct DebugConnection
	{
		public System.Net.IPAddress IP;
		public string Name;
		public DebugConnection(System.Net.IPAddress ip, string name)
		{
			IP = ip;
			Name = name;
		}
	};
}
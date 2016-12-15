/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Text;
using YeloDebug.Exceptions;

namespace YeloDebug
{
	public partial class Xbox : IDisposable
	{
		/// <summary>
		/// Xbox memory stream.
		/// </summary>
		//[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		public XboxMemoryStream MemoryStream;

		/// <summary>
		/// Xbox memory stream reader.
		/// </summary>
		//[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		public BinaryReader MemoryReader;

		/// <summary>
		/// Xbox memory stream writer.
		/// </summary>
		//[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		public BinaryWriter MemoryWriter;


		/// <summary>
		/// Gets the xbox memory statistics.
		/// </summary>
        public MemoryStatistics MemoryStatistics
		{
			get
			{
				SendCommand("mmglobal");
				MemoryStatistics Statistics = new MemoryStatistics();

				string[] stats1 = ReceiveSocketLine().Split(' ');
				uint addr = Convert.ToUInt32(stats1[4].Substring(24), 16);
				string[] stats2 = ReceiveSocketLine().Split(' ');
				uint totalPages = Convert.ToUInt32(stats2[4].Substring(26), 16);
				uint availablePages = Convert.ToUInt32(stats2[5].Substring(19).Replace("\r\n", ""), 16);
				ReceiveSocketLine();    // '.'

				byte[] mem = GetMemory(addr, 0x2C);
				Statistics.AvailablePages = availablePages;
				Statistics.TotalPages = totalPages;
				Statistics.StackPages = BitConverter.ToUInt32(mem, 4);
				Statistics.VirtualPageTablePages = BitConverter.ToUInt32(mem, 8);
				Statistics.SystemPageTablePages = BitConverter.ToUInt32(mem, 12);
				Statistics.PoolPages = BitConverter.ToUInt32(mem, 16);
				Statistics.VirtualMappedPages = BitConverter.ToUInt32(mem, 20);
				Statistics.ImagePages = BitConverter.ToUInt32(mem, 28);
				Statistics.FileCachePages = BitConverter.ToUInt32(mem, 32);
				Statistics.ContiguousPages = BitConverter.ToUInt32(mem, 36);
				Statistics.DebuggerPages = BitConverter.ToUInt32(mem, 40);
				return Statistics;
			}
		}

		/// <summary>
		/// Gets a list of valid xbox memory segments.
		/// </summary>
        [Browsable(false)]
        public List<MemoryRegion> CommittedMemory
		{
			get
			{
				SendCommand("walkmem");
				List<MemoryRegion> mem = new List<MemoryRegion>();

				for (string page = ReceiveSocketLine(); page[0] != '.'; page = ReceiveSocketLine())
				{
					MemoryRegion region = new MemoryRegion();

					region.BaseAddress = (UIntPtr)(uint)Util.GetResponseInfo(page, 0);
					region.Size = (uint)Util.GetResponseInfo(page, 1);
					region.Protect = (MEMORY_FLAGS)(uint)Util.GetResponseInfo(page, 2);
					mem.Add(region);
				}
				return mem;
			}
		}
        [Browsable(false)]
		public List<MemoryRegion> CombinedCommittedMemory
		{
			get
			{
				// combine any contiguous memory regions
				List<MemoryRegion> regions = CommittedMemory;
				for (int i = 0; i < regions.Count; i++)
					if (i > 0 && (uint)regions[i].BaseAddress == (uint)regions[i - 1].BaseAddress + (uint)regions[i - 1].Size)
					{
						regions[i - 1].Size += regions[i].Size;
						regions.RemoveAt(i);
						i--;
					}

				return regions;
			}
		}

		/// <summary>
		/// Calculates the checksum of a block of memory on the xbox.
		/// </summary>
		/// <param name="address">Memory address on the Xbox console of the first byte of memory in the block. This address must be aligned on an 8-byte boundary, and it cannot point to code.</param>
		/// <param name="length">Number of bytes on which to perform the checksum. This value must be a multiple of 8.</param>
		/// <returns></returns>
		public uint GetMemoryChecksum(int address, int length)
		{
			if ((address % 8) != 0) throw new ArgumentException("Address must be aligned on an 8-byte boundary.", "address");
			if ((length % 8) != 0) throw new ArgumentException("Length must be a multiple of 8.", "length");
			SendCommand("getsum addr={0} length={1} blocksize={1}", address, length);
			return BitConverter.ToUInt32(ReceiveBinaryData(4), 0);
		}

		//TODO: add dump options with things to skip code segments/system/debug memory
		// also add option to not dump, but still write full 64 megs locally so data is still at proper addresses

		/// <summary>
		/// A complete dump of xbox memory.
		/// </summary>
		/// <returns></returns>
		public byte[] DumpMemory()
		{
			// combine any contiguous memory regions
			List<MemoryRegion> regions = CombinedCommittedMemory;

			// get total size of dump
			uint dumpSize = 0;
			foreach (MemoryRegion r in regions)
				dumpSize += (uint)r.Size;

			//Pause();
			byte[] XboxMemory = new byte[dumpSize];

			int oldTimeout = timeout;
			int index = 0;
			int read = 0;
			timeout = 7000; // make sure we don't timeout waiting for large memory reads
			//uint skipped = 0;
			foreach (MemoryRegion r in regions)
			{
				// skip code and system memory
				//if ((int)r.BaseAddress == 0x10000 || (uint)r.BaseAddress > 0xb0000000)
				//{
				//    skipped += (uint)r.Size;
				//    continue;
				//}
				MemoryStream.Read(r.BaseAddress.ToUInt32(), (int)r.Size, XboxMemory, index, ref read);
				index += read;
			}
			timeout = oldTimeout;
			//Continue();

			return XboxMemory;
		}


		public void DumpMemoryToFile()
		{
			using(var memdump = new System.IO.FileStream("memory_dump.bin", FileMode.Create, FileAccess.Write))
			using(var meminfo = new System.IO.FileStream("memory_info.txt", FileMode.Create, FileAccess.Write))
			using (var infowriter = new BinaryWriter(meminfo))
			{
				// combine any contiguous memory regions
				List<MemoryRegion> regions = CombinedCommittedMemory;

				// get total size of dump
				uint dumpSize = 0;
				foreach (MemoryRegion r in regions)
					dumpSize += (uint)r.Size;

				Pause();
				byte[] XboxMemory = new byte[dumpSize];

				int oldTimeout = timeout;
				int index = 0;
				int read = 0;
				timeout = 7000; // make sure we don't timeout waiting for large memory reads
				//uint skipped = 0;
				foreach (MemoryRegion r in regions)
				{
					// skip code and system memory
					//if ((int)r.BaseAddress == 0x10000 || (uint)r.BaseAddress > 0xb0000000)
					//{
					//    skipped += (uint)r.Size;
					//    continue;
					//}

					infowriter.Write(ASCIIEncoding.ASCII.GetBytes(string.Format("File Address: 0x{0}\tXbox Address: 0x{1}\t Size: 0x{2}\r\n",
						Convert.ToString(index, 16).PadLeft(8, '0'),
						Convert.ToString((uint)r.BaseAddress, 16).PadLeft(8, '0'),
						Convert.ToString(r.Size, 16).PadLeft(8, '0'))));

					MemoryStream.Read(r.BaseAddress.ToUInt32(), (int)r.Size, XboxMemory, index, ref read);
					index += read;
				}
				timeout = oldTimeout;
				Continue();

				memdump.Write(XboxMemory, 0, XboxMemory.Length);
			}
		}

		public void SaveMemoryToFile()
		{
			Pause();

			using (var memdump = new System.IO.FileStream("memory_dump.bin", FileMode.Create, FileAccess.Write))
			{
				byte[] mem = DumpMemory();
				memdump.Write(mem, 0, mem.Length);
			}

			using (var meminfo = new System.IO.FileStream("memory_info.txt", FileMode.Create, FileAccess.Write))
			{
				SendCommand("walkmem");
				using (var bw = new BinaryWriter(meminfo))
				{
					bw.Write(ASCIIEncoding.ASCII.GetBytes(ReceiveMultilineResponse()));
				}
			}

			Continue();
		}


		/// <summary>
		/// Retrieves an object from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <param name="dataType">Object type.</param>
		/// <returns>Received object.</returns>
		public object GetMemory(uint address, TypeCode dataType)
		{
			switch (dataType)
			{
				case TypeCode.Boolean: return GetBool(address);
				case TypeCode.Byte: return GetByte(address);
				case TypeCode.Char: return GetUInt16(address);
				case TypeCode.Int16: return GetInt16(address);
				case TypeCode.UInt16: return GetUInt16(address);
				case TypeCode.Int32: return GetInt32(address);
				case TypeCode.UInt32: return GetUInt32(address);
				case TypeCode.Int64: return GetInt64(address);
				case TypeCode.UInt64: return GetUInt64(address);
				case TypeCode.Single: return GetSingle(address);
				case TypeCode.Double: return GetDouble(address);
				case TypeCode.String: return GetString(address);
				default: throw new UnsupportedException("Invalid datatype.");
			}
		}

		/// <summary>
		/// Retrieves data from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <param name="length">Length of data to receive.</param>
		/// <returns>Received data.</returns>
		public byte[] GetMemory(uint address, uint length)
		{
			byte[] Buffer = new byte[length];

			MemoryStream.Position = address;
			MemoryReader.Read(Buffer, 0, (int)length);

			return Buffer;
		}

		/// <summary>
		/// Retrieves bool from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received bool.</returns>
		public bool GetBool(uint address) { MemoryStream.Position = address; return MemoryReader.ReadBoolean(); }

		/// <summary>
		/// Retrieves byte from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received byte.</returns>
		public byte GetByte(uint address) { MemoryStream.Position = address; return MemoryReader.ReadByte(); }

		/// <summary>
		/// Retrieves Int16 from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received Int16.</returns>
		public Int16 GetInt16(uint address) { MemoryStream.Position = address; return MemoryReader.ReadInt16(); }

		/// <summary>
		/// Retrieves UInt16 from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received UInt16.</returns>
		public UInt16 GetUInt16(uint address) { MemoryStream.Position = address; return MemoryReader.ReadUInt16(); }

		/// <summary>
		/// Retrieves Int32 from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received Int32.</returns>
		public Int32 GetInt32(uint address) { MemoryStream.Position = address; return MemoryReader.ReadInt32(); }

		/// <summary>
		/// Retrieves UInt32 from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received UInt32.</returns>
		public UInt32 GetUInt32(uint address) { MemoryStream.Position = address; return MemoryReader.ReadUInt32(); }

		/// <summary>
		/// Retrieves Int64 from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received Int64.</returns>
		public Int64 GetInt64(uint address) { MemoryStream.Position = address; return MemoryReader.ReadInt64(); }

		/// <summary>
		/// Retrieves UInt64 from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received UInt64.</returns>
		public UInt64 GetUInt64(uint address) { MemoryStream.Position = address; return MemoryReader.ReadUInt64(); }

		/// <summary>
		/// Retrieves Single from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received Single.</returns>
		public Single GetSingle(uint address) { MemoryStream.Position = address; return MemoryReader.ReadSingle(); }

		/// <summary>
		/// Retrieves Double from xbox memory.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received Double.</returns>
		public Double GetDouble(uint address) { MemoryStream.Position = address; return MemoryReader.ReadDouble(); }

		/// <summary>
		/// Retrieves a string from xbox memory.  Will automatically detect ascii or unicode strings of size greater than 1 char and retrieve as ascii.
		/// Maximum string size of 512 characters.
		/// </summary>
		/// <param name="address"></param>
		/// <returns></returns>
		public string GetString(uint address)
		{
			byte[] StringBuffer = new byte[1026];   // max string size of unicode x 512 + 2 byte terminator
			MemoryStream.Position = address;
			MemoryReader.Read(StringBuffer, 0, 1026);

			// ascii string
			if (StringBuffer[1] != 0)
			{
				string ascii = ASCIIEncoding.ASCII.GetString(StringBuffer);
				return ascii.Remove(ascii.IndexOf('\0'));
			}
			else // unicode
			{
				string unicode = UnicodeEncoding.Unicode.GetString(StringBuffer);
				return unicode.Remove(unicode.IndexOf("\0\0"));
			}
		}

		/// <summary>
		/// Retrieves a string of specified length from xbox memory.
		/// </summary>
		/// <param name="address"></param>
		/// <param name="length"></param>
		/// <returns></returns>
		public string GetString(uint address, uint length)
		{
			return ASCIIEncoding.ASCII.GetString(GetMemory(address, length));
		}

		/// <summary>
		/// Retrieves a null-terminated ascii string from xbox memory.
		/// Maximum length of 512 characters.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received ascii string.</returns>
		public string GetASCIIString(uint address)
		{
			byte[] StringBuffer = new byte[512];
			MemoryStream.Position = address;
			MemoryReader.Read(StringBuffer, 0, 512);

			string Str = ASCIIEncoding.ASCII.GetString(StringBuffer);
			return Str.Remove(Str.IndexOf('\0'));
		}

		/// <summary>
		/// Retrieves a null-terminated unicode string from xbox memory.
		/// Maximum length of 512 characters.
		/// </summary>
		/// <param name="address">Memory location.</param>
		/// <returns>Received unicode string.</returns>
		public string GetUnicodeString(uint address)
		{
			byte[] StringBuffer = new byte[1024];
			MemoryStream.Position = address;
			MemoryReader.Read(StringBuffer, 0, 1024);
			string Str = UnicodeEncoding.Unicode.GetString(StringBuffer);
			return Str.Remove(Str.IndexOf("\0\0"));
		}

		/// <summary>
		/// Determines whether or not the specified address exists in xbox memory.
		/// </summary>
		/// <param name="address"></param>
		/// <returns></returns>
		public bool IsValidAddress(uint address)
		{
			StatusResponse response = SendCommand("getmem addr=0x{0} length=1", Convert.ToString(address, 16));
			string mem = ReceiveSocketLine();
			ReceiveSocketLine();
			return mem != "??";
		}


		/// <summary>
		/// Checks for a valid address range.
		/// </summary>
		public bool IsValidAddressRange(uint address, int size)
		{
			// combine any contiguous memory regions
			List<MemoryRegion> regions = CombinedCommittedMemory;

			// check if memory range is within a region
			foreach (MemoryRegion r in regions)
			{
				if ((address >= (uint)r.BaseAddress) && (address + size <= (uint)r.BaseAddress + r.Size))
					return true;  //valid address
			}
			return false;   // no valid address range found*/
		}

		/// <summary>
		/// Writes object(s) to a specified xbox memory location.
		/// </summary>
		/// <param name="address">Xbox memory location.</param>
		/// <param name="data">Data to write. Specify multiple data 
		/// instances whenever you have multiple contiguous writes
		/// since this function will combine multiple data internally
		/// and then send all at once.</param>
		public void SetMemory(uint address, params object[] data)
		{
			using (var ms = new MemoryStream())
			using (var bw = new BinaryWriter(ms))
			{
				foreach (object obj in data)
				{
					switch (Convert.GetTypeCode(obj))
					{
						case TypeCode.Boolean:
						case TypeCode.Byte:
						case TypeCode.Char: bw.Write(Convert.ToByte(obj)); break;
						case TypeCode.Int16:
						case TypeCode.UInt16: bw.Write(Convert.ToUInt16(obj)); break;
						case TypeCode.Int32:
						case TypeCode.UInt32: bw.Write(Convert.ToUInt32(obj)); break;
						case TypeCode.Int64:
						case TypeCode.UInt64: bw.Write(Convert.ToUInt64(obj)); break;
						case TypeCode.Single: bw.Write(Convert.ToSingle(obj)); break;
						case TypeCode.Double: bw.Write(Convert.ToDouble(obj)); break;
						case TypeCode.String: bw.Write(ASCIIEncoding.ASCII.GetBytes((string)obj + "\0")); break;    // assumes youre writing an ascii string
						case TypeCode.Object:
							byte[] bytes = obj as byte[]; // tries converting unknown object to byte array
							if (bytes != null) bw.Write(bytes);
							else goto default;
							break;

						default: throw new UnsupportedException("Invalid datatype.");
					}
				}
				MemoryStream.Position = address;
				MemoryWriter.Write(ms.ToArray());
			}
		}

		#region Memory Management
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		const int MaxAllocTableSize = 400;  // limit on how big our allocation table can grow...

		/// <summary>
		/// Keeps track of all the memory YeloDebug uses.
		/// </summary>
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		List<AllocationEntry> AllocationTable = new List<AllocationEntry>();

		/// <summary>
		/// Prevents YeloDebug from syncing its allocation table with the xbox.
		/// This is usefull if you are confident that you wont lose your connection,
		/// then you can unblock when you wish to manually sync again.  Otherwise, YeloDebug 
		/// will sync after each memory allocation or release.
		/// </summary>
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		bool BlockAllocationTableSync = false;      // only update on pc

		/// <summary>
		/// Gets or sets whether or not allocation table will be kept in sync with the xbox.
		/// </summary>
		[Browsable(false)]
        public bool AllocationTableSyncing { get { return allocationTableSyncing; } set { allocationTableSyncing = value; } }
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		bool allocationTableSyncing = true;

		/// <summary>
		/// Reads the allocation table from the xbox.
		/// </summary>
		/// <returns></returns>
		List<AllocationEntry> LoadAllocationTable()
		{
			List<AllocationEntry> allocationTable = new List<AllocationEntry>();

			// gets the number of entries in our table
			uint tableCount = GetUInt32(XboxHistory.AllocationTable.CountAddress);
			if (tableCount == 0)
				return new List<AllocationEntry>();
			else if (tableCount > MaxAllocTableSize)
				throw new Exception("Allocation table corruption."); // maximum allocation count has either been exceeded, or theres a corruption

			// read the allocation table from xbox memory
			byte[] allocBuffer = GetMemory(XboxHistory.AllocationTable.BufferAddress, tableCount * 9);
			BinaryReader alloc = new BinaryReader(new System.IO.MemoryStream(allocBuffer));
			alloc.BaseStream.Position = 0;

			// build our alloc table
			for (int i = 0; i < tableCount; i++)
			{
				// check for unaligned addresses as signs of a corrupted table
				uint address = alloc.ReadUInt32();
				if ((address & 0xFFF) > 0)
					throw new Exception("Allocation table corruption.");

				// check for sizes greater than 128mb as signs of corrupted data
				uint size = alloc.ReadUInt32();
				if (size > 0x8000000)
					throw new Exception("Allocation table corruption.");

				// check for invalid type as signs of corrupted data
				AllocationType type = (AllocationType)alloc.ReadByte();
				if ((byte)type > 3)
					throw new Exception("Allocation table corruption.");

				// otherwise add to table
				if (allocationTable.Count < MaxAllocTableSize)
					allocationTable.Add(new AllocationEntry(address, size, type));
			}
			// possibly check if those addresses are still allocated (kernel.isAddressValid), if failed, 
			// remove from table and resave to xbox at the expense of higher network traffic :X

			return allocationTable;
		}

		public void ReloadAllocationTable()
		{
			AllocationTable = LoadAllocationTable();
		}



		/// <summary>
		/// Syncs the allocation table with the xbox.
		/// </summary>
		void SyncAllocationTable()
		{
			// dont update if blocked
			if (BlockAllocationTableSync)
				return;

			// buffer to hold our allocation table
			byte[] allocBuffer = new byte[AllocationTable.Count * 9 + 4];
			using (var alloc = new BinaryWriter(new System.IO.MemoryStream(allocBuffer)))
			{
				alloc.Write(AllocationTable.Count); // prefixed with total count

				// build our alloc table
				foreach (AllocationEntry entry in AllocationTable)
				{
					alloc.Write(entry.Address);
					alloc.Write(entry.Size);
					alloc.Write((byte)entry.Type);
				}

				// store our table in xbox memory
				SetMemory(XboxHistory.AllocationTable.CountAddress, allocBuffer);
			}
		}

		/// <summary>
		/// Removes an entry from the allocation table.
		/// </summary>
		/// <param name="address"></param>
		/// <returns>Allocation size.</returns>
		uint RemoveAllocationEntry(uint address)
		{
			uint size = 0;
			for (int i = 0; i < AllocationTable.Count; i++)
			{
				if (AllocationTable[i].Address == address)  // destroys any duplicates...although there shouldnt be any! ;P
				{
					size = AllocationTable[i].Size;
					AllocationTable.RemoveAt(i);
					i--;
				}
			}

			// update our information page
			SyncAllocationTable();

			return size;
		}

		/// <summary>
		/// Makes sure the given allocation exists.
		/// </summary>
		/// <param name="address"></param>
		/// <returns>Allocation table index.</returns>
		int AssertAllocationExists(uint address)
		{
			for (int i = 0; i < AllocationTable.Count; i++)
				if (address == AllocationTable[i].Address)
					return i;
			throw new Exception("Allocation does not exist.");
		}

		/// <summary>
		/// Makes sure the given allocation does not exist.
		/// </summary>
		/// <param name="address"></param>
		/// <returns></returns>
		void AssertAllocationNonexistant(uint address)
		{
			foreach (AllocationEntry entry in AllocationTable)
				if (address == entry.Address)
					throw new Exception("Allocation already exists.");
			return;
		}

		/// <summary>
		/// Determines a given allocations type.
		/// </summary>
		/// <param name="address"></param>
		/// <returns></returns>
		AllocationType GetAllocationType(uint address)
		{
			int index = AssertAllocationExists(address);
			return AllocationTable[index].Type;
		}

		/// <summary>
		/// Allocates memory on the xbox.
		/// </summary>
		/// <param name="size"></param>
		/// <returns>Allocated address.</returns>
		public uint AllocateMemory(uint size) { return AllocateVirtualMemory(size); }

		/// <summary>
		/// Allocates memory of specified type on the xbox.
		/// </summary>
		/// <param name="type"></param>
		/// <param name="size"></param>
		/// <returns>Allocated address.</returns>
		public uint AllocateMemory(AllocationType type, uint size)
		{
			switch (type)
			{
				case AllocationType.Debug: return AllocateDebugMemory(size);
				case AllocationType.Physical: return AllocatePhysicalMemory(size);
				case AllocationType.System: return AllocateSystemMemory(size);
				case AllocationType.Virtual: return AllocateVirtualMemory(size);
				default: throw new Exception("Invalid allocation type.");
			}
		}

		/// <summary>
		/// Frees xbox memory.
		/// </summary>
		/// <param name="address"></param>
		/// <returns>Size of freed memory.</returns>
		public uint FreeMemory(uint address)
		{
			// frees memory based on allocation type
			switch (GetAllocationType(address))
			{
				case AllocationType.Debug: return FreeDebugMemory(address);
				case AllocationType.Physical: return FreePhysicalMemory(address);
				case AllocationType.System: return FreeSystemMemory(address);
				case AllocationType.Virtual: return FreeVirtualMemory(address);
				default: return 0;   // shouldnt reach here
			}
		}

		/// <summary>
		/// Frees all memory associated with the current allocation table.
		/// </summary>
		/// <returns>Total memory freed.</returns>
		public uint FreeAllMemory()
		{
			// we block so it wont have to save after each free
			// hopefully the connection wont drop inbetween ;P
			BlockAllocationTableSync = true;
			uint totalFreed = 0;

			try
			{
				for (int i = 0; i < AllocationTable.Count; i++)
				{
					totalFreed += FreeMemory(AllocationTable[i].Address);
					i--;
				}
			}
            finally // dont catch the exception, let it propagate
			{
				BlockAllocationTableSync = false; // but be sure to unblock no matter what ;)

				// update our information page
				SyncAllocationTable();
			}

			// return size of memory freed
			return totalFreed;
		}

		/// <summary>
		/// Determines if the requested size of memory can be allocated.  This function will also borrow from the file cache to meet the requested size if needed.
		/// </summary>
		/// <param name="requestedSize"></param>
		/// <returns></returns>
		public bool IsEnoughMemory(uint requestedSize)
		{
			// determine number of pages needed
			uint pagesNeeded = requestedSize / 0x1000;

			uint pagesAvailable = MemoryStatistics.AvailablePages;

			// tests for low memory (todo: try not to take all available pages...leave at least 1)
			if (pagesAvailable < pagesNeeded)
			{
				uint availableCache = GetFileCacheSize() - 1;

				// try to borrow from filecache ;)
				if ((pagesAvailable + availableCache) > pagesNeeded)
					SetFileCacheSize((availableCache + 1) - (pagesNeeded - pagesAvailable));
				else throw new OutOfMemoryException("Failed to allocate additional xbox memory.");  // cant steal any more from filecache ;(
			}
			return true;
		}


		/// <summary>
		/// Allocates debug memory on the xbox. Address space of 0xB0000000 to 0xC0000000.
		/// </summary>
		/// <param name="size">Size of memory to be allocated.  Note that an unaligned size will be rounded up to the next 4kb page boundary.</param>
		/// <returns>Address of allocated memory.</returns>
		public uint AllocateDebugMemory(uint size)
		{
			// calculate actual size of allocation
			size = Util.GetAlignedPageBoundary(size);

			// checks if theres enough memory for allocation to take place
			IsEnoughMemory(size);

			// allocate our debug memory
			uint ptr = (uint)CallAddressEx(Kernel.MmDbgAllocateMemory, null, true, size, 4);

			// add to our allocation table if allocation succeeded
			if (ptr == 0)
				throw new Exception("Xbox memory allocation failed.");
			if (AllocationTable.Count > MaxAllocTableSize)
				throw new Exception("Maximum allocation count has been reached.");
			if (allocationTableSyncing)
				AllocationTable.Add(new AllocationEntry(ptr, size, AllocationType.Debug));

			// update our information page
			SyncAllocationTable();

			return ptr;
		}

		/// <summary>
		/// Frees debug memory on the xbox.
		/// </summary>
		/// <param name="address">Memory address.</param>
		/// <returns>Size of freed memory.</returns>
		public uint FreeDebugMemory(uint address)
		{
			// make sure allocation exists
			AssertAllocationExists(address);

			// make sure we are only trying to free debug memory
			if (GetAllocationType(address) != AllocationType.Debug)
				throw new Exception("Attempting to free memory of a different type.");

			// returns pages freed
			uint result = (uint)CallAddressEx(Kernel.MmDbgFreeMemory, null, true, address, 0);
			if (result == 0)
				throw new Exception("Failure to free xbox memory.");

			// return size of memory freed
			return RemoveAllocationEntry(address);
		}

		/// <summary>
		/// Allocates physical memory on the xbox.  Address space of 0x80000000 to 0x84000000.  Extends to 0x88000000 on a developer kit.
		/// </summary>
		/// <param name="size">Size of memory to be allocated.  Note that an unaligned size will be rounded up to the next 4kb page boundary.</param>
		/// <returns>Address of allocated memory.</returns>
		public uint AllocatePhysicalMemory(uint size)
		{
			// calculate actual size of allocation
			size = Util.GetAlignedPageBoundary(size);

			// checks if theres enough memory for allocation to take place
			IsEnoughMemory(size);

			// allocate the physical memory
			uint ptr = (uint)CallAddressEx(Kernel.MmAllocateContiguousMemory, null, true, size);

			// add to our allocation table if allocation succeeded
			if (ptr == 0)
				throw new Exception("Xbox memory allocation failed.");
			if (AllocationTable.Count > MaxAllocTableSize)
				throw new Exception("Maximum allocation count has been reached.");
			if (allocationTableSyncing)
				AllocationTable.Add(new AllocationEntry(ptr, size, AllocationType.Physical));

			// update our information page
			SyncAllocationTable();

			return ptr;
		}

		/// <summary>
		/// Allocates physical memory on the xbox.  Input range of 0x0 - 0x4000000.  Xbox address space of 0x80000000 to 0x84000000.  Extends to 0x88000000 on a developer kit.
		/// </summary>
		/// <param name="size">Size of memory to be allocated.  Note that an unaligned size will be rounded up to the next 4kb page boundary.</param>
		/// <param name="range">Range of memory to place allocation.</param>
		/// <returns>Address of allocated memory.</returns>
		public uint AllocatePhysicalMemoryEx(uint size, AddressRange range)
		{
			// calculate actual size of allocation
			size = Util.GetAlignedPageBoundary(size);

			// checks if theres enough memory for allocation to take place
			IsEnoughMemory(size);

			// allocate the physical memory
			uint ptr = (uint)CallAddressEx(Kernel.MmAllocateContiguousMemoryEx, null, true, size, range.Low, range.High, 0, 4);

			// add to our allocation table if allocation succeeded
			if (ptr == 0)
				throw new Exception("Xbox memory allocation failed.");
			if (AllocationTable.Count > MaxAllocTableSize)
				throw new Exception("Maximum allocation count has been reached.");
			if (allocationTableSyncing)
				AllocationTable.Add(new AllocationEntry(ptr, size, AllocationType.Physical));

			// update our information page
			SyncAllocationTable();

			return ptr;
		}

		/// <summary>
		/// Frees physical memory on the xbox.
		/// </summary>
		/// <param name="address">Memory address.</param>
		/// <returns>Size of freed memory.</returns>
		public uint FreePhysicalMemory(uint address)
		{
			// make sure allocation exists
			AssertAllocationExists(address);

			// make sure we are only trying to free physical memory
			if (GetAllocationType(address) != AllocationType.Physical)
				throw new Exception("Attempting to free memory of a different type.");

			// free the physical memory
			uint result = (uint)CallAddressEx(Kernel.MmFreeContiguousMemory, null, true, address);
			if (result != 0)
				throw new Exception("Failure to free xbox memory.");

			// return size of memory freed
			return RemoveAllocationEntry(address);
		}

		/// <summary>
		/// Allocates virtual memory on the xbox.  Address space of 0x00000000 to 0x80000000.
		/// </summary>
		/// <param name="size">Size of memory to be allocated.  Note that an unaligned size will be rounded up to the next 4kb page boundary.</param>
		/// <returns>Address of allocated memory.</returns>
		public uint AllocateVirtualMemory(uint size)
		{
			// calculate actual size of allocation
			size = Util.GetAlignedPageBoundary(size);

			// checks if theres enough memory for allocation to take place
			IsEnoughMemory(size);

			// prepares arguments
			uint pSize = XboxHistory.RemoteExecution.ArgumentBuffer;
			uint pAddress = XboxHistory.RemoteExecution.ArgumentBuffer + 4;
			SetMemory(pSize, size); //pSize
			SetMemory(pAddress, 0);  //pAddress

			// allocates virtual memory
			uint result = (uint)CallAddressEx(Kernel.NtAllocateVirtualMemory, null, true, pAddress, 0, pSize, 0x1000, 4);
			uint ptr = GetUInt32(pAddress);

			// add to our allocation table if allocation succeeded
			if (result != 0)
				throw new Exception("Xbox memory allocation failed.");
			if (AllocationTable.Count > MaxAllocTableSize)
				throw new Exception("Maximum allocation count has been reached.");
			if (allocationTableSyncing)
				AllocationTable.Add(new AllocationEntry(ptr, size, AllocationType.Virtual));

			// update our information page
			SyncAllocationTable();

			return ptr;
		}

		/// <summary>
		/// Allocates virtual memory on the xbox.  Address space of 0x00000000 to 0x80000000.
		/// </summary>
		/// <param name="size">Size of memory to be allocated.  Note that an unaligned size will be rounded up to the next 4kb page boundary.</param>
		/// <param name="address">Preferred address.</param>
		/// <returns>Address of allocated memory.</returns>
		public uint AllocateVirtualMemoryEx(uint size, uint address)
		{
			// if they want to allocate at address 0x0, and not want the function to determine where for them ;P
			address |= 1;

			// make sure allocation doesnt already exist
			AssertAllocationNonexistant(address & 0xFFFFF000);  // check for the address it will try to allocate...

			// calculate actual size of allocation
			size = Util.GetAlignedPageBoundary(size);

			// checks if theres enough memory for allocation to take place
			IsEnoughMemory(size);

			// prepares arguments
			uint pSize = XboxHistory.RemoteExecution.ArgumentBuffer;
			uint pAddress = XboxHistory.RemoteExecution.ArgumentBuffer + 4;
			SetMemory(pSize, size); //pSize
			SetMemory(pAddress, address);  //pAddress

			// reserves virtual memory - for some reason you need to do this first when specifying your own address...
			// took forever to figure this one out ;x
			uint res = (uint)CallAddressEx(Kernel.NtAllocateVirtualMemory, null, true, pAddress, 0, pSize, MEMORY_FLAGS.MEM_RESERVE, 4);
			if (res != 0) throw new Exception("Xbox memory allocation failed.");

			// prepares arguments
			SetMemory(pSize, size); //pSize
			SetMemory(pAddress, address);  //pAddress

			// allocates virtual memory
			uint result = (uint)CallAddressEx(Kernel.NtAllocateVirtualMemory, null, true, pAddress, 0, pSize, MEMORY_FLAGS.MEM_COMMIT, 4);
			uint ptr = GetUInt32(pAddress);

			// add to our allocation table if allocation succeeded
			if (result != 0)
				throw new Exception("Xbox memory allocation failed.");
			if (AllocationTable.Count > MaxAllocTableSize)
				throw new Exception("Maximum allocation count has been reached.");
			if (allocationTableSyncing)
				AllocationTable.Add(new AllocationEntry(ptr, size, AllocationType.Virtual));

			// update our information page
			SyncAllocationTable();

			return ptr;
		}

		/// <summary>
		/// Frees virtual memory on the xbox.
		/// </summary>
		/// <param name="address">Memory address.</param>
		/// <returns>Size of freed memory.</returns>
		public uint FreeVirtualMemory(uint address)
		{
			// make sure allocation exists
			AssertAllocationExists(address);

			// make sure we are only trying to free virtual memory
			if (GetAllocationType(address) != AllocationType.Virtual)
				throw new Exception("Attempting to free memory of a different type.");

			// prepares arguments
			uint pSize = XboxHistory.RemoteExecution.ArgumentBuffer;
			uint pAddress = XboxHistory.RemoteExecution.ArgumentBuffer + 4;
			SetMemory(pSize, 0); //pSize
			SetMemory(pAddress, address);  //pAddress

			// free the virtual memory
			uint result = (uint)CallAddressEx(Kernel.NtFreeVirtualMemory, null, true, pAddress, pSize, 0x8000);
			if (result != 0)
				throw new Exception("Failure to free xbox memory.");

			// return size of memory freed
			return RemoveAllocationEntry(address);
		}

		/// <summary>
		/// Allocates system memory on the xbox.  Address space of 0xD0000000 to 0xFFFFFFFC.
		/// </summary>
		/// <param name="size">Size of memory to be allocated.  Note that an unaligned size will be rounded up to the next 4kb page boundary.</param>
		/// <returns>Address of allocated memory.</returns>
		public uint AllocateSystemMemory(uint size)
		{
			// calculate actual size of allocation
			size = Util.GetAlignedPageBoundary(size);

			// checks if theres enough memory for allocation to take place
			IsEnoughMemory(size);

			// allocates system memory
			uint ptr = (uint)CallAddressEx(Kernel.MmAllocateSystemMemory, null, true, size, 4);

			// add to our allocation table if allocation succeeded
			if (ptr == 0)
				throw new Exception("Xbox memory allocation failed.");
			if (AllocationTable.Count > MaxAllocTableSize)
				throw new Exception("Maximum allocation count has been reached.");
			if (allocationTableSyncing)
				AllocationTable.Add(new AllocationEntry(ptr, size, AllocationType.System));

			// update our information page
			SyncAllocationTable();

			return ptr;
		}

		/// <summary>
		/// Frees system memory on the xbox.
		/// </summary>
		/// <param name="address">Memory address.</param>
		/// <returns>Size of freed memory.</returns>
		public uint FreeSystemMemory(uint address)
		{
			// make sure allocation exists
			AssertAllocationExists(address);

			// make sure we are only trying to free system memory
			if (GetAllocationType(address) != AllocationType.System)
				throw new Exception("Attempting to free memory of a different type.");

			// returns pages freed
			uint result = (uint)CallAddressEx(Kernel.MmFreeSystemMemory, null, true, address, 0);
			if (result == 0)
				throw new Exception("Failure to free xbox memory.");

			// return size of memory freed
			return RemoveAllocationEntry(address);
		}

		MEMORY_BASIC_INFORMATION QueryVirtualMemory(uint ptr)
		{
			MEMORY_BASIC_INFORMATION info;
			uint result = (uint)CallAddressEx(Kernel.NtQueryVirtualMemory, null, true, ptr, 0x10008);

			info.BaseAddress = GetUInt32(0x10008);
			info.AllocationBase = GetUInt32(0x1000C);
			info.AllocationProtect = (MEMORY_FLAGS)GetUInt32(0x10010);
			info.RegionSize = GetUInt32(0x10014);
			info.State = (MEMORY_FLAGS)GetUInt32(0x10018);
			info.Protect = (MEMORY_FLAGS)GetUInt32(0x1001C);
			info.Type = (MEMORY_FLAGS)GetUInt32(0x10020);

			return info;
		}

		// could use this to set our pages to noaccess after we are done reading/writing...
		// might be a bit time consuming to give ourselves permission and then deny everyone else, every time we need to access our info...
		// worst case scenario of two calls for one byte read/written...is it worth it?
		void VirtualProtect(uint address, uint size, uint newProtect, out uint oldProtect)
		{
			uint pOldProtect = XboxHistory.RemoteExecution.ArgumentBuffer;
			uint pSize = XboxHistory.RemoteExecution.ArgumentBuffer + 4;
			uint pAddress = XboxHistory.RemoteExecution.ArgumentBuffer + 8;
			SetMemory(pSize, size);
			SetMemory(pAddress, address);

			uint result = (uint)CallAddressEx(Kernel.NtProtectVirtualMemory, null, true, pAddress, pSize, newProtect, pOldProtect);
			oldProtect = GetUInt32(pOldProtect);
		}

		/// <summary>
		/// Retrieves the file cache size associated with the current title.
		/// </summary>
		/// <returns></returns>
		public uint GetFileCacheSize()
		{
			return (uint)CallAddress(Kernel.FscGetCacheSize, true);
		}

		/// <summary>
		/// Allows us to "borrow" extra memory from the file cache :)
		/// </summary>
		/// <param name="size">Page count.</param>
		/// <returns></returns>
		public bool SetFileCacheSize(uint size)
		{
			//make sure you dont take all of the pages, although perfectly acceptable, some games will definitely freak...
			if (size == 0) size = 1;
			return (uint)CallAddressEx(Kernel.FscSetCacheSize, null, true, size) == 0;
		}

		///// <summary>
		///// Restores the file cache by taking some of the available memory and giving it back to the cache.
		///// </summary>
		///// <returns></returns>
		//private void RestoreFileCache()
		//{
		//    // dont take all available, leave at least a page or two
		//    // restore to the default of 16 pages (64kb)

		//}
		#endregion
	};
}
/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Halo3
{
	// bitmap: 0xA4, @ 0x68:0x48, @ 0x8C:0x8

	#region Header
	/// <summary>
	/// Bit values for use in <see cref="Halo3.CacheHeader"/>'s 
	/// <see cref="Blam.Cache.CacheHeaderGen3.Flags"/> field
	/// </summary>
	public static class CacheHeaderFlags
	{
		/// <summary>
		/// Cache has external resources in 'mainmenu.map'
		/// </summary>
		public const uint DependsOnMainMenu = 1 << 0;
		/// <summary>
		/// Cache has external resources in 'shared.map'
		/// </summary>
		public const uint DependsOnShared = 1 << 1;
		/// <summary>
		/// Cache has external resources in 'campaign.map'
		/// </summary>
		public const uint DependsOnCampaign = 1 << 2;
	};

	/// <summary>
	/// Halo 3 implementation of the <see cref="Blam.CacheHeader"/>
	/// </summary>
	public class CacheHeader : Cache.CacheHeaderGen3
	{
		const int kSizeOf = 0x3000;

		internal System.Runtime.InteropServices.ComTypes.FILETIME Filetime;


		void ReadBeta(BlamLib.IO.EndianReader s)
		{
			s.ReadUInt32();

			//s.ReadBool();
			//s.ReadBool(); // false if it belongs to a untracked build
			//s.ReadBool();
			//s.ReadBool();
			// Just read the booleans as a fucking int...
			s.ReadInt32();

			// Filetime.dwHighDateTime = s.ReadInt32();
			// Filetime.dwLowDateTime = s.ReadInt32();
			s.ReadInt32(); s.ReadInt32();

			s.ReadInt32(); s.ReadInt32(); s.ReadInt32();

			#region string id
			// retail doesn't include the 128 byte per string id table, so nix the members
			/*stringIdsBufferAlignedOffset = */s.ReadInt32();
			stringIdsCount = s.ReadInt32();

			stringIdsBufferSize = s.ReadInt32();
			stringIdIndicesOffset = s.ReadInt32();
			stringIdsBufferOffset = s.ReadInt32();
			#endregion

			#region filetimes?
			// pretty sure this is a flags field
			// used to tell which of the following 64bit values
			// are used. Damn sure this are FILETIME structures, but
			// hex workshop doesn't like them so I can't be for sure...
			s.ReadInt32();
			Filetime.dwHighDateTime = s.ReadInt32();
			Filetime.dwLowDateTime = s.ReadInt32();
			if (s.ReadInt32() != 0) flags.Add(CacheHeaderFlags.DependsOnMainMenu); s.ReadInt32();
			if (s.ReadInt32() != 0) flags.Add(CacheHeaderFlags.DependsOnShared); s.ReadInt32();
			if (s.ReadInt32() != 0) flags.Add(CacheHeaderFlags.DependsOnCampaign); s.ReadInt32();
			#endregion

			name = s.ReadTagString();
			s.ReadInt32();
			scenarioPath = s.ReadAsciiString(256);
			needsShared = s.ReadInt32() == 1;

			#region tag names
			tagNamesCount = s.ReadInt32();
			tagNamesBufferOffset = s.ReadInt32();
			tagNamesBufferSize = s.ReadInt32();
			tagNameIndicesOffset = s.ReadInt32();
			#endregion

			checksum = s.ReadUInt32(); // 0x2CC
			s.Seek(32, System.IO.SeekOrigin.Current); // these bytes are always the same

			baseAddress = s.ReadUInt32(); // expected base address
			xdkVersion = s.ReadInt32(); // xdk version

			#region memory partitions
			memoryPartitions = new Partition[3];
			memoryPartitions[0].BaseAddress = s.ReadUInt32(); // address
			memoryPartitions[0].Size = s.ReadInt32(); // size
			memoryPartitions[1].BaseAddress = s.ReadUInt32(); // address
			memoryPartitions[1].Size = s.ReadInt32(); // size
			memoryPartitions[2].BaseAddress = s.ReadUInt32(); // address
			memoryPartitions[2].Size = s.ReadInt32(); // size
			#endregion

			s.Seek(256, System.IO.SeekOrigin.Current);

			s.Seek(1004 + sizeof(uint), System.IO.SeekOrigin.Current);


			Managers.BlamDefinition bdef = Program.GetManager(BlamVersion.Halo3_Beta);

			uint base_address = (s.Owner as Blam.CacheFile).AddressMask = bdef[BlamVersion.Halo3_Beta].CacheTypes.BaseAddress - (uint)MemoryBufferOffset;

			offsetToIndex = (int)(tagIndexAddress - base_address);
			CalculatePartitionOffsets();
		}

		public override void Read(BlamLib.IO.EndianReader s)
		{
			bool is_beta = false;
			bool is_odst = !((s.Owner as Blam.CacheFile).EngineVersion == BlamVersion.Halo3);
			Blam.CacheFile.ValidateHeader(s, kSizeOf);

			s.Seek(4);
			version = s.ReadInt32(); is_beta = version == 9;
			if (version != 9 && version != 11) throw new InvalidCacheFileException(s.FileName);
			fileLength = s.ReadInt32();
			s.ReadInt32();
			tagIndexAddress = s.ReadUInt32();
			memoryBufferOffset = s.ReadInt32();
			memoryBufferSize = s.ReadInt32();

			sourceFile = s.ReadAsciiString(256);
			build = s.ReadTagString();
			cacheType = (Blam.CacheType)s.ReadInt16();

			if (is_beta)
			{
				s.ReadInt16(); sharedType = Cache.SharedType.Beta;
				ReadBeta(s);
				return;
			}
			else
				sharedType = (Cache.SharedType)s.ReadInt16();

			s.ReadBool();
			s.ReadBool(); // false if it belongs to a untracked build
			s.ReadBool();
			s.ReadByte(); // appears to be an ODST-only field

			s.ReadInt32(); s.ReadInt32();
			s.ReadInt32(); s.ReadInt32(); s.ReadInt32();

			#region string id table
			// 0x158
			// old 128 byte aligned string table not used after the Delta builds of Halo 3

			stringIdsCount = s.ReadInt32();
			stringIdsBufferSize = s.ReadInt32(); // cstring buffer total size in bytes
			stringIdIndicesOffset = s.ReadInt32();
			stringIdsBufferOffset = s.ReadInt32(); // cstring buffer
			#endregion

			#region filetimes?
			// pretty sure this is a flags field
			// used to tell which of the following 64bit values
			// are used. Damn sure this are FILETIME structures, but
			// hex workshop doesn't like them so I can't be for sure...
			needsShared = s.ReadInt32() != 0; // just a little 'hack' if you will. if zero, the map is self reliant, so no worries
			Filetime.dwHighDateTime = s.ReadInt32();
			Filetime.dwLowDateTime = s.ReadInt32();
			if (s.ReadInt32() != 0) flags.Add(CacheHeaderFlags.DependsOnMainMenu); s.ReadInt32();
			if (s.ReadInt32() != 0) flags.Add(CacheHeaderFlags.DependsOnShared); s.ReadInt32();
			if (s.ReadInt32() != 0) flags.Add(CacheHeaderFlags.DependsOnCampaign); s.ReadInt32();
			#endregion

			name = s.ReadTagString();
			s.ReadInt32();
			scenarioPath = s.ReadAsciiString(256);
			s.ReadInt32(); // minor version, normally not used

			#region tag names
			tagNamesCount = s.ReadInt32();
			tagNamesBufferOffset = s.ReadInt32(); // cstring buffer
			tagNamesBufferSize = s.ReadInt32(); // cstring buffer total size in bytes
			tagNameIndicesOffset = s.ReadInt32();
			#endregion

			checksum = s.ReadUInt32(); // 0x2C4
			s.Seek(32, System.IO.SeekOrigin.Current); // these bytes are always the same

			baseAddress = s.ReadUInt32(); // expected base address
			xdkVersion = s.ReadInt32(); // xdk version

			#region memory partitions
			// 0x2E8

			// memory partitions
			memoryPartitions = new Partition[6];
			memoryPartitions[0].BaseAddress = s.ReadUInt32(); // cache resource buffer
			memoryPartitions[0].Size = s.ReadInt32();

			// readonly
			memoryPartitions[1].BaseAddress = s.ReadUInt32(); // sound cache resource buffer
			memoryPartitions[1].Size = s.ReadInt32();

			memoryPartitions[2].BaseAddress = s.ReadUInt32(); // global tags buffer
			memoryPartitions[2].Size = s.ReadInt32();
			memoryPartitions[3].BaseAddress = s.ReadUInt32(); // shared tag blocks?
			memoryPartitions[3].Size = s.ReadInt32();
			memoryPartitions[4].BaseAddress = s.ReadUInt32(); // address
			memoryPartitions[4].Size = s.ReadInt32();

			// readonly
			memoryPartitions[5].BaseAddress = s.ReadUInt32(); // map tags buffer
			memoryPartitions[5].Size = s.ReadInt32();
			#endregion

			int count = s.ReadInt32();
			s.Seek(12, System.IO.SeekOrigin.Current); // these bytes are always the same
			// if there is a hash in the header, this is the ONLY
			// place where it can be
			s.Seek(20 /*SHA1*/ + 40 + 256 /*RSA*/, System.IO.SeekOrigin.Current); // ???

			// 0x46C
			cacheInterop.Read(s);
			cacheInterop.PostprocessForCacheRead(kSizeOf);

			s.Seek(16, System.IO.SeekOrigin.Current); // GUID?, these bytes are always the same. ODST is different from Halo 3

			#region blah 1
			// 0x4AC

			// campaign has a shit load of these
			// but shared doesn't nor mainmenu
			// I compared the sc110 french and english and both have the SAME counts and element data. So 
			// I don't think this is a hash or something. At least, if it is, it's not runtime relative so 
			// nothing we have to worry about

			s.ReadInt16(); // I've only seen this be two different values (besides zero).
			count = s.ReadInt16();
			s.ReadInt32(); // seems to always be zero
			CompressionGuid = new Guid(s.ReadBytes(16));

			s.Seek(count * 28, System.IO.SeekOrigin.Current); // seek past the elements
			// dword
			// long
			// buffer [0x14] (probably a sha1 hash)
			s.Seek((320 - count) * 28, System.IO.SeekOrigin.Current); // seek past the unused elements
			#endregion

			#region blah 2
			{
				// 0x27C4
				// This on the other hand, sc110 french and english had MINOR differences. Those differences were in 
				// DWORDs @ 0x4 and 0x8

				// 005 - 1
				// 010 - 3
				// 020 - 3
				// 030 - 2
				// 040 - 2
				// 050 - 2
				// 070 - 3
				// 100 - 3
				// 110 - 2
				// 120 - 2
				// 130 - 1

				// c100  - 1
				// c200  - 1
				// h100  - 9
				// l200  - 7
				// l300  - 8
				// sc100 - 5
				// sc110 - 5
				// sc120 - 6
				// sc130 - 6
				// sc140 - 6
				// sc150 - 6

				// going to punt and just assume there is a max count of 10 of these possible

				// maybe related to bsp\'zones'?
				int blah2_sizeof = !is_odst ? 172 : 180;

				count = (int)(s.ReadUInt32() >> 24); // did someone forget to fucking byte swap something?
				s.Seek(count * blah2_sizeof, System.IO.SeekOrigin.Current); // seek past the elements
				s.Seek((10 - count) * blah2_sizeof, System.IO.SeekOrigin.Current); // seek past the unused elements
			}
			#endregion

			s.Seek((!is_odst ? 380 : 300) + sizeof(uint), System.IO.SeekOrigin.Current); // zero


			ReadPostprocessForInterop();

			ReadPostprocessForBaseAddresses(s);
		}
	};
	#endregion

	#region Index
	/// <summary>
	/// Halo 3 engine base implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public abstract class CacheIndexBase : Blam.Cache.CacheIndexGen3
	{
		#region GroupTags
		protected abstract Cache.CacheItemGroupTagGen3[] CreateEngineItemGroupTags(int count);
		protected abstract Cache.CacheItemGroupTagGen3 CreateEngineItemGroupTag();
		#endregion

		public override void Read(BlamLib.IO.EndianReader s)
		{
			CacheFileBase cache = s.Owner as CacheFileBase;

			#region read body
			groupTagsCount = s.ReadInt32();
			groupTagsOffset = (groupTagsAddress = s.ReadUInt32()) - cache.AddressMask;

			tagCount = s.ReadInt32();
			tagsOffset = (address = s.ReadUInt32()) - cache.AddressMask;

			dependentTagsCount = s.ReadInt32();
			dependentTagsOffset = (dependentTagsAddress = s.ReadUInt32()) - cache.AddressMask;

			s.ReadInt32();
			s.ReadUInt32(); // seems to be the same address as the dependent tags

			s.ReadUInt32(); // crc?
			s.ReadInt32(); // 'tags'
			#endregion

			ReadDependents(s);

			ReadTagInstances(s);

			ReadGroupTags(s);

			#region fixup group tag info
			for (int x = 0; x < tagCount; x++)
			{
				var item = items[x];
				if (!item.IsEmpty)
				{
					item.GroupTag = groupTags[item.Datum.Index].GroupTag1;
					item.GroupTagInt = item.GroupTag.ID;
					item.Fixup((ushort)x); // Fix the hacks Bungie did for Halo3
				}
			}
			#endregion

			#region Load tag names
			s.Seek(cache.HeaderHalo3.TagNamesBufferOffset);
			string file_name;
			foreach (var ci in items)
			{
				if (!ci.IsEmpty)
				{
					ci.TagNameOffset = s.PositionUnsigned;
					file_name = s.ReadCString();
					if (file_name == "") file_name = "NONE";
				}
				else
				{
					ci.TagNameOffset = uint.MaxValue;
					ci.ReferenceName = DatumIndex.Null;
					continue;
				}

				ci.ReferenceName = cache.References.AddOptimized(ci.GroupTag, file_name);
			}
			#endregion
		}

		void ReadGroupTags(BlamLib.IO.EndianReader s)
		{
			groupTags = CreateEngineItemGroupTags(groupTagsCount);
			s.Seek(groupTagsOffset, System.IO.SeekOrigin.Begin);
			for (int x = 0; x < groupTagsCount; x++)
				(groupTags[x] = CreateEngineItemGroupTag()).Read(s);
		}
	};

	/// <summary>
	/// Halo 3 implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public sealed class CacheIndex : CacheIndexBase
	{
		protected override Cache.CacheItemGroupTagGen3[] CreateEngineItemGroupTags(int count)
		{
			return new CacheItemGroupTag[count];
		}

		protected override Cache.CacheItemGroupTagGen3 CreateEngineItemGroupTag()
		{
			return new CacheItemGroupTag();
		}
	};
	#endregion

	#region ItemGroupTag
	public sealed class CacheItemGroupTag : Cache.CacheItemGroupTagGen3
	{
		public override void Read(BlamLib.IO.EndianReader s)
		{
			base.ReadGroupTags(Program.Halo3.Manager, s);

			Name.Read(s, (s.Owner as CacheFile).StringIds.Definition.Description);
		}
	};
	#endregion

	#region File
	/// <summary>
	/// Halo 3 engine base implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public abstract class CacheFileBase : Blam.Cache.CacheFileGen3
	{
		#region Header
		protected Halo3.CacheHeader cacheHeader = null;
		public override BlamLib.Blam.CacheHeader Header { get { return cacheHeader; } }

		public Halo3.CacheHeader HeaderHalo3 { get { return cacheHeader; } }
		#endregion

		public override string GetUniqueName()
		{
			var ft = HeaderHalo3.Filetime;
			return string.Format("{0}_{1}{2}", HeaderHalo3.Name, ft.dwHighDateTime.ToString("X8"), ft.dwLowDateTime.ToString("X8"));
		}

		#region Index
		protected Halo3.CacheIndexBase cacheIndex = null;
		public override BlamLib.Blam.CacheIndex Index { get { return cacheIndex; } }

		public Halo3.CacheIndexBase IndexHalo3 { get { return cacheIndex; } }
		#endregion

		protected abstract bool SharableReferenceXbox(string path);
		protected virtual bool SharableReferencePc(string path) { return false; }

		protected CacheFileBase(string map_name)
		{
			if (!SharableReferenceXbox(map_name) && !SharableReferencePc(map_name))
			{
				InputStream = new IO.EndianReader(map_name, IO.EndianState.Big, this);
				if (!CacheIsReadonly(map_name))
					OutputStream = new IO.EndianWriter(map_name, IO.EndianState.Big, this);
			}

			cacheHeader = new CacheHeader();
		}

		internal override void ReadResourceCache()
		{
			if (isLoaded) return;

			if (engineVersion == BlamVersion.Halo3_Beta) return;
			cacheHeader.Read(InputStream);

			isLoaded = true;
		}

		protected abstract void DetermineEngineVersion();

		protected void TagIndexInitializeAndRead(BlamLib.IO.EndianReader s)
		{
			base.StringIdManagerInitializeAndRead();
			base.InitializeReferenceManager(s.FileName);
			base.InitializeTagIndexManager();

			s.Seek(cacheHeader.OffsetToIndex, System.IO.SeekOrigin.Begin);
			cacheIndex.Read(s);
		}

		public override void Read(BlamLib.IO.EndianReader s)
		{
			if (isLoaded) return;

			// goto the start if we're leaching off another cache's IO
			if (IsSharedReference)
				s.Seek(0);

			cacheHeader.Read(s);

			DetermineEngineVersion();

			// Shared caches can't be loaded from here
			if (cacheHeader.SharedType == Cache.SharedType.Shared || 
				cacheHeader.SharedType == Cache.SharedType.Campaign) 
				throw new SharedCacheAccessException();

			TagIndexInitializeAndRead(s);

			isLoaded = true;
		}
	};

	/// <summary>
	/// Halo 3 implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public sealed class CacheFile : CacheFileBase
	{
		protected override bool SharableReferenceXbox(string path)
		{
			if (SharableReference(path, Program.Halo3.XboxMainmenu)) ShareCacheStreams(this, Program.Halo3.XboxMainmenu);
			else if (SharableReference(path, Program.Halo3.XboxShared)) ShareCacheStreams(this, Program.Halo3.XboxShared);
			else if (SharableReference(path, Program.Halo3.XboxCampaign)) ShareCacheStreams(this, Program.Halo3.XboxCampaign);
			else return false;

			return true;
		}

		public CacheFile(string map_name) : base(map_name)
		{
			engineVersion = BlamVersion.Halo3;

			cacheIndex = new CacheIndex();
		}

		protected override void DetermineEngineVersion()
		{
			switch (cacheHeader.Build.Substring(20))
			{
				case "delta":
					engineVersion = BlamVersion.Halo3_Beta;
					break;

				case "release":
				case "main": // TODO: for now, I've spotted no pressing differences in the epsilon's that would need to use different definition data
					//engineVersion = BlamVersion.Halo3_Epsilon;
					//break;

				case "halo3_ship":
					engineVersion = BlamVersion.Halo3_Xbox;
					break;

				default:
					throw new Debug.ExceptionLog("Unreachable!");
			}
		}
	};
	#endregion
}
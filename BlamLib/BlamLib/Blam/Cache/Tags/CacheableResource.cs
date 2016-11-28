/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
#if !NO_HALO2
	public enum geometry_block_resource_type
	{
		TagBlock,
		TagData,
		VertexBuffer,
	};

	#region geometry_block_resource_block
	[TI.Definition(1, 16)]
	public class geometry_block_resource_block : TI.Definition
	{
		public TI.Enum Type;
		/// <summary>
		/// Resource alignment bit
		/// </summary>
		public TI.ByteInteger AlignmentBit;
		/// <summary>
		/// Runtime (cache) field offset
		/// </summary>
		public TI.ShortInteger FieldOffset;
		/// <summary>
		/// Offset in the section header
		/// </summary>
		public TI.ShortInteger PrimaryLocater;
		/// <summary>
		/// Size of a single instance
		/// </summary>
		public TI.ShortInteger SecondaryLocater;
		/// <summary>
		/// Total size
		/// </summary>
		public TI.LongInteger Size;
		/// <summary>
		/// Offset in the geometry block
		/// </summary>
		public TI.LongInteger Offset; // ResourcePtr

		public geometry_block_resource_block() : base(8)
		{
			Add(Type = new TI.Enum(TI.FieldType.ByteEnum));
			Add(AlignmentBit = new TI.ByteInteger());
			Add(FieldOffset = new TI.ShortInteger());
			Add(PrimaryLocater = new TI.ShortInteger());
			Add(SecondaryLocater = new TI.ShortInteger());
			Add(Size = new TI.LongInteger());
			Add(Offset = new TI.LongInteger());
		}

		public bool IsTagBlock { get { return Type == (uint)geometry_block_resource_type.TagBlock; } }
		public bool IsTagData { get { return Type == (uint)geometry_block_resource_type.TagData; } }
		public bool IsVertexBuffer { get { return Type == (uint)geometry_block_resource_type.VertexBuffer; } }

		public int GetHeaderOffset() { return PrimaryLocater.Value; }
		public int GetSizeOf() { return SecondaryLocater.Value; }
		public int GetCount()
		{
			return Size.Value / GetSizeOf();
		}
	};
	#endregion

	#region geometry_block_info_struct
	[TI.Struct((int)Blam.Halo2.StructGroups.Enumerated.BLOK, 1, 40)]
	public class geometry_block_info_struct : TI.Definition
	{
		#region Fields
		public TI.LongInteger BlockOffset; // ResourcePtr
		public TI.LongInteger BlockSize;
		public TI.LongInteger SectionSize;
		public TI.LongInteger ResourceSize;
		public TI.Block<geometry_block_resource_block> Resources;
		public TI.LongInteger Original; // DatumIndex
		public TI.ShortInteger RuntimeSectionOffset;
		public TI.ByteInteger RuntimeLinked;
		public TI.ByteInteger RuntimeLoaded; // Flags
		public TI.LongInteger RuntimeCacheIndex; // DatumIndex
		#endregion

		#region Ctor
		public geometry_block_info_struct() : base(10)
		{
			Add(BlockOffset = new TI.LongInteger());
			Add(BlockSize = new TI.LongInteger());
			Add(SectionSize = new TI.LongInteger());
			Add(ResourceSize = new TI.LongInteger());
			Add(Resources = new TI.Block<geometry_block_resource_block>(this, 1024));
			Add(Original = new TI.LongInteger());
			Add(RuntimeSectionOffset = new TI.ShortInteger());
			Add(RuntimeLinked = new TI.ByteInteger());
			Add(RuntimeLoaded = new TI.ByteInteger()); // Flags
			Add(RuntimeCacheIndex = new TI.LongInteger());
		}
		#endregion

		public ResourcePtr GetBlockOffset() { return BlockOffset.Value; }
		public int GetBlockSize() { return BlockSize.Value; }
		public int GetSectionSize() { return SectionSize.Value; }
		public int GetSize() { return ResourceSize.Value; }
		public DatumIndex GetOriginal() { return Original.Value; }

		#region Reconstruct
		bool Reconstruct(int index, CacheFile c, out byte[] data)
		{
			geometry_block_resource_block gbr = Resources[index];

			// seek to the actual data
			c.InputStream.Seek(GetBlockOffset().Offset + 
				8 +
				GetSectionSize() +
				gbr.Offset);
			
			// read all of the geometry resource block data
			data = c.InputStream.ReadBytes(gbr.Size);

			return true;
		}

		internal byte[][] GeometryBlock;
		internal override bool Reconstruct(Blam.CacheFile c)
		{
			byte[][] data = null;
			int x;

			var rsrc_cache = Program.Halo2.FromLocation(c as Halo2.CacheFile, GetBlockOffset());

			// the shared cache isn't loaded, break
			if (rsrc_cache == null)
				return false;

			data = new byte[Resources.Count][];
			for (x = 0; x < Resources.Count; x++)
				Reconstruct(x, rsrc_cache, out data[x]);

			GeometryBlock = data;

			return true;
		}

		/// <summary>
		/// Postprocess a tag block that has been streamed to a cache block
		/// </summary>
		/// <param name="locator">Offset of the tag block header in the section</param>
		/// <param name="block"></param>
		/// <returns></returns>
		internal bool ReconstructTagBlock(short locator, TagInterface.IBlock block)
		{
			int index = 0;

			if (GeometryBlock == null) return false;

			foreach (geometry_block_resource_block gb in Resources)
			{
				using (IO.EndianReader er = new BlamLib.IO.EndianReader(GeometryBlock[index]))
				{
					switch (gb.Type.Value)
					{
						#region TagBlock
						case (int)geometry_block_resource_type.TagBlock:
							int count = gb.GetCount();
							if (gb.PrimaryLocater.Value == locator)
							{
								block.Resize(count);
								block.Read(er);
							}
							break;
						#endregion
					}

					index++;
				}
			}

			return true;
		}

		/// <summary>
		/// Postprocess tag data that has been streamed to a cache block
		/// </summary>
		/// <param name="locator">Offset of the tag data header in the section</param>
		/// <param name="data"></param>
		/// <returns></returns>
		internal bool ReconstructTagData(short locator, TagInterface.Data data)
		{
			int index = 0;
			IO.EndianReader er;

			if (GeometryBlock == null) return false;

			foreach (geometry_block_resource_block gb in Resources)
			{
				er = new BlamLib.IO.EndianReader(GeometryBlock[index]);

				switch (gb.Type.Value)
				{
					#region TagData
					case (int)geometry_block_resource_type.TagData:
						if(gb.PrimaryLocater.Value == locator)
								data.Reset(er.ReadBytes(gb.Size));
						break;
					#endregion
				}

				index++;
				er.Close();
				er = null;
			}

			return true;
		}

		/// <summary>
		/// Call after the owner of this structure finishes reconstructing itself
		/// </summary>
		internal void ClearPostReconstruction()
		{
			GeometryBlock = null; // just in case

			BlockOffset.Value = 0;
			BlockSize.Value = 0;
			SectionSize.Value = 0;
			ResourceSize.Value = 0;
			Resources.DeleteAll();
			Original.Value = 0;
			RuntimeSectionOffset.Value = 0;
			RuntimeLinked.Value = 0;
			RuntimeLoaded.Value = 0;
			RuntimeCacheIndex.Value = 0;
		}
		#endregion
	};
	#endregion
#endif
}
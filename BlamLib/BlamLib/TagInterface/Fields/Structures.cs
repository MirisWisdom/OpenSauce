/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Reflection;

//using DX = Microsoft.DirectX;
//using D3D = Microsoft.DirectX.Direct3D;

namespace BlamLib.TagInterface
{
	#region IStructureOwner
	/// <summary>
	/// Interface for objects which can own fields which are considered structure types
	/// </summary>
	public interface IStructureOwner
	{
		/// <summary>
		/// Handle to the index object to which this object is somehow dependent on
		/// </summary>
		Blam.DatumIndex OwnerId { get; }
		/// <summary>
		/// Handle to the object that exists in <see cref="OwnerId"/> which owns or IS this object
		/// </summary>
		Blam.DatumIndex TagIndex { get; }
		/// <summary>
		/// Reference to next parent in the owner hierarchy, or null if none
		/// </summary>
		IStructureOwner OwnerObject { get; }
	};
	#endregion

	#region Data
	/// <summary>
	/// Tag data byte descriptors
	/// </summary>
	public enum DataType
	{
		/// <summary>
		/// Generic binary data
		/// </summary>
		Generic,
		/// <summary>
		/// Bitmap related data
		/// </summary>
		Bitmap,
		/// <summary>
		/// Sound related data
		/// </summary>
		Sound,
		/// <summary>
		/// Script node data
		/// </summary>
		ScriptNode,
		/// <summary>
		/// Animation frame info data
		/// </summary>
		FrameInfo,
		/// <summary>
		/// Animation default data 
		/// </summary>
		DefaultData,
		/// <summary>
		/// Animation frame data
		/// </summary>
		FrameData,
		/// <summary>
		/// Geometry vertex data
		/// </summary>
		Vertex,
		/// <summary>
		/// Geometry vertex data (compressed)
		/// </summary>
		VertexCompressed,
		/// <summary>
		/// String data (ASCII)
		/// </summary>
		String,
		/// <summary>
		/// String data (Unicode)
		/// </summary>
		Unicode,
	};

	/// <summary>
	/// Arbitrary structure thats just stored as a single buffer
	/// </summary>
	public sealed class Data : FieldWithPointers
	{
		#region Owner
		IStructureOwner owner = null;
		public IStructureOwner Owner
		{
			get { return owner; }
			internal set { owner = value; }
		}
		#endregion

		#region Size
		int size = 0;
		/// <summary>
		/// Size of the binary data
		/// </summary>
		public int Size { get { return size; } }
		#endregion

		#region Offset
		/// <summary>
		/// Offset of the actual data in the file
		/// </summary>
		public uint Offset { get { return relativeOffset; } }
		#endregion

		public DataType DataType = DataType.Generic;
		public IO.ByteSwap.ByteSwapDelegate bysw;

		#region Value
		private byte[] mValue = null;

		/// <summary>
		/// Data byte array
		/// </summary>
		public byte[] Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		public byte this[int index]
		{
			get { return Value[index]; }
			set
			{
				Value[index] = value;
				OnPropertyChanged("Value");
			}
		}

		public void Delete()
		{
			size = 0;
			Value = null;
		}

		/// <summary>
		/// Resets the data with a single or multiple data array(s)
		/// </summary>
		/// <param name="arrays"></param>
		public void Reset(params byte[][] arrays)
		{
			int new_size = 0;
			foreach(byte[] b in arrays) if(b != null) new_size += b.Length;

			Value = new byte[new_size];
			new_size = 0;
			foreach (byte[] b in arrays)
			{
				if (b == null || b.Length == 0) continue; // skip zero length arrays
				b.CopyTo(Value, new_size);
				new_size += b.Length;
			}
		}

		/// <summary>
		/// Interfaces with this Data's byte[] data
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as byte[]; }
		}
		#endregion

		#region Constructers
		/// <summary>
		/// Create a tag data field
		/// </summary>
		public Data() : base(FieldType.Data) {}
		/// <summary>
		/// Create a tag data field from an existing one
		/// </summary>
		/// <param name="value"></param>
		public Data(Data value) : this()
		{
			DataType = value.DataType;

			Value = new byte[value.Value.Length];
			value.Value.CopyTo(Value, 0);

			size = Value.Length;
		}
		/// <summary>
		/// Create a tag data field from an existing one
		/// </summary>
		/// <param name="value"></param>
		/// <param name="owner"></param>
		public Data(Data value, IStructureOwner owner) : this(value) { this.owner = owner; }

		/// <summary>
		/// Create a tag data field
		/// with a specified owner
		/// </summary>
		/// <param name="owner">Owner of this tag data</param>
		public Data(IStructureOwner owner) : this() { this.owner = owner; }
		/// <summary>
		/// Create a tag data field
		/// with a specified type
		/// </summary>
		/// <param name="type">Type of the data stored in this</param>
		public Data(DataType type) : this()	{ DataType = type; }
		/// <summary>
		/// Create a tag data field
		/// with a specified owner and type
		/// </summary>
		/// <param name="owner">Owner of this tag data</param>
		/// <param name="type">Type of the data stored in this</param>
		public Data(IStructureOwner owner, DataType type) : this(owner) { DataType = type; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new Data( (from as Data).DataType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()	{ return new Data(this); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <param name="owner"></param>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone(IStructureOwner owner) { return new Data(this, owner); }
		#endregion

		/// <summary>
		/// Copy this data field's data to another data field, without creating an entire new field object
		/// </summary>
		/// <param name="field"></param>
		public override void CopyTo(Field field)
		{
			Data d = field as Data;

			d.Value = new byte[Value.Length];
			Value.CopyTo(d.Value, 0);
		}

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, (int)DataType); }

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Data field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		#region Cache
		/// <summary>
		/// Stream the field's header data from a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void ReadHeader(BlamLib.Blam.CacheFile c)
		{
			if ((c.EngineVersion & BlamVersion.Halo1) != 0)
			{
				size = c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadInt32();
			}
			else if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
			{
				size = c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadInt32();
			}
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)
			{
				size = c.InputStream.ReadInt32();
				relativeOffset = c.InputStream.ReadPointer();
			}
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				size = c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				relativeOffset = c.InputStream.ReadPointer();
				c.InputStream.ReadInt32();
			}
			else if ((c.EngineVersion & BlamVersion.Stubbs) != 0)
			{
				size = c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				c.InputStream.ReadInt32();
				relativeOffset = c.InputStream.ReadUInt32();
				c.InputStream.ReadInt32();

				// ie, sound samples are stored as a resource, not in tag data
				if (relativeOffset > c.Header.OffsetToIndex) relativeOffset -= c.AddressMask;
			}
		}
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void Read(BlamLib.Blam.CacheFile c)
		{
			if (size == 0 || relativeOffset == 0) return;

			c.InputStream.Seek(relativeOffset, System.IO.SeekOrigin.Begin);
			Value = c.InputStream.ReadBytes(size);

			if (bysw != null && c.InputStream.State == IO.EndianState.Big) bysw(this.owner, this);
		}
		/// <summary>
		/// Stream the field's header data to a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void WriteHeader(BlamLib.Blam.CacheFile c)
		{
			if ((c.EngineVersion & BlamVersion.Halo1) != 0)
			{
				c.OutputStream.Write(Value != null ? Value.Length : 0);
				c.OutputStream.Write(0);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0); // file position
				c.OutputStream.Write(0); // pointer
				c.OutputStream.Write(0); // definition
			}
			else if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
			{
				c.OutputStream.Write(Value != null ? Value.Length : 0);
				c.OutputStream.Write(0);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0); // file position
				c.OutputStream.Write(0); // pointer
				c.OutputStream.Write(0); // definition
			}
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)
			{
				c.OutputStream.Write(Value != null ? Value.Length : 0);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0);
			}
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				c.OutputStream.Write(Value != null ? Value.Length : 0);
				c.OutputStream.Write(0);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0); // file position
				c.OutputStream.Write(0); // pointer
				c.OutputStream.Write(0); // definition
			}
			else if ((c.EngineVersion & BlamVersion.Stubbs) != 0) // TODO: i guess
			{
				c.OutputStream.Write(Value != null ? Value.Length : 0);
				c.OutputStream.Write(0);
				headerOffset = c.OutputStream.PositionUnsigned;
				c.OutputStream.Write(0); // file position
				c.OutputStream.Write(0); // pointer
				c.OutputStream.Write(0); // definition
			}
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void Write(BlamLib.Blam.CacheFile c)
		{
			if (bysw != null && c.OutputStream.State == IO.EndianState.Big) bysw(this.owner, this);

			relativeOffset = c.OutputStream.PositionUnsigned; // store offset
			c.OutputStream.Seek(headerOffset); // go to the reflexive header
			c.OutputStream.WritePointer(relativeOffset); // write address
			c.OutputStream.Seek(relativeOffset); // go back to where we were

			if (Value != null) c.OutputStream.Write(Value);
		}
		#endregion

		#region Tag
		/// <summary>
		/// Stream the field's header data from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidTagData"></exception>
		public override void ReadHeader(IO.ITagStream ts)
		{
			IO.EndianReader s = ts.GetInputStream();
			headerOffset = s.PositionUnsigned; // nifty for debugging
			try
			{
				size = s.ReadInt32(); // TODO: for the future, perform some kind of size sanity check
				s.ReadInt32();
				if (ts.Flags.Test(IO.ITagStreamFlags.Halo3VertexBufferMegaHack)) s.ReadInt32();
				relativeOffset = s.ReadPointer();
				s.ReadInt32(); s.ReadInt32();
			}
			catch (Exception ex)
			{
				throw new Exceptions.InvalidTagData(ex,
					base.headerOffset, uint.MaxValue,
					this.owner, ts);
			}
		}

		/// <summary>
		/// Stream the field from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidTagData"></exception>
		public override void Read(IO.ITagStream ts)
		{
			IO.EndianReader s = ts.GetInputStream();

			if (!ts.Flags.Test(IO.ITagStreamFlags.UseStreamPositions))
				relativeOffset = s.PositionUnsigned;
			else
				s.PositionUnsigned = relativeOffset;

			try { Value = s.ReadBytes(size); }
			catch (Exception ex)
			{
				throw new Exceptions.InvalidTagData(ex,
					base.headerOffset, base.relativeOffset,
					this.owner, ts);
			}

			if (bysw != null && s.State == IO.EndianState.Big) bysw(this.owner, this);
		}

		/// <summary>
		/// Stream the field's header data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void WriteHeader(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();

			s.Write(Value != null ? Value.Length : 0);
			s.Write(0);
			if (ts.Flags.Test(IO.ITagStreamFlags.Halo3VertexBufferMegaHack)) s.Write(0);
			headerOffset = s.PositionUnsigned; // we'll come back here and write the element's offset
			s.Write(0); // stream position
			s.Write(0); // pointer
			s.Write(0); // definition
		}

		/// <summary>
		/// Stream the field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void Write(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();
			if (bysw != null && s.State == IO.EndianState.Big) bysw(this.owner, this);

			if (ts.Flags.Test(IO.ITagStreamFlags.UseStreamPositions))
			{
				relativeOffset = s.PositionUnsigned;// store offset
				s.PositionUnsigned = headerOffset;	// go to the reflexive header
				s.WritePointer(relativeOffset);		// write offset
				s.PositionUnsigned = relativeOffset;// go back to where we were
			}

			if (Value != null) s.Write(Value);
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Stream the field's header data from a buffer
		/// </summary>
		/// <param name="s"></param>
		/// <exception cref="Exceptions.InvalidTagData"></exception>
		public override void ReadHeader(IO.EndianReader s)
		{
			headerOffset = s.PositionUnsigned; // nifty for debugging

			size = s.ReadInt32(); // TODO: for the future, perform some kind of size sanity check
			s.ReadInt32(); s.ReadInt32(); s.ReadPointer(); s.ReadInt32();
		}

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Read(IO.EndianReader s)	{}

		/// <summary>
		/// Stream the field's header data to a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void WriteHeader(IO.EndianWriter s)
		{
			s.Write(Value != null ? Value.Length : 0);
			s.Write(0);
			headerOffset = s.PositionUnsigned; // we'll come back here and write the element's offset
			s.Write(0); // stream position
			s.Write(0); // pointer
			s.Write(0); // definition
		}

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Write(IO.EndianWriter s)	{}
		#endregion
		#endregion
	};
	#endregion

	#region TagReference
	/// <summary>
	/// TagReference definition class; Filename, Group Tag, Tag Index
	/// </summary>
	public sealed class TagReference : FieldWithPointers, Managers.IReferenceMangerObject, Managers.ITagDatabaseAddable
	{
		#region Owner
		IStructureOwner owner = null;
		/// <summary>
		/// Owner object of this TagReference
		/// </summary>
		public IStructureOwner Owner
		{
			get { return owner; }
			internal set { owner = value; }
		}

		void ChangeOwner(IStructureOwner new_owner)
		{
			Blam.DatumIndex new_ownerid = new_owner.OwnerId;
			if (new_ownerid != OwnerId) // are we changing to a new owner?
			{
				if (Managers.BlamDefinition.IsCacheHandle(new_ownerid))
					throw new Debug.ExceptionLog("Can't change owner to a cache! {0}\t{1}", OwnerId, new_ownerid);

				ReferenceId = Managers.ReferenceManager.CopyHandle(
					Program.GetTagIndex(OwnerId).References, // TODO: maybe we should validate this isn't a cache handle either?
					Program.GetTagIndex(new_ownerid).References,
					ReferenceId);
			}

			this.ParentReferenceId = new_owner.TagIndex;
			this.owner = new_owner;
		}
		#endregion

		#region Value
		/// <summary>
		/// This is either a handle to a <see cref="BlamLib.Blam.CacheFile"/> or to a 
		/// <see cref="BlamLib.Managers.ITagIndex"/>, depending on where this string was
		/// loaded from
		/// </summary>
		internal Blam.DatumIndex OwnerId = Blam.DatumIndex.Null;
		/// <summary>
		/// The reference name handle of the tag definition that ultimately owns this tag reference
		/// </summary>
		/// <remarks>Only set during tag file reads\writes, not during cache ops</remarks>
		internal Blam.DatumIndex ParentReferenceId = Blam.DatumIndex.Null;
		/// <summary>
		/// The reference name handle to the group_tag\path pair that this tag reference uses
		/// </summary>
		internal Blam.DatumIndex ReferenceId = Blam.DatumIndex.Null;

		#region tag_reference fields
		private Blam.DatumIndex mDatum = Blam.DatumIndex.Null;
		private TagGroup mGroupTag = null;

		/// <summary>
		/// Group tag definition of the tag instance this references
		/// </summary>
		public TagGroup GroupTag
		{
			get
			{
				return mGroupTag;
			}
			set
			{
				mGroupTag = value;
				OnPropertyChanged("GroupTag");
			}
		}
		/// <summary>
		/// Group tag of the tag instance this references
		/// </summary>
		internal uint GroupTagInt;

		/// <summary>
		/// datum_index of the tag instance this references
		/// </summary>
		// TODO: probably expose this as a read-only property later and use 
		// a method for changing
		public Blam.DatumIndex Datum
		{
			get { return mDatum; }
		}
		#endregion

		public string GetTagPath()
		{
			if (OwnerId != Blam.DatumIndex.Null && ReferenceId != Blam.DatumIndex.Null)
				return Program.GetTagIndex(OwnerId).References[ReferenceId];

			return "";
		}

		public void SetDatum(Blam.DatumIndex datumIndex)
		{
			mDatum = datumIndex;
			OnPropertyChanged("Datum");
		}

		/// <summary>
		/// Interfaces with a object[] built of: OwnerId, ReferenceId, GroupTag, and DatumIndex
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>OwnerId @ 0, DatumIndex</item>
		/// <item>ReferenceId @ 1, DatumIndex</item>
		/// <item>GroupTag @ 2, TagGroup</item>
		/// <item>GroupTagInt @ 3, uint</item>
		/// <item>Datum @ 4, DatumIndex</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get
			{
				return new object[] {
										OwnerId,
										ReferenceId,
										GroupTag,
										GroupTagInt,
										Datum,
									};
			}
			set
			{
				object[] val = value as object[];
				OwnerId = (Blam.DatumIndex)val[0];
				ReferenceId = (Blam.DatumIndex)val[1];
				GroupTag = val[2] as TagGroup;
				GroupTagInt = (uint)val[3];
				SetDatum((Blam.DatumIndex)val[4]);
			}
		}

		/// <summary>
		/// Reset this reference's group tag definition to be something else
		/// </summary>
		/// <param name="group"></param>
		/// <remarks>Made only for Definitions which have <see cref="field_block{T}"/> Block fields</remarks>
		public void ResetGroupTag(TagGroup group)	{ GroupTagInt = (GroupTag = group).ID; }

		/// <summary>
		/// Tag file's name
		/// </summary>
		/// <returns><see cref="GetTagPath()"/></returns>
		public override string ToString() { return GetTagPath(); }
		#endregion

		#region IReferenceMangerObject Members
		Blam.DatumIndex Managers.IReferenceMangerObject.ReferenceId			{ get { return ReferenceId; } }

		Blam.DatumIndex Managers.IReferenceMangerObject.ParentReferenceId	{ get { return ParentReferenceId; } }

		IEnumerable<Blam.DatumIndex> Managers.IReferenceMangerObject.GetReferenceIdEnumerator() { yield break; }

		bool Managers.IReferenceMangerObject.UpdateReferenceId(Managers.ReferenceManager manager, Blam.DatumIndex new_datum)
		{
			this.ReferenceId = new_datum;

			return true;
		}
		#endregion

		#region Ctor
		/// <summary>
		/// Create a tag reference field
		/// </summary>
		/// <remarks>
		/// Made only for Definitions which have <see cref="field_block{T}"/> Block fields.
		/// Call <see cref="ResetGroupTag"/> after initiailizing the tag block field
		/// </remarks>
		public TagReference() : base(FieldType.TagReference) { GroupTagInt = uint.MaxValue; GroupTag = TagGroup.Null; }
		/// <summary>
		/// Create a tag reference field with a specified group tag
		/// </summary>
		/// <param name="tag"></param>
		TagReference(TagGroup tag) : base(FieldType.TagReference) { GroupTag = tag; GroupTagInt = tag.ID; }
		/// <summary>
		/// Create a tag reference field with a specified owner
		/// </summary>
		/// <param name="owner"></param>
		public TagReference(IStructureOwner owner) : this() { this.owner = owner; }
		/// <summary>
		/// Create a tag reference field with a specified owner and group tag
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="tag"></param>
		public TagReference(IStructureOwner owner, TagGroup tag) : this(tag) { this.owner = owner; }
		/// <summary>
		/// Create a tag reference field from another tag reference
		/// </summary>
		/// <param name="value">reference to base this one off of</param>
		public TagReference(TagReference value) : this(value.GroupTag) { OwnerId = value.OwnerId; ReferenceId = value.ReferenceId; SetDatum(value.Datum); }
		/// <summary>
		/// Create a tag reference field from another tag reference
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="value">reference to base this one off of</param>
		public TagReference(IStructureOwner owner, TagReference value) : this(value) { ChangeOwner(owner); }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new TagReference((from as TagReference).GroupTag); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone() { return new TagReference(this); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <param name="owner"></param>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone(IStructureOwner owner) { return new TagReference(owner, this); }
		#endregion

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, GroupTag.Handle); }

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a TagReference field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		#region Cache
		/// <summary>
		/// Stream the field's header data from a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void ReadHeader(BlamLib.Blam.CacheFile c)
		{
			OwnerId = c.TagIndexManager.IndexId;

			if ((c.EngineVersion & BlamVersion.Halo1) != 0)
			{
				GroupTagInt = c.InputStream.ReadTagUInt();
				relativeOffset = c.InputStream.ReadPointer(); // tag filename pointer
				c.InputStream.ReadInt32(); // tag filename length
				Datum.Read(c.InputStream);
			}
			else if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
			{
				GroupTagInt = c.InputStream.ReadTagUInt();
				relativeOffset = c.InputStream.ReadPointer(); // tag filename pointer
				c.InputStream.ReadInt32(); // tag filename length
				Datum.Read(c.InputStream);
			}
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)
			{
				GroupTagInt = c.InputStream.ReadTagUInt();
				Datum.Read(c.InputStream);
			}
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				GroupTagInt = c.InputStream.ReadTagUInt();
				c.InputStream.ReadInt32(); // unused tag filename pointer
				c.InputStream.ReadInt32(); // unused tag filename length
				Datum.Read(c.InputStream);
			}
			else if ((c.EngineVersion & BlamVersion.Stubbs) != 0)
			{
				GroupTagInt = c.InputStream.ReadTagUInt();
				relativeOffset = c.InputStream.ReadPointer(); // tag filename pointer
				c.InputStream.ReadInt32(); // tag filename length
				Datum.Read(c.InputStream);
			}

			if (GroupTag == TagGroup.Null)
				GroupTag = Blam.MiscGroups.TagGroupFrom(c.EngineVersion, GroupTagInt);

			// if this reference is valid, update the referencing tag's tracking list so this field is linked to it as a referencer
			if (Datum != Blam.DatumIndex.Null)
			{
				if(Datum.Index < 0 || Datum.Index >= c.Index.Tags.Length)
					return;//throw new Exceptions.InvalidTagReference

				ReferenceId = c.Index.Tags[Datum.Index].ReferenceName;
				bool was_changed = false;

				if (c.ExtractionState != null)
				{
					// If the extractor changes the reference, we don't want to 
					// queue what it was changed to as the actual datum maybe be 
					// fake. This is for cases where a certain tag isn't in the cache 
					// but the tag will exist on disk so the reference will be good later
					if (!c.TagIndexManager.ExtractionTagReferenceChange(this))
						c.ExtractionState.Queue(Datum);
				}

				if(!was_changed)
					c.References.AddReferencer(ReferenceId, this);
			}
		}
		/// <summary>
		/// Overridden, tag name isn't stored in the tag data in cache files
		/// </summary>
		/// <param name="c"></param>
		/// <remarks>
		/// However this is where the name field of this object is retrieved from the cache
		/// 
		/// Also, the datum index is added to the cache's extraction queue if one is setup
		/// </remarks>
		public override void Read(BlamLib.Blam.CacheFile c)
		{
// 			try
// 			{
// 				if (Datum != Blam.DatumIndex.Null)
// 				{
// 					ReferenceId = c.Index.Tags[Datum.Index].ReferenceName;
// 					if (c.ExtractionState != null)
// 					{
// 						// If the extractor changes the reference, we don't want to 
// 						// queue what it was changed to as the actual datum maybe be 
// 						// fake. This is for cases where a certain tag isn't in the cache 
// 						// but the tag will exist on disk so the reference will be good later
// 						if(!c.TagIndexManager.ExtractionTagReferenceChange(this))
// 							c.ExtractionState.Queue(Datum);
// 					}
// 				}
// 			}
// 			catch (NullReferenceException)
// 			{
// 				Debug.LogFile.WriteLine("{0} is a bad reference to a {1} definition", Datum, new string(TagGroup.FromUInt(GroupTagInt, true)));
// 			}
		}
		/// <summary>
		/// Stream the field's header data from a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void WriteHeader(BlamLib.Blam.CacheFile c)
		{
			OwnerId = c.TagIndexManager.IndexId;

			int filename_length = 0;
			if(Datum != Blam.DatumIndex.Null)
				filename_length = c.GetReferenceName(c.Index.Tags[Datum.Index]).Length;

			if ((c.EngineVersion & BlamVersion.Halo1) != 0)
			{
				c.OutputStream.WriteTag(GroupTagInt);
				c.OutputStream.WritePointer(relativeOffset); // tag filename
				c.OutputStream.Write(filename_length);
				Datum.Write(c.OutputStream);
			}
			else if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
			{
				c.OutputStream.WriteTag(GroupTagInt);
				c.OutputStream.WritePointer(relativeOffset); // tag filename
				c.OutputStream.Write(filename_length);
				Datum.Write(c.OutputStream);
			}
			else if ((c.EngineVersion & BlamVersion.Halo2) != 0)
			{
				c.OutputStream.WriteTag(GroupTagInt);
				Datum.Write(c.OutputStream);
			}
			else if ((c.EngineVersion & BlamVersion.Halo3) != 0 || (c.EngineVersion & BlamVersion.HaloOdst) != 0 ||
					 (c.EngineVersion & BlamVersion.HaloReach) != 0 || (c.EngineVersion & BlamVersion.Halo4) != 0)
			{
				c.OutputStream.WriteTag(GroupTagInt);
				c.OutputStream.Write(0); // unused tag filename pointer
				c.OutputStream.Write(0); // unused tag filename length
				Datum.Write(c.OutputStream);
			}
			else if ((c.EngineVersion & BlamVersion.Stubbs) != 0)
			{
				c.OutputStream.WriteTag(GroupTagInt);
				c.OutputStream.WritePointer(relativeOffset); // tag filename
				c.OutputStream.Write(filename_length);
				Datum.Write(c.OutputStream);
			}
		}
		/// <summary>
		/// Overridden, tag name isn't store in the tag data in caches
		/// </summary>
		/// <param name="c"></param>
		public override void Write(BlamLib.Blam.CacheFile c) { }
		#endregion

		#region Tag
		void LoadDependents(Managers.TagIndex tag_index, IO.ITagStream ts, string tag_name)
		{
			if (ts.Flags.Test(IO.ITagStreamFlags.LoadDependents)) // don't load dependents? not a problem
			{
				TagGroup tg = Blam.MiscGroups.TagGroupFrom(ts.Engine, GroupTagInt);
				Debug.Assert.If(tg != null); // whaaaaaaaattt?! who forgot to add the tag group to the collection...
				Managers.TagManager tm = ts as Managers.TagManager;

				// I thought about making this only predict that we'll need to load the reference
				// then load the dependents after the referencing tag is done being read
				// instead of actually loading it right here, but then we can't exactly
				// tell if its a bad reference (ie to an old tag).
				SetDatum(tag_index.Open(tag_name, tg, IO.ITagStreamFlags.LoadDependents));
				if (Datum == Blam.DatumIndex.Null || Managers.TagIndex.IsSentinel(Datum))
				{
//					Debug.LogFile.WriteLine(
//						"'{0}'{1}\tFailed to load '{2}.{3}'",
//						tm.Path, Program.NewLine, Value, tg.Name);
					if (!ts.Flags.Test(IO.ITagStreamFlags.DontTrackTagManagerReferences) && tm != null)
						tm.BadReferencesAdd(this);
				}
				else if (tm != null)
				{
					if (!ts.Flags.Test(IO.ITagStreamFlags.DontTrackTagManagerReferences) && !tm.ReferencesContains(Datum))
						tm.ReferencesAdd(Datum);

					ReferenceId = tag_index[Datum].ReferenceName;
					if (!ts.Flags.Test(IO.ITagStreamFlags.DontTrackTagReferencers))
						tag_index.References.AddReferencer(ReferenceId, this);
				}
			}
		}

		/// <summary>
		/// Stream the field's header data from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void ReadHeader(IO.ITagStream ts)
		{
			IO.EndianReader s = ts.GetInputStream();
			headerOffset = s.PositionUnsigned; // nifty for debugging
			GroupTagInt = s.ReadTagUInt();
			s.ReadInt32(); // tag filename pointer
			SetDatum(new BlamLib.Blam.DatumIndex((ushort)s.ReadInt32(), Datum.Salt)); // little HACK for reusing values which aren't needed yet, used for storing the tag filename length

			// if we're not wanting to stream string data, chances are
			// our datum value is of use (ie from a cache file, who'd thunk it!)
			if (!ts.Flags.Test(IO.ITagStreamFlags.DontStreamStringData))
				s.ReadUInt32();//Datum.Read(s); // tag_index
			else
				Datum.Read(s);
		}

		/// <summary>
		/// Stream the field from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidTagReference"></exception>
		public override void Read(IO.ITagStream ts)
		{
			if (!ts.Flags.Test(IO.ITagStreamFlags.DontStreamStringData) && Datum.Index > 0) // little HACK for reusing values which aren't needed yet, used for storing the tag filename length
			{
				int filename_length = Datum.Index;
				SetDatum(Blam.DatumIndex.Null); // overwrite our little HACK

				OwnerId = ts.OwnerId; // copy the ITagIndex handle
				ParentReferenceId = ts.ReferenceName;

				var tag_index = Program.GetTagIndex(OwnerId) as Managers.TagIndex;

				string value = null;
				char[] chars;
				try
				{
					IO.EndianReader s = ts.GetInputStream();
					relativeOffset = s.PositionUnsigned;
					//value = s.ReadCString();
					chars = s.ReadChars(filename_length + 1);
					value = new string(chars, 0, filename_length);
					ReferenceId = tag_index.References.Add(this, GroupTagInt, value);
				}
				catch (Exception ex)
				{
					throw new Exceptions.InvalidTagReference(ex,
						base.headerOffset, base.relativeOffset,
						owner, ts,
						GroupTagInt, value);
				}

				LoadDependents(tag_index, ts, value);
			}
			// we don't want to set Datum to null if we expect it to be of use
			else if (!ts.Flags.Test(IO.ITagStreamFlags.DontStreamStringData))
			{
				ReferenceId = Blam.DatumIndex.Null;
				SetDatum(Blam.DatumIndex.Null);
			}
			else ReferenceId = Blam.DatumIndex.Null;
		}

		/// <summary>
		/// Stream the field's header data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void WriteHeader(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();

			// If we read this tag reference from a cache file first, OwnerId won't be null
			// We don't want to reset OwnerId in this case because it's not owned by a regular TagIndex
			if (OwnerId == Blam.DatumIndex.Null)
			{
				OwnerId = ts.OwnerId;
				ParentReferenceId = ts.ReferenceName;
			}

			s.WriteTag(GroupTagInt);
			headerOffset = s.PositionUnsigned;
			s.Write(0); // tag filename pointer

			// write reference path's length (because we want to allow the files to be read by bungie's tools too)
			if (ReferenceId != Blam.DatumIndex.Null)
				s.Write(Program.GetTagIndex(OwnerId).References[ReferenceId].Length);
			else
				s.Write(0);

			s.Write(-1);//Datum.Write(s); // tag_index
		}

		/// <summary>
		/// Stream the field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void Write(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();

			if (ts.Flags.Test(IO.ITagStreamFlags.UseStreamPositions))
			{
				relativeOffset = s.PositionUnsigned;// store offset
				s.PositionUnsigned = headerOffset;	// goto the pointer header
				s.WritePointer(relativeOffset);		// write the offset
				s.PositionUnsigned = relativeOffset;// go back to where we were
			}

			// write reference path
			if (ReferenceId != Blam.DatumIndex.Null && !ts.Flags.Test(IO.ITagStreamFlags.DontStreamStringData))
				s.Write(Program.GetTagIndex(OwnerId).References[ReferenceId], true);
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Stream the field's header data from a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void ReadHeader(IO.EndianReader s)
		{
			headerOffset = s.PositionUnsigned; // nifty for debugging
			GroupTagInt = s.ReadTagUInt();
			s.ReadInt32(); // tag filename pointer
			s.ReadInt32(); // the tag filename length
			s.ReadUInt32();//Datum.Read(s); // tag_index
		}

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Read(IO.EndianReader s)	{}

		/// <summary>
		/// Stream the field's header data to a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void WriteHeader(IO.EndianWriter s)
		{
			s.WriteTag(GroupTagInt);
			relativeOffset = s.PositionUnsigned;
			s.Write(0); // tag filename pointer
			s.Write(0);
			s.Write(-1);//Datum.Write(s); // tag_index
		}

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Write(IO.EndianWriter s)	{}
		#endregion
		#endregion

		#region ITagDatabaseAddable Members
		string Managers.ITagDatabaseAddable.GetTagName()	{ return GetTagPath(); }

		char[] Managers.ITagDatabaseAddable.GetGroupTag()	{ return TagGroup.FromUInt(GroupTagInt); }
		#endregion
	};
	#endregion

	#region VertexBuffer
	public sealed class VertexBuffer : Field, IDisposable
	{
		#region Value
		public byte TypeIndex = byte.MaxValue;
		public byte StrideSize = byte.MaxValue;

		/// <summary>
		/// Interfaces with a object[] built of: TypeIndex, and StrideSize
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>TypeIndex @ 0, int</item>
		/// <item>StrideSize @ 1, byte[]</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get
			{
				return new object[] {
					TypeIndex,
					StrideSize,
				};
			}
			set
			{
				object[] val = value as object[];
				TypeIndex = (byte)val[0];
				StrideSize = (byte)val[1];
			}
		}
		#endregion

		/// <summary>
		/// Is this vertex buffer null? (read: zero size, zero elements)
		/// </summary>
		internal bool IsNull { get { return TypeIndex == 0; } }

		//public int StreamIndex = -1;
		//public Blam.Halo2.Render.VertexBufferDefinition VertexDeclaration = null;
		// TODO: update this shit fool
		//public D3D.VertexBuffer Buffer = null;
		//byte[] BufferData = null;

		#region Ctor
		public VertexBuffer() : base(FieldType.VertexBuffer) { }
		public VertexBuffer(VertexBuffer value) : this()
		{
			this.TypeIndex = value.TypeIndex;
			this.StrideSize = value.StrideSize;
		}

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new VertexBuffer((from as VertexBuffer)); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone() { return new VertexBuffer(this); }
		#endregion

		#region Data Exchange
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }
		#endregion

		#region I\O
		// 0x2: word (set to zero when cache building)
		// 0x4: word
		// 0x6: pad?
		// 0x8: dword (offset?)
		// 0xC: dword (set to zero when cache building)
		// 0x10: dword (set to zero when cache building) (ID3DResource pointer)
		// 0x14: 0xC bytes set to zero when building the cache

		public override void Read(IO.EndianReader input)
		{
			TypeIndex = input.ReadByte();
			StrideSize = input.ReadByte();
			input.Seek(30, System.IO.SeekOrigin.Current);
		}

		public override void Write(IO.EndianWriter output)
		{
			output.Write(TypeIndex);
			output.Write(StrideSize);
			output.Write(new byte[30]);
		}
		#endregion

		internal void InitializeStreamReader(Render.VertexBufferInterface.VertexBuffersGen2 vb_defs,
			out Render.VertexBufferInterface.StreamReader reader)
		{
			var def = vb_defs.DefinitionFind(TypeIndex);

			if (!IsNull)
				Debug.Assert.If(def.GetSize() == StrideSize);

			reader = new Render.VertexBufferInterface.StreamReader(def);
		}

		internal void OnCreateVertexBuffer(object sender, EventArgs e)
		{
			// TODO: update this shit fool
// 			if(Buffer != sender)
// 				Buffer = (D3D.VertexBuffer)sender;
// 			Buffer.SetData(BufferData, 0, D3D.LockFlags.None);
// 			Buffer.Unlock();
		}

		internal void Reconstruct(BlamVersion version, short index, byte[] data)
		{
			//StreamIndex = index;
			//BufferData = data;

			if ((version & BlamVersion.Halo2) != 0)
			{
// 				if (BlamUtil.IsPc(version))
// 					VertexDeclaration = Blam.Halo2.Render.VertexBuffersPc[TypeIndex].Clone();
// 				else if (BlamUtil.IsXbox1(version))
// 					VertexDeclaration = Blam.Halo2.Render.VertexBuffersXbox[TypeIndex].Clone();
// 				VertexDeclaration.SetStream(index);

				// TODO: update this shit fool
// 				Buffer = new D3D.VertexBuffer(
// 					Editor.Renderer.GetDevice(), data.Length, D3D.Usage.WriteOnly, D3D.VertexFormats.None, D3D.Pool.Managed);
// 				Buffer.Created += new EventHandler(OnCreateVertexBuffer);
// 				this.OnCreateVertexBuffer(Buffer, null);
			}
		}

		public void Dispose()
		{
			// TODO: update this shit fool
// 			if (Buffer != null)
// 			{
// 				Buffer.Dispose();
// 				Buffer = null;
// 			}
			//BufferData = null;
			//VertexDeclaration = null;
			//StreamIndex = -1;
		}
	};
	#endregion


	#region ElementArray
	/// <summary>
	/// Allows us to interface easily with <c>ElementArray</c> since its a template class
	/// </summary>
	/// <remarks>Most useful in <c>BlamLib.Forms</c> code</remarks>
	public interface IElementArray : IO.ITagStreamable, IO.IStreamable, ICloneable, System.Collections.IList
	{
		/// <summary>
		/// Definition Field indexer
		/// </summary>
		/// <param name="element">Index of the Definition which holds the wanted field</param>
		/// <param name="field">Field index of the Field to retrieve</param>
		/// <returns>Field of one of the Definitions in this collection</returns>
		Field this[int element, int field] { get; set; }

		/// <summary>
		/// Definition indexer
		/// </summary>
		/// <param name="element">Index of the definition we want to retrieve</param>
		/// <returns>Definition at <paramref name="element"/></returns>
		Definition GetElement(int element);

		/// <summary>
		/// Calculate how much memory this element array consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		int CalculateRuntimeSize(BlamVersion engine, bool cache);
	};

	/// <summary>
	/// Type Safe array of fields
	/// </summary>
	public sealed class ElementArray<T> : List<T>, IElementArray where T : Definition, new()
	{
		#region Ctor
		public ElementArray() { }
		public ElementArray(int capacity) : base(capacity) { }

		/// <summary>
		/// Perform a deep copy of this object
		/// </summary>
		/// <returns></returns>
		public object Clone()
		{
			ElementArray<T> array = new ElementArray<T>();
			foreach (T def in this) array.Add(def.Clone() as T);

			return array;
		}

		/// <summary>
		/// Perform a deep copy of this object
		/// </summary>
		/// <param name="owner"></param>
		/// <returns></returns>
		public object Clone(IStructureOwner owner)
		{
			ElementArray<T> array = new ElementArray<T>();
			foreach (T def in this) array.Add(def.Clone(owner) as T);

			return array;
		}
		#endregion

		#region IElementArray
		/// <summary>
		/// Definition Field indexer
		/// </summary>
		/// <param name="element">Index of the Definition which holds the wanted field</param>
		/// <param name="field">Field index of the Field to retrieve</param>
		/// <returns>Field of one of the Definitions in this collection</returns>
		public Field this[int element, int field]
		{
			get { return base[element][field]; }
			set { base[element][field] = value; }
		}

		/// <summary>
		/// Definition indexer
		/// </summary>
		/// <param name="element">Index of the definition we want to retrieve</param>
		/// <returns>Definition at <paramref name="element"/></returns>
		public Definition GetElement(int element) { return this[element]; }

		/// <summary>
		/// Calculate how much memory this element array consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		public int CalculateRuntimeSize(BlamVersion engine, bool cache)
		{
			int val = 0;
			foreach (Definition def in this)
				val += def.CalculateRuntimeSize(engine, cache);

			return val;
		}
		#endregion

		#region IStreamable Members
		/// <summary>
		/// Upgrade all the elements to the newest version
		/// </summary>
		/// <returns></returns>
		public bool Upgrade()
		{
			bool sanity = true;

			int index = 0; // debug only

			foreach (T def in this)
			{
				sanity &= def.Upgrade();

				if (!sanity)
					Debug.Assert.If(false, "Failed to upgrade definition. {0} #{1} {2}", typeof(T).Name, index++, def.ToVersionString());
			}

			return sanity;
		}

		#region Cache
		/// <summary>
		/// Process the elements from a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public void Read(BlamLib.Blam.CacheFile c)
		{
			if (this.Count == 0) return;

			foreach (T def in this)	def.Read(c, IOProcess.NoPointerData);
			foreach (T def in this)	def.Read(c, IOProcess.BlockDataReflexive);
		}

		/// <summary>
		/// Process the elements to a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public void Write(BlamLib.Blam.CacheFile c)
		{
			if (this.Count == 0) return;

			foreach (T def in this)	def.Write(c, IOProcess.NoPointerData);
			foreach (T def in this)	def.Write(c, IOProcess.BlockDataReflexive);
		}
		#endregion

		#region Tag
		/// <summary>
		/// Process the elements from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public void Read(IO.ITagStream ts)
		{
			if (this.Count == 0) return;

			foreach (T def in this) def.Read(ts, IOProcess.NoPointerData);
			foreach (T def in this) def.Read(ts, IOProcess.BlockDataReflexive);
		}

		/// <summary>
		/// Process the elements to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public void Write(IO.ITagStream ts)
		{
			if (this.Count == 0) return;

			foreach (T def in this) def.Write(ts, IOProcess.NoPointerData);
			foreach (T def in this) def.Write(ts, IOProcess.BlockDataReflexive);
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Process the elements from the stream
		/// </summary>
		/// <param name="s">Stream</param>
		public void Read(IO.EndianReader s)
		{
			if (this.Count == 0) return;

			foreach (T def in this) def.Read(s, IOProcess.NoPointerData);
			foreach (T def in this) def.Read(s, IOProcess.BlockDataReflexive);
		}

		/// <summary>
		/// Process the elements to the stream
		/// </summary>
		/// <param name="s">Stream</param>
		public void Write(IO.EndianWriter s)
		{
			if (this.Count == 0) return;

			foreach (T def in this) def.Write(s, IOProcess.NoPointerData);
			foreach (T def in this) def.Write(s, IOProcess.BlockDataReflexive);
		}
		#endregion
		#endregion
	}
	#endregion
}
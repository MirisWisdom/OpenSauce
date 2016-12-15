/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace BlamLib.TagInterface
{
	#region FieldType
	/// <summary>
	/// Enumeration of all supported field types
	/// </summary>
	/// <remarks>
	/// When adding new field types, be sure to update the following functions:
	/// TagInterface\Attributes.cs
	///		FieldControls
	/// TagInterface\FieldUtil.cs
	///		FieldUtil::Sizeof
	///		FieldUtil::CreateFromDefinitionItem
	/// TagInterface\Scanner.cs
	///		Scanner::CreateDefinition
	///		Scanner::Add*
	/// 
	/// 
	/// And depending on the purpose of the field, the following functions:
	/// TagInterface\Field.cs
	///		Field::UsesRecursiveIo
	/// TagInterface\Structures.cs
	///		DefinitionState::ProcessTypes
	///		DefinitionState::ProcessAtrributes
	///		Definition::WriteFieldDataRecursive
	/// TagInterface\Util.cs
	///		DefinitionFile::Create
	///		DefinitionFile::Parse
	/// 	DefinitionConversionFile::Create
	///		DefinitionConversionFile::Parse
	/// </remarks>
	public enum FieldType : byte
	{
		/// <summary>
		/// 32 character string
		/// </summary>
		String,
		/// <summary>
		/// Either a 4 byte handle to a 128 character string
		/// or just a 128 character string
		/// </summary>
		StringId,
		/// <summary>
		/// 28 character 'preview' string of the 128 character string,
		/// then the 4 byte handle
		/// </summary>
		OldStringId,

		/// <summary>
		/// 8 bit integer
		/// </summary>
		ByteInteger,
		/// <summary>
		/// 16 bit integer
		/// </summary>
		ShortInteger,
		/// <summary>
		/// 32 bit integer
		/// </summary>
		LongInteger,
		/// <summary>
		/// 32 bit degrees floating point
		/// </summary>
		Angle,
		/// <summary>
		/// Four character code; Group tag
		/// </summary>
		Tag,

		/// <summary>
		/// 8 bit enumeration value
		/// </summary>
		ByteEnum,
		/// <summary>
		/// 16 bit enumeration value
		/// </summary>
		Enum,
		/// <summary>
		/// 32 bit enumeration value
		/// </summary>
		LongEnum,

		/// <summary>
		/// 8 bits representing booleans
		/// </summary>
		ByteFlags,
		/// <summary>
		/// 16 bits representing booleans
		/// </summary>
		WordFlags,
		/// <summary>
		/// 32 bits representing booleans
		/// </summary>
		LongFlags,

		/// <summary>
		/// 16 bit X, Y value
		/// </summary>
		Point2D,
		/// <summary>
		/// 16 bit Top, Left, Right, Bottom value
		/// </summary>
		Rectangle2D,
		/// <summary>
		/// 32 bit color in the rgb space
		/// </summary>
		RgbColor,
		/// <summary>
		/// 32 bit color in the argb space
		/// </summary>
		ArgbColor,

		/// <summary>
		/// 32 bit radian floating point
		/// </summary>
		Real,
		RealFraction,
		/// <summary>
		/// 32 bit real X, Y value
		/// </summary>
		RealPoint2D,
		/// <summary>
		/// 32 bit real X, Y, Z value
		/// </summary>
		RealPoint3D,
		/// <summary>
		/// 32 bit real I, J value
		/// </summary>
		RealVector2D,
		/// <summary>
		/// 32 bit real I, J, K value
		/// </summary>
		RealVector3D,
		/// <summary>
		/// 32 bit real I, J, K, W value
		/// </summary>
		RealQuaternion,
		/// <summary>
		/// 32 bit real Yaw, Pitch value
		/// </summary>
		RealEulerAngles2D,
		/// <summary>
		/// 32 bit real Yaw, Pitch, Roll value
		/// </summary>
		RealEulerAngles3D,
		/// <summary>
		/// 32 bit real X, Y, D value
		/// </summary>
		RealPlane2D,
		/// <summary>
		/// 32 bit real X, Y, Z, D value
		/// </summary>
		RealPlane3D,
		/// <summary>
		/// 32 bit real Red, Green, Blue value
		/// </summary>
		RealRgbColor,
		/// <summary>
		/// 32 bit real Alpha, Red, Green, Blue value
		/// </summary>
		RealArgbColor,

		/// <summary>
		/// 16 bit Lower and Upper value
		/// </summary>
		ShortBounds,
		/// <summary>
		/// 32 bit real Lower and Upper value
		/// </summary>
		AngleBounds,
		/// <summary>
		/// 32 bit real Lower and Upper value
		/// </summary>
		RealBounds,
		RealFractionBounds,

		/// <summary>
		/// Reference to a tag group definition
		/// </summary>
		TagReference,
		/// <summary>
		/// Reference data to a 0-n list of a specific structure definition
		/// </summary>
		Block,

		/// <summary>
		/// 8 bit reference to a block element
		/// </summary>
		/// <remarks>Reminder, like the other block index fields, this is a signed value</remarks>
		ByteBlockIndex,
		/// <summary>
		/// 16 bit reference to a block element
		/// </summary>
		ShortBlockIndex,
		/// <summary>
		/// 32 bit reference to a block element
		/// </summary>
		LongBlockIndex,

		/// <summary>
		/// Reference data to a binary blob of data (non-uniform so can't be handled via a block)
		/// </summary>
		Data,

		VertexBuffer,
		/// <summary>
		/// In-place structure inside of an existing structure definition which can define it's own versioning information
		/// </summary>
		Struct,

		/// <summary>
		/// A reference (pointer) to data which can also be null
		/// </summary>
		StructReference,
		/// <summary>
		/// A reference to resource specific data. Reference can be null
		/// </summary>
		/// <remarks>
		/// TODO: to replace <see cref="StructReference"/> as the field is currently acting as a fancy hack
		/// 
		/// For Halo 3 support and it's treatment of data as a resource.
		/// IE a 's_tag_d3d_vertex_buffer'
		/// </remarks>
		ResourceReference,

		/// <summary>
		/// Following this field is the first field in the field list which will be repeated N times
		/// </summary>
		ArrayStart,
		/// <summary>
		/// End of the array field list
		/// </summary>
		ArrayEnd,
		/// <summary>
		/// Field to align fields or to hold data which can't be defined with our field types
		/// </summary>
		Pad,
		/// <summary>
		/// Misc. bytes which are unknown to contain fields or just to align fields
		/// </summary>
		UnknownPad,
		/// <summary>
		/// Place holder empty bytes used for future expansion, deprecated after the versioning system was made
		/// </summary>
		UselessPad,
		/// <summary>
		/// Bytes which hold data which can't be defined correctly with our field types or aren't suppose to be byte swapped
		/// </summary>
		Skip,
		/// <summary>
		/// Editor related field
		/// </summary>
		Custom,
		/// <summary>
		/// Non-editor related field which is inline struct like in nature,
		/// but doesn't use any version related information.
		/// </summary>
		/// <remarks>
		/// Useful when for instance, there is a <see cref="System.Runtime.InteropServices.ComTypes.FILETIME"/>
		/// field in a definition and we don't want to use <see cref="TagInterface.Skip"/> fields
		/// </remarks>
		CustomData,

		#region Reserved
		// While it is not recommended to do the following in .NET (following MS's standards), this enumeration 
		// will possibly see an IO persistence system one day and I don't want to be arsed in versioning this...yet

		zUnused0,
		zUnused1,
		zUnused2,
		zUnused3,
		zUnused4,
		zUnused5,
		zUnused6,
		zUnused7,
		zUnused8,
		zUnused9,
		#endregion

		/// <summary>
		/// End of a list of fields
		/// </summary>
		Terminator,
		None = 0xFF,
	};

	internal static class FieldTypeExtensions
	{
		/// <summary>
		/// Checks to see if the type isn't editable by user tag editors (even in expert mode)
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		public static bool IsNonEditorField(this FieldType type)
		{
			return
				type == FieldType.VertexBuffer || 
				type == FieldType.Struct || 
				type == FieldType.ArrayStart || 
				type == FieldType.ArrayEnd || 
				type == FieldType.Pad ||
				type == FieldType.UnknownPad ||
				type == FieldType.UselessPad ||
				type == FieldType.Skip ||
				type == FieldType.CustomData ||
				type == FieldType.Terminator;
		}

		/// <summary>
		/// Checks to see if the type uses the recursive I\O scheme (a la tag files)
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		public static bool UsesRecursiveIo(this FieldType type)
		{
			return
				type == FieldType.OldStringId ||
				type == FieldType.StringId ||
				type == FieldType.TagReference ||
				type == FieldType.Block ||
				type == FieldType.Data ||
				type == FieldType.StructReference;
		}
	};
	#endregion

	#region IIOProcessable
	/// <summary>
	/// Formatting options used when performing I\O operations
	/// </summary>
	public enum IOProcess : byte
	{
		/// <summary>
		/// Process all data but skip the actual
		/// data our pointers (IE, to blocks) point to
		/// </summary>
		NoPointerData,

		/// <summary>
		/// Process all block data and other complex fields
		/// </summary>
		BlockDataReflexive,
	};

	/// <summary>
	/// Interface for formatted I\O operations
	/// </summary>
	public interface IIOProcessable
	{
		#region Cache
		/// <summary>
		/// Read data from a cache file using the specified formatting option
		/// </summary>
		/// <param name="c">Input</param>
		/// <param name="iop">formatting option</param>
		void Read(BlamLib.Blam.CacheFile c, IOProcess iop);

		/// <summary>
		/// Write data to a cache file using the specified formatting option
		/// </summary>
		/// <param name="c">Output</param>
		/// <param name="iop">formatting option</param>
		void Write(BlamLib.Blam.CacheFile c, IOProcess iop);
		#endregion

		#region Tag
		/// <summary>
		/// Read data from a tag stream using the specified formatting option
		/// </summary>
		/// <param name="ts"></param>
		/// <param name="iop">formatting option</param>
		void Read(IO.ITagStream ts, IOProcess iop);

		/// <summary>
		/// Write data to a tag stream using the specified formatting option
		/// </summary>
		/// <param name="ts"></param>
		/// <param name="iop">formatting option</param>
		void Write(IO.ITagStream ts, IOProcess iop);
		#endregion

		#region Generic Stream
		/// <summary>
		/// Read data from a stream using the specified formatting option
		/// </summary>
		/// <param name="input">Input</param>
		/// <param name="iop">formatting option</param>
		void Read(IO.EndianReader input, IOProcess iop);

		/// <summary>
		/// Write data to a stream using the specified formatting option
		/// </summary>
		/// <param name="output">Output</param>
		/// <param name="iop">formatting option</param>
		void Write(IO.EndianWriter output, IOProcess iop);
		#endregion
	};
	#endregion

	/// <summary>
	/// Interface for data that is usually based in memory but is then stored
	/// in other types of streams (IE memory to tag file on disk)
	/// </summary>
	public interface IMemoryStreamable : IO.IStreamable
	{
		/// <summary>
		/// Read data from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		void ReadHeader(IO.ITagStream ts);

		/// <summary>
		/// Write data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		void WriteHeader(IO.ITagStream ts);
	};

	/// <summary>
	/// The starting point for the tag interface system
	/// </summary>
	public abstract class Field
		: Blam.ICacheStreamable
		, IMemoryStreamable
		, IO.ITagStreamable
		, ICloneable
		, Blam.Cache.ICacheObject
		, INotifyPropertyChanged
	{
		#region FieldType
		protected FieldType fieldType;
		/// <summary>
		/// This field's type identifier
		/// </summary>
		public FieldType FieldType { get { return fieldType; } }
		#endregion

		#region Field Value
		/// <summary>
		/// Returns the value of the field
		/// </summary>
		public abstract object FieldValue { get; set; }
		#endregion

		#region Constructor stuff
		/// <summary>
		/// All inheriting classes need to declare what they are
		/// </summary>
		/// <param name="t"></param>
		protected Field(FieldType t) { fieldType = t; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused in most cases)</param>
		/// <returns>new field of this type</returns>
		public abstract Field CreateInstance(Field from);

		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guaranteed not to reference any tag data which we copied</returns>
		public abstract object Clone();

		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <param name="owner">New owner of this field</param>
		/// <returns>Complete copy guaranteed not to reference any tag data which we copied</returns>
		/// <remarks>Default implementation just calls <see cref="Clone()"/></remarks>
		public virtual object Clone(IStructureOwner owner) { return Clone(); }

		/// <summary>
		/// Copy this field's contents to another field of the same type, without creating an entire new Field object
		/// </summary>
		/// <param name="field"></param>
		public virtual void CopyTo(Field field)
		{
			Debug.Assert.If(field.fieldType == this.fieldType, "field was of another type, {0}. Should be {1}", field.fieldType, this.fieldType);
			// TODO: why did I have this following statement: ?
			//field.FieldValue = this.FieldValue;
		}
		#endregion

		#region IStreamable Members
		#region Cache
		/// <summary>
		/// Read the pointer data for this field
		/// </summary>
		/// <param name="c"></param>
		public virtual void ReadHeader(BlamLib.Blam.CacheFile c)	{ }

		/// <summary>
		/// Read the main body of this field
		/// </summary>
		/// <param name="c"></param>
		public virtual void Read(BlamLib.Blam.CacheFile c)			{ this.Read(c.InputStream); }

		/// <summary>
		/// Write the pointer data of this field
		/// </summary>
		/// <param name="c"></param>
		public virtual void WriteHeader(BlamLib.Blam.CacheFile c)	{ }

		/// <summary>
		/// Write the main body of this field
		/// </summary>
		/// <param name="c"></param>
		public virtual void Write(BlamLib.Blam.CacheFile c)			{ this.Write(c.OutputStream); }
		#endregion

		#region Tag
		/// <summary>
		/// Read the pointer data for this field from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public virtual void ReadHeader(IO.ITagStream ts)	{ }

		/// <summary>
		/// Read the main body of this field from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public virtual void Read(IO.ITagStream ts)			{ this.Read(ts.GetInputStream()); }

		/// <summary>
		/// Write the pointer data of this field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public virtual void WriteHeader(IO.ITagStream ts)	{ }

		/// <summary>
		/// Write the main body of this field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public virtual void Write(IO.ITagStream ts)			{ this.Write(ts.GetOutputStream()); }
		#endregion

		#region Generic Stream
		/// <summary>
		/// Read the pointer data for this field
		/// </summary>
		/// <param name="s"></param>
		public virtual void ReadHeader(BlamLib.IO.EndianReader s)	{ }

		/// <summary>
		/// Read the main body of this field
		/// </summary>
		/// <param name="s"></param>
		public abstract void Read(BlamLib.IO.EndianReader s);

		/// <summary>
		/// Write the pointer data of this field
		/// </summary>
		/// <param name="s"></param>
		public virtual void WriteHeader(BlamLib.IO.EndianWriter s)	{ }

		/// <summary>
		/// Write the main body of this field
		/// </summary>
		/// <param name="s"></param>
		public abstract void Write(BlamLib.IO.EndianWriter s);
		#endregion
		#endregion

		#region ICacheObject Members
		/// <summary>
		/// Preprocesses the field before it does any building
		/// </summary>
		/// <param name="owner"></param>
		public virtual void PreProcess(BlamLib.Blam.Cache.BuilderBase owner)	{ }

		/// <summary>
		/// Builds the field to the cache file.
		/// </summary>
		/// <remarks>By default, it calls the <c>Write(EndianWriter)</c> method.</remarks>
		/// <param name="owner"></param>
		/// <returns></returns>
		public virtual bool Build(BlamLib.Blam.Cache.BuilderBase owner)			{ this.Write(owner.CurrentStream); return true; }

		/// <summary>
		/// Postprocesses the field after building it into the cache file
		/// </summary>
		/// <param name="owner"></param>
		public virtual void PostProcess(BlamLib.Blam.Cache.BuilderBase owner)	{ }
		#endregion

		/// <summary>
		/// Overload to perform and special preprocessing
		/// on adding to a Definition object.
		/// </summary>
		/// <remarks>
		/// Always call this method definition (using 'base')
		/// before the override call's exit of execution
		/// </remarks>
		/// <param name="def"></param>
		public virtual void OnAddToDefinition(Definition def) { def.AddField(this); }

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public virtual DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, 0); }

		/// <summary>
		/// Exchanges data with a field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public abstract void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args);

		#region Util
		/// <summary>
		/// Checks to see if the field uses the recursive I\O scheme (a la tag files)
		/// </summary>
		/// <param name="fi"></param>
		/// <returns></returns>
		public static bool UsesRecursiveIo(Field fi) { return fi.fieldType.UsesRecursiveIo(); }
		#endregion

		#region INotifyPropertyChanged Members
		public event PropertyChangedEventHandler PropertyChanged;

		protected void OnPropertyChanged(string propertyName)
		{
			var propertyChanged = PropertyChanged;

			if (propertyChanged != null)
			{
				propertyChanged(this, new PropertyChangedEventArgs(propertyName));
			}
		}
		#endregion
	};

	/// <summary>
	/// A field which stores pointer(s) to the real data. IE first a header then real data, like a tag block.
	/// </summary>
	/// <see cref="TagInterface.Block{T}"/>
	/// <see cref="TagInterface.Data"/>
	/// <see cref="TagInterface.TagReference"/>
	public abstract class FieldWithPointers : Field
	{
		protected FieldWithPointers(FieldType t) : base(t) { }

		/// <summary>
		/// The pointer header offset
		/// </summary>
		protected uint headerOffset = 0;
		/// <summary>
		/// The offset were the field data begins in the stream
		/// </summary>
		protected uint relativeOffset = 0;
	};

	#region EditorField
	/// <summary>
	/// Field only used when applying an editor-only attribute to a member of a definition
	/// </summary>
	/// <remarks>Don't actually Add() the instance to the definition</remarks>
	public sealed class EditorField : Field
	{
		public EditorField() : base(FieldType.Custom) {}
		public override Field CreateInstance(Field from) { return new EditorField(); }
		public override object Clone() { return new EditorField(); }

		#region Field Value
		public override object FieldValue { get { return null; } set { } }
		#endregion

		#region IMemoryStreamable Members
		public override void Read(BlamLib.IO.EndianReader s) { }
		public override void Write(BlamLib.IO.EndianWriter s) { }
		#endregion

		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }
	};
	#endregion
}
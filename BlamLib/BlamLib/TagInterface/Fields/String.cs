/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.TagInterface
{
	#region String
	/// <summary>
	/// Types of strings supported by our string field
	/// </summary>
	public enum StringType : byte
	{
		/// <summary>
		/// Normal, 32 character string
		/// </summary>
		Normal,

		/// <summary>
		/// Unicode string
		/// </summary>
		Unicode,

		/// <summary>
		/// Ascii string
		/// </summary>
		Ascii,

		/// <summary>
		/// Null terminated string
		/// </summary>
		CString,

		/// <summary>
		/// 12 character Unicode string
		/// </summary>
		Halo1Profile,

		/// <summary>
		/// 16 character Unicode string
		/// </summary>
		Halo2Profile,
	};

	/// <summary>
	/// Blam String definition class
	/// </summary> [
	public sealed class String : Field
	{
		/// <summary>
		/// Type of supported string this field contains
		/// </summary>
		public StringType StringType;
		/// <summary>
		/// Max length this string can be
		/// </summary>
		public short Length = 0;

		#region Value
		private string mValue = "";

		/// <summary>
		/// The string
		/// </summary>
		public string Value
		{
			get { return mValue; }
			set
			{
				mValue = value;

				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with the string value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as string; }
		}

		/// <summary>
		/// Calls <c>Value</c>'s <c>Equals</c> method
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public override bool Equals(object obj)	{ return Value.Equals(obj); }
		/// <summary>
		/// Calls <c>Value</c>'s <c>GetHashCode</c> method
		/// </summary>
		/// <returns></returns>
		public override int GetHashCode()		{ return Value.GetHashCode(); }
		/// <summary>
		/// Calls <c>Value</c>'s <c>ToString</c> method
		/// </summary>
		/// <returns></returns>
		public override string ToString()		{ return Value; }
		#endregion

		#region Ctor
		/// <summary>
		/// Constructs a 32 character Blam string
		/// </summary>
		public String() :								this(StringType.Normal)					{}
		/// <summary>
		/// Construct a string field
		/// </summary>
		/// <param name="t">specific string type</param>
		public String(StringType t) :					base(FieldType.String)					{ StringType = t; }

		/// <summary>
		/// Constructs a Unicode string with the specified length
		/// </summary>
		/// <param name="length">max length of the string</param>
		public String(int length) :						this(StringType.Unicode)				{ Length = (short)length; }
		/// <summary>
		/// Construct a string field
		/// </summary>
		/// <param name="t">specific string type</param>
		/// <param name="length">max length of the string</param>
		public String(StringType t, int length) :		this(t)									{ Length = (short)length; }

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public String(String value) :					this(value.StringType, value.Length)	{ Value = value.Value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new String( (from as String).StringType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new String(this); }

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem()
		{
			//short length = Length;
			//short type = (short)StringType;
			//int flags = (int)((length << 24) & 0xff000000) |
			//			((length << 8) & 0x00ff0000) |
			//			((type >> 8) & 0x0000ff00) |
			//			((type >> 24) & 0x000000ff);

			//return new DefinitionFile.Item(this.fieldType, flags);
			Debug.Assert.If(false, "This is broken. Fix it :|"); // TODO: do it
			return DefinitionFile.Item.Null;
		}
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a String field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		/// <returns>True if exchange performed without error</returns>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input)
		{
			if		(StringType == StringType.Normal)		Value = input.ReadTagString();
			else if (StringType == StringType.Unicode)		Value = input.ReadUnicodeString(Length);
			else if (StringType == StringType.Ascii)		Value = input.ReadAsciiString(Length);
			else if (StringType == StringType.Halo1Profile)	Value = input.ReadUnicodeString(12);
			else if (StringType == StringType.Halo2Profile)	Value = input.ReadUnicodeString(16);
			else if (StringType == StringType.CString)		Value = input.ReadCString();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output)
		{
			if		(StringType == StringType.Normal)		output.Write(Value, false);
			else if (StringType == StringType.Unicode)		output.WriteUnicodeString(Value, Length);
			else if (StringType == StringType.Ascii)		output.Write(Value, Length);
			else if (StringType == StringType.Halo1Profile)	output.WriteUnicodeString(Value, 12);
			else if (StringType == StringType.Halo2Profile)	output.WriteUnicodeString(Value, 16);
			else if (StringType == StringType.CString)		output.Write(Value, true);
		}
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a string
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator string(String value) { return value.Value; }
		/// <summary>
		/// Implicit cast to a character array
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value as character array</returns>
		public static implicit operator char[](String value) { return value.Value.ToCharArray(); }
		#endregion

		/// <summary>
		/// Get a character in the field's string value
		/// </summary>
		/// <param name="index">index of the character to get</param>
		/// <returns>character at the index</returns>
		public char this[int index] { get { return this.Value[index]; } }

		/// <summary>
		/// Compare one field to a string (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs"></param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(TagInterface.String lhs, string rhs)				{ return lhs.Value == rhs; }
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(TagInterface.String lhs, TagInterface.String rhs){ return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare one field to a string (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(TagInterface.String lhs, string rhs)				{ return lhs.Value != rhs; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(TagInterface.String lhs, TagInterface.String rhs){ return lhs.Value != rhs.Value; }
		#endregion

		public int GetFieldSize() { return GetFieldSize(this); }

		static int GetFieldSize(String f)
		{
			switch(f.StringType)
			{
				case StringType.Normal: return 32;
				case StringType.Unicode: return f.Length * 2;
				case StringType.Ascii: return f.Length;
				case StringType.Halo1Profile: return 12 * 2;
				case StringType.Halo2Profile: return 16 * 2;

				case StringType.CString:
				default: throw new Debug.Exceptions.UnreachableException(f.StringType);
			}
		}

		#region Util
		/// <summary>
		/// Returns a new String field with long_string attributes
		/// </summary>
		public static String LongString { get { return new String(StringType.Ascii, 256); } }
		#endregion
	};
	#endregion

	#region StringId
	/// <summary>
	/// Blam String ID definition class
	/// </summary>
	public sealed class StringId : FieldWithPointers
	{
		/// <summary>
		/// Value returned by <see cref="GetStringValue()"/> when the value can't be retrieved 
		/// because the cache has encrypted debug data which can't be decrypted
		/// </summary>
		public const string kEncryptedResult = "ERROR_ENCRYPTED";

		public Blam.StringId Handle = Blam.StringId.Null;
		/// <summary>
		/// This is either a handle to a <see cref="BlamLib.Blam.CacheFile"/> or to a 
		/// <see cref="BlamLib.Managers.ITagIndex"/>, depending on where this string was
		/// loaded from
		/// </summary>
		internal Blam.DatumIndex OwnerId = Blam.DatumIndex.Null;

		#region Value
		/// <summary>
		/// Interfaces with a object[] built of: Handle, and OwnerId
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>Handle @ 0, Blam.StringID</item>
		/// <item>OwnerId @ 1, Blam.DatumIndex</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get
			{
				return new object[] {
										Handle,
										OwnerId,
									};
			}
			set
			{
				object[] val = value as object[];
				Handle = (Blam.StringId)val[0];
				OwnerId = (Blam.DatumIndex)val[1];

				OnPropertyChanged("FieldValue");
			}
		}

		public string GetStringValue()
		{
			if (OwnerId != Blam.DatumIndex.Null && Handle != Blam.StringId.Null)
			{
				var ti = Program.GetTagIndex(OwnerId);
				// TODO: can't decrypt these yet!
				//if (ti.Engine != BlamVersion.HaloReach_Xbox)
					return ti.StringIds.GetStringIdValue(Handle);
				//else
				//	return kEncryptedResult;
			}

			return "";
		}

		/// <summary>
		/// StringId's string value
		/// </summary>
		/// <returns><see cref="GetStringValue()"/></returns>
		public override string ToString() { return GetStringValue(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a string id field
		/// </summary>
		public StringId() :								base(FieldType.StringId)	{}
		/// <summary>
		/// Construct an old string id field
		/// </summary>
		/// <param name="is_old_string_id"></param>
		public StringId(bool is_old_string_id)	:		base(is_old_string_id ? FieldType.OldStringId : FieldType.StringId) {}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public StringId(StringId value) :				base(value.fieldType)		{ Handle = value.Handle; OwnerId = value.OwnerId; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new StringId(from.FieldType == FieldType.OldStringId); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new StringId(this); }
		#endregion

		/// <summary>
		/// Reset the value of this string id from a pure string value
		/// </summary>
		/// <param name="str"></param>
		public void ResetFromString(string str)
		{
			if (OwnerId != Blam.DatumIndex.Null && str != null)
			{
				str = str.Replace(' ', '_').ToLower();
				Program.GetTagIndex(OwnerId).StringIds.TryAndGetStringId(str, out Handle);
			}
		}

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a StringId field control
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
		#region Cache I\O
		static byte[] NullPreviewString = new byte[28];
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="c"></param>
		public override void ReadHeader(BlamLib.Blam.CacheFile c)
		{
			OwnerId = c.TagIndexManager.IndexId;

			if (c.EngineVersion == BlamVersion.Halo2_Alpha) // old string id
				c.InputStream.Seek(28, System.IO.SeekOrigin.Current); // preview string

			Handle.Read(c.InputStream, c.StringIds.Definition.Description);
		}
		/// <summary>
		/// Overridden so the code doesn't read anything (string data isn't stored in the tag data)
		/// </summary>
		/// <param name="c"></param>
		public override void Read(BlamLib.Blam.CacheFile c)	{}
		/// <summary>
		/// Stream the field to a cache
		/// </summary>
		/// <param name="c"></param>
		public override void WriteHeader(BlamLib.Blam.CacheFile c)
		{
			if (c.EngineVersion == BlamVersion.Halo2_Alpha) // old string id
				c.OutputStream.Write(NullPreviewString);

			Handle.Write(c.OutputStream);
		}
		/// <summary>
		/// Overridden so the code doesn't write anything (string data isn't stored in the tag data)
		/// </summary>
		/// <param name="c"></param>
		public override void Write(BlamLib.Blam.CacheFile c) {}
		#endregion

		#region Tag
		/// <summary>
		/// Stream the field header data from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidStringId"></exception>
		public override void ReadHeader(IO.ITagStream ts)
		{
			IO.EndianReader s = ts.GetInputStream();
			headerOffset = s.PositionUnsigned; // nifty for debugging
			OwnerId = ts.OwnerId;

			string value = null;

			try
			{
				if (fieldType == FieldType.OldStringId && ts.Flags.Test(IO.ITagStreamFlags.Halo2OldFormat_StringId))
					value = s.ReadAsciiString(28); // max of 28 characters in the string id in old builds

				var owner = Program.GetTagIndex(OwnerId);
				Handle.Read(s, owner.StringIds.Definition.Description);

				if (value != null /*&& Handle != Blam.StringID.Null*/)
				{
					owner.StringIds.TryAndGetStringId(value, out Handle);
					Handle = new Blam.StringId(Handle.Description, Handle.Index, Handle.Length, 
						-1); // HACK used to tell Read that we already read the string data (as this is an old halo 2 tag)
				}
			}
			catch (Exception ex)
			{
				throw new Exceptions.InvalidStringId(ex,
					base.headerOffset, uint.MaxValue,
					ts, Handle.Length, value);
			}
		}

		/// <summary>
		/// Stream the field from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		/// <exception cref="Exceptions.InvalidStringId"></exception>
		public override void Read(IO.ITagStream ts)
		{
			if (Handle.Set == -1) // HACK used to tell Read that we already read the string data (as this is an old halo 2 tag)
			{
				Handle = new Blam.StringId(Handle.Description, Handle.Index, Handle.Length, 0);
				return;
			}

			if (!ts.Flags.Test(IO.ITagStreamFlags.DontStreamStringData)) // stream the string id value if we can
			{
				IO.EndianReader s = ts.GetInputStream();
				relativeOffset = s.PositionUnsigned;
				string value = null;
				try
				{
					value = new string(s.ReadChars(Handle.Length));
					if (Handle != Blam.StringId.Null)
						Program.GetTagIndex(OwnerId).StringIds.TryAndGetStringId(value, out Handle);
				}
				catch (Exception ex)
				{
					throw new Exceptions.InvalidStringId(ex,
						base.headerOffset, base.relativeOffset, ts,
						Handle.Length, value);
				}
			}
		}

		/// <summary>
		/// Stream the field header data to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void WriteHeader(IO.ITagStream ts)
		{
			IO.EndianWriter s = ts.GetOutputStream();
			OwnerId = ts.OwnerId;

			Handle.Write(s);
		}

		/// <summary>
		/// Stream the field to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public override void Write(IO.ITagStream ts)
		{
			if (Handle != Blam.StringId.Null && !ts.Flags.Test(IO.ITagStreamFlags.DontStreamStringData))
			{
				string value = Program.GetTagIndex(OwnerId).StringIds.GetStringIdValue(Handle);
				ts.GetOutputStream().Write(value, value.Length);
			}
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Stream the field header data from a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void ReadHeader(IO.EndianReader s)
		{
			headerOffset = s.PositionUnsigned; // nifty for debugging
			var owner = Program.GetTagIndex(OwnerId);
			Handle.Read(s, owner.StringIds.Definition.Description);
		}

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Read(IO.EndianReader s)		{ }

		/// <summary>
		/// Stream the field header data to a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void WriteHeader(IO.EndianWriter s)	{ Handle.Write(s); }

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="s"></param>
		public override void Write(IO.EndianWriter s)		{ }
		#endregion
		#endregion
	};
	#endregion
}
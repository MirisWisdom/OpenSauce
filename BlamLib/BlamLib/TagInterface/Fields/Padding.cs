/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/﻿using System;
using System.Collections.Generic;

namespace BlamLib.TagInterface
{
	#region Pad
	/// <summary>
	/// Padding definition class
	/// </summary>
	public sealed class Pad : Field
	{
		#region Trace
		/// <summary>
		/// Pad field debug tracing log file reference
		/// </summary>
		/// <remarks>Mainly used for logging non-zero data in pad fields</remarks>
		private static Debug.Trace trace;

		/// <summary>
		/// Open the pad debug tracing log file
		/// </summary>
		[System.Diagnostics.Conditional("DEBUG")]
		static void OpenLog()
		{
			trace = new BlamLib.Debug.Trace("Pad Testing", "Tests for non-zero data in 'pad' fields");
		}

		static Pad() { OpenLog(); }

		/// <summary>
		/// Close the pad debug tracing log file
		/// </summary>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void CloseLog()
		{
			if (trace == null)
				System.Windows.Forms.MessageBox.Show(
					"CloseLog: Pad Testing.txt not open!",
					"Whoops");
			else
			{
				trace.CloseLog();
				trace = null;
			}
		}
		#endregion

		#region Value
		private int mValue = 0;

		/// <summary>
		/// The size of the padding in bytes
		/// </summary>
		public int Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with this Pad's length
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (int)value; }
		}
		#endregion

		#region Ctor
		/// <summary>
		/// Create a pad field
		/// </summary>
		private Pad() : base(FieldType.Pad) { }
		/// <summary>
		/// Create a pad field
		/// with a specified length
		/// </summary>
		/// <param name="length">Length in bytes of this pad</param>
		public Pad(int length) : this() { Value = length; }
		/// <summary>
		/// Create a pad field
		/// from another
		/// </summary>
		/// <param name="value"></param>
		public Pad(Pad value) : this(value.Value) { }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new Pad( (from as Pad).Value ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone() { return new Pad(this); }
		#endregion

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, this.Value); }

		#region Data Exchange
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input)
		{
#if false//DEBUG
			if (Value < 5) input.Seek(Value, System.IO.SeekOrigin.Current);
			else
			{
				byte[] data = input.ReadBytes(Value);
				for (int x = 0, y = 0, z = 0; x < data.Length; x++)
					if(data[x] != 0)
					{
						if (y == 0 || x == 0) y = x;
						z++;
					}
					else if (x != 0 && data[x - 1] != 0)
					{
						trace.WriteLine("Found data at offset 0x{0:X} covering 0x{1:X} bytes, in a pad field with a count of {1}", y, z, Value);
					}
				data = null;
			}
#else
			input.Seek(Value, System.IO.SeekOrigin.Current);
#endif
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output)
		{
			byte[] temp = new byte[Value];
			temp.Initialize();
			output.Write(temp);
			temp = null;
		}
		#endregion

		#region Util
		/// <summary>
		/// Returns a new pad field with the length of 1 byte
		/// </summary>
		public static Pad Byte { get { return new Pad(1); } }
		/// <summary>
		/// Returns a new pad field with the length of 2 bytes
		/// </summary>
		public static Pad Word { get { return new Pad(2); } }
		/// <summary>
		/// Returns a new pad field with the length of 4 bytes
		/// </summary>
		public static Pad DWord { get { return new Pad(4); } }
		/// <summary>
		/// Returns a new pad field with the length of 24 bits
		/// </summary>
		public static Pad _24 { get { return new Pad(3); } }
		/// <summary>
		/// Returns a new pad field with the length of 12 bytes
		/// </summary>
		public static Pad BlockHalo1 { get { return new Pad(12); } }
		/// <summary>
		/// Returns a new pad field with the length of 8 bytes
		/// </summary>
		/// <remarks>Cache based tag block</remarks>
		public static Pad BlockHalo2 { get { return new Pad(8); } }
		/// <summary>
		/// Returns a new pad field with the length of 12 bytes
		/// </summary>
		public static Pad BlockHalo3 { get { return new Pad(12); } }
		/// <summary>
		/// Returns a new pad field with the length of 20 bytes
		/// </summary>
		public static Pad DataHalo1 { get { return new Pad(20); } }
		/// <summary>
		/// Returns a new pad field with the length of 8 bytes
		/// </summary>
		/// <remarks>Cache based tag data</remarks>
		public static Pad DataHalo2 { get { return new Pad(8); } }
		/// <summary>
		/// Returns a new pad field with the length of 20 bytes
		/// </summary>
		public static Pad DataHalo3 { get { return new Pad(20); } }
		/// <summary>
		/// Returns a new pad field with the length of 16 bytes
		/// </summary>
		public static Pad ReferenceHalo1 { get { return new Pad(16); } }
		/// <summary>
		/// Returns a new pad field with the length of 8 bytes
		/// </summary>
		/// <remarks>Cache based tag reference</remarks>
		public static Pad ReferenceHalo2 { get { return new Pad(8); } }
		/// <summary>
		/// Returns a new pad field with the length of 16 bytes
		/// </summary>
		public static Pad ReferenceHalo3 { get { return new Pad(16); } }
		/// <summary>
		/// Returns a new pad field with the length of 32 bytes
		/// </summary>
		public static Pad VertexBuffer { get { return new Pad(32); } }
		#endregion
	};
	#endregion

	#region UnknownPad
	/// <summary>
	/// Unknown fields definition class
	/// </summary>
	public sealed class UnknownPad : Field
	{
		#region Trace
		internal bool TracingEnabled = false;
		/// <summary>
		/// Pad field debug tracing log file reference
		/// </summary>
		/// <remarks>Mainly used for logging non-zero data in pad fields</remarks>
		private static Debug.Trace trace;

		/// <summary>
		/// Open the pad debug tracing log file
		/// </summary>
		[System.Diagnostics.Conditional("DEBUG")]
		static void OpenLog()
		{
			trace = new BlamLib.Debug.Trace("Unknown Pad Testing", "Tests for non-zero data in 'unknown pad' fields");
		}

		static UnknownPad() { OpenLog(); }

		/// <summary>
		/// Close the pad debug tracing log file
		/// </summary>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void CloseLog()
		{
			if (trace == null)
				System.Windows.Forms.MessageBox.Show(
					"CloseLog: Unknown Pad Testing.txt not open!",
					"Whoops");
			else
			{
				trace.CloseLog();
				trace = null;
			}
		}
		#endregion

		#region Value
		private int mValue = 0;

		/// <summary>
		/// The size of the padding
		/// </summary>
		public int Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with this Pad's length
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (int)value; }
		}
		#endregion

		#region Ctor
		/// <summary>
		/// Create a pad field
		/// </summary>
		private UnknownPad() : base(FieldType.UnknownPad) { }
		/// <summary>
		/// Create a pad field
		/// with a specified length
		/// </summary>
		/// <param name="length">Length in bytes of this pad</param>
		public UnknownPad(int length) : this() { Value = length; }
		/// <summary>
		/// Create a pad field
		/// from another
		/// </summary>
		/// <param name="value"></param>
		public UnknownPad(UnknownPad value) : this(value.Value) { }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field </param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new UnknownPad((from as UnknownPad).Value); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone() { return new UnknownPad(this); }
		#endregion

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, this.Value); }

		#region Data Exchange
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }
		#endregion

		#region I\O
		byte[] Internal;
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input)
		{
			Internal = input.ReadBytes(Value);
#if DEBUG
			if (TracingEnabled && Value > 5)
			{
				byte[] data = input.ReadBytes(Value);
				for (int x = 0, y = 0, z = 0; x < data.Length; x++)
					if (data[x] != 0)
					{
						if (y == 0 || x == 0) y = x;
						z++;
					}
					else if (x != 0 && data[x - 1] != 0)
					{
						trace.WriteLine("Found data at offset 0x{0:X} covering 0x{1:X} bytes, in a pad field with a count of {1}", y, z, Value);
					}
				data = null;
			}
#endif
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output)
		{
			if (Internal != null)
				output.Write(Internal);
			else
			{
				byte[] temp = new byte[Value];
				temp.Initialize();
				output.Write(temp);
				temp = null;
			}
		}
		#endregion

		#region Util
		/// <summary>
		/// Returns a new pad field with the length of 1 byte
		/// </summary>
		public static UnknownPad Byte { get { return new UnknownPad(1); } }
		/// <summary>
		/// Returns a new pad field with the length of 2 bytes
		/// </summary>
		public static UnknownPad Word { get { return new UnknownPad(2); } }
		/// <summary>
		/// Returns a new pad field with the length of 4 bytes
		/// </summary>
		public static UnknownPad DWord { get { return new UnknownPad(4); } }
		/// <summary>
		/// Returns a new pad field with the length of 8 bytes
		/// </summary>
		/// <remarks>Cache based tag block</remarks>
		public static UnknownPad BlockHalo2 { get { return new UnknownPad(8); } }
		/// <summary>
		/// Returns a new pad field with the length of 12 bytes
		/// </summary>
		public static UnknownPad BlockHalo3 { get { return new UnknownPad(12); } }
		/// <summary>
		/// Returns a new pad field with the length of 8 bytes
		/// </summary>
		/// <remarks>Cache based tag data</remarks>
		public static UnknownPad DataHalo2 { get { return new UnknownPad(8); } }
		/// <summary>
		/// Returns a new pad field with the length of 20 bytes
		/// </summary>
		public static UnknownPad DataHalo3 { get { return new UnknownPad(20); } }
		/// <summary>
		/// Returns a new pad field with the length of 8 bytes
		/// </summary>
		/// <remarks>Cache based tag reference</remarks>
		public static UnknownPad ReferenceHalo2 { get { return new UnknownPad(8); } }
		/// <summary>
		/// Returns a new pad field with the length of 16 bytes
		/// </summary>
		public static UnknownPad ReferenceHalo3 { get { return new UnknownPad(16); } }
		/// <summary>
		/// Returns a new pad field with the length of 32 bytes
		/// </summary>
		public static UnknownPad VertexBuffer { get { return new UnknownPad(32); } }
		#endregion
	};
	#endregion

	#region UselessPad
	/// <summary>
	/// Padding definition class
	/// </summary>
	/// <remarks>
	/// Would appear that this is an internal feature that doesn't affect
	/// tag files and of course cache files. Its only for skipping over
	/// actual padding used for adding new fields to definitions
	/// </remarks>
	public sealed class UselessPad : Field
	{
		#region Value
		private int mValue = 0;

		/// <summary>
		/// The size of the padding
		/// </summary>
		public int Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with this Pad's length
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (int)value; }
		}
		#endregion

		#region Ctor
		/// <summary>
		/// Create useless pad
		/// </summary>
		private UselessPad() : base(FieldType.UselessPad) { }
		/// <summary>
		/// Create useless pad
		/// with a specified length
		/// </summary>
		/// <param name="length">Length of the useless padding in bytes</param>
		public UselessPad(int length) : this() { Value = length; }
		/// <summary>
		/// Create useless pad
		/// from another
		/// </summary>
		/// <param name="value"></param>
		public UselessPad(UselessPad value) : this(value.Value) { }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new UselessPad((from as UselessPad).Value); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone() { return new UselessPad(this); }
		#endregion

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, this.Value); }

		#region Data Exchange
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }
		#endregion

		#region I\O
		// TODO: I believe useless pad was added after alpha halo, so we should check for that and read\write if in a cache\tag
		#region Cache
		/// <summary>
		/// Seek the stream ahead however many bytes the size of this pad is
		/// </summary>
		/// <param name="c"></param>
		public override void Read(BlamLib.Blam.CacheFile c)
		{
			if (c.EngineVersion == BlamVersion.Halo2_Alpha || c.EngineVersion == BlamVersion.Halo2_Epsilon)
				c.InputStream.Seek(Value, System.IO.SeekOrigin.Current);
		}
		/// <summary>
		/// Overridden because it's useless
		/// </summary>
		/// <param name="c"></param>
		public override void Write(BlamLib.Blam.CacheFile c) { }
		#endregion

		#region Tag
		/// <summary>
		/// Seek the stream ahead however many bytes the size of this pad is
		/// </summary>
		/// <param name="ts"></param>
		public override void Read(IO.ITagStream ts)
		{
			if (ts.Flags.Test(IO.ITagStreamFlags.Halo2OldFormat_UselessPadding))
				ts.GetInputStream().Seek(Value, System.IO.SeekOrigin.Current);
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Overridden because it's useless
		/// </summary>
		/// <param name="s"></param>
		public override void Read(BlamLib.IO.EndianReader s) { }
		/// <summary>
		/// Overridden because it's useless
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output) { /*for (int x = 0; x < Value; x++) output.Write((byte)0);*/ }
		#endregion
		#endregion
	};
	#endregion

	#region Skip
	/// <summary>
	/// Skips byteswapping on an array of bytes (holds them in a byte array for you to do whatever with)
	/// </summary>
	public sealed class Skip : Field
	{
		#region Value
		private int mValue = 0;
		private byte[] mData;

		/// <summary>
		/// The # of bytes to skip
		/// </summary>
		public int Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// The skip data
		/// </summary>
		public byte[] Data
		{
			get { return mData; }
			set
			{
				mData = value;
				OnPropertyChanged("Data");
			}
		}

		/// <summary>
		/// Interfaces with a object[] built of: Value, and Data
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>Value @ 0, int</item>
		/// <item>Data @ 1, byte[]</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get
			{
				return new object[] {
					Value,
					Data,
				};
			}
			set
			{
				object[] val = value as object[];
				Value = (int)val[0];
				Data = val[1] as byte[];
			}
		}
		#endregion

		#region Conversion
		/// <summary>
		/// Get a 16-bit signed integer
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public short ToInt16(int index) { return BitConverter.ToInt16(Data, index); }
		/// <summary>
		/// Get a 16-bit unsigned integer
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public ushort ToUInt16(int index) { return BitConverter.ToUInt16(Data, index); }

		/// <summary>
		/// Get a 32-bit signed integer
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public int ToInt32(int index) { return BitConverter.ToInt32(Data, index); }
		/// <summary>
		/// Get a 32-bit unsigned integer
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public uint ToUInt32(int index) { return BitConverter.ToUInt32(Data, index); }

		/// <summary>
		/// Get a 32-bit real number
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public float ToSingle(int index) { return BitConverter.ToSingle(Data, index); }
		#endregion

		#region Ctor
		/// <summary>
		/// Create a skip field
		/// </summary>
		private Skip() : base(FieldType.Skip) { }
		/// <summary>
		/// Create a skip field
		/// with a specified length
		/// </summary>
		/// <param name="length">Length in bytes to skip</param>
		public Skip(int length) : this() { Value = length; }
		/// <summary>
		/// Create a skip field
		/// from another
		/// </summary>
		/// <param name="value"></param>
		public Skip(Skip value) : this(value.Value)
		{
			if (value.Data != null)
			{
				Data = new byte[Value];
				value.Data.CopyTo(Data, 0);
			}
		}

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from) { return new Skip( (from as Skip).Value ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone() { return new Skip(this); }
		#endregion

		/// <summary>
		/// Creates an object that can persist this field's properties to a stream
		/// </summary>
		/// <returns>Item with persist information</returns>
		/// <see cref="FieldUtil.CreateFromDefinitionItem"/>
		public override DefinitionFile.Item ToDefinitionItem() { return new DefinitionFile.Item(this.fieldType, this.Value); }

		#region Data Exchange
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args) { }
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input) { Data = input.ReadBytes(Value); }
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output)
		{
			bool nullit = false;
			if (Data == null)
			{
				nullit = true;
				Data = new byte[Value];
			}

			output.Write(Data);

			if (nullit) Data = null;
		}
		#endregion

		#region Util
		/// <summary>
		/// Get an array of bytes from the skip buffer
		/// </summary>
		/// <param name="offset">Index of the bytes to get</param>
		/// <param name="_out">Buffer to hold retrieved bytes</param>
		/// <param name="count">How many bytes to get</param>
		public void Get(int offset, byte[] _out, int count) { for (int x = 0; x < count; x++) _out[x] = Data[offset + x]; }
		/// <summary>
		/// Get a value from the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to get</param>
		/// <param name="value">16-bit signed integer</param>
		public void Get(int offset, out short value) { value = BitConverter.ToInt16(Data, offset); }
		/// <summary>
		/// Get a value from the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to get</param>
		/// <param name="value">16-bit unsigned integer</param>
		public void Get(int offset, out ushort value) { value = BitConverter.ToUInt16(Data, offset); }
		/// <summary>
		/// Get a value from the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to get</param>
		/// <param name="value">32-bit signed integer</param>
		public void Get(int offset, out int value) { value = BitConverter.ToInt32(Data, offset); }
		/// <summary>
		/// Get a value from the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to get</param>
		/// <param name="value">32-bit unsigned integer</param>
		public void Get(int offset, out uint value) { value = BitConverter.ToUInt32(Data, offset); }
		/// <summary>
		/// Get a value from the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to get</param>
		/// <param name="value">32-bit real number</param>
		public void Get(int offset, out float value) { value = BitConverter.ToSingle(Data, offset); }

		/// <summary>
		/// Set an array of bytes in the skip buffer
		/// </summary>
		/// <param name="offset">Index of the bytes to set</param>
		/// <param name="_in">Buffer of bytes to copy to the skip buffer</param>
		/// <param name="count">How many bytes to copy</param>
		public void Set(int offset, byte[] _in, int count) { for (int x = 0; x < count; x++) Data[offset + x] = _in[x]; }
		/// <summary>
		/// Set a value in the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to set</param>
		/// <param name="value">16-bit signed integer</param>
		public void Set(int offset, short value) { Set(offset, BitConverter.GetBytes(value), sizeof(short)); }
		/// <summary>
		/// Set a value in the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to set</param>
		/// <param name="value">16-bit unsigned integer</param>
		public void Set(int offset, ushort value) { Set(offset, BitConverter.GetBytes(value), sizeof(ushort)); }
		/// <summary>
		/// Set a value in the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to set</param>
		/// <param name="value">2-bit signed integer</param>
		public void Set(int offset, int value) { Set(offset, BitConverter.GetBytes(value), sizeof(int)); }
		/// <summary>
		/// Set a value in the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to set</param>
		/// <param name="value">32-bit unsigned integer</param>
		public void Set(int offset, uint value) { Set(offset, BitConverter.GetBytes(value), sizeof(uint)); }
		/// <summary>
		/// Set a value in the skip buffer
		/// </summary>
		/// <param name="offset">Index of the value to set</param>
		/// <param name="value">32-bit real number</param>
		public void Set(int offset, float value) { Set(offset, BitConverter.GetBytes(value), sizeof(float)); }
		#endregion
	};
	#endregion
}
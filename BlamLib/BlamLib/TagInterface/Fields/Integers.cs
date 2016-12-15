/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.TagInterface
{
	#region Byte Integer
	/// <summary>
	/// Byte Integer definition class
	/// </summary>
	public sealed class ByteInteger : Field
	{
		#region Value
		private byte mValue = 0;

		/// <summary>
		/// The value of this char integer (in a byte)
		/// </summary>
		public byte Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with the byte value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (byte)value; }
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
		public override string ToString()		{ return Value.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a byte field
		/// </summary>
		public ByteInteger() :							base(FieldType.ByteInteger)	{}
		/// <summary>
		/// Construct a byte field
		/// </summary>
		/// <param name="value">field value</param>
		public ByteInteger(byte value) :				this()						{ Value = value; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public ByteInteger(ByteInteger value) :			this()						{ this.Value = value.Value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new ByteInteger(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new ByteInteger(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a ByteInteger field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.IByteInteger).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.IByteInteger).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input) { Value = input.ReadByte(); }
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output) { output.Write(Value); }
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a unsigned byte
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>the field's value</returns>
		public static implicit operator byte(ByteInteger value) { return value.Value; }
		/// <summary>
		/// Explicit cast to a signed byte
		/// </summary>
		/// <param name="value">field being cast</param>
		/// <returns>the field's value</returns>
		public static explicit operator sbyte(ByteInteger value) { return (sbyte)value.Value; }
		/// <summary>
		/// Implicit cast from a unsigned byte to a byte field
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>byte field</returns>
		public static implicit operator ByteInteger(byte value) { return new ByteInteger(value); }
		/// <summary>
		/// Explicit cast to a signed byte
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>byte field</returns>
		public static explicit operator ByteInteger(sbyte value) { return new ByteInteger((byte)value); }
		#endregion

		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(ByteInteger lhs, ByteInteger rhs) { return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(ByteInteger lhs, ByteInteger rhs) { return lhs.Value != rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(ByteInteger lhs, ByteInteger rhs) { return lhs.Value >= rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(ByteInteger lhs, ByteInteger rhs) { return lhs.Value <= rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator > (ByteInteger lhs, ByteInteger rhs) { return lhs.Value > rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator < (ByteInteger lhs, ByteInteger rhs) { return lhs.Value < rhs.Value; }
		#endregion

		#region Math
		/// <summary>
		/// Perform mathematical operation (post-increment)
		/// </summary>
		/// <param name="value"></param>
		/// <returns>the same field passed to this function</returns>
		public static ByteInteger operator ++(ByteInteger value) { value.Value++; return value; }
		/// <summary>
		/// Perform mathematical operation (post-decrement)
		/// </summary>
		/// <param name="value"></param>
		/// <returns>the same field passed to this function</returns>
		public static ByteInteger operator --(ByteInteger value) { value.Value--; return value; }
		/// <summary>
		/// Perform mathematical operation (Add)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> + <paramref name="rhs"/></returns>
		public static byte operator +(ByteInteger lhs, ByteInteger rhs) { return (byte)(lhs.Value + rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Subtract)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> - <paramref name="rhs"/></returns>
		public static byte operator -(ByteInteger lhs, ByteInteger rhs) { return (byte)(lhs.Value - rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Multiply)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> * <paramref name="rhs"/></returns>
		public static byte operator *(ByteInteger lhs, ByteInteger rhs) { return (byte)(lhs.Value * rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Divide)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> / <paramref name="rhs"/></returns>
		public static byte operator /(ByteInteger lhs, ByteInteger rhs) { return (byte)(lhs.Value / rhs.Value); }
		#endregion
		#endregion
	};
	#endregion

	#region Short Integer
	/// <summary>
	/// Short Integer definition class
	/// </summary>
	public sealed class ShortInteger : Field
	{
		#region Value
		private short mValue = 0;

		/// <summary>
		/// The value of this short integer
		/// </summary>
		public short Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with the short value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (short)value; }
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
		public override string ToString()		{ return Value.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a short field
		/// </summary>
		public ShortInteger() :							base(FieldType.ShortInteger)	{}
		/// <summary>
		/// Construct a short field
		/// </summary>
		/// <param name="value">field value</param>
		public ShortInteger(short value) :				this()							{ Value = value; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public ShortInteger(ShortInteger value) :		this()							{ this.Value = value.Value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)								{ return new ShortInteger(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()													{ return new ShortInteger(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a ShortInteger field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.IShortInteger).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.IShortInteger).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input) { Value = input.ReadInt16(); }
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output) { output.Write(Value); }
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a signed integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator short(ShortInteger value) { return value.Value; }
		/// <summary>
		/// Implicit cast to a unsigned integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static explicit operator ushort(ShortInteger value) { return (ushort)value.Value; }
		/// <summary>
		/// Implicit cast from a signed integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new field with passed value</returns>
		public static implicit operator ShortInteger(short value) { return new ShortInteger(value); }
		/// <summary>
		/// Explicit cast from a unsigned integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new field with passed value</returns>
		public static explicit operator ShortInteger(ushort value) { return new ShortInteger((short)value); }
		#endregion

		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(ShortInteger lhs, ShortInteger rhs) { return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(ShortInteger lhs, ShortInteger rhs) { return lhs.Value != rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(ShortInteger lhs, ShortInteger rhs) { return lhs.Value >= rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(ShortInteger lhs, ShortInteger rhs) { return lhs.Value <= rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator >(ShortInteger lhs, ShortInteger rhs) { return lhs.Value > rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator <(ShortInteger lhs, ShortInteger rhs) { return lhs.Value < rhs.Value; }
		#endregion

		#region Math
		/// <summary>
		/// Perform mathematical operation (post-increment)
		/// </summary>
		/// <param name="value"></param>
		/// <returns>the same field passed to this function</returns>
		public static ShortInteger operator ++(ShortInteger value) { value.Value++; return value; }
		/// <summary>
		/// Perform mathematical operation (post-decrement)
		/// </summary>
		/// <param name="value"></param>
		/// <returns>the same field passed to this function</returns>
		public static ShortInteger operator --(ShortInteger value) { value.Value--; return value; }
		/// <summary>
		/// Perform mathematical operation (Add)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> + <paramref name="rhs"/></returns>
		public static short operator +(ShortInteger lhs, ShortInteger rhs) { return (short)(lhs.Value + rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Subtract)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> - <paramref name="rhs"/></returns>
		public static short operator -(ShortInteger lhs, ShortInteger rhs) { return (short)(lhs.Value - rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Multiply)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> * <paramref name="rhs"/></returns>
		public static short operator *(ShortInteger lhs, ShortInteger rhs) { return (short)(lhs.Value * rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Divide)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> / <paramref name="rhs"/></returns>
		public static short operator /(ShortInteger lhs, ShortInteger rhs) { return (short)(lhs.Value / rhs.Value); }
		#endregion
		#endregion
	};
	#endregion

	#region Long Integer
	/// <summary>
	/// Long Integer definition class
	/// </summary>
	public sealed class LongInteger : Field
	{
		#region Value
		private int mValue = 0;

		/// <summary>
		/// The value of this long integer (in a int value)
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
		/// Interfaces with the int value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (int)value; }
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
		public override string ToString()		{ return Value.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a long field
		/// </summary>
		public LongInteger() :							base(FieldType.LongInteger) {}
		/// <summary>
		/// Construct a long field
		/// </summary>
		/// <param name="value">field value</param>
		public LongInteger(int value) :					this()						{ Value = value; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public LongInteger(LongInteger value) :			this()						{ this.Value = value.Value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new LongInteger(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new LongInteger(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a LongInteger field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.ILongInteger).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.ILongInteger).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="input"></param>
		public override void Read(IO.EndianReader input) { Value = input.ReadInt32(); }
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="output"></param>
		public override void Write(IO.EndianWriter output) { output.Write(Value); }
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a signed integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator int(LongInteger value) { return value.Value; }
		/// <summary>
		/// Implicit cast to a unsigned integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator uint(LongInteger value) { return (uint)value.Value; }
		/// <summary>
		/// Implicit cast from a signed integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new field with passed value</returns>
		public static implicit operator LongInteger(int value) { return new LongInteger(value); }
		/// <summary>
		/// Explicit cast from a unsigned integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new field with passed value</returns>
		public static explicit operator LongInteger(uint value) { return new LongInteger((int)value); }
		#endregion

		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(LongInteger lhs, LongInteger rhs) { return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(LongInteger lhs, LongInteger rhs) { return lhs.Value != rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(LongInteger lhs, LongInteger rhs) { return lhs.Value >= rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(LongInteger lhs, LongInteger rhs) { return lhs.Value <= rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator >(LongInteger lhs, LongInteger rhs) { return lhs.Value > rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator <(LongInteger lhs, LongInteger rhs) { return lhs.Value < rhs.Value; }
		#endregion

		#region Math
		/// <summary>
		/// Perform mathematical operation (post-increment)
		/// </summary>
		/// <param name="value"></param>
		/// <returns>the same field passed to this function</returns>
		public static LongInteger operator ++(LongInteger value) { value.Value++; return value; }
		/// <summary>
		/// Perform mathematical operation (post-decrement)
		/// </summary>
		/// <param name="value"></param>
		/// <returns>the same field passed to this function</returns>
		public static LongInteger operator --(LongInteger value) { value.Value--; return value; }
		/// <summary>
		/// Perform mathematical operation (Add)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> + <paramref name="rhs"/></returns>
		public static int operator +(LongInteger lhs, LongInteger rhs) { return lhs.Value + rhs.Value; }
		/// <summary>
		/// Perform mathematical operation (Subtract)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> - <paramref name="rhs"/></returns>
		public static int operator -(LongInteger lhs, LongInteger rhs) { return lhs.Value - rhs.Value; }
		/// <summary>
		/// Perform mathematical operation (Multiply)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> * <paramref name="rhs"/></returns>
		public static int operator *(LongInteger lhs, LongInteger rhs) { return lhs.Value * rhs.Value; }
		/// <summary>
		/// Perform mathematical operation (Divide)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> / <paramref name="rhs"/></returns>
		public static int operator /(LongInteger lhs, LongInteger rhs) { return lhs.Value / rhs.Value; }
		#endregion
		#endregion
	};
	#endregion

	#region Tag
	/// <summary>
	/// Group tag id definition class
	/// </summary>
	public sealed class Tag : Field
	{
		#region Value
		private char[] mValue = new char[4];

		/// <summary>
		/// The value of this tag field (in a four character array)
		/// </summary>
		public char[] Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with the group tag value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as char[]; }
		}

		/// <summary>
		/// Build a new string containing the characters in this field's value
		/// </summary>
		/// <returns><c>Value</c> as a string</returns>
		public override string ToString() { return new string(Value); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a tag field
		/// </summary>
		public Tag() :									base(FieldType.Tag)	{}
		/// <summary>
		/// Construct a tag field
		/// </summary>
		/// <param name="tag"></param>
		public Tag(char[] tag) :						this()				{ this.Value = tag; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Tag(Tag value) :							this()				{ this.Value = value.Value; }
		/// <summary>
		/// Construct a tag field
		/// </summary>
		/// <param name="value"></param>
		public Tag(TagGroup value) :					this()				{ value.Tag.CopyTo(this.Value, 0); }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)					{ return new Tag(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()										{ return new Tag(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Tag field control
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
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er) { Value = er.ReadTag(); }
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew) { ew.WriteTag(Value); }
		#endregion
	};
	#endregion

	#region Enum
	/// <summary>
	/// Enum definition class
	/// </summary>
	public sealed class Enum : Field
	{
		#region Value
		private int mValue = 0;

		/// <summary>
		/// The value of this enumeration value (in int form)
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
		/// Interfaces with the int value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (int)value; }
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
		public override string ToString()		{ return Value.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a enum field
		/// </summary>
		public Enum() :										base(FieldType.Enum)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Enum(Enum value) :							base(value.fieldType)	{ this.Value = value.Value; }
		/// <summary>
		/// Construct a enum field
		/// </summary>
		/// <param name="t">specific enum type</param>
		public Enum(FieldType t) :							base(t)					{}
		/// <summary>
		/// Construct a enum field
		/// </summary>
		/// <param name="t">specific enum type</param>
		/// <param name="value">field value</param>
		public Enum(FieldType t, int value) :				base(t)					{ this.Value = value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new Enum( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new Enum(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Enum field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.IEnum).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.IEnum).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			if		(this.fieldType == FieldType.Enum)		Value = er.ReadInt16();
			else if (this.fieldType == FieldType.ByteEnum)	Value = er.ReadByte();
			else if (this.fieldType == FieldType.LongEnum)	Value = er.ReadInt32();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			if		(this.fieldType == FieldType.Enum)		ew.Write((short)Value);
			else if (this.fieldType == FieldType.ByteEnum)	ew.Write((byte)Value);
			else if (this.fieldType == FieldType.LongEnum)	ew.Write(Value);
		}
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a signed integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator int(Enum value) { return value.Value; }
		#endregion

		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(Enum lhs, Enum rhs) { return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(Enum lhs, Enum rhs) { return lhs.Value != rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(Enum lhs, Enum rhs) { return lhs.Value >= rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(Enum lhs, Enum rhs) { return lhs.Value <= rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator >(Enum lhs, Enum rhs) { return lhs.Value > rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator <(Enum lhs, Enum rhs) { return lhs.Value < rhs.Value; }
		#endregion

		#region Math
		/// <summary>
		/// Perform mathematical operation (Add)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> + <paramref name="rhs"/></returns>
		public static short operator +(Enum lhs, Enum rhs) { return (short)(lhs.Value + rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Subtract)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> - <paramref name="rhs"/></returns>
		public static short operator -(Enum lhs, Enum rhs) { return (short)(lhs.Value - rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Multiply)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> * <paramref name="rhs"/></returns>
		public static short operator *(Enum lhs, Enum rhs) { return (short)(lhs.Value * rhs.Value); }
		/// <summary>
		/// Perform mathematical operation (Divide)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> / <paramref name="rhs"/></returns>
		public static short operator /(Enum lhs, Enum rhs) { return (short)(lhs.Value / rhs.Value); }
		#endregion
		#endregion

		#region Util
		/// <summary>
		/// Returns a new Enum field with byte_enum attributes
		/// </summary>
		public static Enum Byte { get { return new Enum(FieldType.ByteEnum); } }
		/// <summary>
		/// Returns a new Enum field with long_enum attributes
		/// </summary>
		public static Enum Long { get { return new Enum(FieldType.LongEnum); } }
		#endregion
	};
	#endregion

	#region Flags
	/// <summary>
	/// Flags definition class
	/// </summary>
	public sealed class Flags : Field
	{
		#region Value
		private uint mValue = 0;

		/// <summary>
		/// The value of this flags field (in unsigned integer form)
		/// </summary>
		public uint Value
		{
			get { return mValue; }
			set
			{
				mValue = value;
				OnPropertyChanged("Value");
			}
		}

		/// <summary>
		/// Interfaces with the unsigned integer value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (uint)value; }
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
		public override string ToString()		{ return Value.ToString(); }

		/// <summary>
		/// Tests if <c>Value</c> has 'flag'
		/// </summary>
		/// <param name="flag">flag to test</param>
		/// <returns>True if <paramref name="flag"/> is set</returns>
		public bool Test(uint flag)				{ return (Value & flag) != 0; }
		/// <summary>
		/// Adds 'flags' to <c>Value</c>
		/// </summary>
		/// <param name="flags">flags to add</param>
		public void Add(uint flags)				{ Value |= flags; }
		/// <summary>
		/// Removes 'flags' from <c>Value</c>
		/// </summary>
		/// <param name="flags">flags to remove</param>
		public void Remove(uint flags)			{ Value &= ~flags; }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a flags field (LongFlags)
		/// </summary>
		public Flags() :									this(FieldType.LongFlags)	{}
		/// <summary>
		/// Construct a flags field
		/// </summary>
		/// <param name="_type">specific flags type</param>
		public Flags(FieldType _type) :						base(_type)					{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Flags(Flags value) :							base(value.fieldType)		{ this.Value = value.Value; }
		/// <summary>
		/// Construct a flags field
		/// </summary>
		/// <param name="t">specific flags type</param>
		/// <param name="value">field value</param>
		public Flags(FieldType t, uint value) :				base(t)						{ this.Value = value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)								{ return new Flags( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()													{ return new Flags(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Flags field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		/// <returns>True if exchange performed without error</returns>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.IFlags).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.IFlags).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			if		(this.fieldType == FieldType.ByteFlags)	Value = er.ReadByte();
			else if (this.fieldType == FieldType.WordFlags)	Value = er.ReadUInt16();
			else if (this.fieldType == FieldType.LongFlags)	Value = er.ReadUInt32();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			if		(this.fieldType == FieldType.ByteFlags)	ew.Write((byte)Value);
			else if (this.fieldType == FieldType.WordFlags)	ew.Write((ushort)Value);
			else if (this.fieldType == FieldType.LongFlags)	ew.Write(Value);
		}
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a signed integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator int(Flags value) { return (int)value.Value; }
		/// <summary>
		/// Implicit cast to a unsigned integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator uint(Flags value) { return value.Value; }
		/// <summary>
		/// Implicit cast from a signed integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new field with passed value</returns>
		public static implicit operator Flags(int value) { return new Flags(value); }
		/// <summary>
		/// Explicit cast from a unsigned integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new field with passed value</returns>
		public static explicit operator Flags(uint value) { return new Flags((int)value); }
		#endregion

		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(Flags lhs, Flags rhs) { return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(Flags lhs, Flags rhs) { return lhs.Value != rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(Flags lhs, Flags rhs) { return lhs.Value >= rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(Flags lhs, Flags rhs) { return lhs.Value <= rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator >(Flags lhs, Flags rhs) { return lhs.Value > rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator <(Flags lhs, Flags rhs) { return lhs.Value < rhs.Value; }
		#endregion
		#endregion

		#region Util
		/// <summary>
		/// Returns a new Flags field with byte_flags attributes
		/// </summary>
		public static Flags Byte { get { return new Flags(FieldType.ByteFlags); } }
		/// <summary>
		/// Returns a new Flags field with word_flags attributes
		/// </summary>
		public static Flags Word { get { return new Flags(FieldType.WordFlags); } }
		/// <summary>
		/// Returns a new Flags field with long_flags attributes
		/// </summary>
		public static Flags Long { get { return new Flags(); } }
		#endregion
	};
	#endregion

	#region Point2D
	/// <summary>
	/// Point2D definition class; X, Y
	/// </summary>
	public sealed class Point2D : Field
	{
		#region Value
		private short mX = 0;
		private short mY = 0;

		/// <summary>
		/// X value of this point
		/// </summary>
		public short X
		{
			get { return mX; }
			set
			{
				mX = value;
				OnPropertyChanged("X");
			}
		}

		/// <summary>
		/// Y value of this point
		/// </summary>
		public short Y
		{
			get { return mY; }
			set
			{
				mY = value;
				OnPropertyChanged("Y");
			}
		}

		/// <summary>
		/// short x, y
		/// </summary>
		public short[] Value
		{
			get { return new short[] { X, Y }; }
			set { X = value[0]; Y = value[1]; }
		}
		/// <summary>
		/// Interfaces with a short[] built of: X and Y
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>X @ 0</item>
		/// <item>Y @ 1</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as short[]; }
		}

		/// <summary>
		/// This field as a string
		/// </summary>
		/// <returns>"x:<c>X</c> y:<c>Y</c>"</returns>
		public override string ToString() { return string.Format("x:{0} y:{1}", X, Y); }
		#endregion

		/// <summary>
		/// Calls <c>Field</c>'s (our base class) <c>Equals</c> method
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public override bool Equals(object obj) { return base.Equals(obj); }
		/// <summary>
		/// Calls <c>Field</c>'s (our base class) <c>GetHashCode</c> method
		/// </summary>
		/// <returns></returns>
		public override int GetHashCode() { return base.GetHashCode(); }

		#region Construction
		/// <summary>
		/// Construct a point2d field
		/// </summary>
		public Point2D() :								base(FieldType.Point2D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Point2D(Point2D value) :					this()					{ this.X = value.X; this.Y = value.Y; }
		/// <summary>
		/// Construct a point2d field
		/// </summary>
		/// <param name="x">field value (X)</param>
		/// <param name="y">field value (Y)</param>
		public Point2D(short x, short y) :				this()					{ X = x; Y = y; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)						{ return new Point2D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()											{ return new Point2D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Point2D field control
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
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			X = er.ReadInt16();
			Y = er.ReadInt16();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(X);
			ew.Write(Y);
		}
		#endregion

		#region Operators
		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(Point2D lhs, Point2D rhs) { return (lhs.X == rhs.X) && (lhs.Y == rhs.Y); }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(Point2D lhs, Point2D rhs) { return (lhs.X != rhs.X) || (lhs.Y != rhs.Y); }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(Point2D lhs, Point2D rhs) { return (lhs.X >= rhs.X) && (lhs.Y >= rhs.Y); }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(Point2D lhs, Point2D rhs) { return (lhs.X <= rhs.X) && (lhs.Y <= rhs.Y); }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator >(Point2D lhs, Point2D rhs) { return (lhs.X > rhs.X) && (lhs.Y > rhs.Y); }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator <(Point2D lhs, Point2D rhs) { return (lhs.X < rhs.X) && (lhs.Y < rhs.Y); }
		#endregion

		#region Math
		/// <summary>
		/// Perform mathematical operation (Add)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> + <paramref name="rhs"/></returns>
		public static Point2D operator +(Point2D lhs, Point2D rhs) { return new Point2D((short)(lhs.X + rhs.X), (short)(lhs.Y + rhs.Y)); }
		/// <summary>
		/// Perform mathematical operation (Subtract)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> - <paramref name="rhs"/></returns>
		public static Point2D operator -(Point2D lhs, Point2D rhs) { return new Point2D((short)(lhs.X - rhs.X), (short)(lhs.Y - rhs.Y)); }
		/// <summary>
		/// Perform mathematical operation (Multiply)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> * <paramref name="rhs"/></returns>
		public static Point2D operator *(Point2D lhs, Point2D rhs) { return new Point2D((short)(lhs.X * rhs.X), (short)(lhs.Y * rhs.Y)); }
		/// <summary>
		/// Perform mathematical operation (Divide)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> / <paramref name="rhs"/></returns>
		public static Point2D operator /(Point2D lhs, Point2D rhs) { return new Point2D((short)(lhs.X / rhs.X), (short)(lhs.Y / rhs.Y)); }
		#endregion
		#endregion
	};
	#endregion

	#region Recangle2D
	/// <summary>
	/// Rectangle 2D definition class; Top, Left, Bottom, Right
	/// </summary>
	public sealed class Rectangle2D : Field
	{
		#region Value
		private short mT = 0;
		private short mL = 0;
		private short mB = 0;
		private short mR = 0;

		/// <summary>
		/// Top value
		/// </summary>
		public short T
		{
			get { return mT; }
			set
			{
				mT = value;
				OnPropertyChanged("T");
			}
		}

		/// <summary>
		/// Left value
		/// </summary>
		public short L
		{
			get { return mL; }
			set
			{
				mL = value;
				OnPropertyChanged("L");
			}
		}

		/// <summary>
		/// Bottom value
		/// </summary>
		public short B
		{
			get { return mB; }
			set
			{
				mB = value;
				OnPropertyChanged("B");
			}
		}

		/// <summary>
		/// Right value
		/// </summary>
		public short R
		{
			get { return mR; }
			set
			{
				mR = value;
				OnPropertyChanged("R");
			}
		}

		/// <summary>
		/// short t, l, b, r
		/// </summary>
		public short[] Value
		{
			get { return new short[] { T, L, B, R }; }
			set { T = value[0]; L = value[1]; B = value[2]; R = value[3]; }
		}

		/// <summary>
		/// Interfaces with a short[] built of: T, L, B and R
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>T @ 0</item>
		/// <item>L @ 1</item>
		/// <item>B @ 2</item>
		/// <item>R @ 3</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as short[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a rectangle2d field
		/// </summary>
		public Rectangle2D() :										base(FieldType.Rectangle2D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Rectangle2D(Rectangle2D value) :						this()						{ this.T = value.T; this.L = value.L; this.B = value.B; this.R = value.R; }
		/// <summary>
		/// Construct a rectangle2d field
		/// </summary>
		/// <param name="t">field value (T)</param>
		/// <param name="l">field value (L)</param>
		/// <param name="b">field value (B)</param>
		/// <param name="r">field value (R)</param>
		public Rectangle2D(short t, short l, short b, short r) :	this()						{ this.T = t; this.L = l; this.B = b; this.R = r; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new Rectangle2D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new Rectangle2D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Rectangle2D field control
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
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			T = er.ReadInt16();
			L = er.ReadInt16();
			B = er.ReadInt16();
			R = er.ReadInt16();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(T);
			ew.Write(L);
			ew.Write(B);
			ew.Write(R);
		}
		#endregion
	};
	#endregion

	#region Color
	/// <summary>
	/// Color definition class; A, R, G, B
	/// </summary>
	public sealed class Color : Field
	{
		#region Value
		private byte mA = 0;
		private byte mR = 0;
		private byte mG = 0;
		private byte mB = 0;

		/// <summary>
		/// Alpha value of this field
		/// </summary>
		public byte A
		{
			get { return mA; }
			set
			{
				mA = value;
				OnPropertyChanged("A");
			}
		}

		/// <summary>
		/// Red value of this field
		/// </summary>
		public byte R
		{
			get { return mR; }
			set
			{
				mR = value;
				OnPropertyChanged("R");
			}
		}

		/// <summary>
		/// Green value of this field
		/// </summary>
		public byte G
		{
			get { return mG; }
			set
			{
				mG = value;
				OnPropertyChanged("G");
			}
		}

		/// <summary>
		/// Blue value of this field
		/// </summary>
		public byte B
		{
			get { return mB; }
			set
			{
				mB = value;
				OnPropertyChanged("B");
			}
		}

		/// <summary>
		/// byte a, r, g, b
		/// </summary>
		public byte[] Value
		{
			get { return new byte[] { A, R, G, B }; }
			set { A = value[0]; R = value[1]; G = value[2]; B = value[3]; }
		}
		/// <summary>
		/// Interfaces with a byte[] built of: A, R, G, and B
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>A @ 0</item>
		/// <item>R @ 1</item>
		/// <item>G @ 2</item>
		/// <item>B @ 3</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as byte[]; }
		}

		/// <summary>
		/// This field as a string
		/// </summary>
		/// <returns>"a:<c>A</c> r:<c>R</c> g:<c>G</c> b:<c>B</c>"</returns>
		public override string ToString() { return string.Format("a:{0} r:{1} g:{2} b:{3}", A, R, G, B); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a Argb Color field
		/// </summary>
		public Color() : base(FieldType.ArgbColor)									{}
		/// <summary>
		/// Construct a color field
		/// </summary>
		/// <param name="t">specific color type</param>
		public Color(FieldType t) :						base(t)						{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Color(Color value) :						base(value.fieldType)		{ this.A = value.A; this.R = value.R; this.G = value.G; this.B = value.B; }
		/// <summary>
		/// Construct a Argb Color field
		/// </summary>
		/// <param name="a">field value (A)</param>
		/// <param name="r">field value (R)</param>
		/// <param name="g">field value (G)</param>
		/// <param name="b">field value (B)</param>
		public Color(byte a, byte r, byte g, byte b) :	this()						{ this.A = a; this.R = r; this.G = g; this.B = b; }
		/// <summary>
		/// Construct a Rgb Color field
		/// </summary>
		/// <param name="r">field value (R)</param>
		/// <param name="g">field value (G)</param>
		/// <param name="b">field value (B)</param>
		public Color(byte r, byte g, byte b) :			base(FieldType.RgbColor)	{ this.A = 0; this.R = r; this.G = g; this.B = b; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new Color( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new Color(this); }
		#endregion

		#region Util
		/// <summary>
		/// Convert to a real rgb color field
		/// </summary>
		/// <returns>new real (rgb) color field using this field's values</returns>
		public RealColor ToRgbColor()			{ return new RealColor(							R != 0 ? R / 255 : 0, G != 0 ? G / 255 : G, B != 0 ? B / 255 : B); }
		/// <summary>
		/// Convert to a real argb color field
		/// </summary>
		/// <returns>new real (argb) color field using this field's values</returns>
		public RealColor ToArgbColor()			{ return new RealColor(A != 0 ? A / 255 : 0,	R != 0 ? R / 255 : 0, G != 0 ? G / 255 : G, B != 0 ? B / 255 : B); }
		/// <summary>
		/// Convert to a .NET color object
		/// </summary>
		/// <returns>a new system color object using this field's values</returns>
		public System.Drawing.Color ToColor()	{ return System.Drawing.Color.FromArgb(A, R, G, B); }

		/// <summary>
		/// Returns a new Color field with rgb_color attributes
		/// </summary>
		public static Color Rgb { get { return new Color(FieldType.RgbColor); } }
		/// <summary>
		/// Returns a new Color field with argb_color attributes
		/// </summary>
		public static Color Argb { get { return new Color(); } }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Color field control
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
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void Read(IO.EndianReader s)
		{
			BlamVersion ver;
			if (s.Owner == null)				ver = BlamVersion.Unknown;
			else if (s.Owner is Blam.CacheFile)	ver = (s.Owner as Blam.CacheFile).EngineVersion;
			else								ver = (s.Owner as Managers.TagManager).Engine;

			if ((ver & BlamVersion.Halo1) != 0)
			{
				A = s.ReadByte(); // TODO: just bit operate this too
				R = s.ReadByte();
				G = s.ReadByte();
				B = s.ReadByte();
			}
			else
			{
				uint color = s.ReadUInt32();
				A = (byte)((color >> 24) & 0xFF);
				R = (byte)((color >> 16) & 0xFF);
				G = (byte)((color >> 8) & 0xFF);
				B = (byte) (color & 0xFF);
			}
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="s"></param>
		public override void Write(IO.EndianWriter s)
		{
			BlamVersion ver;
			if (s.Owner == null)				ver = BlamVersion.Unknown;
			else if (s.Owner is Blam.CacheFile)	ver = (s.Owner as Blam.CacheFile).EngineVersion;
			else								ver = (s.Owner as Managers.TagManager).Engine;

			if ((ver & BlamVersion.Halo1) != 0)
			{
				s.Write(A); // TODO: just bit operate this too
				s.Write(R);
				s.Write(G);
				s.Write(B);
			}
			else
				s.Write( (A << 24) | (R << 16) | (G << 8) | B );
		}
		#endregion
	};
	#endregion

	#region Short Bounds
	/// <summary>
	/// Short Integer Bounds definition class; Lower, Upper
	/// </summary>
	public sealed class ShortIntegerBounds : Field
	{
		#region Value
		private short mLower = 0;
		private short mUpper = 0;

		/// <summary>
		/// Lower bound of this field
		/// </summary>
		public short Lower
		{
			get { return mLower; }
			set
			{
				mLower = value;
				OnPropertyChanged("Lower");
			}
		}

		/// <summary>
		/// Upper bound of this field
		/// </summary>
		public short Upper
		{
			get { return mUpper; }
			set
			{
				mUpper = value;
				OnPropertyChanged("Upper");
			}
		}

		/// <summary>
		/// short lower, upper
		/// </summary>
		public short[] Value
		{
			get { return new short[] { Lower, Upper }; }
			set { Lower = value[0]; Upper = value[1]; }
		}
		/// <summary>
		/// Interfaces with a short[] built of: Lower and Upper
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>Lower @ 0</item>
		/// <item>Upper @ 1</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as short[]; }
		}

		/// <summary>
		/// This field as a string
		/// </summary>
		/// <returns>"<c>Lower</c> to <c>Upper</c>"</returns>
		public override string ToString() { return string.Format("{0} to {1}", Lower, Upper); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a short bounds field
		/// </summary>
		public ShortIntegerBounds() :							base(FieldType.ShortBounds) {}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public ShortIntegerBounds(ShortIntegerBounds value) :	this()						{ this.Lower = value.Lower; this.Upper = value.Upper; }
		/// <summary>
		/// Construct a short bounds field
		/// </summary>
		/// <param name="lower">field value (Lower)</param>
		/// <param name="upper">field value (Upper)</param>
		public ShortIntegerBounds(short lower, short upper) :	this()						{ this.Lower = lower; this.Upper = upper; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)									{ return new ShortIntegerBounds(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()														{ return new ShortIntegerBounds(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a ShortBounds field control
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
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			Lower = er.ReadInt16();
			Upper = er.ReadInt16();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(Lower);
			ew.Write(Upper);
		}
		#endregion
	};
	#endregion

	#region BlockIndex
	/// <summary>
	/// Block Index definition class
	/// </summary>
	public sealed class BlockIndex : Field
	{
		#region Value
		private int mValue = -1;

		/// <summary>
		/// Indexer value of this field
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
		/// Interfaces with the signed integer value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (int)value; }
		}

		/// <summary>
		/// Calls <c>Value</c>'s <c>ToString</c> method
		/// </summary>
		/// <returns></returns>
		public override string ToString()		{ return Value.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a block index field (ShortBlockIndex)
		/// </summary>
		public BlockIndex() :						this(FieldType.ShortBlockIndex)	{}
		/// <summary>
		/// Construct a block index field
		/// </summary>
		/// <param name="type">specific block index type</param>
		public BlockIndex(FieldType type) :			base(type)						{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public BlockIndex(BlockIndex value) :		base(value.fieldType)			{ this.Value = value.Value; }
		/// <summary>
		/// Construct a block index field
		/// </summary>
		/// <param name="t">specific block index type</param>
		/// <param name="value">field value</param>
		public BlockIndex(FieldType t, int value) :	base(t)							{ this.Value = value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new BlockIndex( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new BlockIndex(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a BlockIndex field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.IBlockIndex).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.IBlockIndex).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			if		(this.fieldType == FieldType.ByteBlockIndex)	Value = er.ReadSByte();
			else if (this.fieldType == FieldType.ShortBlockIndex)	Value = er.ReadInt16();
			else if (this.fieldType == FieldType.LongBlockIndex)	Value = er.ReadInt32();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			if		(this.fieldType == FieldType.ByteBlockIndex)	ew.Write((sbyte)Value);
			else if (this.fieldType == FieldType.ShortBlockIndex)	ew.Write((short)Value);
			else if (this.fieldType == FieldType.LongBlockIndex)	ew.Write(Value);
		}
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a integer
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator int(BlockIndex value) { return value.Value; }
		#endregion
		#endregion

		#region Util
		/// <summary>
		/// Returns a new BlockIndex field with byte_flags attributes
		/// </summary>
		public static BlockIndex Byte { get { return new BlockIndex(FieldType.ByteBlockIndex); } }
		/// <summary>
		/// Returns a new BlockIndex field with short_block_index attributes
		/// </summary>
		public static BlockIndex Word { get { return new BlockIndex(); } }
		/// <summary>
		/// Returns a new BlockIndex field with long_block_index attributes
		/// </summary>
		public static BlockIndex Long { get { return new BlockIndex(FieldType.LongBlockIndex); } }
		#endregion
	};
	#endregion
}
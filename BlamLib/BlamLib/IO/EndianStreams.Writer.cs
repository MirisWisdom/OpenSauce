/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;

namespace BlamLib.IO
{
	/// <summary>
	/// A binary stream with the ability to write data in big or little endian format
	/// </summary>
	public sealed class EndianWriter : EndianStream
	{
		#region Stream Properties
		/// <summary>
		/// Do we own the <see cref="file"/> object?
		/// </summary>
		bool streamOwner = true;
		/// <summary>
		/// Our stream
		/// </summary>
		BinaryWriter file = null;

		#region AutoFlush
		private bool autoFlush = true;
		/// <summary>
		/// Gets or sets a value indicating whether the <c>EndianWriter</c> will flush its 
		/// buffer to the underlying stream after every call to <c>EndianWriter.Write</c>*
		/// </summary>
		public bool AutoFlush
		{
			get { return autoFlush; }
			set { autoFlush = value; }
		}
		#endregion

		/// <summary>
		/// Closes the stream
		/// </summary>
		public override void Close()
		{
			if (file != null && streamOwner)
			{
				file.BaseStream.Flush();
				file.Close();
			}
			file = null;
			BaseClose();
		}
		#endregion

		#region Constructers
		/// <summary>
		/// Opens a binary file (little endian is default)
		/// </summary>
		/// <param name="filename">file</param>
		public EndianWriter(string filename)
		{
			try
			{
				FileStream stream = new FileStream(filename, FileMode.Open, FileAccess.Write, FileShare.Read);
				file = new BinaryWriter(stream);
				baseStream = file.BaseStream;
				fileName = filename;
			}
			catch (Exception ex)
			{
				throw new Debug.ExceptionLog(ex, "Failed to open {0}", filename);
			}
		}

		/// <summary>
		/// Opens a memory stream
		/// </summary>
		/// <param name="stream"></param>
		/// <remarks><see cref="Close"/> will not dispose <paramref name="stream"/>'s data</remarks>
		public EndianWriter(MemoryStream stream)
		{
			streamOwner = false;
			file = new BinaryWriter(baseStream = stream);
			fileName = "Memory";
		}

		/// <summary>
		/// Opens a memory stream using a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		public EndianWriter(byte[] bytes)
		{
			baseStream = new MemoryStream(bytes);
			file = new BinaryWriter(baseStream);
			fileName = "Buffer";
		}

		/// <summary>
		/// Opens a binary file (little endian is default)
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(string filename, object _owner) : this(filename)
		{ owner = _owner; }

		/// <summary>
		/// Opens a memory stream
		/// </summary>
		/// <param name="stream"></param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(MemoryStream stream, object _owner) : this(stream)
		{ owner = _owner; }

		/// <summary>
		/// Opens a memory stream using a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(byte[] bytes, object _owner) : this(bytes)
		{ owner = _owner; }

		/// <summary>
		/// Opens a binary file (little endian is default)
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="create">Create the file if it doesn't exist</param>
		public EndianWriter(string filename, bool create)
		{
			if (create && !File.Exists(filename))
				File.Create(filename).Close();

			try
			{
				FileStream stream = new FileStream(filename, FileMode.Open, FileAccess.Write, FileShare.Read);
				file = new BinaryWriter(stream);
				baseStream = file.BaseStream;
				fileName = filename;
			}
			catch (Exception ex)
			{
				throw new Debug.ExceptionLog(ex, "Failed to open {0}", filename);
			}
		}

		/// <summary>
		/// Opens a binary file (little endian is default)
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="create"></param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(string filename, object _owner, bool create) : this(filename, create)
		{ owner = _owner; }

		/// <summary>
		/// Opens a binary file
		/// </summary>
		/// <param name="filename">file</param>
		/// <param name="_state">bit ordering</param>
		public EndianWriter(string filename, EndianState _state) : this(filename)
		{ state = _state; }

		/// <summary>
		/// Opens a memory stream
		/// </summary>
		/// <param name="stream"></param>
		/// <param name="_state">bit ordering</param>
		public EndianWriter(MemoryStream stream, EndianState _state) : this(stream)
		{ state = _state; }

		/// <summary>
		/// Opens a memory stream using a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		/// <param name="_state">bit ordering</param>
		public EndianWriter(byte[] bytes, EndianState _state) : this(bytes)
		{ state = _state; }

		/// <summary>
		/// Opens a binary file
		/// </summary>
		/// <param name="filename">file</param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(string filename, EndianState _state, object _owner) : this(filename, _state)
		{ owner = _owner; }

		/// <summary>
		/// Opens a memory stream
		/// </summary>
		/// <param name="stream"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(MemoryStream stream, EndianState _state, object _owner) : this(stream, _state)
		{ owner = _owner; }

		/// <summary>
		/// Opens a memory stream using a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(byte[] bytes, EndianState _state, object _owner) : this(bytes, _state)
		{ owner = _owner; }

		/// <summary>
		/// Opens a binary file
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="create">Create the file if it doesn't exist</param>
		public EndianWriter(string filename, EndianState _state, bool create) : this(filename, create)
		{ state = _state; }

		/// <summary>
		/// Opens a binary file
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="create">Create the file if it doesn't exist</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianWriter(string filename, EndianState _state, object _owner, bool create) : this(filename, _state, create)
		{ owner = _owner; }
		#endregion

		#region Write
		/// <summary>
		/// Writes a boolean value
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(bool)"/>
		public void Write(bool value) { file.Write(value); if(autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a signed byte
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(sbyte)"/>
		public void Write(sbyte value) { file.Write(value); if (autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a unsigned byte
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(byte)"/>
		public void Write(byte value) { file.Write(value); if (autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a unsigned byte array
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(byte[])"/>
		public void Write(byte[] value)	{ file.Write(value); if (autoFlush) file.Flush(); }
		/// <summary>
		/// Writes a unsigned byte array
		/// </summary>
		/// <param name="value"></param>
		/// <param name="count"></param>
		/// <seealso cref="BinaryWriter.Write(byte[], int, int)"/>
		public void Write(byte[] value, int count) { file.Write(value, 0, count); if (autoFlush) file.Flush(); }
		/// <summary>
		/// Writes a unsigned byte array
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		/// <param name="count"></param>
		/// <seealso cref="BinaryWriter.Write(byte[], int, int)"/>
		public void Write(byte[] value, int index, int count) { file.Write(value, index, count); if (autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a character array
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(char[])"/>
		public void Write(char[] value) { file.Write(value); if (autoFlush) file.Flush(); }
		/// <summary>
		/// Writes a character array
		/// </summary>
		/// <param name="value"></param>
		/// <param name="count"></param>
		/// <seealso cref="BinaryWriter.Write(char[], int, int)"/>
		public void Write(char[] value, int count) { file.Write(value, 0, count); if (autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a blam group tag (4 char version)
		/// </summary>
		/// <param name="tag"></param>
		public void WriteTag(char[] tag)
		{
			if (state == EndianState.Little)
			{
				file.Write((byte)tag[3]);
				file.Write((byte)tag[2]);
				file.Write((byte)tag[1]);
				file.Write((byte)tag[0]);
			}
			else
			{
				file.Write((byte)tag[0]);
				file.Write((byte)tag[1]);
				file.Write((byte)tag[2]);
				file.Write((byte)tag[3]);
			}
			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a blam group tag (in a big endian
		/// byte ordering integer)
		/// </summary>
		/// <param name="tag"></param>
		public void WriteTag(uint tag)
		{
			if (state == EndianState.Little) ByteSwap.SwapUDWord(ref tag);
			file.Write(tag);
			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a blam group tag (4 char version)
		/// </summary>
		/// <param name="tag"></param>
		public void WriteTag(string tag) { WriteTag(tag.ToCharArray(0, 4)); if (autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a blam group tag (4 char version),
		/// extra control over if it should be reversed
		/// </summary>
		/// <param name="tag">tag fourCC</param>
		/// <param name="swap"></param>
		public void WriteTag(char[] tag, bool swap)
		{
			if (swap)
			{
				file.Write((byte)tag[3]);
				file.Write((byte)tag[2]);
				file.Write((byte)tag[1]);
				file.Write((byte)tag[0]);
			}
			else
			{
				file.Write((byte)tag[0]);
				file.Write((byte)tag[1]);
				file.Write((byte)tag[2]);
				file.Write((byte)tag[3]);
			}
			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a blam group tag (4 char version),
		/// extra control over if it should be reversed
		/// </summary>
		/// <param name="tag"></param>
		/// <param name="swap"></param>
		public void WriteTag(string tag, bool swap)
		{
			WriteTag(tag.ToCharArray(0, 4), swap);
			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a signed short
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(short)"/>
		public void Write(short value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapWord(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a unsigned short
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(ushort)"/>
		public void Write(ushort value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapUWord(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a signed int
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(int)"/>
		public void Write(int value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapDWord(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a unsigned int
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(uint)"/>
		public void Write(uint value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapUDWord(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a signed long
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(long)"/>
		public void Write(long value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapQWord(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a unsigned long
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(ulong)"/>
		public void Write(ulong value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapUQWord(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a floating point number (aka, real)
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(float)"/>
		public void Write(float value)
		{
			if (state == EndianState.Little)file.Write(value);
			else							file.Write(ByteSwap.SwapFloat(value));

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a string (prefixed with a length header, then the string)
		/// </summary>
		/// <param name="value"></param>
		/// <seealso cref="BinaryWriter.Write(string)"/>
		public void Write(string value) { file.Write(value); if (autoFlush) file.Flush(); }

		/// <summary>
		/// Writes a CString or Blam Tag String
		/// </summary>
		/// <param name="value"></param>
		/// <param name="cstring">True: writes value as a CString, False: writes value as a Blam Tag String</param>
		public void Write(string value, bool cstring)
		{
			if (!cstring)
			{
				for (int x = 0; x < 31; x++)
					file.Write(x < value.Length ? value[x] : '\0');
			}
			else
			{
				if (value.Length != 0)
					file.Write(value.ToCharArray());
			}

			file.Write(byte.MinValue); // write the null terminator

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Write an ASCII string with a certain length
		/// </summary>
		/// <param name="value"></param>
		/// <param name="length">Length of the string, including the null terminator</param>
		public void Write(string value, int length)
		{
			if (length == 0) return;

			length--; // adjust for the null terminator
			for (int x = 0; x < length; x++)
				file.Write(x < value.Length ? value[x] : '\0');

			file.Write(byte.MinValue); // write the null terminator

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Writes a Unicode string with a certain length
		/// </summary>
		/// <param name="value"></param>
		/// <param name="length">Length of the string, including the null terminator</param>
		public void WriteUnicodeString(string value, int length)
		{
			if (length == 0) return;

			length--; // adjust for the null terminator
			for (int x = 0; x < length; x++)
				Write(x < value.Length ? (ushort)value[x] : (ushort)'\0');

			file.Write(ushort.MinValue); // write the null terminator

			if (autoFlush) file.Flush();
		}

		/// <summary>
		/// Takes [value] and adds [BaseAddress] to it and writes the result
		/// </summary>
		/// <param name="value">Value to treat as a pointer</param>
		public void WritePointer(uint value)
		{
			if (value != 0)
				this.Write(value + this.baseAddress);
			else
				this.Write(uint.MinValue);
			
			if (autoFlush) file.Flush();
		}
		#endregion
	};
}
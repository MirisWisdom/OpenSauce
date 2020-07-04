/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;

namespace BlamLib.IO
{
	/// <summary>
	/// A binary stream with the ability to read data in big or little endian format
	/// </summary>
	public sealed class EndianReader : EndianStream
	{
		#region Stream Properties
		/// <summary>
		/// Do we own the <see cref="file"/> object?
		/// </summary>
		bool streamOwner = true;
		/// <summary>
		/// Our stream
		/// </summary>
		BinaryReader file = null;

		/// <summary>
		/// Closes the stream
		/// </summary>
		public override void Close()
		{
			if (file != null && streamOwner)
				file.Close();
			file = null;
			BaseClose();
		}
		#endregion

		#region Constructers
		/// <summary>
		/// Creates a new Endian specific stream. Default is little endian
		/// </summary>
		/// <param name="filename"></param>
		public EndianReader(string filename)
		{
			try
			{
				file = new BinaryReader(new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite));
				baseStream = file.BaseStream;
				fileName = filename;
			}
			catch (Exception ex)
			{
				throw new Debug.ExceptionLog(ex, "Failed to open {0}", filename);
			}
		}

		/// <summary>
		/// Creates a new Endian specific stream from a manifest file. Default is little endian
		/// </summary>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		public EndianReader(string path, string name)
		{
			file = new BinaryReader(Assembly.File.Open(path, name));
			baseStream = file.BaseStream;
			fileName = Path.Combine(path, name);
		}

		/// <summary>
		/// Creates a new Endian specific stream from a manifest file. Default is little endian
		/// </summary>
		/// <param name="ass">Assembly to get the manifest file from</param>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		public EndianReader(System.Reflection.Assembly ass, string path, string name)
		{
			file = new BinaryReader(Assembly.File.Open(ass, path, name));
			baseStream = file.BaseStream;
			fileName = string.Format("{0}+{1}", ass.FullName, Path.Combine(path, name));
		}

		/// <summary>
		/// Creates a new Endian specific stream from memory
		/// </summary>
		/// <param name="stream"></param>
		/// <remarks><see cref="Close"/> will not dispose <paramref name="stream"/>'s data</remarks>
		public EndianReader(MemoryStream stream)
		{
			streamOwner = false;
			file = new BinaryReader(baseStream = stream);
			fileName = "Memory";
		}

		/// <summary>
		/// Creates a new Endian specific stream from a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		public EndianReader(byte[] bytes)
		{
			baseStream = new MemoryStream(bytes);
			file = new BinaryReader(baseStream);
			fileName = "Buffer";
		}

		/// <summary>
		/// Creates a new Endian specific stream, with a certain format.
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="_state">bit ordering</param>
		public EndianReader(string filename, EndianState _state) : this(filename)
		{ state = _state; }

		/// <summary>
		/// Creates a new Endian specific stream from a manifest file, with a certain format.
		/// </summary>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		/// <param name="_state">bit ordering</param>
		public EndianReader(string path, string name, EndianState _state) : this(path, name)
		{ state = _state; }

		/// <summary>
		/// Creates a new Endian specific stream from a manifest file, with a certain format.
		/// </summary>
		/// <param name="ass">Assembly to get the manifest file from</param>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		/// <param name="_state">bit ordering</param>
		public EndianReader(System.Reflection.Assembly ass, string path, string name, EndianState _state) : this(ass, path, name)
		{ state = _state; }

		/// <summary>
		/// Creates a new Endian specific stream from memory
		/// </summary>
		/// <param name="stream"></param>
		/// <param name="_state">bit ordering</param>
		public EndianReader(MemoryStream stream, EndianState _state) : this(stream)
		{ state = _state; }

		/// <summary>
		/// Creates a new Endian specific stream from a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		/// <param name="_state">bit ordering</param>
		public EndianReader(byte[] bytes, EndianState _state) : this(bytes)
		{ state = _state; }

		/// <summary>
		/// Creates a new Endian specific stream, with a certain format.
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianReader(string filename, EndianState _state, object _owner) : this(filename, _state)
		{ owner = _owner; }

		/// <summary>
		/// Creates a new Endian specific stream from a manifest file, with a certain format.
		/// </summary>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianReader(string path, string name, EndianState _state, object _owner) : this(path, name, _state)
		{ owner = _owner; }

		/// <summary>
		/// Creates a new Endian specific stream from a manifest file, with a certain format.
		/// </summary>
		/// <param name="ass">Assembly to get the manifest file from</param>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianReader(System.Reflection.Assembly ass, string path, string name, EndianState _state, object _owner) : this(ass, path, name, _state)
		{ owner = _owner; }

		/// <summary>
		/// Creates a new Endian specific stream from memory
		/// </summary>
		/// <param name="stream"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianReader(MemoryStream stream, EndianState _state, object _owner) : this(stream, _state)
		{ owner = _owner; }

		/// <summary>
		/// Creates a new Endian specific stream from a byte buffer
		/// </summary>
		/// <param name="bytes"></param>
		/// <param name="_state">bit ordering</param>
		/// <param name="_owner">owner of this stream</param>
		public EndianReader(byte[] bytes, EndianState _state, object _owner) : this(bytes, _state)
		{ owner = _owner; }
		#endregion

		#region Read
		/// <summary>
		/// Peeks ahead and shit without advancing the stream position
		/// </summary>
		/// <returns>Next 32bits in the stream</returns>
		public int Peek() { return file.PeekChar(); }

		/// <summary>
		/// Reads a boolean value
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadBoolean()"/>
		public bool ReadBool() { return file.ReadBoolean(); }

		/// <summary>
		/// Gets a signed byte
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadSByte()"/>
		public sbyte ReadSByte() { return file.ReadSByte(); }

		/// <summary>
		/// Gets a unsigned byte
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadByte()"/>
		public byte ReadByte() { return file.ReadByte(); }

		/// <summary>
		/// Reads <paramref name="count"/> bytes from the stream with <paramref name="index"/> as the starting point in the byte array.
		/// </summary>
		/// <param name="buffer">The buffer to read data into.</param>
		/// <param name="index">The starting point in the buffer at which to begin reading into the buffer.</param>
		/// <param name="count">The number of characters to read.</param>
		/// <returns>
		/// The number of characters read into <paramref name="buffer"/>. This might be less than the number of bytes requested if that 
		/// many bytes are not available, or it might be zero if the end of the stream is reached.
		/// </returns>
		/// <seealso cref="BinaryReader.Read(byte[], int, int)"/>
		public int Read(byte[] buffer, int index, int count) { return file.Read(buffer, index, count); }

		/// <summary>
		/// Get an array of bytes
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadBytes(int)"/>
		public byte[] ReadBytes(int count) { return file.ReadBytes(count); }

		/// <summary>
		/// Get an array of bytes
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadByte()"/>
		public byte[] ReadBytes(uint count)
		{
			byte[] data = new byte[count];
			for (uint x = 0; x < count; x++)
				data[x] = file.ReadByte();
			return data;
		}

		/// <summary>
		/// Reads <paramref name="count"/> characters from the stream with <paramref name="index"/> as the starting point in the character array.
		/// </summary>
		/// <param name="buffer">The buffer to read data into.</param>
		/// <param name="index">The starting point in the buffer at which to begin reading into the buffer.</param>
		/// <param name="count">The number of characters to read.</param>
		/// <returns>
		/// The number of characters read into <paramref name="buffer"/>. This might be less than the number of bytes requested if that 
		/// many bytes are not available, or it might be zero if the end of the stream is reached.
		/// </returns>
		/// <seealso cref="BinaryReader.Read(char[], int, int)"/>
		public int Read(char[] buffer, int index, int count) { return file.Read(buffer, index, count); }

		/// <summary>
		/// Get an array of characters
		/// </summary>
		/// <param name="count"></param>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadChars(int)"/>
		public char[] ReadChars(int count) { return file.ReadChars(count); }

		/// <summary>
		/// Gets a tag id (4 chars)
		/// </summary>
		/// <returns>tag id</returns>
		public char[] ReadTag()
		{
			char[] tag = new char[4];
			if (state == EndianState.Little)
			{
				tag[3] = (char)file.ReadByte();
				tag[2] = (char)file.ReadByte();
				tag[1] = (char)file.ReadByte();
				tag[0] = (char)file.ReadByte();
			}
			else
			{
				tag[0] = (char)file.ReadByte();
				tag[1] = (char)file.ReadByte();
				tag[2] = (char)file.ReadByte();
				tag[3] = (char)file.ReadByte();
			}
			
			return tag;
		}

		/// <summary>
		/// Get a tag id
		/// </summary>
		/// <returns></returns>
		/// <remarks>Result will aways be in big endian byte ordering</remarks>
		/// <seealso cref="BinaryReader.ReadUInt32()"/>
		public uint ReadTagUInt()
		{
			uint value = file.ReadUInt32();
			if (state == EndianState.Little) ByteSwap.SwapUDWord(ref value);

			return value;
		}

		/// <summary>
		/// Gets a signed short
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadInt16()"/>
		public short ReadInt16()
		{
			if (state == EndianState.Little)return file.ReadInt16();
			else							return ByteSwap.SwapWord(file.ReadInt16());
		}

		/// <summary>
		/// Gets a unsigned short
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadUInt16()"/>
		public ushort ReadUInt16()
		{
			if (state == EndianState.Little)return file.ReadUInt16();
			else							return ByteSwap.SwapUWord(file.ReadUInt16());
		}

		/// <summary>
		/// Gets a signed int
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadInt32()"/>
		public int ReadInt32()
		{
			if (state == EndianState.Little)return file.ReadInt32();
			else							return ByteSwap.SwapDWord(file.ReadInt32());
		}

		/// <summary>
		/// Gets a unsigned int
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadUInt32()"/>
		public uint ReadUInt32()
		{
			if (state == EndianState.Little)return file.ReadUInt32();
			else							return ByteSwap.SwapUDWord(file.ReadUInt32());
		}

		/// <summary>
		/// Gets a signed long
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadInt64()"/>
		public long ReadInt64()
		{
			if (state == EndianState.Little)return file.ReadInt64();
			else							return ByteSwap.SwapQWord(file.ReadInt64());
		}

		/// <summary>
		/// Gets a unsigned long
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadUInt64()"/>
		public ulong ReadUInt64()
		{
			if (state == EndianState.Little)return file.ReadUInt64();
			else							return ByteSwap.SwapUQWord(file.ReadUInt64());
		}

		/// <summary>
		/// Gets a real number
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadSingle()"/>
		public float ReadSingle()
		{
			if (state == EndianState.Little)return file.ReadSingle();
			else							return ByteSwap.SwapFloat(file.ReadSingle());
		}

		/// <summary>
		/// Get a Blam String (31 chars + null terminator)
		/// </summary>
		/// <returns>halo string from stream</returns>
		/// <seealso cref="BinaryReader.ReadChars(int)"/>
		public string ReadTagString()
		{
			char[] buf = file.ReadChars(32);
			int x;
			for (x = 0; x < 31; x++)
				if (buf[x] == '\0') break;

			return new string(buf, 0, x);
		}

		/// <summary>
		/// Get a string from the stream (first byte is the length header, then comes the string)
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadString()"/>
		public string ReadString() { return file.ReadString(); }

		/// <summary>
		/// Reads a null terminated unicode (utf-16) string.
		/// </summary>
		/// <returns></returns>
		public string ReadUnicodeString()
		{
			System.Text.StringBuilder builder = new System.Text.StringBuilder();
			ushort ch = ReadUInt16();
			while (ch != 0)
			{
				builder.Append((char)ch);
				ch = ReadUInt16();
			}

			if (builder.Length != 0) return builder.ToString();
			
			return string.Empty;
		}

		/// <summary>
		/// Get a Blam Unicode String (variable char length)
		/// </summary>
		/// <param name="length">Length of the string, including the null terminator</param>
		/// <returns></returns>
		public string ReadUnicodeString(int length)
		{
			System.Text.StringBuilder builder = new System.Text.StringBuilder();
			length--; // adjust for the null terminator
			ushort ch = ReadUInt16();
			while (ch != 0 && --length > 0) // read until the first null character
			{
				builder.Append((char)ch);
				ch = ReadUInt16();
			}
			length++; // re-adjust for the null terminator

			if (length > 0) file.BaseStream.Seek(length * sizeof(ushort), SeekOrigin.Current);

			if (builder.Length != 0) return builder.ToString();

			return string.Empty;
		}

		/// <summary>
		/// Get a Blam ASCII String (variable char length)
		/// </summary>
		/// <param name="length">Length of the string, including the null terminator</param>
		/// <remarks>Optimized for reading fixed sized strings that may have not been memset'd 
		/// correctly (causing garbage data) but still have terminating zero</remarks>
		/// <returns></returns>
		public string ReadAsciiString(int length)
		{
			if (length == 0) return string.Empty;

			// due to encoding shit and cases where strings aren't memset'd before IO'd causing
			// garbage data to be IO'd with it, we have to read it as a byte array
			byte[] chars = file.ReadBytes(length);
			chars[length-1] = 0; // force a null terminator

			int end = 0;
			for (; end < chars.Length; end++) if (chars[end] == 0) break;
			if (end == 0) return string.Empty;
			char[] array = new char[end];
			for (int x = 0; x < end; x++) array[x] = (char)chars[x];
			return new string(array);
		}

		/// <summary>
		/// Reads a null terminated string from a  stream
		/// </summary>
		/// <returns>A string from the cstring that was loaded</returns>
		public string ReadCString()
		{
			byte btchar = 0;
			System.Text.StringBuilder cstring = new System.Text.StringBuilder();

			do
			{
				btchar = file.ReadByte();
				if (btchar != 0) cstring.Append((char)btchar);
			} while (btchar != 0);

			if (cstring.Length != 0) return cstring.ToString();

			return string.Empty;
		}

		/// <summary>
		/// Reads the next UInt32 value from the stream subtracting [BaseAddress] from the value and returning the result
		/// </summary>
		/// <returns></returns>
		/// <seealso cref="BinaryReader.ReadUInt32()"/>
		public uint ReadPointer()
		{
			uint ptr = this.ReadUInt32();

			if (ptr != 0)
				ptr -= this.baseAddress;

			return ptr;
		}
		#endregion
	};
}
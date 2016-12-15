/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;

namespace BlamLib.IO
{
	/// <summary>
	/// Interface an object with the endian streams
	/// </summary>
	public interface IStreamable
	{
		/// <summary>
		/// Reads the object from the endian stream object
		/// </summary>
		/// <param name="s"></param>
		void Read(EndianReader s);
		/// <summary>
		/// Writes the object to the endian stream object
		/// </summary>
		/// <param name="s"></param>
		void Write(EndianWriter s);
	};

	/// <summary>
	/// Byte ordering
	/// </summary>
	public enum EndianState
	{
		/// <summary>
		/// Stored in little endian byte arrangement
		/// </summary>
		Little,

		/// <summary>
		/// Stored in big endian byte arrangement
		/// </summary>
		Big,
	};

	/// <summary>
	/// Abstraction of the Endian stream data
	/// </summary>
	public abstract class EndianStream : IDisposable
	{
		#region State
		protected EndianState state = EndianState.Little;
		/// <summary>
		/// The byte order of the file
		/// </summary>
		public EndianState State
		{
			get { return state; }
			set { state = value; }
		}
		#endregion

		#region Owner
		protected object owner = null;
		/// <summary>
		/// Owner of this stream
		/// </summary>
		public object Owner { get { return owner; } }
		#endregion

		#region FileName
		protected string fileName;
		/// <summary>
		/// File that this stream is handling
		/// </summary>
		public string FileName { get { return fileName; } }
		#endregion

		#region BaseAddress
		protected uint baseAddress;
		/// <summary>
		/// Base address used for simulating pointers in the file
		/// </summary>
		public uint BaseAddress
		{
			get { return baseAddress; }
			set { baseAddress = value; }
		}
		#endregion


		#region Stream Properties
		protected Stream baseStream;
		/// <summary>
		/// returns the file's base stream
		/// </summary>
		public Stream BaseStream { get { return baseStream; } }

		/// <summary>
		/// Gets\Sets the stream's cursor position
		/// </summary>
		public int Position
		{
			get { return (int)baseStream.Position; }
			set { baseStream.Position = value; }
		}
		/// <summary>
		/// Gets\Sets the stream's cursor position
		/// </summary>
		public uint PositionUnsigned
		{
			get { return (uint)baseStream.Position; }
			set { baseStream.Position = value; }
		}

		/// <summary>
		/// Moves the stream cursor to 'offset' relative to the beginning of the file
		/// </summary>
		/// <param name="offset"></param>
		public void Seek(uint offset) { baseStream.Seek(offset, SeekOrigin.Begin); }

		/// <summary>
		/// Moves the stream cursor to 'offset' relative to origin
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="origin"></param>
		public void Seek(uint offset, SeekOrigin origin) { baseStream.Seek(offset, origin); }

		/// <summary>
		/// Moves the stream cursor to 'offset' relative to the beginning of the file
		/// </summary>
		/// <param name="offset"></param>
		public void Seek(int offset) { baseStream.Seek(offset, SeekOrigin.Begin); }

		/// <summary>
		/// Moves the stream cursor to 'offset' relative to origin
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="origin"></param>
		public void Seek(int offset, SeekOrigin origin) { baseStream.Seek(offset, origin); }

		/// <summary>
		/// Returns the length of the stream
		/// </summary>
		public int Length { get { return (int)baseStream.Length; } }

		/// <summary>
		/// Disposes EndianStream data
		/// </summary>
		protected void BaseClose()
		{
			baseStream = null;

			state = EndianState.Little;
		}

		/// <summary>
		/// Closes the stream
		/// </summary>
		public abstract void Close();
		#endregion

		#region IDisposable Members
		/// <summary>
		/// Calls <see cref="Close()"/>
		/// </summary>
		public void Dispose() { Close(); }
		#endregion
	};
}
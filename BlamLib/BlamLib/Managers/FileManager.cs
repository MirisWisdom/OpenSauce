/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using BlamLib.TagInterface;

namespace BlamLib.Managers
{
	/// <summary>
	/// 
	/// </summary>
	/// <remarks>
	/// Only the high order bits in the flags are streamed to file (16 to 31)
	/// </remarks>
	public sealed class FileManager
	{
		#region Constants
		/// <summary>
		/// Size of the file header
		/// </summary>
		const int HeaderSize = 
			2 +				// endian switch
			2 + 4 + 2 +		// version, signature, high order flag bits
			2 + 4 +			// data version, data signature

			4 + 4 +			// reserved (data directories)
			4 + 4 +			// reserved (file data)
			0
			;

		/// <summary>
		/// Little endian id
		/// </summary>
		const ushort HeaderEndian = 0xFF00;
		/// <summary>
		/// Big endian id
		/// </summary>
		const ushort HeaderEndianSwap = 0x00FF;
		/// <summary>
		/// File header version
		/// </summary>
		const short HeaderVersion = 0;

		/// <summary>
		/// Bit representing that the file data is compressed
		/// </summary>
		internal const uint CompressedBit = 1 << 16;
		/// <summary>
		/// Bit representing that the file data is encrypted
		/// </summary>
		/// <remarks>File data should ALWAYS be compressed first if it will use compression and encryption</remarks>
		internal const uint EncryptedBit = 1 << 17;
		#endregion

		#region Owner
		private object owner;
		/// <summary>
		/// Owner object of this file manager
		/// </summary>
		public object Owner { get { return owner; } }
		#endregion

		#region Flags
		private Util.Flags flags = new Util.Flags(0);
		/// <summary>
		/// Special flags
		/// </summary>
		public Util.Flags Flags
		{
			get { return flags; }
			set { flags = value; }
		}
		#endregion

		#region Path
		private string path;
		/// <summary>
		/// Full path to the file
		/// </summary>
		public string Path { get { return path; } }
		#endregion

		#region EndianState
		private IO.EndianState endianState;
		/// <summary>
		/// Byte ordering for the file
		/// </summary>
		public IO.EndianState EndianState { get { return endianState; } }

		private void SetEndianState(IO.EndianState state)
		{
			endianState = state;
			if (InputStream != null) InputStream.State = state;
			if (OutputStream != null) OutputStream.State = state;
		}
		#endregion

		#region Data
		IO.FileManageable data;
		/// <summary>
		/// Data for the file
		/// </summary>
		public IO.FileManageable Data { get { return data; } }
		#endregion

		#region Manage
		/// <summary>
		/// Manages a streamable object
		/// </summary>
		/// <param name="obj">Object containing stream data</param>
		public void Manage(IO.FileManageable obj)
		{
			Debug.Assert.If(obj, "File: Couldn't manage!");

			data = obj;
		}
		#endregion

		#region Streams
		IO.EndianReader InputStream;
		IO.EndianWriter OutputStream;
		#endregion

		#region Ctor
		/// <summary>
		/// Manage the file <paramref name="path"/>
		/// </summary>
		/// <param name="path">path to file we are to manage</param>
		public FileManager(string path) { this.path = path; }

		/// <summary>
		/// Manage the file <paramref name="path"/>, defaulting to the endian ordering of <paramref name="endian_state"/>
		/// </summary>
		/// <param name="path">path to file we are to manage</param>
		/// <param name="endian_state">endian ordering to default to</param>
		public FileManager(string path, IO.EndianState endian_state) : this(path) { endianState = endian_state; }

		/// <summary>
		/// Manage the file <paramref name="path"/>, using <paramref name="owner"/> as this file manager's owner
		/// </summary>
		/// <param name="path">path to file we are to manage</param>
		/// <param name="owner">owner object of this manager</param>
		public FileManager(string path, object owner) : this(path) { this.owner = owner; }

		/// <summary>
		/// Manage the file <paramref name="path"/>, defaulting to the endian ordering of <paramref name="endian_state"/>, 
		/// using <paramref name="owner"/> as this file manager's owner
		/// </summary>
		/// <param name="path">path to file we are to manage</param>
		/// <param name="endian_state">endian ordering to default to</param>
		/// <param name="owner">owner object of this manager</param>
		public FileManager(string path, IO.EndianState endian_state, object owner) : this(path, endian_state) { this.owner = owner; }
		#endregion

		#region Create\Open\Close
		private void SetupBaseAddress()
		{
			if (InputStream != null) InputStream.BaseAddress = 0x0;
			if (OutputStream != null) OutputStream.BaseAddress = 0x0;
		}

		/// <summary>
		/// Creates the file
		/// </summary>
		/// <remarks>Closes existing file streams</remarks>
		public void Create()
		{
			string dir = System.IO.Path.GetDirectoryName(path);
			if (!System.IO.Directory.Exists(dir)) System.IO.Directory.CreateDirectory(dir);

			if (!System.IO.File.Exists(path)) System.IO.File.Create(path).Close();

			if (InputStream != null) { InputStream.Close(); InputStream = null; }
			if (OutputStream != null) { OutputStream.Close(); OutputStream = null; }
		}

		/// <summary>
		/// Creates the file and opens
		/// it for writing
		/// </summary>
		/// <remarks>Closes existing file streams</remarks>
		public void CreateForWrite()
		{
			Create();

			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this, true);

			SetupBaseAddress();
		}

		/// <summary>
		/// Creates the file and opens
		/// it for reading and writing
		/// </summary>
		/// <remarks>Closes existing file streams</remarks>
		public void CreateForReadWrite()
		{
			Create();

			InputStream = new BlamLib.IO.EndianReader(path, endianState, this);
			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this, true);

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens the file for reading
		/// </summary>
		/// <remarks>Closes existing file streams</remarks>
		public void OpenForRead()
		{
			Close();

			InputStream = new BlamLib.IO.EndianReader(path, endianState, this);

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens the file for writing
		/// </summary>
		/// <remarks>Closes existing file streams</remarks>
		public void OpenForWrite()
		{
			Close();

			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this);

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens the file for reading and writing
		/// </summary>
		/// <remarks>Closes existing file streams</remarks>
		public void OpenForReadWrite()
		{
			Close();

			InputStream = new BlamLib.IO.EndianReader(path, endianState, this);
			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this);

			SetupBaseAddress();
		}

		/// <summary>
		/// Closes the file manager's streams
		/// </summary>
		public void Close()
		{
			if (this.InputStream != null) { InputStream.Close(); InputStream = null; }
			if (this.OutputStream != null) { OutputStream.Close(); OutputStream = null; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Read and preprocess header information
		/// </summary>
		private void ReadPreprocess()
		{
			char[] gtag;
			#region Endian Switch
			ushort us_temp = InputStream.ReadUInt16();
			if (us_temp == HeaderEndian) SetEndianState(IO.EndianState.Little);
			else if (us_temp == HeaderEndianSwap) SetEndianState(IO.EndianState.Big);
			else throw new Debug.ExceptionLog("File: Invalid switch! {0:X4}", us_temp);
			#endregion

			#region Version
			us_temp = InputStream.ReadUInt16();
			Debug.Assert.If(us_temp == HeaderVersion,
				"File: mismatching file version! [{0}, !{1}",
				HeaderVersion, us_temp);
			#endregion
			#region Signature
			gtag = InputStream.ReadTag();
			Debug.Assert.If(TagGroup.Test(gtag, (char[])IO.TagGroups.BlamLib),
				"File: invalid signature! [{0} !{1}]",
				IO.TagGroups.BlamLib.TagToString(), new string(gtag));
			#endregion
			flags.Reset((uint)(InputStream.ReadUInt16() << 16) | flags);

			#region Data Version
			us_temp = InputStream.ReadUInt16();
			Debug.Assert.If(us_temp == data.Attribute.Version,
				"File: mismatching data version! [{0}, !{1}",
				data.Attribute.Version, us_temp);
			#endregion
			#region Data Signature
			gtag = InputStream.ReadTag();
			Debug.Assert.If(TagGroup.Test(gtag, (char[])data.GroupTag),
				"File: invalid data signature! [{0} !{1}]",
				data.GroupTag.TagToString(), new string(gtag));
			#endregion

			#region Data Directories
			InputStream.ReadInt32();
			InputStream.ReadInt32();
			#endregion
			#region Data
			InputStream.ReadInt32();
			int data_offset = InputStream.ReadInt32();
			#endregion

			Debug.Assert.If(InputStream.Position == HeaderSize, 
				"File: Update header size! [{0}, !{1}]", HeaderSize, InputStream.Position);

			InputStream.Seek(data_offset);
		}

		/// <summary>
		/// Read the object from the supplied I\O stream
		/// </summary>
		public void Read()
		{
			Debug.Assert.If(InputStream, "Tried to read a file that we didn't open. {0}", path);

			ReadPreprocess();

			data.Read(InputStream);
		}


		/// <summary>
		/// Write the header information
		/// </summary>
		private void WritePreprocess()
		{
			OutputStream.Write(HeaderEndian);

			OutputStream.Write(HeaderVersion);
			OutputStream.WriteTag(IO.TagGroups.BlamLib.Tag);
			OutputStream.Write((ushort)(flags >> 16));

			OutputStream.Write(data.Attribute.Version);
			OutputStream.WriteTag(data.GroupTag.Tag);

			OutputStream.Write((int)0);
			OutputStream.Write((int)0);

			OutputStream.Write((int)0);
			OutputStream.Write(OutputStream.Position + sizeof(int));
			
			Debug.Assert.If(OutputStream.Position == HeaderSize, 
				"File: Update header size! [{0}, !{1}]", HeaderSize, OutputStream.Position);
		}

		/// <summary>
		/// Write the object to the supplied I\O stream
		/// </summary>
		public void Write()
		{
			Debug.Assert.If(OutputStream, "Tried to write a tag that we didn't open. {0}", path);

			WritePreprocess();

			data.Write(OutputStream);
		}
		#endregion
	};
}
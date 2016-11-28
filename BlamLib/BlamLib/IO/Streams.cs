/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;
using System.Xml;

namespace BlamLib.IO
{
	/// <summary>
	/// Encapsulates the manipulation of a process's memory into a Stream object.
	/// </summary>
	public sealed class ProcessMemoryStream : Stream
	{
		#region Stream Properties
		#region Position
		long position;
		/// <summary>
		/// Gets\Sets the stream's cursor position
		/// </summary>
		public override long Position
		{
			get { return position; }
			set { position = value; }
		}
		#endregion

		#region Handle
		uint processId;
		IntPtr handle;
		/// <summary>
		/// Handle of the process whose memory we are currently streaming data to\from
		/// </summary>
		public IntPtr Handle { get { return handle; } }
		#endregion

		/// <summary>
		/// Always returns zero
		/// </summary>
		public override long Length { get { return 0; } }
		/// <summary>
		/// Always returns <c>true</c>
		/// </summary>
		public override bool CanRead { get { return true; } }
		/// <summary>
		/// Always returns <c>true</c>
		/// </summary>
		public override bool CanSeek { get { return true; } }
		/// <summary>
		/// Always returns <c>true</c>
		/// </summary>
		public override bool CanWrite { get { return true; } }
		#endregion

		public ProcessMemoryStream(uint process_id)
		{
			processId = process_id;
			handle = (IntPtr)Windows.OpenProcess(0x001F0FFF, false, process_id);
		}

		public override void Close()
		{
			base.Close();
			Windows.CloseHandle(handle);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="origin"></param>
		/// <returns></returns>
		/// <remarks><see cref="SeekOrigin.End"/> is not implemented</remarks>
		public override long Seek(long offset, SeekOrigin origin)
		{
			if (origin == SeekOrigin.Begin) position = offset;
			if (origin == SeekOrigin.Current) position += offset;
			if (origin == SeekOrigin.End) throw new NotImplementedException(string.Format("offset={0}", offset.ToString("X8")));//position = streamLength - offset;
			return position;
		}

		/// <summary>
		/// Not implemented, throws exception
		/// </summary>
		/// <param name="value"></param>
		public override void SetLength(long value)
		{
			//streamLength = value;
			throw new NotImplementedException(string.Format("Length={0}", value.ToString("X8")));
		}

		public override int Read(byte[] buffer, int offset, int count)
		{
#if _WIN64
			ulong 
#else
			uint 
#endif
				dummy = 0;

			Windows.ReadProcessMemory(handle, (IntPtr)position, buffer, (uint)count, out dummy);
			position += count;
			return count;
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
#if _WIN64
			ulong
#else
			uint 
#endif
				dummy = 0;

			Windows.WriteProcessMemory(handle, (IntPtr)position, buffer, (uint)count, out dummy);
			position += count;
		}

		/// <summary>
		/// Does nothing
		/// </summary>
		public override void Flush() { }
	};

	#region Xml streams
	/// <summary>
	/// Class to simplify the programming of blamlib object xml interfacing
	/// </summary>
	public sealed class XmlStream : IDisposable
	{
		#region FileName
		string fileName;
		/// <summary>
		/// File that this stream is handling
		/// </summary>
		public string FileName { get { return fileName; } }
		#endregion

		#region Owner
		object owner = null;
		/// <summary>
		/// Owner of this stream
		/// </summary>
		public object Owner { get { return owner; } }
		#endregion

		#region Cursor (node)
		XmlNode node;
		/// <summary>
		/// Node data we are streaming data to and from
		/// </summary>
		public XmlNode Cursor
		{
			get { return node; }
			set
			{
				if(value == null) throw new Debug.ExceptionLog("Xml stream cursors can't be null!");
				node = value;
			}
		}

		System.Collections.Generic.Stack<XmlNode> nodeStack;
		/// <summary>
		/// Saves the current stream cursor so a new one can be specified,
		/// but then later restored to the saved cursor
		/// </summary>
		public void SaveCursor()
		{
			if (nodeStack == null) nodeStack = new System.Collections.Generic.Stack<XmlNode>();
			if(nodeStack.Count > 0)
				Debug.Warn.If(nodeStack.Peek() != node, "Saving the cursor more than once! {0} in {1}", node, owner);
			nodeStack.Push(node);
		}

		/// <summary>
		/// Saves the current stream cursor and sets <paramref name="new_cursor"/>
		/// to be the new cursor for the stream
		/// </summary>
		/// <param name="new_cursor"></param>
		public void SaveCursor(XmlNode new_cursor)
		{
			SaveCursor();
			Cursor = new_cursor;
		}

		/// <summary>
		/// Returns the cursor to the last saved cursor value
		/// </summary>
		public void RestoreCursor()
		{
			Debug.Assert.If(nodeStack != null, "Can't restore a cursor that wasn't saved!");
			node = nodeStack.Pop();
		}
		#endregion

		#region Constructors
		/// <summary>
		/// Setup an xml stream with <paramref name="cursor"/> with no owning object
		/// </summary>
		/// <param name="cursor"></param>
		public XmlStream(XmlNode cursor) : this(cursor, null) { }
		/// <summary>
		/// Setup an xml stream with <paramref name="cursor"/>
		/// and using <paramref name="owner"/> as the parent object
		/// </summary>
		/// <param name="cursor"></param>
		/// <param name="owner">owner object</param>
		public XmlStream(XmlNode cursor, object owner) { node = cursor; this.owner = owner; fileName = string.Format("Cursor+{0}", cursor.Name); }
		/// <summary>
		/// Setup an xml stream using <paramref name="path"/> as the source 
		/// with no owning object
		/// </summary>
		/// <param name="path">XML file path</param>
		public XmlStream(string path) : this(path, null) {}
		/// <summary>
		/// Setup an xml stream using <paramref name="path"/> as the source 
		/// and using <paramref name="owner"/> as the parent object
		/// </summary>
		/// <param name="path">XML file path</param>
		/// <param name="owner">owner object</param>
		public XmlStream(string path, object owner)
		{
			XmlDocument doc = new XmlDocument();
			doc.Load(fileName = path);
			node = doc.DocumentElement;
			this.owner = owner;
		}

		/// <summary>
		/// Setup an xml stream using a manifest file identified by it's path and name
		/// </summary>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		public XmlStream(string path, string name) : this(path, name, null) {}
		/// <summary>
		/// Setup an xml stream using a manifest file identified by it's path and name
		/// </summary>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		/// <param name="owner">owner object</param>
		public XmlStream(string path, string name, object owner)
		{
			XmlDocument doc = new XmlDocument();
			using (Stream s = Assembly.File.Open(path, name))
			{ doc.Load(s); }
			node = doc.DocumentElement;
			this.owner = owner;

			fileName = Path.Combine(path, name);
		}
		/// <summary>
		/// Setup an xml stream using a manifest file identified by it's path and name
		/// </summary>
		/// <param name="ass">Assembly to get the manifest file from</param>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		public XmlStream(System.Reflection.Assembly ass, string path, string name) : this(path, name, null) { }
		/// <summary>
		/// Setup an xml stream using a manifest file identified by it's path and name
		/// </summary>
		/// <param name="ass">Assembly to get the manifest file from</param>
		/// <param name="path">Type path to the file</param>
		/// <param name="name">File name (with extension)</param>
		/// <param name="owner">owner object</param>
		public XmlStream(System.Reflection.Assembly ass, string path, string name, object owner)
		{
			XmlDocument doc = new XmlDocument();
			using (Stream s = Assembly.File.Open(ass, path, name))
			{ doc.Load(s); }
			node = doc.DocumentElement;
			this.owner = owner;

			fileName = string.Format("{0}+{1}", ass.FullName, Path.Combine(path, name));
		}
		#endregion

		#region Read
		// NOTE:
		// We use the constraint "where TEnum : struct" because you can't use "enum" or "System.Enum", for reasons I would like to know...

		// TODO: document that 'ref value' will equal the streamed value or 'null' after returning, depending on success
		// TODO: Element/Attribute exists assertions are no longer debug only. Need to update documentation

		#region ReadElement
		/// <summary>
		/// Streams out the inner text of <paramref name="name"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		/// <returns></returns>
		private string ReadElement(string name)
		{
			XmlElement n = node[name];
			if (n == null)
				throw new Debug.ExceptionLog("Tried to read element '{0}' from node '{1}' in {2}", name, node.Name, owner);
			return n.InnerText;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, ref string value) { value = ReadElement(name); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, ref char value) { value = ReadElement(name)[0]; }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, ref bool value) { value = Convert.ToBoolean(ReadElement(name)); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, int from_base, ref sbyte value) { value = Convert.ToSByte(ReadElement(name), from_base); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, int from_base, ref byte value) { value = Convert.ToByte(ReadElement(name), from_base); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, int from_base, ref short value) { value = Convert.ToInt16(ReadElement(name), from_base); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, int from_base, ref ushort value) { value = Convert.ToUInt16(ReadElement(name), from_base); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, int from_base, ref int value) { value = Convert.ToInt32(ReadElement(name), from_base); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, int from_base, ref uint value) { value = Convert.ToUInt32(ReadElement(name), from_base); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement(string name, ref float value) { value = Convert.ToSingle(ReadElement(name)); }

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into the enum <paramref name="value"/>
		/// </summary>
		/// <typeparam name="TEnum">Enumeration type</typeparam>
		/// <param name="name">Element name</param>
		/// <param name="enum_value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the element exists</remarks>
		public void ReadElement<TEnum>(string name, ref TEnum enum_value) where TEnum : struct
		{ enum_value = (TEnum)Enum.Parse(typeof(TEnum), ReadElement(name)); }
		#endregion

		#region ReadAttribute
		/// <summary>
		/// Streams out the attribute data of <paramref name="name"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		/// <returns></returns>
		private string ReadAttribute(string name)
		{
			XmlNode n = node.Attributes[name];
			if(n == null)
				throw new Debug.ExceptionLog("Tried to read attribute '{0}' from node '{1}' in {2}", name, node.Name, owner);
			return n.Value;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, ref string value) { value = ReadAttribute(name); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, ref char value) { value = ReadAttribute(name)[0]; }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, ref bool value) { value = Convert.ToBoolean(ReadAttribute(name)); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, int from_base, ref sbyte value) { value = Convert.ToSByte(ReadAttribute(name), from_base); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, int from_base, ref byte value) { value = Convert.ToByte(ReadAttribute(name), from_base); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, int from_base, ref short value) { value = Convert.ToInt16(ReadAttribute(name), from_base); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, int from_base, ref ushort value) { value = Convert.ToUInt16(ReadAttribute(name), from_base); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, int from_base, ref int value) { value = Convert.ToInt32(ReadAttribute(name), from_base); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, int from_base, ref uint value) { value = Convert.ToUInt32(ReadAttribute(name), from_base); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute(string name, ref float value) { value = Convert.ToSingle(ReadAttribute(name)); }

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into enum <paramref name="value"/>
		/// </summary>
		/// <typeparam name="TEnum">Enumeration type</typeparam>
		/// <param name="name">Attribute name</param>
		/// <param name="enum_value">enum value to receive the data</param>
		/// <remarks>DEBUG ONLY: Asserts that the attribute exists</remarks>
		public void ReadAttribute<TEnum>(string name, ref TEnum enum_value) where TEnum : struct
		{ enum_value = (TEnum)Enum.Parse(typeof(TEnum), ReadAttribute(name)); }
		#endregion

		#region ReadElementOpt
		/// <summary>
		/// Streams out the inner text of <paramref name="name"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		private string ReadElementOpt(string name)
		{
			XmlElement n = node[name];
			if (n != null && n.InnerText != string.Empty)	return n.InnerText;
			else											return null;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, ref string value)
		{
			value = ReadElementOpt(name);
			return value != null;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, ref char value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = str[0]; return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, ref bool value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToBoolean(str); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, int from_base, ref sbyte value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToSByte(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, int from_base, ref byte value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToByte(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, int from_base, ref short value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToInt16(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, int from_base, ref ushort value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToUInt16(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, int from_base, ref int value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToInt32(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, int from_base, ref uint value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToUInt32(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Element name</param>
		/// <param name="value">value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt(string name, ref float value)
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			value = Convert.ToSingle(str); return true;
		}

		/// <summary>
		/// Stream out the inner text of <paramref name="name"/>
		/// into enum <paramref name="value"/>
		/// </summary>
		/// <typeparam name="TEnum">Enumeration type</typeparam>
		/// <param name="name">Element name</param>
		/// <param name="enum_value">enum value to receive the data</param>
		/// <remarks>If inner text is just an empty string, the stream ignores it's existance</remarks>
		/// <returns>true if the value exists</returns>
		public bool ReadElementOpt<TEnum>(string name, ref TEnum enum_value) where TEnum : struct
		{
			string str = ReadElementOpt(name); if (str == null) return false;
			enum_value = (TEnum)Enum.Parse(typeof(TEnum), str); return true;
		}
		#endregion

		#region ReadAttributeOpt
		/// <summary>
		/// Streams out the attribute data of <paramref name="name"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <returns>true if the value exists</returns>
		private string ReadAttributeOpt(string name)
		{
			XmlNode n = node.Attributes[name];
			if (n != null)	return n.Value;
			else			return null;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, ref string value)
		{
			value = ReadAttributeOpt(name);
			return value != null;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, ref char value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = str[0]; return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, ref bool value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToBoolean(str); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, int from_base, ref sbyte value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToSByte(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, int from_base, ref byte value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToByte(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, int from_base, ref short value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToInt16(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, int from_base, ref ushort value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToUInt16(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, int from_base, ref int value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToInt32(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// using numerical base of <paramref name="base"/> into 
		/// <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="from_base">numerical base to use (ie 2, 8, 10, 16)</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, int from_base, ref uint value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToUInt32(str, from_base); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into <paramref name="value"/>
		/// </summary>
		/// <param name="name">Attribute name</param>
		/// <param name="value">value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt(string name, ref float value)
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			value = Convert.ToSingle(str); return true;
		}

		/// <summary>
		/// Stream out the attribute data of <paramref name="name"/>
		/// into enum <paramref name="value"/>
		/// </summary>
		/// <typeparam name="TEnum">Enumeration type</typeparam>
		/// <param name="name">Attribute name</param>
		/// <param name="enum_value">enum value to receive the data</param>
		/// <returns>true if the value exists</returns>
		public bool ReadAttributeOpt<TEnum>(string name, ref TEnum enum_value) where TEnum : struct
		{
			string str = ReadAttributeOpt(name); if (str == null) return false;
			enum_value = (TEnum)Enum.Parse(typeof(TEnum), str); return true;
		}
		#endregion
		#endregion

		#region Write
		#endregion

		#region Util
		/// <summary>
		/// Checks to see if the current scope has a fully defined attribute named <paramref name="name"/>
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		public bool AttributeExists(string name)
		{
			XmlNode n = Cursor.Attributes[name];
			if (n != null) return true;
			
			return false;
		}

		/// <summary>
		/// Checks to see if the current scope has a fully defined element named <paramref name="name"/>
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		public bool ElementsExists(string name)
		{
			XmlElement n = Cursor[name];
			if (n != null && n.InnerText != string.Empty) return true;

			return false;
		}

		/// <summary>
		/// Checks to see if the current scope has elements
		/// </summary>
		/// <returns></returns>
		public bool ElementsExist { get { return Cursor.HasChildNodes; } }
		#endregion

		public void Dispose()
		{
			if (nodeStack != null)
				Debug.Warn.If(nodeStack.Count == 0, "Closing a XML stream while stack still in use: depth = {0}", nodeStack.Count);
			node = null;
			nodeStack = null;

			owner = null;
		}
	};
	#endregion
}
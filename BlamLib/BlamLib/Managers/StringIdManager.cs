/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Managers
{
	/// <summary>
	/// Implemented by a tag collection for single access to string ids
	/// </summary>
	public interface IStringIdContainer
	{
		/// <summary>
		/// Provides an interface to enumerate through all the string ids in this container
		/// </summary>
		/// <returns>Interface for enumerating through each string id's handle and value in this container</returns>
		IEnumerable<KeyValuePair<uint, string>> StringIds();

		/// <summary>
		/// Retrieve a string id value via it's handle
		/// </summary>
		/// <param name="sid">string id handle</param>
		/// <returns>string id's value, or null if it doesn't exist</returns>
		string Get(Blam.StringID sid);

		/// <summary>
		/// Retrieve a global string id value via it's index
		/// </summary>
		/// <param name="index">index of the string id</param>
		/// <returns>string id's value, or null if it doesn't exist</returns>
		string GetAbsolute(short index);

		/// <summary>
		/// Find a string's handle. If it is not found, the implementing class has the option of 
		/// adding the string (to the global list) and returning a *new* handle. It all depends 
		/// on the context and if <see cref="IsReadOnly"/> is false
		/// </summary>
		/// <param name="value">string value to find</param>
		/// <param name="handle">Reference to receive the handle, or a null value if this fails</param>
		/// <returns>True if the string exists</returns>
		bool TryAndGet(string value, out Blam.StringID handle);

		/// <summary>
		/// Does this container allow new handles to be made from new strings?
		/// </summary>
		bool IsReadOnly { get; }
	};

	/// <summary>
	/// Manages all string ids for a cache\tag-system instance
	/// </summary>
	/// <remarks>
	/// Due to the internal design of this class, only a tag interface OR a cache interface can 
	/// exist at a single moment for a single engine version (<see cref="BlamLib.BlamVersion"/>).
	/// 
	/// IE, if there is a tag interface for Halo2_Xbox currently in memory, the system shouldn't be 
	/// loading a Halo2_Xbox cache interface. If it does, BAD THINGS WILL HAPPEN when string id code is executed!
	/// 
	/// This has no effect on Halo 1 or Stubbs systems of course, but the same limitation should be imposed 
	/// just to be consistent.
	/// </remarks>
	public sealed class StringIdManager : IDisposable, IStringIdContainer, BlamDefinition.IGameResource
	{
		/// <summary>
		/// What extra method to use when generating a id linked to a string
		/// </summary>
		/// <remarks>Index is always included in id generation</remarks>
		public enum GenerateIdMethod
		{
			/// <summary>
			/// Use string length as part of the id
			/// </summary>
			ByLength,
			/// <summary>
			/// Use group identifiers as part of the id
			/// </summary>
			ByGroup,
		};

		#region HandleMethod
		GenerateIdMethod handleMethod = GenerateIdMethod.ByLength;
		/// <summary>
		/// Method used which to create new handles
		/// </summary>
		public GenerateIdMethod HandleMethod	{ get { return handleMethod; } }
		#endregion

		Dictionary<uint, string>[] Groups = null;

		#region Count
		int predefinedCount;
		/// <summary>
		/// Number of string ids which are predefined by a engine's code
		/// </summary>
		public int PredefinedCount	{ get { return predefinedCount; } }

		/// <summary>
		/// Number of string ids in the 'main' string id group
		/// </summary>
		public int MainGroupCount { get { return Groups[0].Count; } }
		
		/// <summary>
		/// Total number of string ids in use
		/// </summary>
		public int Count {
			get {
				int count = 0;
				foreach (Dictionary<uint, string> dic in Groups) count += dic.Count;
				return count;
			}
		}
		#endregion

		#region Loading
		void LoadGroups(IO.XmlStream s)
		{
			int key = -1;
			string val = string.Empty;
			Groups = new Dictionary<uint, string>[s.Cursor.ChildNodes.Count];
			foreach (System.Xml.XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "entry") continue;

				s.SaveCursor(n);
				s.ReadAttribute("key", 10, ref key);
				s.ReadAttribute("value", ref val);
				Groups[key] = new Dictionary<uint, string>();
				s.RestoreCursor();
			}

#if DEBUG
			for (int x = 0; x < Groups.Length; x++)
				Debug.Assert.If(Groups[x] != null, "Resource failed to define a group [{0}].", x);
#endif
		}

		void LoadConstants(IO.XmlStream s)
		{
			Blam.StringID sid = new BlamLib.Blam.StringID();
			string val = string.Empty;
			Dictionary<uint, string> dic = null;
			byte dicSet = byte.MaxValue;
			foreach (System.Xml.XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "entry") continue;

				s.SaveCursor(n);
				s.ReadAttribute("key", 16, ref sid.Handle);
				s.ReadAttribute("value", ref val);

				if(sid.Set != dicSet) // this is so we're not always performing a hash lookup on every add
					dic = Groups[dicSet = sid.Set];
				dic.Add(sid, val);

				s.RestoreCursor();
			}
		}

		/// <summary>
		/// Load the string id data from an xml manifest file
		/// </summary>
		/// <param name="path"></param>
		/// <param name="name"></param>
		public bool Load(string path, string name)
		{
			using(IO.XmlStream s = new BlamLib.IO.XmlStream(path, name, this))
			{
				s.ReadAttribute("method", ref handleMethod);

				foreach (System.Xml.XmlNode n in s.Cursor.ChildNodes)
					if (n.Name == "groups")
					{
						Debug.Warn.If(handleMethod == GenerateIdMethod.ByGroup, "File defines id groups, but doesn't use a group method (or failed to say it does). {0}{1}", path, name);
						s.SaveCursor(n);
						LoadGroups(s);
						s.RestoreCursor();
					}
					else if (n.Name == "predefined")
					{
						if(Groups == null) // this system uses a by-length method, so we define a single global group
						{
							Groups = new Dictionary<uint, string>[1];
							Groups[0] = new Dictionary<uint, string>();
						}

						s.SaveCursor(n);
						LoadConstants(s);
						s.RestoreCursor();
					}

				predefinedCount = Count;
			}
			return true;
		}

		public void Close()
		{
			if (Groups != null)
			{
				for (int x = 0; x < Groups.Length; x++)
				{
					Groups[x].Clear();
					Groups[x] = null;
				}
				Groups = null;
			}
		}
		#endregion

		#region IDisposable Members
		public void Dispose()	{ Close(); }
		#endregion

		#region IStringIdContainer Members
		/// <summary>
		/// Provides an interface to enumerate through all the string ids in this container
		/// </summary>
		/// <returns>Interface for enumerating through each string id's handle and value in this container</returns>
		public IEnumerable<KeyValuePair<uint, string>> StringIds()
		{
			foreach (Dictionary<uint, string> dic in Groups)
				foreach (KeyValuePair<uint, string> kv in dic)
					yield return kv;
		}

		/// <summary>
		/// Retrieve a string id value via it's handle
		/// </summary>
		/// <param name="sid">string id handle</param>
		/// <returns>string id's value, or null if it doesn't exist</returns>
		public string Get(BlamLib.Blam.StringID sid)
		{
			if (sid.Handle == 0) return string.Empty; // little optimization

			string ret = null;
			if (sid.Set < Groups.Length && Groups[sid.Set].TryGetValue(sid.Handle, out ret))
				return ret;
			return null;
		}
		/// <summary>
		/// Retrieve a global string id value via it's index
		/// </summary>
		/// <param name="index">index of the string id</param>
		/// <returns>string id's value, or null if it doesn't exist</returns>
		public string GetAbsolute(short index)
		{
			if(index < Groups[0].Count)
			{
				Dictionary<uint, string> dic = Groups[0];
				foreach (uint k in dic.Keys)
					if (Blam.StringID.ToIndex(k) == index)
						return dic[k];
			}

			return null;
		}
		/// <summary>
		/// Find a string's handle. If it is not found, the string will be added 
		/// (to the global list) and a *new* handle will be returned, depending on
		/// if <see cref="IsReadOnly"/> is false and <paramref name="add"/> is true.
		/// </summary>
		/// <param name="value">string value to find</param>
		/// <param name="handle">Reference to receive the handle, or a null value if this fails</param>
		/// <param name="add"></param>
		/// <returns>True if the string exists</returns>
		public bool TryAndGet(string value, out BlamLib.Blam.StringID handle, bool add)
		{
			handle = Blam.StringID.Null;
			#region try to find
			foreach (Dictionary<uint, string> dic in Groups)
			{
				foreach (KeyValuePair<uint, string> pair in dic)
				{
					if (pair.Value == value)
					{
						handle.Handle = pair.Key;
						return true;
					}
				}
			}
			#endregion
			#region add value
			if (!IsReadOnly && add) // only add value if this collection can be modified
			{
				Dictionary<uint, string> dic = Groups[0];
				switch (handleMethod)
				{
					case GenerateIdMethod.ByLength: handle = new BlamLib.Blam.StringID((short)(dic.Count - 1), (byte)value.Length, 0); break;
					case GenerateIdMethod.ByGroup: handle = new BlamLib.Blam.StringID((short)(dic.Count - 1), 0, 0); break;
					default: throw new Debug.Exceptions.UnreachableException(handleMethod);
				}
				dic.Add(handle.Handle, value);
				return true;
			}
			#endregion
			return false;
		}
		/// <summary>
		/// Find a string's handle. If it is not found, the string will be added 
		/// (to the global list) and a *new* handle will be returned.
		/// </summary>
		/// <param name="value">string value to find</param>
		/// <param name="handle">Reference to receive the handle, or a null value if this fails</param>
		/// <returns>True if the string exists</returns>
		public bool TryAndGet(string value, out BlamLib.Blam.StringID handle)	{ return TryAndGet(value, out handle, true); }

		/// <summary>
		/// Does this container allow new handles to be made from new strings?
		/// </summary>
		/// <remarks>ALWAYS returns false</remarks>
		public bool IsReadOnly { get { return false; } }
		#endregion

		/// <summary>
		/// Build the debug streams used in cache files which store the string id values
		/// </summary>
		/// <remarks>
		/// When setting <paramref name="pack"/> to true, see <see cref="ReferenceManager.GenerateDebugStream"/> 
		/// in regards to special conditions you may want to take about <paramref name="buffer"/>
		/// </remarks>
		/// <param name="pack">If true, the strings are stored as null terminated strings, else in 128 character blocks</param>
		/// <param name="offsets">Buffer containing the offsets of the string values in <paramref name="buffer"/></param>
		/// <param name="buffer">Buffer containing the string values</param>
		public void GenerateDebugStream(bool pack, out System.IO.MemoryStream offsets, out System.IO.MemoryStream buffer)
		{
			int count = this.Count;
			offsets = new System.IO.MemoryStream(count * sizeof(int));
			buffer = new System.IO.MemoryStream(pack ? 0 : count * 128); // TODO: figure out a good starting buffer size for packed

			using(var offsets_s = new BlamLib.IO.EndianWriter(offsets))
			using(var buffer_s = new BlamLib.IO.EndianWriter(buffer))
			{
				foreach(KeyValuePair<uint, string> kv in StringIds())
				{
					offsets_s.Write(buffer_s.Position);
					if (pack) buffer_s.Write(kv.Value, true);
					else buffer_s.Write(kv.Value, 128);
				}
			}
		}
	};
}
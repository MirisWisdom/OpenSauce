/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using StringId = BlamLib.Blam.StringId;
using GenerateIdMethod = BlamLib.Blam.StringIdDesc.GenerateIdMethod;

namespace BlamLib.Managers
{
	/// <summary>Manages an interface to both a static and dynamic set(s) of string IDs and their values.</summary>
	public sealed class StringIdManager : IStringIdContainer
	{
		StringIdStaticCollection m_staticCollection;
		/// <summary>Container object for all static (predefined) string IDs</summary>
		public IStringIdContainer StaticIdsContainer { get { return m_staticCollection; } }

		StringIdDynamicCollection m_dynamicCollection;
		/// <summary>Container object for all dynamic string IDs</summary>
		public IStringIdContainer DynamicIdsContainer { get { return m_dynamicCollection; } }

		/// <summary>The base definition of this manager</summary>
		internal StringIdCollectionDefintion Definition { get { return m_staticCollection.Definition; } }

		#region Count
		/// <summary>Number of strings which are predefined to a specific value</summary>
		public int StaticCount { get { return m_staticCollection.Count; } }

		/// <summary>Total number of string ids in use</summary>
		public int Count { get { return StaticCount + m_dynamicCollection.Count; } }
		#endregion

		#region Ctor
		internal StringIdManager(StringIdStaticCollection static_collection)
		{
			if (static_collection == null) throw new ArgumentNullException("static_collection", "");

			m_staticCollection = static_collection;

			m_dynamicCollection = new StringIdDynamicCollection(this, 0);
		}
		#endregion

		#region IStringIdContainer Members
		public IEnumerable<KeyValuePair<StringId, string>> StringIdsEnumerator()
		{
			foreach (var kv in (m_staticCollection as IStringIdContainer).StringIdsEnumerator())
				yield return kv;
			foreach (var kv in (m_dynamicCollection as IStringIdContainer).StringIdsEnumerator())
				yield return kv;
		}

		/// <summary>Determines if the ID exists in this container</summary>
		/// <param name="sid"></param>
		/// <returns></returns>
		public bool ContainsStringId(StringId sid)
		{
			return m_staticCollection.ContainsStringId(sid) || m_dynamicCollection.ContainsStringId(sid);
		}

		public string GetStringIdValue(StringId sid)
		{
			string value = m_staticCollection.GetValue(sid);

			if (value == null)
				value = m_dynamicCollection.GetValue(sid);

			if (value == null)
				throw new KeyNotFoundException(sid.ToString());

			return value;
		}

		public string GetStringIdValueUnsafe(int absolute_index)
		{
			string value = m_staticCollection.GetValueUnsafe(absolute_index);

			if (value == null)
			{
				absolute_index -= StaticCount;
				value = m_dynamicCollection.GetValueUnsafe(absolute_index);
			}

			if(value == null)
				throw new ArgumentOutOfRangeException(absolute_index.ToString("X4"));

			return value;
		}

		public bool TryAndGetStringId(string value, out StringId sid)
		{
			bool result = m_staticCollection.TryAndGetStringId(value, out sid);

			return result || m_dynamicCollection.TryAndGetStringId(value, out sid);
		}

		/// <summary>Does this container allow new ids to be added from dynamic strings?</summary>
		public bool IsReadOnly
		{
			get { return m_dynamicCollection.IsReadOnly; }
			internal set { m_dynamicCollection.IsReadOnly = value; }
		}
		#endregion

		#region DebugStream Utils
		/// <summary>Build debug streams which foreign native code can easily interop with</summary>
		/// <param name="offsets">Buffer containing the offsets of the string values in <paramref name="buffer"/></param>
		/// <param name="buffer">Buffer containing the string values</param>
		/// <param name="pack">If true, the strings are stored as null terminated strings, else in 128 character strings</param>
		/// <remarks>
		/// <paramref name="buffer"/> will most likely contain more bytes than needed for its data 
		/// so you may want to use <paramref name="buffer"/>.<see cref="System.IO.MemoryStream.ToArray()"/> 
		/// when writing the data to another stream. Note that ToArray returns a COPY of the underlying 
		/// byte[] so you, in some cases, may just want to use GetBuffer instead but use the 
		/// <see cref="System.IO.MemoryStream.Length"/> field (compared to <see cref="System.IO.MemoryStream.Capacity"/>) 
		/// to only write non-zero (ie, unused) bytes
		/// </remarks>
		public void GenerateDebugStream(out System.IO.MemoryStream offsets, out System.IO.MemoryStream buffer, bool pack)
		{
			int fixed_length = pack ? 0 : Definition.Description.ValueBufferSize; // TODO: figure out a good starting buffer size for packed

			int count = Count;
			offsets = new System.IO.MemoryStream(count * sizeof(int));
			buffer = new System.IO.MemoryStream(count * fixed_length);

			using (var offsets_s = new IO.EndianWriter(offsets))
			using (var buffer_s = new IO.EndianWriter(buffer))
			{
				m_staticCollection.ToDebugStream(offsets_s, buffer_s, pack);
				m_dynamicCollection.ToDebugStream(offsets_s, buffer_s, pack);
			}
		}
		/// <summary></summary>
		/// <param name="offsets">Buffer containing the offsets of the string values in <paramref name="buffer"/></param>
		/// <param name="buffer">Buffer containing the string values</param>
		/// <param name="is_packed">If true, reads the strings as null terminated strings, else as 128 character strings</param>
		/// <param name="count">Total number of string values (both static and dynamic)</param>
		/// <param name="has_static_data">True if the buffers have the the static collection data</param>
		public void FromDebugStream(IO.EndianReader offsets, IO.EndianReader buffer, bool is_packed, int count, bool has_static_data)
		{
			//Contract.Requires<ArgumentNullException>(offsets != null);
			//Contract.Requires<ArgumentNullException>(buffer != null);
			//Contract.Requires<ArgumentOutOfRangeException>(count > StaticCount);

			if(has_static_data)
				m_staticCollection.FromDebugStream(offsets, buffer, is_packed);
			count -= StaticCount;
			m_dynamicCollection.FromDebugStream(offsets, buffer, is_packed, count);
		}
		#endregion
	};
}
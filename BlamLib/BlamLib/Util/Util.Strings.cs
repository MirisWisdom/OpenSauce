/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/﻿using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib
{
	partial class Util
	{
		#region StringArray
		/// <summary>
		/// String list with a special ToString method
		/// </summary>
		public sealed class StringArray : List<string>
		{
			public StringArray() : base() { }
			/// <summary>
			/// Returns a string that contains every single in this array, but
			/// separated by <see cref="Program.NewLine"/>
			/// </summary>
			/// <returns></returns>
			public override string ToString()
			{
				StringBuilder ret = new StringBuilder(this.Count * 32);
				for (int x = 0; x < Count; x++) ret.Append(this[x] + Program.NewLine);

				return ret.ToString();
			}
		};
		#endregion

		#region StringPool
		/// <summary>
		/// String list container using memory pool specifications
		/// </summary>
		public sealed class StringPool : IO.IStreamable, IEnumerable<string>
		{
			/// <summary>
			/// Sentinel used for bad offsets
			/// </summary>
			public const uint NullSentinel = uint.MaxValue;

			List<string> pool = new List<string>(64);
			List<uint> offsets = new List<uint>(64);
			uint nullOffset = NullSentinel; // null string offset starts off null incase the user doesn't want a implicit null
			bool internalNull = false;

			#region BaseAddress
			uint baseAddress;
			/// <summary>
			/// Base address in memory for this pool
			/// </summary>
			public uint BaseAddress
			{
				get { return baseAddress; }
				set { baseAddress = value; }
			}
			#endregion

			#region Size
			int size = 0;
			/// <summary>
			/// Total size in bytes of the pool
			/// </summary>
			public int Size { get { return size; } }
			#endregion

			#region Count
			/// <summary>
			/// Get the number of strings in the pool
			/// </summary>
			public int Count { get { return pool.Count; } }
			#endregion


			#region Ctor
			/// <summary>
			/// 
			/// </summary>
			/// <param name="implicit_null">automatically add an empty string entry to the pool</param>
			/// <param name="base_address">address to start the offsets at</param>
			public StringPool(bool implicit_null, uint base_address)
			{
				baseAddress = base_address;
				if (internalNull = implicit_null)
				{
					AddInteral(""); // empty string always first
					nullOffset = baseAddress;
				}
			}

			/// <summary>
			/// 
			/// </summary>
			public StringPool() : this(false, 0) { }

			/// <summary>
			/// 
			/// </summary>
			/// <param name="implicit_null">automatically add an empty string entry to the pool</param>
			public StringPool(bool implicit_null) : this(implicit_null, 0) { }
			#endregion

			#region Add
			/// <summary>
			/// Takes a string and adds it to the pool as long the same
			/// string of characters don't exist in the pool
			/// </summary>
			/// <param name="str">value to add</param>
			/// <returns>offset of the string</returns>
			public uint Add(string str)
			{
				int index;

				if (str == null || str == string.Empty)
				{
					if(internalNull) return baseAddress; // if we're setup to use a implicit null string, its always the first string in the pool

					if (nullOffset == NullSentinel) // if not, check to see if a null string has been added yet
					{
						index = pool.Count;
						this.AddInteral("");
						nullOffset = baseAddress + offsets[index];
					}
					return nullOffset;
				}

				if ((index = pool.IndexOf(str)) == -1)
				{
					index = pool.Count;
					this.AddInteral(str);
				}

				return offsets[index];
			}

			private void AddInteral(string value)
			{
				offsets.Add(baseAddress + (uint)size);
				pool.Add(value);

				size += value.Length + 1; // we count leading zero remember!
			}
			#endregion

			#region Get
			/// <summary>
			/// Takes a string and gets the address
			/// it would be if the pool was located at
			/// [baseAddress] in memory
			/// </summary>
			/// <param name="value"></param>
			/// <returns></returns>
			public uint GetAddress(string value)
			{
				int index = pool.IndexOf(value);
				if (index != -1) return baseAddress + offsets[index];
				
				return 0;
			}

			/// <summary>
			/// Get the address of the string at <paramref name="offset"/>
			/// </summary>
			/// <param name="offset"></param>
			/// <returns></returns>
			/// <remarks>Actually returns the index used internally tracking strings\offsets</remarks>
			int GetAddress(uint offset)
			{
				int index = offsets.BinarySearch(offset);
				if (index >= 0) return index;

				Debug.Assert.If(false, "GetAddress couldn't find offset! {0}", offset);
				return -1;
			}

			public bool TryAndGet(uint offset, out string value)
			{
				int index = offsets.BinarySearch(offset);

				if(index >= 0)
				{
					value = pool[index];
					return true;
				}

				value = null;
				return false;
			}

			/// <summary>
			/// Get the address of the 'null' string
			/// </summary>
			/// <returns></returns>
			public uint GetNull() { return nullOffset; }

			/// <summary>
			/// Get the string thats located at [offset]. Will throw an exception if
			/// the offset doesn't start a new string.
			/// </summary>
			/// <param name="offset"></param>
			/// <returns></returns>
			public string this[uint offset] { get { return pool[GetAddress(offset)]; } }
			#endregion

			#region IStreamable Members
			public void Read(IO.EndianReader stream) { throw new Exception("The method or operation is not implemented."); }

			public void Write(IO.EndianWriter stream)
			{
				foreach (string s in pool)
				{
					if(s == string.Empty)
						stream.Write((byte)0); // stream won't write a empty string so we hard code it :/
					else
						stream.Write(s, true);
				}
			}

			/// <summary>
			/// Write all of the string offsets to a stream
			/// </summary>
			/// <param name="stream"></param>
			public void WriteOffsets(IO.EndianWriter stream)
			{
				foreach (int i in offsets) stream.Write(i);
			}
			#endregion

			#region IEnumerable<string> Members
			public IEnumerator<string> GetEnumerator() { return pool.GetEnumerator(); }

			System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return ((System.Collections.IEnumerable)pool).GetEnumerator(); }
			#endregion
		};
		#endregion

		#region Functions
		/// <summary>
		/// Splits up a string into a string array based on the terminator
		/// </summary>
		/// <param name="ex"></param>
		/// <param name="term"></param>
		/// <returns></returns>
		public static string[] ParseRegEx(string ex, string term) { return new System.Text.RegularExpressions.Regex(term).Split(ex); }

		#region String Utils
		/// <summary>
		/// Looks for "0", "off", or "false" in [str] for a false boolean.
		/// Anything else is a true boolean
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		public static bool ParseBooleanLazy(string str)
		{
			if (str == "0" || str == "off" || str == "false") return false;

			return true;
		}

		/// <summary>
		/// Figure out if <paramref name="str"/> exists in <paramref name="array"/>
		/// </summary>
		/// <param name="array">data to search in</param>
		/// <param name="str">value to search for</param>
		/// <returns></returns>
		public static bool FindStringInArray(string[] array, string str)
		{
			foreach (string s in array) if (s == str) return true;
			return false;
		}

		/// <summary>
		/// Replaces all occurrences old string with the new string within the original string. 
		/// When no instance of the old string is found within the original string, the original string is returned unchanged.
		/// </summary>
		/// <param name="original">The string containing the original string representation.</param>
		/// <param name="old">The string to find within the original string.</param>
		/// <param name="new">The string to replace within the original string.</param>
		/// <returns>Returns the string representation after the replacement has been performed.</returns>
		public static string ReplaceAll(string original, string old, string @new)
		{
			int i = original.IndexOf(old);
			if (i < 0) return original;

			StringBuilder sb = new StringBuilder(original);
			for(; i >= 0; i = original.IndexOf(old, i))
			{
				sb.Replace(old, @new, i + old.Length, @new.Length);
				original = sb.ToString();
				i += @new.Length;
			}

			return original;
		}

		/// <summary>
		/// Replaces the invalid characters (tab, carriage-return, etc) in the specified string with spaces.
		/// </summary>
		/// <param name="str">The string being changed.</param>
		/// <returns>Returns the new string representation.</returns>
		public static string ReplaceFormatCharacters(string str)
		{
			if (str != null)
				str = str.Replace('\r', ' ').Replace('\t', ' ');

			return str;
		}

		/// <summary>
		/// Converts text to a one line string
		/// </summary>
		/// <param name="str">Text to flatten</param>
		/// <returns>Returns one line string</returns>
		public static string FlattenString(string str)
		{
			StringBuilder sb = new StringBuilder();
			bool flag = true;
			foreach(char c in str)
			{
				if(c == '\n' || c == '\r')
				{
					if (!flag) sb.Append(" /");

					flag = true;
				}
				else
				{
					sb.Append(c);
					flag = false;
				}
			}

			return sb.ToString();
		}

		/// <summary>
		/// Joins all of the strings in the array into a single string.
		/// </summary>
		/// <param name="array"></param>
		/// <returns></returns>
		/// <remarks>Puts each string in single quote marks followed by a space</remarks>
		public static string JoinStringArray(params string[] array)
		{
			StringBuilder sb = new StringBuilder();
			foreach(string s in array)
			{
				sb.Append('\'');
				sb.Append(s);
				sb.Append('\'');
				sb.Append(' ');
			}

			return sb.ToString().Trim();
		}

		/// <summary>
		/// Converts the specified string from its standard string representation to the XML representation. 
		/// All special single characters are converted to quoted character representation (" &lt; &gt; ' &amp;).
		/// </summary>
		/// <param name="content">The string being checked and converted.</param>
		/// <returns>Returns the XML representation.</returns>
		public static string ToXmlString(string content)
		{
			if (content == null || content.Length == 0) return content;

			foreach(string[] a in Program.Constants.XmlTranslation)
			{
				string str1 = a[0], str2 = a[1];
				content = ReplaceAll(content, str1, str2);
			}

			return content;
		}

		/// <summary>
		/// Converts the specified string from its XML representation to the standard string representation. 
		/// All of quoted characters (" &lt; &gt; ' &amp;) are converted back to their single character representations.
		/// </summary>
		/// <param name="content">The string being checked and converted.</param>
		/// <returns>Returns the standard string representation.</returns>
		public static string FromXmlString(string content)
		{
			if (content == null || content.Length == 0) return content;

			foreach (string[] a in Program.Constants.XmlTranslation)
			{
				string str1 = a[0], str2 = a[1];
				content = ReplaceAll(content, str2, str1);
			}

			return content;
		}

		#region Byte arrays
		/// <summary>
		/// Converts an array of bytes to a hex string
		/// </summary>
		/// <param name="data"></param>
		/// <example>"1337BEEF"</example>
		/// <returns></returns>
		public static string ByteArrayToString(byte[] data) { return ByteArrayToString(data, 0); }

		/// <summary>
		/// Converts an array of bytes to a hex string
		/// </summary>
		/// <param name="data"></param>
		/// <param name="start_index"></param>
		/// <example>"1337BEEF"</example>
		/// <returns></returns>
		public static string ByteArrayToString(byte[] data, int start_index)
		{
			if (start_index >= data.Length) throw new ArgumentException("start_index >= data.Length", "start_index");
			//Debug.Assert.If(start_index < data.Length, "Bad start index: {0}", start_index.ToString("X"));

			StringBuilder sb = new StringBuilder(data.Length * 2);
			for (int x = start_index; x < data.Length; x++)
				sb.Append(data[x].ToString("X2"));

			return sb.ToString();
		}

		public static string ByteArrayToString(byte[] data, int start_index, int end_index)
		{
			if (start_index >= data.Length) throw new ArgumentException("start_index >= data.Length", "start_index");
			//Debug.Assert.If(start_index < data.Length, "Bad start index: {0}", start_index.ToString("X"));
			if (end_index < start_index || end_index > data.Length) throw new ArgumentException("end_index < start_index || end_index > data.Length", "end_index");
			//Debug.Assert.If(end_index > start_index && end_index <= data.Length, "Bad end index: {0}", end_index.ToString("X"));

			StringBuilder sb = new StringBuilder((end_index - start_index) * 2);
			for (int x = start_index; x < end_index; x++)
				sb.Append(data[x].ToString("X").PadLeft(2, '0'));

			return sb.ToString();
		}

		public static string ByteArrayToAlignedString(byte[] data, string padding)
		{
			const string new_line = Program.NewLine;

			int blocks = data.Length / 16;
			int leftovers = data.Length % 16;

			StringBuilder sb = new StringBuilder(
				(data.Length * 2) +
				(new_line.Length * blocks) + // calculate how many new line characters we'll need
				(padding.Length * (leftovers == 0 ? blocks : blocks + 1)) // calculate how many characters the padding on each line will take
				);

			int index = 0;
			for (int b = 0; b < blocks; b++, index+=16)
				sb.AppendFormat("{0}{1}{2}", padding, ByteArrayToString(data, index, index + 16), new_line);
			
			if (leftovers > 0)
				sb.AppendFormat("{0}{1}", padding, ByteArrayToString(data, index));

			return sb.ToString();
		}

		/// <summary>
		/// Converts a string containing hex values into a byte array
		/// </summary>
		/// <param name="data"></param>
		/// <returns></returns>
		public static byte[] ByteStringToArray(string data)
		{
			Debug.Assert.If((data.Length % 2) == 0, "can't byte-ify a string thats not even!");
			byte[] ret = new byte[data.Length / 2];
			for (int x = 0; x < ret.Length; x++)
				ret[x] = Convert.ToByte(data.Substring(x * 2, 2), 16);
			return ret;
		}
		#endregion

		#region Enums
		/// <summary>
		/// Takes an enum value and return it's string representation
		/// </summary>
		/// <param name="value">Enum value to convert to a string</param>
		/// <returns></returns>
		public static string EnumToString(System.Enum value) { return value.ToString("G"); }
		/// <summary>
		/// Takes an enum value (whose type is assumed to be attributed as <c>Flags</c>) and return it's string representation 
		/// using commas to separate each flag name that is set
		/// </summary>
		/// <param name="value">Enum value to convert to a string</param>
		/// <returns></returns>
		public static string EnumToFlagsString(System.Enum value) { return value.ToString("F"); }
		/// <summary>
		/// Takes an enum value and returns it in a hexadecimal formatted string
		/// </summary>
		/// <param name="value">Enum value to convert to a string</param>
		/// <returns></returns>
		public static string EnumToHexString(System.Enum value) { return value.ToString("X"); }
		#endregion
		#endregion

		#endregion
	};
}
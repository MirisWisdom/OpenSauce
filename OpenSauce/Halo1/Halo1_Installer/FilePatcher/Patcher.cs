/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using System.ComponentModel;
using System.IO;
using System.Security.Cryptography;

namespace FilePatcher
{
	[XmlRoot(ElementName="FilePatch")]
	public class FilePatch
	{
		public enum ValueType
		{
			[XmlEnum(Name = "string")]
			String,
			[XmlEnum(Name = "uint32")]
			UInteger32,
			[XmlEnum(Name = "uint16")]
			UInteger16,
			[XmlEnum(Name = "byte")]
			Byte,
			[XmlEnum(Name = "bytes")]
			Bytes
		}

		public class Address
		{
			[XmlAttribute(AttributeName = "ptr")]
			public string PTRString = "";

			[XmlAttribute(AttributeName="value")]
			public string Value = "";
		}

		public class Data
		{
			[XmlAttribute(AttributeName="id")]
			public string ID = "";

			[XmlAttribute(AttributeName="type")]
			public ValueType Type;

			[XmlAttribute(AttributeName="length"), DefaultValue(0)]
			public int Length = 0;

			[XmlAttribute(AttributeName = "swapendian"), DefaultValue(false)]
			public bool SwapEndian = false;

			[XmlAttribute(AttributeName = "writenull"), DefaultValue(true)]
			public bool WriteNull = true;

			[XmlText]
			public string Value;
		}

		public class Patch
		{
			[XmlAttribute(AttributeName="id")]
			public string ID = "";

			[XmlElement(ElementName="Data")]
			public List<Data> Data = new List<Data>();

			[XmlElement(ElementName="Address")]
			public List<Address> Addresses = new List<Address>();
		}

		[XmlAttribute(AttributeName = "inputfile")]
		public string InputFile = "";

		[XmlAttribute(AttributeName = "outputfile")]
		public string OutputFile = "";

		[XmlAttribute(AttributeName = "peaddressmask")]
		public string PEAddressMaskString = "";

		[XmlAttribute(AttributeName = "md5_pre")]
		public string MD5Pre = "";

		[XmlAttribute(AttributeName = "md5_post")]
		public string MD5Post = "";

		[XmlElement(ElementName="Patch")]
		public List<Patch> Patches = new List<Patch>();
	}

	public class Patcher
	{
		public List<string> ErrorMessages { get; private set; }

		FilePatch m_file_patch = null;

		private static bool ParseHex(string hex_value, out byte output)
		{
			output = 0;

			// remove the "0x" prefix if present
			string hex = hex_value;
			if (hex.StartsWith("0x"))
				hex = hex.Substring(2);

			if (hex.Length > 2)
				return false;

			bool success = true;
			try
			{
				output = byte.Parse(hex, System.Globalization.NumberStyles.HexNumber);
			}
			catch (Exception)
			{
				success = false;
			}
			return success;
		}

		private static bool ParseHex(string hex_value, out ushort output)
		{
			output = 0;

			// remove the "0x" prefix if present
			string hex = hex_value;
			if (hex.StartsWith("0x"))
				hex = hex.Substring(2);

			if (hex.Length > 4)
				return false;

			bool success = true;
			try
			{
				output = ushort.Parse(hex, System.Globalization.NumberStyles.HexNumber);
			}
			catch (Exception)
			{
				success = false;
			}
			return success;
		}

		private static bool ParseHex(string hex_value, out uint output)
		{
			output = 0;

			// remove the "0x" prefix if present
			string hex = hex_value;
			if (hex.StartsWith("0x"))
				hex = hex.Substring(2);

			if (hex.Length > 8)
				return false;

			bool success = true;
			try
			{
				output = uint.Parse(hex, System.Globalization.NumberStyles.HexNumber);
			}
			catch (Exception)
			{
				success = false;
			}
			return success;
		}

		private static void CopyStream(Stream stream_in, Stream stream_out)
		{
			// set both streams to the beginning
			stream_in.Seek(0, SeekOrigin.Begin);
			stream_out.Seek(0, SeekOrigin.Begin);

			byte[] buffer = new byte[4096];

			// read the source stream and write to the target stream in blocks of 4096 bytes
			long count = stream_in.Length;
			while (count > 0)
			{
				long read_count = count < 4096 ? count : 4096;

				stream_in.Read(buffer, 0, (int)read_count);
				stream_out.Write(buffer, 0, (int)read_count);

				count -= read_count;
			}
		}

		private static string MD5Stream(Stream stream_in)
		{
			string md5_string = "";

			// check the source files md5 matches before patching
			using (MD5 md5 = MD5.Create())
			{
				if (md5 == null)
					return null;

				stream_in.Seek(0, SeekOrigin.Begin);
				byte[] md5_hash = md5.ComputeHash(stream_in);

				for (int x = 0; x < md5_hash.Length; x++)
					md5_string += md5_hash[x].ToString("X2");
			}

			return md5_string;
		}

		public Patcher(string patch_id)
		{
			// build the location of the patch definition resource
			// open the resource for reading
			Stream xml_stream = null;
			xml_stream = GetType().Assembly.GetManifestResourceStream(String.Format("FilePatcher.PatchDefinitions.{0}.xml", patch_id));
			if (xml_stream == null)
				throw new Exception(String.Format("Matching patch definition for {0} was not found. Inform the developers of the problem.", patch_id));
			
			// deserialize the definition xml
			XmlSerializer serializer = new XmlSerializer(typeof(FilePatch));
			m_file_patch = serializer.Deserialize(xml_stream) as FilePatch;

			if (m_file_patch == null)
				throw new Exception(String.Format("Failed to deserialize a file patch xml {0}. Inform the developers of the problem.", patch_id));

			// create the message list
			ErrorMessages = new List<string>();
		}

		private bool WriteData(uint address, FilePatch.Data data, Stream output)
		{
			uint peaddress = 0;
			if(!ParseHex(m_file_patch.PEAddressMaskString, out peaddress))
			{
				ErrorMessages.Add(String.Format("Failed to parse PE address mask string ({0}). Inform the developers of this.", data.ID));
				return false;
			}

			// calculate the byte address and move to it
			long write_address = address - peaddress;

			if (write_address >= output.Length)
			{
				ErrorMessages.Add(String.Format("Failed to write patch data ({0}) as the pointer is beyond the end of the file. Inform the developers of this.", data.ID));
				return false;
			}

			output.Seek(write_address, SeekOrigin.Begin);
			
			int write_length = data.Length;
			switch (data.Type)
			{
				case FilePatch.ValueType.String:
					{
						// default write length to te length of the string if zero
						if(write_length == 0)
							write_length = data.Value.Length;

						if ((write_address + write_length) >= output.Length)
						{
							ErrorMessages.Add(String.Format("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID));
							return false;
						}

						// write the strings bytes as ASCII
						byte[] bytes = Encoding.ASCII.GetBytes(data.Value);
						output.Write(bytes, 0, write_length);

						// if necessary, write a null character to terminate the string
						if (data.WriteNull)
							output.WriteByte(0);
					}
					break;
				case FilePatch.ValueType.UInteger32:
					{
						if ((write_address + 4) >= output.Length)
						{
							ErrorMessages.Add(String.Format("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID));
							return false;
						}

						// try and parse the hex string
						uint parsed_value;
						if (!ParseHex(data.Value, out parsed_value))
						{
							ErrorMessages.Add(String.Format("Failed to parse a uint32 ({0}). Inform the developers of this.", data.ID));
							return false;
						}

						// swap the uint's endianness if required
						if (data.SwapEndian)
						{
							uint endian_swapped = 0;
							endian_swapped |= (uint)((parsed_value & 0xff000000) >> 24);
							endian_swapped |= (uint)((parsed_value & 0x00ff0000) >> 8);
							endian_swapped |= (uint)((parsed_value & 0x0000ff00) << 8);
							endian_swapped |= (uint)((parsed_value & 0x000000ff) << 24);

							parsed_value = endian_swapped;
						}

						// write the value to the stream
						output.Write(BitConverter.GetBytes(parsed_value), 0, 4);
					}
					break;
				case FilePatch.ValueType.UInteger16:
					{
						if ((write_address + 2) >= output.Length)
						{
							ErrorMessages.Add(String.Format("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID));
							return false;
						}

						// try and parse the hex string
						ushort parsed_value;
						if (!ParseHex(data.Value, out parsed_value))
						{
							ErrorMessages.Add(String.Format("Failed to parse a uint16 ({0}). Inform the developers of this.", data.ID));
							return false;
						}

						// swap the uint's endianness if required
						if (data.SwapEndian)
						{
							ushort endian_swapped = 0;
							endian_swapped |= (ushort)((parsed_value & 0xff00) >> 8);
							endian_swapped |= (ushort)((parsed_value & 0x00ff) << 8);

							parsed_value = endian_swapped;
						}

						// write the value to the stream
						output.Write(BitConverter.GetBytes(parsed_value), 0, 2);
					}
					break;
				case FilePatch.ValueType.Byte:
					{
						if ((write_address + 1) >= output.Length)
						{
							ErrorMessages.Add(String.Format("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID));
							return false;
						}

						// try and parse the hex string
						byte parsed_value;
						if (!ParseHex(data.Value, out parsed_value))
						{
							ErrorMessages.Add(String.Format("Failed to parse a byte ({0}). Inform the developers of this.", data.ID));
							return false;
						}

						// write the value to the stream
						output.WriteByte(parsed_value);
					}
					break;
				case FilePatch.ValueType.Bytes:
					{
						// check the strings length is a multiple of two
						if (data.Value.Length % 2 != 0)
						{
							ErrorMessages.Add(String.Format("The length of a hex byte string ({0}) in the file patcher is odd. Inform the developers.", data.ID));
							return false;
						}

						// create the byte array
						using (MemoryStream byte_stream = new MemoryStream())
						{
							// convert every two characters into a byte
							for (int i = 0; i < data.Value.Length; i += 2)
							{
								byte parsed_byte = 0;
								if(!ParseHex(data.Value.Substring(i, 2), out parsed_byte))
								{
									ErrorMessages.Add(String.Format("Failed to parse a byte array ({0}). Inform the developers of this.", data.ID));
									return false;
								}

								byte_stream.WriteByte(parsed_byte);
							}

							// default the write length to the array length if zero
							if (write_length == 0)
								write_length = (int)byte_stream.Length;

							if ((write_address + write_length) >= output.Length)
							{
								ErrorMessages.Add(String.Format("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID));
								return false;
							}

							// write the byte array to the stream
							output.Write(byte_stream.GetBuffer(), 0, write_length);
						}
					}
					break;
			}

			return true;
		}

		private bool ApplyPatch(FilePatch.Patch patch, Stream file)
		{
			// apply each patch
			foreach (var address in patch.Addresses)
			{
				// find a matching data block
				FilePatch.Data data = patch.Data.Find(
					delegate(FilePatch.Data value)
					{
						if (address.Value.Equals(value.ID))
							return true;
						return false;
					}
				);

				// error if a data block was not found
				if (data == null)
				{
					ErrorMessages.Add(String.Format("A patch address references an invalid data id ({0}). Inform the developers of this", address.Value));
					return false;
				}

				// parse the address
				uint pointer = 0;
				if (!ParseHex(address.PTRString, out pointer))
				{
					ErrorMessages.Add(String.Format("Failed to parse an addresses pointer string ({0}). Inform the developers of this", address.Value));
					return false;
				}

				// write the data to the current address
				if (!WriteData(pointer, data, file))
					return false;
			}
			return true;
		}

		public void Patch(string srcfile, string outpath)
		{
			MemoryStream patched_file = null;

			// open the source file
			using (FileStream opened_file = File.Open(srcfile, FileMode.Open))
			{
				if(opened_file == null)
				{
					ErrorMessages.Add(String.Format("Failed to open {0}, please close all open applications before installing", Path.GetFileName(srcfile)));
					return;
				}

				// check the source files md5 matches before patching
				string file_md5 = MD5Stream(opened_file);
				if (file_md5 == null)
				{
					ErrorMessages.Add(String.Format("Failed to create an MD5 checksum for {0}. Inform the developers of this.", m_file_patch.InputFile));
					return;
				}

				// error if the md5's don't match
				if (!file_md5.Equals(m_file_patch.MD5Pre, StringComparison.CurrentCultureIgnoreCase))
				{
					ErrorMessages.Add(String.Format("Your copy of {0} is not compatible, please replace it with a compatible version", m_file_patch.InputFile));
					return;
				}

				// create a new stream in memory and copy the source file to it
				patched_file = new MemoryStream((int)opened_file.Length);

				try
				{
					CopyStream(opened_file, patched_file);
				}
				catch (Exception)
				{
					ErrorMessages.Add(String.Format("Failed to create a copy of {0} in memory. Inform the developers of this.", m_file_patch.InputFile));
					return;
				}
			}

			// error if we failed to create the target stream for an unexpected reason
			if(patched_file == null)
			{
				ErrorMessages.Add(String.Format("Failed to patch {0}", Path.GetFileName(srcfile)));
				return;
			}

			// apply all of the files patches
			foreach (var patch in m_file_patch.Patches)
			{
				if (!ApplyPatch(patch, patched_file))
					return;
			}

			// create an md5 hash of the patched data
			string new_file_md5 = MD5Stream(patched_file);
			if (new_file_md5 == null)
			{
				ErrorMessages.Add("Failed to create an MD5 checksum for the patched data. Inform the developers of this.");
				return;
			}

			// check the md5 matches post patch
			if (!new_file_md5.Equals(m_file_patch.MD5Post, StringComparison.CurrentCultureIgnoreCase))
			{
				ErrorMessages.Add("The applied patches have not produced the expected file. Inform the developers of this.");
				return;
			}

			// save the patched file
			string output_path = Path.Combine(outpath, m_file_patch.OutputFile);
			using (FileStream output_file = new FileStream(output_path, FileMode.Create))
			{
				if (output_file == null)
				{
					ErrorMessages.Add(String.Format("Failed to create {0}, please close all applications before installing", m_file_patch.OutputFile));
					return;
				}

				// copy the patched data to the new file
				try
				{
					CopyStream(patched_file, output_file);
				}
				catch (Exception)
				{
					ErrorMessages.Add(String.Format("Failed to write to {0}, please close all applications before installing", m_file_patch.OutputFile));
					output_file.Close();

					File.Delete(output_path);

					return;
				}
			}
		}
	}
}

/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace FilePatcher.Patcher
{
	public class PatchApplier
	{
		#region Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Additional information for patch error events. </summary>
		public class PatchErrorEventArgs : EventArgs
		{
			public string Message;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Event queue for all listeners interested in ErrorOccurred events. </summary>
		public event EventHandler<PatchErrorEventArgs> ErrorOccurred;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the error occurred action. </summary>
		///
		/// <param name="message">	The message. </param>
		private void OnErrorOccurred(string message)
		{
			var handler = ErrorOccurred;

			if (handler != null)
			{
				handler(this, new PatchErrorEventArgs() { Message = message });
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the error occurred action. </summary>
		///
		/// <param name="format">	The message format. </param>
		/// <param name="args">  	A variable-length parameters list containing arguments. </param>
		private void OnErrorOccurred(string format, params object[] args)
		{
			OnErrorOccurred(String.Format(format, args));
		}
		#endregion
		
		#region Patch Definition
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Deserialize a patch definition. </summary>
		///
		/// <param name="xmlStream">	The XML stream. </param>
		///
		/// <returns>	A PatchDefinition. </returns>
		public static PatchDefinition DeserializeDefinition(XmlReader xmlStream)
		{
			if (xmlStream == null)
			{
				return null;
			}

			// Deserialize the definition xml
			var serializer = new XmlSerializer(typeof(PatchDefinition));
			if (!serializer.CanDeserialize(xmlStream))
			{
				return null;
			}

			return serializer.Deserialize(xmlStream) as PatchDefinition;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a patch definition from an embedded resource. </summary>
		///
		/// <param name="patchId">	Identifier for the patch resource. </param>
		///
		/// <returns>	The patch definition. </returns>
		private PatchDefinition GetDefinition(string patchId)
		{
			// Build the location of the patch definition resource
			// Open the resource for reading
			var patchResource = String.Format("FilePatcher.Definitions.{0}.xml", patchId);

			using(var xmlStream = XmlReader.Create(GetType().Assembly.GetManifestResourceStream(patchResource)))
			{
				return DeserializeDefinition(xmlStream);
			}
		}
		#endregion

		#region Write Data
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes a byte patch value to a stream. </summary>
		///
		/// <param name="target">	   	The target stream. </param>
		/// <param name="writeAddress">	The address to write to. </param>
		/// <param name="data">		   	The data to write. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool WriteByte(MemoryStream target
			, long writeAddress
			, PatchDefinition.Data data)
		{
			if ((writeAddress + 1) >= target.Length)
			{
				OnErrorOccurred("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID);
				return false;
			}

			// Try and parse the hex string
			byte parsedValue;
			if (!Utilities.Parse.TryParseHex(data.Value, out parsedValue))
			{
				OnErrorOccurred("Failed to parse value ({0}). Inform the developers of this.", data.ID);
				return false;
			}

			// Write the value to the stream
			target.Seek(writeAddress, SeekOrigin.Begin);
			target.WriteByte(parsedValue);
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes a ushort patch value to a stream. </summary>
		///
		/// <param name="target">	   	The target stream. </param>
		/// <param name="writeAddress">	The address to write to. </param>
		/// <param name="data">		   	The data to write. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool WriteUInt16(MemoryStream target
			, long writeAddress
			, PatchDefinition.Data data)
		{
			if ((writeAddress + 2) >= target.Length)
			{
				OnErrorOccurred("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID);
				return false;
			}

			// Try and parse the hex string
			ushort parsedValue;
			if (!Utilities.Parse.TryParseHex(data.Value, out parsedValue))
			{
				OnErrorOccurred("Failed to parse value ({0}). Inform the developers of this.", data.ID);
				return false;
			}

			// Swap the value's endianness if required
			if (data.SwapEndian)
			{
				ushort endianSwapped = 0;

				endianSwapped |= (ushort)((parsedValue & 0xff00) >> 8);
				endianSwapped |= (ushort)((parsedValue & 0x00ff) << 8);

				parsedValue = endianSwapped;
			}

			// Write the value to the stream
			target.Seek(writeAddress, SeekOrigin.Begin);
			target.Write(BitConverter.GetBytes(parsedValue), 0, 2);
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes a uint patch value to a stream. </summary>
		///
		/// <param name="target">	   	The target stream. </param>
		/// <param name="writeAddress">	The address to write to. </param>
		/// <param name="data">		   	The data to write. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool WriteUInt32(MemoryStream target
			, long writeAddress
			, PatchDefinition.Data data)
		{
			if ((writeAddress + 4) >= target.Length)
			{
				OnErrorOccurred("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID);
				return false;
			}

			// Try and parse the hex string
			uint parsedValue;
			if (!Utilities.Parse.TryParseHex(data.Value, out parsedValue))
			{
				OnErrorOccurred("Failed to parse value ({0}). Inform the developers of this.", data.ID);
				return false;
			}

			// Swap the value's endianness if required
			if (data.SwapEndian)
			{
				uint endianSwapped = 0;

				endianSwapped |= (uint)((parsedValue & 0xff000000) >> 24);
				endianSwapped |= (uint)((parsedValue & 0x00ff0000) >> 8);
				endianSwapped |= (uint)((parsedValue & 0x0000ff00) << 8);
				endianSwapped |= (uint)((parsedValue & 0x000000ff) << 24);

				parsedValue = endianSwapped;
			}

			// Write the value to the stream
			target.Seek(writeAddress, SeekOrigin.Begin);
			target.Write(BitConverter.GetBytes(parsedValue), 0, 4);
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes a string patch value to a stream. </summary>
		///
		/// <param name="target">	   	The target stream. </param>
		/// <param name="writeAddress">	The address to write to. </param>
		/// <param name="data">		   	The data to write. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool WriteString(Stream target
			, long writeAddress
			, PatchDefinition.Data data)
		{
			// Default write length to the length of the string if zero
			int writeLength = data.Length;
			if (writeLength == 0)
			{
				writeLength = data.Value.Length;
			}

			// Check we are not writing out of bounds
			if ((writeAddress + writeLength) >= target.Length)
			{
				OnErrorOccurred("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID);
				return false;
			}

			// Write the strings bytes as ASCII
			var bytes = Encoding.ASCII.GetBytes(data.Value);

			target.Seek(writeAddress, SeekOrigin.Begin);
			target.Write(bytes, 0, writeLength);

			// If necessary, write a null character to terminate the string
			if (data.WriteNull)
			{
				target.WriteByte(0);
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes a byte array patch value to a stream. </summary>
		///
		/// <param name="target">	   	The target stream. </param>
		/// <param name="writeAddress">	The address to write to. </param>
		/// <param name="data">		   	The data to write. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool WriteBytes(Stream target
			, long writeAddress
			, PatchDefinition.Data data)
		{
			// Check the strings length is a multiple of two
			if (data.Value.Length % 2 != 0)
			{
				OnErrorOccurred("The length of a hex byte string ({0}) in the file patcher is odd. Inform the developers.", data.ID);
				return false;
			}

			// Create the byte array
			byte[] bytes = null;
			try
			{
				// Convert every two characters into a byte
				bytes = Enumerable.Range(0, data.Value.Length)
					.Where(index => (index % 2) == 0)
					.Select(index => Convert.ToByte(data.Value.Substring(index, 2), 16))
					.ToArray();
			}
			catch(Exception)
			{
				OnErrorOccurred("Failed to parse a byte array ({0}). Inform the developers of this.", data.ID);
				return false;
			}

			// Default the write length to the array length if zero
			int writeLength = data.Length;
			if (writeLength == 0)
			{
				writeLength = bytes.Length;
			}

			if ((writeAddress + writeLength) >= target.Length)
			{
				OnErrorOccurred("Failed to write patch data ({0}) as the data would write beyond the end of the file. Inform the developers of this.", data.ID);
				return false;
			}

			// Write the byte array to the stream
			target.Seek(writeAddress, SeekOrigin.Begin);
			target.Write(bytes, 0, writeLength);
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes patch data to a stream. </summary>
		///
		/// <param name="output">   	The output stream. </param>
		/// <param name="address">  	The write address. </param>
		/// <param name="imageBase">	The image base address. </param>
		/// <param name="data">			The data to write. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool WriteData(MemoryStream output, uint address, int imageBase, PatchDefinition.Data data)
		{
			// Calculate the byte address and move to it
			long writeAddress = address - imageBase;
			if (writeAddress >= output.Length)
			{
				OnErrorOccurred("Failed to write patch data ({0}) as the pointer is beyond the end of the file. Inform the developers of this.", data.ID);
				return false;
			}
			output.Seek(writeAddress, SeekOrigin.Begin);
			
			bool success = false;

			switch (data.Type)
			{
				case PatchDefinition.ValueType.String:
					success = WriteString(output, writeAddress, data);
					break;
				case PatchDefinition.ValueType.UInteger32:
					success = WriteUInt32(output, writeAddress, data);
					break;
				case PatchDefinition.ValueType.UInteger16:
					success = WriteUInt16(output, writeAddress, data);
					break;
				case PatchDefinition.ValueType.Byte:
					success = WriteByte(output, writeAddress, data);
					break;
				case PatchDefinition.ValueType.Bytes:
					success = WriteBytes(output, writeAddress, data);
					break;
			}

			return success;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Applies a patch. </summary>
		///
		/// <param name="file">			The file to patch. </param>
		/// <param name="imageBase">	The image base address. </param>
		/// <param name="patch">		The patch to apply. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool ApplyPatch(MemoryStream file, int imageBase, PatchDefinition.Patch patch)
		{
			// Apply each patch
			foreach (var address in patch.Addresses)
			{
				// Find a matching data block
				var data = patch.Data.Find(value => address.Value.Equals(value.ID));
				if (data == null)
				{
					OnErrorOccurred("A patch address references an invalid data id ({0}). Inform the developers of this", address.Value);
					return false;
				}

				// Parse the address
				uint pointer = 0;
				try
				{
					pointer = Convert.ToUInt32(address.PTRString, 16);
				}
				catch
				{
					OnErrorOccurred("Failed to parse an addresses pointer string ({0}). Inform the developers of this", address.Value);
					return false;
				}

				// Write the data to the current address
				if (!WriteData(file, pointer, imageBase, data))
				{
					return false;
				}
			}
			return true;
		}
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Patches a file. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="patchDefinition">	The patch definition to apply. </param>
		/// <param name="sourceFile">	  	Source file. </param>
		/// <param name="outputDirectory">	Pathname of the output directory. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public bool Patch(PatchDefinition patchDefinition, string sourceFile, string outputDirectory)
		{
			if((patchDefinition == null) || (sourceFile == null) || (outputDirectory == null))
			{
				throw new ArgumentNullException("Null arguments are not valid");
			}

			if (sourceFile == String.Empty)
			{
				throw new ArgumentException("The source file path cannot be empty");
			}

			if (outputDirectory == String.Empty)
			{
				throw new ArgumentException("The output path cannot be empty");
			}

			string fileName = Path.GetFileName(sourceFile);

			// Get the start point for addresses in the file
			int imageBase = 0;
			try
			{
				imageBase = Convert.ToInt32(patchDefinition.PEAddressMaskString, 16);
			}
			catch
			{
				OnErrorOccurred("Failed to parse PE address mask string. Inform the developers of this.");
				return false;
			}

			// Load the source file into memory
			using (var patchedFile = Utilities.StreamIO.ReadFileToMemory(sourceFile, patchDefinition.MD5Pre))
			{
				if (patchedFile == null)
				{
					OnErrorOccurred("Failed to open {0}, please close all open applications before installing", fileName);
					return false;
				}

				// Apply all of the files patches
				foreach (var patch in patchDefinition.Patches)
				{
					if (!ApplyPatch(patchedFile, imageBase, patch))
					{
						return false;
					}
				}

				// Create an md5 hash of the patched data
				string patchedMd5 = Utilities.StreamIO.MD5Stream(patchedFile);
				if ((patchedMd5 == null) || !patchedMd5.Equals(patchDefinition.MD5Post, StringComparison.CurrentCultureIgnoreCase))
				{
					OnErrorOccurred("Failed to create an MD5 checksum for the patched data, of the checksum did not match. Inform the developers of this.");
					return false;
				}

				// Save the patched file
				if (!Directory.Exists(outputDirectory))
				{
					try
					{
						Directory.CreateDirectory(outputDirectory);
					}
					catch (Exception)
					{
						OnErrorOccurred("Failed to create the output directory for {0}. Inform the developers of this.", patchDefinition.OutputFile);
						return false;
					}
				}

				string outputPath = Path.Combine(outputDirectory, patchDefinition.OutputFile);
				using (var outputFile = File.OpenWrite(outputPath))
				{
					if(outputFile == null)
					{
						OnErrorOccurred("Failed to create {0}, please close all applications before installing", patchDefinition.OutputFile);
						return false;
					}

					outputFile.Write(patchedFile.GetBuffer(), 0, (int)patchedFile.Length);
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Patches a file. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		///
		/// <param name="patchId">		  	Identifier for the patch resource. </param>
		/// <param name="sourceFile">	  	Source file. </param>
		/// <param name="outputDirectory">	Pathname of the output directory. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public bool Patch(string patchId, string sourceFile, string outputDirectory)
		{
			if (patchId == null)
			{
				throw new ArgumentNullException("Patch id cannot be null");
			}

			if (patchId == String.Empty)
			{
				throw new ArgumentNullException("Patch id cannot be empty");
			}

			// Get the patch definition
			var definition = GetDefinition(patchId);
			if (definition == null)
			{
				OnErrorOccurred("Failed to find a patch for {0}. Inform the developers of this.", Path.GetFileName(sourceFile));
				return false;
			}

			return Patch(definition, sourceFile, outputDirectory);
		}
	}
}

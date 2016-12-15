/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.IO;
using FilePatcher.Utilities;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.FilePatcherUnitTests.Utilities
{
	[TestClass]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileOne.txt")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.txt")]
	public class StreamIOTests
	{
		#region CopyStream
		[TestMethod]
		public void CopyStream_WithTwoValidStreams_CopiesOneStreamsDataToAnother()
		{
			string result = "";

			using (var targetStream = new MemoryStream())
			using (var fileInput = File.OpenRead("DummyFileOne.txt"))
			{
				StreamIO.CopyStream(fileInput, targetStream);

				targetStream.Seek(0, SeekOrigin.Begin);
				result = new StreamReader(targetStream).ReadLine();
			}

			Assert.AreEqual("0123456789", result, "Failed to copy from stream to stream");
		}
		
		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when given null streams")]
		public void CopyStream_WithNullStreams_Throws()
		{
			StreamIO.CopyStream(null, null);
		}

		[TestMethod, ExpectedException(typeof(NotSupportedException), "Did not throw when given a write only stream to read from")]
		public void CopyStream_WithWriteOnlyStreamAsSource_Throws()
		{
			using (var sourceStream = File.OpenWrite("DummyFileOne.txt"))
			using (var targetStream = File.OpenWrite("DummyFileTwo.txt"))
			{
				StreamIO.CopyStream(sourceStream, targetStream);
			}
		}

		[TestMethod, ExpectedException(typeof(NotSupportedException), "Did not throw when given a read only stream to write to")]
		public void CopyStream_WithReadOnlyStreamAsTarget_Throws()
		{
			using (var sourceStream = File.OpenRead("DummyFileOne.txt"))
			using (var targetStream = File.OpenRead("DummyFileTwo.txt"))
			{
				StreamIO.CopyStream(sourceStream, targetStream);
			}
		}
		#endregion

		#region MD5Stream
		[TestMethod]
		public void MD5Stream_WithAValidStream_CreatesAnMD5Checksum()
		{
			string result = "";

			using (var fileInput = File.OpenRead("DummyFileOne.txt"))
			{
				result = StreamIO.MD5Stream(fileInput);
			}

			Assert.AreEqual("BFAF8AE957937AF5B8652A2137F1D95A", result, "Failed to create an MD5 checksum for a file stream");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when given a null stream")]
		public void MD5Stream_WithANullStream_Throws()
		{
			StreamIO.MD5Stream(null);
		}

		[TestMethod, ExpectedException(typeof(NotSupportedException), "Did not throw when given a write only stream to read from")]
		public void MD5Stream_WithAWriteOnlyStream_Throws()
		{
			string result = "";

			using (var fileInput = File.OpenWrite("DummyFileOne.txt"))
			{
				result = StreamIO.MD5Stream(fileInput);
			}
		}
		#endregion

		#region ReadFileToMemory
		[TestMethod]
		public void ReadFileToMemory_WithAnExistingFileAndValidChecksum_ReturnsADataStream()
		{
			var result = StreamIO.ReadFileToMemory("DummyFileOne.txt", "BFAF8AE957937AF5B8652A2137F1D95A");

			Assert.IsNotNull(result, "Failed to read a valid file to memory");
		}

		[TestMethod]
		public void ReadFileToMemory_WithAnExistingFileAndNoChecksum_ReturnsADataStream()
		{
			var result = StreamIO.ReadFileToMemory("DummyFileOne.txt");

			Assert.IsNotNull(result, "Failed to read a valid file to memory");
		}

		[TestMethod]
		public void ReadFileToMemory_WithAnExistingFileAndAnIncorrectChecksum_ReturnsNull()
		{
			var result = StreamIO.ReadFileToMemory("DummyFileOne.txt", "INCORRECTCHECKSUM");

			Assert.IsNull(result, "Read a file into memory when the checksum did not match");
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw when given an empty checksum")]
		public void ReadFileToMemory_WithAnExistingFileAndAnEmptyChecksum_Throws()
		{
			StreamIO.ReadFileToMemory("DummyFileOne.txt", "");
		}

		[TestMethod]
		public void ReadFileToMemory_WithAMissingFile_ReturnsNull()
		{
			var result = StreamIO.ReadFileToMemory("MissingFile.txt");

			Assert.IsNull(result, "Did not return null when attempting to read a non-existant file");
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw when given an empty file path")]
		public void ReadFileToMemory_WithAnEmptyFilePath_Throws()
		{
			StreamIO.ReadFileToMemory("");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when given a null file path")]
		public void ReadFileToMemory_WithANullFilePath_Throws()
		{
			StreamIO.ReadFileToMemory(null);
		}
		#endregion
	}
}
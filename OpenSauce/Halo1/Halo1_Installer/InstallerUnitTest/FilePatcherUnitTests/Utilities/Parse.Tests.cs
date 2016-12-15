/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.FilePatcherUnitTests.Utilities
{
	[TestClass]
	public class ParseTests
	{
		private void TestTryParseHex<T>(string hexValue, T expected, bool shouldSucceed = true)
		{
			T value = default(T);

			var result = FilePatcher.Utilities.Parse.TryParseHex(hexValue, out value);

			if(shouldSucceed)
			{
				Assert.IsTrue(result, "Failed to parse a hexidecimal string value");
				Assert.AreEqual(expected, value, "Failed to parse a hexidecimal string value");
			}
			else
			{
				Assert.IsFalse(result, "Returned true when parsing should have failed");
			}
		}

		#region Byte
		[TestMethod]
		public void TryParseHex_WithMaxByte_ParsesTheValue()
		{
			TestTryParseHex("0xFF", Byte.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinByte_ParsesTheValue()
		{
			TestTryParseHex("0x00", Byte.MinValue);
		}

		[TestMethod]
		public void TryParseHex_WithMaxSByte_ParsesTheValue()
		{
			TestTryParseHex("0x7F", SByte.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinSByte_ParsesTheValue()
		{
			TestTryParseHex("0x80", SByte.MinValue);
		}
		#endregion

		#region Short
		[TestMethod]
		public void TryParseHex_WithMaxInt16_ParsesTheValue()
		{
			TestTryParseHex("0x7FFF", Int16.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinInt16_ParsesTheValue()
		{
			TestTryParseHex("0x8000", Int16.MinValue);
		}

		[TestMethod]
		public void TryParseHex_WithMaxUInt16_ParsesTheValue()
		{
			TestTryParseHex("0xFFFF", UInt16.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinUInt16_ParsesTheValue()
		{
			TestTryParseHex("0x0000", UInt16.MinValue);
		}
		#endregion

		#region Int
		[TestMethod]
		public void TryParseHex_WithMaxInt32_ParsesTheValue()
		{
			TestTryParseHex("0x7FFFFFFF", Int32.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinInt32_ParsesTheValue()
		{
			TestTryParseHex("0x80000000", Int32.MinValue);
		}

		[TestMethod]
		public void TryParseHex_WithMaxUInt32_ParsesTheValue()
		{
			TestTryParseHex("0xFFFFFFFF", UInt32.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinUInt32_ParsesTheValue()
		{
			TestTryParseHex("0x00000000", UInt32.MinValue);
		}
		#endregion

		#region Long
		[TestMethod]
		public void TryParseHex_WithMaxInt64_ParsesTheValue()
		{
			TestTryParseHex("0x7FFFFFFFFFFFFFFF", Int64.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinInt64_ParsesTheValue()
		{
			TestTryParseHex("0x8000000000000000", Int64.MinValue);
		}

		[TestMethod]
		public void TryParseHex_WithMaxUInt64_ParsesTheValue()
		{
			TestTryParseHex("0xFFFFFFFFFFFFFFFF", UInt64.MaxValue);
		}

		[TestMethod]
		public void TryParseHex_WithMinUInt64_ParsesTheValue()
		{
			TestTryParseHex("0x0000000000000000", UInt64.MinValue);
		}
		#endregion
	}
}

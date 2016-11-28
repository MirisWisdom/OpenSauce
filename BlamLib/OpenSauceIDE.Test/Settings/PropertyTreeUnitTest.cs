using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.IO;

namespace OpenSauceIDE.Test
{
	[TestClass]
	public class PropertyTreeUnitTest
	{
		public enum PropertyTreeTestEnum
		{
			Value1,
			Value2,
			Value3,
		}

		private readonly bool mBool = true;
		private readonly byte mByte = 64;
		private readonly sbyte mSByte = -64;
		private readonly ushort mUShort = 4096;
		private readonly short mShort = -4096;
		private readonly uint mUInt = 131070;
		private readonly int mInt = -131070;
		private readonly float mFloat = 1.88888f;
		private readonly double mDouble = 1.44444;
		private readonly char mChar = 'W';
		private readonly string mString = "TestString";
		private readonly PropertyTreeTestEnum mEnum1 = PropertyTreeTestEnum.Value1;
		private readonly PropertyTreeTestEnum mEnum2 = PropertyTreeTestEnum.Value2;
		private readonly PropertyTreeTestEnum mEnum3 = PropertyTreeTestEnum.Value3;
		private readonly bool[] mBoolList = { true, false, true, true, false, false };

		private OpenSauceIDE.Settings.PropertyTree BuildPropertyTree()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = new OpenSauceIDE.Settings.PropertyTree('.');

			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestBool", mBool);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestByte", mByte);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestSByte", mSByte);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestUShort", mUShort);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestShort", mShort);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestUInt", mUInt);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestInt", mInt);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestFloat", mFloat);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestDouble", mDouble);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestChar", mChar);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestString", mString);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestEnum1", mEnum1);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestEnum2", mEnum2);
			propertyTree.PutValue("OpenSauceTest.SubList1.SubList2.TestEnum3", mEnum3);

			foreach (var boolValue in mBoolList)
			{
				propertyTree.PutValue("OpenSauceTest.SubList1.TestBoolList", boolValue);
			}

			return propertyTree;
		}

		private bool TestPropertyTree(OpenSauceIDE.Settings.PropertyTree propertyTree)
		{
			bool getBool = propertyTree.GetValue<bool>("OpenSauceTest.SubList1.SubList2.TestBool");
			byte getByte = propertyTree.GetValue<byte>("OpenSauceTest.SubList1.SubList2.TestByte");
			sbyte getSByte = propertyTree.GetValue<sbyte>("OpenSauceTest.SubList1.SubList2.TestSByte");
			ushort getUShort = propertyTree.GetValue<ushort>("OpenSauceTest.SubList1.SubList2.TestUShort");
			short getShort = propertyTree.GetValue<short>("OpenSauceTest.SubList1.SubList2.TestShort");
			uint getUInt = propertyTree.GetValue<uint>("OpenSauceTest.SubList1.SubList2.TestUInt");
			int getInt = propertyTree.GetValue<int>("OpenSauceTest.SubList1.SubList2.TestInt");
			float getFloat = propertyTree.GetValue<float>("OpenSauceTest.SubList1.SubList2.TestFloat");
			double getDouble = propertyTree.GetValue<double>("OpenSauceTest.SubList1.SubList2.TestDouble");
			char getChar = propertyTree.GetValue<char>("OpenSauceTest.SubList1.SubList2.TestChar");
			string getString = propertyTree.GetValue<string>("OpenSauceTest.SubList1.SubList2.TestString");
			PropertyTreeTestEnum getEnum1 = propertyTree.GetValue<PropertyTreeTestEnum>("OpenSauceTest.SubList1.SubList2.TestEnum1");
			PropertyTreeTestEnum getEnum2 = propertyTree.GetValue<PropertyTreeTestEnum>("OpenSauceTest.SubList1.SubList2.TestEnum2");
			PropertyTreeTestEnum getEnum3 = propertyTree.GetValue<PropertyTreeTestEnum>("OpenSauceTest.SubList1.SubList2.TestEnum3");

			Assert.AreEqual(mBool, getBool);
			Assert.AreEqual(mByte, getByte);
			Assert.AreEqual(mSByte, getSByte);
			Assert.AreEqual(mUShort, getUShort);
			Assert.AreEqual(mShort, getShort);
			Assert.AreEqual(mUInt, getUInt);
			Assert.AreEqual(mInt, getInt);
			Assert.AreEqual(mFloat, getFloat);
			Assert.AreEqual(mDouble, getDouble);
			Assert.AreEqual(mChar, getChar);
			Assert.AreEqual(mString, getString);
			Assert.AreEqual(mEnum1, getEnum1);
			Assert.AreEqual(mEnum2, getEnum2);
			Assert.AreEqual(mEnum3, getEnum3);

			var boolEnumerator = propertyTree.GetValueEnumerator<bool>("OpenSauceTest.SubList1.TestBoolList");
			foreach (var boolValue in mBoolList)
			{
				Assert.IsTrue(boolEnumerator.MoveNext());
				Assert.AreEqual(boolValue, boolEnumerator.Current);
			}

			return true;
		}

		[TestMethod]
		public void PropertyTreePutGet()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = BuildPropertyTree();
			TestPropertyTree(propertyTree);
		}

		[TestMethod]
		public void PropertyTreePutExceptions()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = BuildPropertyTree();

			try
			{
				propertyTree.PutValue<bool>("", false);
				Assert.Fail("Did not exception when passing an empty path.");
			}
			catch { };

			try
			{
				propertyTree.PutValue<bool>("IncorrectRoot.SubList1.SubList2.SubList3.TestBool", false);
				Assert.Fail("Did not exception when the root node name was incorrect.");
			}
			catch { };
		}

		[TestMethod]
		public void PropertyTreeGetExistingExceptions()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = BuildPropertyTree();

			try
			{
				propertyTree.GetValue<bool>("");
				Assert.Fail("Did not exception when passing an empty path.");
			}
			catch { };

			try
			{
				propertyTree.GetValue<bool>("IncorrectRoot.SubList1.SubList2.SubList3.TestBool");
				Assert.Fail("Did not exception when the root node name was incorrect.");
			}
			catch { };

			try
			{
				propertyTree.GetValue<bool>("OpenSauceTest.SubList1.SubList2.SubList3.TestBool");
				Assert.Fail("Did not exception when an requested path node was missing.");
			}
			catch { };
		}

		[TestMethod]
		public void PropertyTreeGetCreateExceptions()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = BuildPropertyTree();

			try
			{
				propertyTree.GetValue<int>("", 0xBEEF);
				Assert.Fail("Did not exception when passing an empty path.");
			}
			catch { };

			try
			{
				propertyTree.GetValue<int>("IncorrectRoot.SubList1.SubList2.SubList3.TestBool", 0xBEEF);
				Assert.Fail("Did not exception when the root node name was incorrect.");
			}
			catch { };

			// This one should not exception if a missing node is used
			try
			{
				int value = propertyTree.GetValue<int>("OpenSauceTest.SubList1.SubList2.SubList3.TestBool", 0xBEEF);
				Assert.AreEqual(0xBEEF, value, "Did not return the default value when a requested node was missing.");
			}
			catch
			{
				Assert.Fail("Exceptioned when requesting the value of a missing node that should have been created automatically.");
			};
		}

		[TestMethod]
		public void PropertyTreeGetEnumeratorExceptions()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = BuildPropertyTree();
			
			try
			{
				propertyTree.GetValueEnumerator<bool>("");
				Assert.Fail("Did not exception when passing an empty path.");
			}
			catch { };

			try
			{
				propertyTree.GetValueEnumerator<bool>("OpenSauceTest");
				Assert.Fail("Did not exception when attempting to enumerate root nodes.");
			}
			catch { };

			try
			{
				propertyTree.GetValueEnumerator<bool>("IncorrectRoot.SubList1.TestBoolList");
				Assert.Fail("Did not exception when the root node name was incorrect");
			}
			catch { };

			try
			{
				propertyTree.GetValueEnumerator<bool>("OpenSauceTest.SubList1.SubList2.TestBoolList");
				Assert.Fail("Did not exception when an requested path node was missing.");
			}
			catch { };
		}

		[TestMethod]
		public void PropertyTreeSaveReadXML()
		{
			OpenSauceIDE.Settings.PropertyTree propertyTree = BuildPropertyTree();
			propertyTree.SaveXml(Path.Combine(Directory.GetCurrentDirectory(), "PropertyTreeSaveReadXML.xml"));

			propertyTree = new OpenSauceIDE.Settings.PropertyTree('.');
			propertyTree.ReadXml(Path.Combine(Directory.GetCurrentDirectory(), "PropertyTreeSaveReadXML.xml"));

			TestPropertyTree(propertyTree);
		}
	}
}

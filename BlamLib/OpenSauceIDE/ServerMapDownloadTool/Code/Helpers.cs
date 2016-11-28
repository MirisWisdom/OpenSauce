using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	// used to wrap the string type for use in data binding
	[Serializable()]
	public class StringWrapperClass
	{
		public string Value { get; set; }

		StringWrapperClass() { }
		public StringWrapperClass(string value)
		{ Value = value; }
	};

	// used to wrap the compression dictionary size enum, with an associated display string for data binding
	public class DictionarySizeWrapper
	{
		public int Value { get; set; }
		public string DisplayValue { get; set; }

		DictionarySizeWrapper() { }
		public DictionarySizeWrapper(int value, string display_value)
		{ Value = value; DisplayValue = display_value; }
	};
}

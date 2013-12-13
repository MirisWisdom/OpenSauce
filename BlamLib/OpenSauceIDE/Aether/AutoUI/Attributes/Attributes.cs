using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.Aether.AutoUI.Attributes
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Attribute to indicate that the auto ui system should use the member. </summary>
	///-------------------------------------------------------------------------------------------------
	[AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
	public class AutoUIAttribute : Attribute { }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Attribute to indicate that the auto ui system should ignore a member. </summary>
	///-------------------------------------------------------------------------------------------------
	[AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
	public class AutoUIIgnoreAttribute : Attribute { }

	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// 	Attribute for classes to define the member to use as the ui name, with formatting.
	/// </summary>
	///-------------------------------------------------------------------------------------------------
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
	public class AutoUINameFormattedAttribute : Attribute
	{
		public string Format { get; private set; }
		public string Source { get; private set; }

		public AutoUINameFormattedAttribute(string format, string source)
		{
			Format = format;
			Source = source;
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Attribute for classes to define the name to use. </summary>
	///-------------------------------------------------------------------------------------------------
	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Enum | AttributeTargets.Field | AttributeTargets.Property | AttributeTargets.Struct, AllowMultiple = false)]
	public class AutoUINameAttribute : Attribute
	{
		public string Name { get; private set; }

		public AutoUINameAttribute(string name)
		{
			Name = name;
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Attribute for limiting the size of controls. </summary>
	///-------------------------------------------------------------------------------------------------
	[AttributeUsage(AttributeTargets.Class)]
	public class AutoUISizeAttribute : Attribute
	{
		public int MinWidth { get; private set; }
		public int MinHeight { get; private set; }
		public int MaxWidth { get; private set; }
		public int MaxHeight { get; private set; }

		public AutoUISizeAttribute(int minWidth, int minHeight, int maxWidth, int maxHeight)
		{
			MinWidth = minWidth;
			MinHeight = minHeight;
			MaxWidth = maxWidth;
			MaxHeight = maxHeight;
		}
	}
}

using OpenSauceIDE.Aether.AutoUI.Controls;
using System;

namespace OpenSauceIDE.Aether.AutoUI.Attributes
{
	/// <summary>	Attribute to indicate that the auto ui system should use the member. </summary>
	[AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
	public class AutoUIAttribute : Attribute { }

	/// <summary>	Attribute to indicate that the auto ui system should ignore a member. </summary>
	[AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
	public class AutoUIIgnoreAttribute : Attribute { }

	/// <summary>	Attribute for classes to define the member to use as the ui name, with formatting. </summary>
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

	/// <summary>	Attribute for defining the name to use. </summary>
	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Enum | AttributeTargets.Field | AttributeTargets.Property | AttributeTargets.Struct, AllowMultiple = false)]
	public class AutoUINameAttribute : Attribute
	{
		public string Name { get; private set; }

		public AutoUINameAttribute(string name)
		{
			Name = name;
		}
	}

	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Enum | AttributeTargets.Field | AttributeTargets.Property | AttributeTargets.Struct, AllowMultiple = false)]
	public class AutoUIControlTypeAttribute : Attribute
	{
		public Type ControlType { get; private set; }

		public AutoUIControlTypeAttribute(Type controlType)
		{
			if(!typeof(IAutoUIControl).IsAssignableFrom(controlType))
			{
				throw new Exception("An AutoUIControlType attribute was defined with an object type that does not implement IAutoUIControl");
			}

			ControlType = controlType;
		}
	}
}

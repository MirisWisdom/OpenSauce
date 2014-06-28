using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam multi byte control. </summary>
	public class AutoUIBlamMultiByte : AutoUIBlamMultiValue<AutoUIBlamByteInteger>
	{
		public AutoUIBlamMultiByte(params string[] members)
			: base(members)
		{ }

		public AutoUIBlamMultiByte()
			: this(null)
		{ }
	}
}

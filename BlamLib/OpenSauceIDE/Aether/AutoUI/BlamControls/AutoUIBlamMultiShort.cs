using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam multi short control. </summary>
	public class AutoUIBlamMultiShort : AutoUIBlamMultiValue<AutoUIBlamShortInteger>
	{
		public AutoUIBlamMultiShort(params string[] members)
			: base(members)
		{ }

		public AutoUIBlamMultiShort()
			: this(null)
		{ }
	}
}

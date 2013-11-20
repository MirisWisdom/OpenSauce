using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
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

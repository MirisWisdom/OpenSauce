using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	public class AutoUIBlamRectangle2D : AutoUIBlamMultiValue<AutoUIBlamShortInteger>
	{
		public AutoUIBlamRectangle2D()
			: base("T", "L", "B", "R")
		{ }
	}
}

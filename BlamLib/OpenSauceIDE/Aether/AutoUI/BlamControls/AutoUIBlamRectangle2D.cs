using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam rectangle 2d control. </summary>
	public class AutoUIBlamRectangle2D : AutoUIBlamMultiValue<AutoUIBlamShortInteger>
	{
		public AutoUIBlamRectangle2D()
			: base("T", "L", "B", "R")
		{ }
	}
}

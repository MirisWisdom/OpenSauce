using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam point 2d control. </summary>
	public class AutoUIBlamPoint2D : AutoUIBlamMultiValue<AutoUIBlamShortInteger>
	{
		public AutoUIBlamPoint2D()
			: base("X", "Y")
		{ }
	}
}

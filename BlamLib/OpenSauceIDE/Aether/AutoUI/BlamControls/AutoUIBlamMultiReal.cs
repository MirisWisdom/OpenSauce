using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam multi real control. </summary>
	public class AutoUIBlamMultiReal : AutoUIBlamMultiValue<AutoUIBlamReal>
	{
		public AutoUIBlamMultiReal(params string[] members)
			: base(members)
		{ }

		public AutoUIBlamMultiReal()
			: this(null)
		{ }
	}
}

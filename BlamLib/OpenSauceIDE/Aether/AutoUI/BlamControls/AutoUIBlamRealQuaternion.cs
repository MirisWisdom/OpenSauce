using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam real quaternion control. </summary>
	public class AutoUIBlamRealQuaternion : AutoUIBlamMultiReal
	{
		public AutoUIBlamRealQuaternion()
			: base("I", "J", "K", "W")
		{ }
	}
}

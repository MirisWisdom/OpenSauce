using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam real plane 3d control. </summary>
	public class AutoUIBlamRealPlane3D : AutoUIBlamMultiReal
	{
		public AutoUIBlamRealPlane3D()
			: base("I", "J", "K", "D")
		{ }
	}
}

using System;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam real euler angles 3d control. </summary>
	public class AutoUIBlamRealEulerAngles3D : AutoUIBlamMultiReal
	{
		public AutoUIBlamRealEulerAngles3D()
			: base("Y", "P", "R")
		{ }
	}
}

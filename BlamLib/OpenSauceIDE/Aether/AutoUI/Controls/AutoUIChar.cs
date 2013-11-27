using System;
using System.Windows.Forms;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	An automatic user interface text box for a single character. </summary>
	///-------------------------------------------------------------------------------------------------
	[Attributes.AutoUISize(50, 0, 50, 0)]
	public class AutoUIChar : AutoUIString
	{
		public AutoUIChar()
			: base(1)
		{ }
	}
}

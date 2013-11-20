using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
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

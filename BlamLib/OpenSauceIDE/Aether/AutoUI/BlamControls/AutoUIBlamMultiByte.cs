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
	public class AutoUIBlamMultiByte : AutoUIBlamMultiValue<AutoUIBlamByteInteger>
	{
		public AutoUIBlamMultiByte(params string[] members)
			: base(members)
		{ }

		public AutoUIBlamMultiByte()
			: this(null)
		{ }
	}
}

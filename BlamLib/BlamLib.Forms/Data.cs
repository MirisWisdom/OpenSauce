/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BlamLib.Forms
{
	public partial class Data : BlamLib.Forms.Field
	{
		public Data()
		{
			InitializeComponent();

			_Setup(name, units);
		}

		public override void Clear() { fieldSize.Text = ""; }
	};
}
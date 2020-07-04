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
	public partial class Enum : BlamLib.Forms.Field
	{
		public int Field
		{
			get { return field.SelectedIndex; }
			set
			{
				try { field.SelectedIndex = value; }
				catch (ArgumentOutOfRangeException) { throw new Debug.ExceptionLog("The \'{0}\' enum field was given a value #{1} ({2})", ControlName, value, field.Items.Count); }
			}
		}

		public Enum()
		{
			InitializeComponent();
			_Setup(name);
		}

		public override void Clear() { field.SelectedIndex = -1; }

		public override void AddEventHandlers(params object[] handlers) { field.SelectedIndexChanged += (handlers[1] as EventHandler); }
	};
}